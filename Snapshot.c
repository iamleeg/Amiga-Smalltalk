#include "Snapshot.h"
#include "Types.h"
#include "ObjectMemory.h"
#include "ObjectMemory_FreeList.h"
#include "RealWordMemory.h"
#include <assert.h>
#include <exec/types.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <stdio.h>

/*
 * -- SAVE -------------------------------------------------------------------------------
 */

/*
 * start at the current position and pad zeros up to a 512 word boundary
 */
Bool _padToPage(BPTR filehandle) {
	LONG pos = Seek(filehandle, 0, OFFSET_CURRENT);
	LONG desired = ((pos + 512 -1 ) / 512) * 512;
	WORD zero = 0L;
	LONG pads = (desired - pos) / sizeof(zero);
	while( pads-- > 0 ) {
		Write(filehandle, (APTR)&zero, sizeof(zero));
	}
	return YES;
}


ObjectPointer ObjectMemory_lastUsedObjectPointer() {
	ObjectPointer lastUsedObjectPointer = NilPointer;
	ObjectPointer iteratingPointer = NilPointer;
	
	for( iteratingPointer = 2; iteratingPointer < ObjectTableSize; iteratingPointer += 2) {
		if( ObjectMemory_hasObject(iteratingPointer) ) {
			lastUsedObjectPointer = iteratingPointer;
		}
	}
	return lastUsedObjectPointer;
}

Bool _saveSnapshotHeader(BPTR filehandle) {
	LONG placeholder[2] = {0};
	BYTE  interchange[2] = {0};
	
	/* We will fill these placeholder values in at the end once we calculate them */
	if( Write(filehandle, &placeholder, sizeof(placeholder)) != sizeof(placeholder) ) {
		return NO;
	}
	/* we leave this empty */
	
	if( Write(filehandle, &interchange, sizeof(interchange)) != sizeof(interchange) ) {
		return NO;
	}	
	return YES;
}

LONG _writeObjects(BPTR filehandle, WORD storedObjectTableLength) {
	LONG objectSpaceLength = 0;
	WORD objectSize = 0;
	ObjectPointer anObject = NilPointer;
	WORD header[2];
	WORD wordLengthOfObject = 0;
	WORD wordIndex = 0;
	Word word = 0;

	printf("WRITING OBJECTS, POSITION = %ld\n", Seek(filehandle, 0, OFFSET_CURRENT));
	for( anObject = FirstUnusedObjectPointer; anObject < storedObjectTableLength; anObject += 2) {
		if(!ObjectMemory_hasObject(anObject)) continue;
		
		objectSize = (WORD)ObjectMemory_sizeBitsOf(anObject);
		printf("\nO[%lu]", anObject);
		printf("[%d]", objectSize);
		header[0] = objectSize;
		header[1] = (WORD)ObjectMemory_fetchClassOf(anObject);
		printf("WRITING AN OBJECT, POSITION = %ld\n", Seek(filehandle, 0, OFFSET_CURRENT));
		if( Write(filehandle, (APTR)&header, sizeof(header)) != sizeof(header) ) {
			return 0;
		}
		wordLengthOfObject = ObjectMemory_fetchWordLengthOf(anObject);
		printf("[%d]", wordLengthOfObject);
		for(wordIndex = 0; wordIndex < wordLengthOfObject; wordIndex++) {
			word = ObjectMemory_fetchWord_ofObject( wordIndex, anObject);
			printf("{%d:%ld}", wordIndex, word);
			if( Write(filehandle, (APTR)&word, sizeof(word)) != sizeof(word) ) {
				return 0;
			}
		}
		objectSpaceLength += objectSize;
	}
	printf("\nOSLEN = %ld\n", objectSpaceLength);
	return objectSpaceLength;
}

Bool _writeObjectTable(BPTR filehandle, WORD storedObjectTableLength) {
    int objectImageWordAddress = 0;
    ObjectPointer objectPointer = NilPointer;
    WORD oldOTValue = 0;
    WORD oldOTLocation = 0;
    WORD objectSize = 0;
    WORD words[2] = {0};
    
    int written = 0;
    printf("WRITING OBJECT TABLE, POSITION = %ld\n", Seek(filehandle, 0, OFFSET_CURRENT));

    for( objectPointer = FirstUnusedObjectPointer; objectPointer < storedObjectTableLength; objectPointer += 2)
    {
        oldOTValue = ObjectMemory_ot(objectPointer);
        oldOTLocation = ObjectMemory_locationBitsOf(objectPointer);
        
        if (objectPointer >= 2)
        {
            if (!ObjectMemory_freeBitOf(objectPointer) && ObjectMemory_countBitsOf(objectPointer) == 0)
            {
               /*// This entry was for a free chunk of memory, but we don't save
                // free space in the image. Store as a
                // an available OT entry by setting free bit*/
                ObjectMemory_freeBitOf_put(objectPointer, 1);
            }
            
            if (ObjectMemory_freeBitOf(objectPointer))
            {
                /*// manual.pdf - page 3: free entries have freeBit set and other bits in both
                  // words are 0.*/
                ObjectMemory_ot_put(objectPointer, 0);
                ObjectMemory_freeBitOf_put(objectPointer, 1);
                ObjectMemory_locationBitsOf_put(objectPointer, 0);
            }
            else
            {
                objectSize = (WORD) ObjectMemory_sizeBitsOf(objectPointer);
            
                /*// Modify the location of the object table entry... we do this once we no longer
                  // process this object table entry*/
                ObjectMemory_segmentBitsOf_put(objectPointer, objectImageWordAddress >> 16);
                ObjectMemory_locationBitsOf_put(objectPointer, objectImageWordAddress & 0xffff);
                objectImageWordAddress += objectSize;
            }
        }
        
        /* Assemble object table entry */

        words[0] = ObjectMemory_ot(objectPointer);
        words[1] = ObjectMemory_locationBitsOf(objectPointer);
   
        /* Restore OT entry */
        ObjectMemory_ot_put(objectPointer, oldOTValue);
        ObjectMemory_locationBitsOf_put(objectPointer, oldOTLocation);
        
        /* Write this entry */
        written = Write(filehandle, (APTR) &words, sizeof(words));
        if (written != sizeof(words)) {
        	printf("[%d != %lu]", written, sizeof(words) );
            return NO;
        }
    }
    return YES;
}

Bool _saveObjects(BPTR filehandle) {
	LONG objectSpaceLength = 0;
	LONG storedObjectTableLength = ObjectMemory_lastUsedObjectPointer() + 2;

	if(!_saveSnapshotHeader(filehandle)) {
	printf("header");
		return NO;
	}
	printf("WROTE HEADER, POSITION = %ld\n", Seek(filehandle, 0, OFFSET_CURRENT));

	
	if(!_padToPage(filehandle)) {
	printf("pad1");
		return NO;
	}
	printf("PADDED POSITION = %ld\n", Seek(filehandle, 0, OFFSET_CURRENT));

	objectSpaceLength = _writeObjects(filehandle, storedObjectTableLength);
	if(objectSpaceLength == 0) {
	printf("objects");
		return NO;
	}	
	printf("WROTE OBJECTS = %ld\n", Seek(filehandle, 0, OFFSET_CURRENT));

	if(!_padToPage(filehandle)) {
	printf("pad2");
		return NO;
	}
	printf("PADDED POSITION = %ld\n", Seek(filehandle, 0, OFFSET_CURRENT));
	
	if(!_writeObjectTable(filehandle, storedObjectTableLength)) {
	printf("table");
		return NO;
	}
	printf("WROTE TABLE = %ld\n", Seek(filehandle, 0, OFFSET_CURRENT));
    
	printf("WROTE OSLENGTH = %ld\n", objectSpaceLength);
    /* Now we can go back fill in the values for the image header. */
    Seek(filehandle, 0, OFFSET_BEGINNING);
    Write( filehandle, (APTR)&objectSpaceLength, sizeof(objectSpaceLength) );
    Write( filehandle, (APTR)&storedObjectTableLength, sizeof(storedObjectTableLength) );
    Flush( filehandle );
	
	return YES;
}

Bool ObjectMemory_saveSnapshot(CONST_STRPTR filename) {
	Bool result = NO;
	BPTR snapshotFile = 0;
	printf("[START]");
	snapshotFile = Open(filename, MODE_READWRITE);
	if( snapshotFile != (BPTR)NULL ) {
		printf("[fileok]");
		result = _saveObjects(snapshotFile);
		Close( snapshotFile );
	}
		printf("[END]");
	return result;
}




/*
 * -- LOAD -------------------------------------------------------------------------------
 */
Bool _loadObjectTable(BPTR filehandle) {
    /* First two 32-bit values have the object space length and object table lengths in words */
    LONG objectSpaceLength = -1;
    LONG objectTableLength = -1;
    LONG fileSize = -1;
    ObjectPointer objectPointer = NilPointer;
    

    
    if (Seek(filehandle, 0, OFFSET_BEGINNING) == -1) /* Skip over object space length */
        return NO;
        
    if (Read(filehandle, (APTR)&objectSpaceLength, sizeof(objectSpaceLength)) != sizeof(objectSpaceLength))
        return NO;

    if (Seek(filehandle, 4, OFFSET_BEGINNING) == -1) /* Skip over object space length */
        return NO;
        
    if (Read(filehandle, (APTR)&objectTableLength, sizeof(objectTableLength)) != sizeof(objectTableLength))
        return NO;
            
            
    printf("LOADED OBJECT SPACE LENGTH %ld\n", objectSpaceLength);
    printf("LOADED OBJECT TABLE LENGTH %ld\n", objectTableLength);
    
    if (Seek(filehandle, objectTableLength * -2, OFFSET_END) == -1) /* Reposition to start of object table */
        return NO;

    printf("READING OBJECT TABLE, POSITION = %ld\n", Seek(filehandle, 0, OFFSET_CURRENT));

    for(objectPointer = FirstUnusedObjectPointer; objectPointer < objectTableLength; objectPointer+=2)
    {
        WORD words[2];
        if (Read(filehandle, (APTR)&words, sizeof(words)) != sizeof(words))
            return NO;
        ObjectMemory_ot_put(objectPointer, words[0]);
        ObjectMemory_locationBitsOf_put(objectPointer, words[1]);
    }
     
    ObjectMemory_headOfFreePointerList_put(NilPointer);
    
    /* Initialize the remaining entries as free */

    for(objectPointer = objectTableLength; objectPointer < ObjectTableSize; objectPointer += 2)
    {
        ObjectMemory_ot_put(objectPointer, 0);
        ObjectMemory_freeBitOf_put(objectPointer, 1);
        ObjectMemory_locationBitsOf_put(objectPointer, 0);
    }
    
    /*
    // Build the OT entry free list, Go backwards so we have lower entries first on free list.
    // Why? Makes the OT easier to see in debugger.
    // Note we skip oop 0, which is considered reserved and invalid. (page 2, Xerox Virtual Image booklet) */
    
    /* in the dbanay code, its OTSize-2, but we have an odd OTSize so needs to be -1 */
    for(objectPointer = ObjectTableSize-1; objectPointer >= 2; objectPointer -= 2) {
        if (ObjectMemory_freeBitOf(objectPointer))
            ObjectMemory_toFreePointerListAdd(objectPointer);
    }
    
    
    return YES;
}

Bool _loadObjects(BPTR filehandle) {
   int SegmentHeapSpaceSize = HeapSpaceStop + 1;
    
    int* heapSpaceRemaining;
    int segment = FirstHeapSegment;
    int destinationSegment = FirstHeapSegment;
    int destinationWord = 0;
    ObjectPointer objectPointer = NilPointer;
    int objectImageWordAddress = 0;
    WORD objectSize;
    int extraSpace = 0;
    int space = 0;
    
    WORD classBits = 0;
    int wordIndex = 0;
    WORD word = 0;
    
    int freeChunkSize = 0;
    int freeChunkLocation = 0;
    
    int size = 0;
    
    heapSpaceRemaining = (int*)malloc(sizeof(int) * HeapSegmentCount);
    
    /* Track amount of free space available for objects in each segment */

    for(segment = FirstHeapSegment; segment <= LastHeapSegment; segment++)
        heapSpaceRemaining[segment - FirstHeapSegment] = SegmentHeapSpaceSize;
    
    /* Load objects from the virtual image into the heap segments
       being careful to not split an object across a segment boundary */
    
    for(objectPointer = FirstUnusedObjectPointer; objectPointer < ObjectTableSize; objectPointer += 2)
    {
        if (ObjectMemory_freeBitOf(objectPointer)) continue;
        /* A free chunk has it's COUNT field set to zero but the free bit is clear */
        assert (ObjectMemory_countBitsOf(objectPointer) != 0); /* SANITY Make sure a freeChunk wasn't saved!*/ 
        
        /* On disk objects are stored contiguously as if a large 20-bit WORD addressed space
        // In this scheme, the OT segment and locations combine to form a WORD address */
        objectImageWordAddress = (ObjectMemory_segmentBitsOf(objectPointer) << 16) + ObjectMemory_locationBitsOf(objectPointer);
        
        Seek(filehandle, 512 + objectImageWordAddress * sizeof(WORD), OFFSET_BEGINNING);
        printf("READING AN OBJECT FROM POSITION %ld\n", Seek(filehandle, 0, OFFSET_CURRENT));
        
        Read(filehandle, (APTR) &objectSize, sizeof(objectSize));
        
        
        /* Account for the extra word used by HugeSize objects */
        extraSpace = objectSize < HugeSize || ObjectMemory_pointerBitOf(objectPointer) == 0 ? 0 : 1;
        space = objectSize + extraSpace; /* space in memory */
        
        if (space > heapSpaceRemaining[destinationSegment - FirstHeapSegment])
        {
            /* No room left in the current segment, move to next */
            destinationSegment++;
            if (destinationSegment == HeapSegmentCount) {
            	free(heapSpaceRemaining);
            	return NO; /* Full */
            }
            destinationWord = 0;
        }
        
        /* Update OT entry so that it references the object location in ObjectMemory vs the disk image */
        ObjectMemory_segmentBitsOf_put(objectPointer, destinationSegment);
        ObjectMemory_locationBitsOf_put(objectPointer, destinationWord);
        
        /* Store the object in the image into word memory
        // First comes the size...*/
        
        ObjectMemory_sizeBitsOf_put(objectPointer, objectSize );
        
        /* Next is the class... */
        Read(filehandle, (APTR) &classBits, sizeof(classBits));
        
        ObjectMemory_classBitsOf_put(objectPointer, classBits);
        
        /* Followed by the fields... */
        for(wordIndex = 0; wordIndex < objectSize-HeaderSize; wordIndex++)
        {
            Read(filehandle, (APTR) &word, sizeof(word));
            /* use heap chunk */
            ObjectMemory_storeWord_ofObject_withValue(wordIndex, objectPointer, word);
        }
        
        destinationWord += space;
        
        heapSpaceRemaining[destinationSegment - FirstHeapSegment] -= space;
    }
    
    /* Initialize the free chunk lists for each heap segment with the sentinel */
    for(segment = FirstHeapSegment; segment <= LastHeapSegment; segment++)
    {
        for(size = HeaderSize; size <= BigSize; size++)
            ObjectMemory_resetFreeChunkList_inSegment(size, segment);
    }
    
    /* freeWords = 0; */
    
    /*
    // Place any remaining space in each segment onto it's free chunk list, which is
    // is a linked list of object pointers.
    // The chunks are linked using the class field of an object. The size field of
    // the object contains the actual size of the free chunk.*/
    for(segment = FirstHeapSegment; segment <= LastHeapSegment; segment++)
    {
        freeChunkSize = heapSpaceRemaining[segment - FirstHeapSegment];
        /*freeWords += freeChunkSize;*/
        if (freeChunkSize >= HeaderSize)
        {
            freeChunkLocation = SegmentHeapSpaceSize - freeChunkSize;
            /* G&R pg 665 - each free chunk has an OT entry */
            currentSegment = segment; /* Set special segment register */
            objectPointer = ObjectMemory_obtainPointer_location(freeChunkSize, freeChunkLocation);
            if( freeChunkSize > BigSize ) {
            	freeChunkSize = BigSize;
            }
            ObjectMemory_toFreeChunkList_add(freeChunkSize, objectPointer);
        }
    }
    
    currentSegment = FirstHeapSegment;
    
    free(heapSpaceRemaining);
    return YES;
}

Bool ObjectMemory_loadSnapshot(CONST_STRPTR filename) {
	BPTR snapshotfile = Open(filename, MODE_OLDFILE);
	Bool result = NO;
	if( snapshotfile != (BPTR)NULL ) {
		result = _loadObjectTable(snapshotfile) && _loadObjects(snapshotfile);
		Close(snapshotfile);
	}
	return result;
}


