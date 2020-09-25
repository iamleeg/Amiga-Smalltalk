#include "Snapshot.h"
#include "Types.h"
#include "ObjectMemory.h"
#include <exec/types.h>
#include <proto/dos.h>
#include <dos/dos.h>

/*
 * -- SAVE -------------------------------------------------------------------------------
 */

/*
 * start at the current position and pad zeros up to a 512 word boundary
 */
Bool _padToPage(BPTR filehandle) {
	LONG pos = Seek(filehandle, 0, OFFSET_CURRENT);
	LONG desired = ((pos + 512 -1 ) / 512) * 512;
	LONG zero = 0L;
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

	
	for( anObject = 2; anObject < storedObjectTableLength; anObject += 2) {
		if(!ObjectMemory_hasObject(anObject)) continue;
		
		objectSize = (WORD)ObjectMemory_sizeBitsOf(anObject);
		header[0] = objectSize;
		header[1] = (WORD)ObjectMemory_fetchClassOf(anObject);
		if( Write(filehandle, (APTR)&header, sizeof(header)) != sizeof(header) ) {
			return 0;
		}
		wordLengthOfObject = ObjectMemory_fetchWordLengthOf(anObject);
		for(wordIndex = 0; wordIndex < wordLengthOfObject; wordIndex++) {
			word = ObjectMemory_fetchWord_ofObject( wordIndex, anObject);
			if( Write(filehandle, (APTR)&word, sizeof(word)) != sizeof(word) ) {
				return 0;
			}
		}
		objectSpaceLength += objectSize;
	}
	
	return objectSpaceLength;
}

Bool _writeObjectTable(BPTR filehandle, WORD storedObjectTableLength) {
    int objectImageWordAddress = 0;
    ObjectPointer objectPointer = NilPointer;
    WORD oldOTValue = 0;
    WORD oldOTLocation = 0;
    WORD objectSize = 0;
    WORD words[2] = {0};
    for( objectPointer = 0; objectPointer < storedObjectTableLength; objectPointer += 2)
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
        if (Write(filehandle, (APTR) &words, sizeof(words)) != sizeof(words))
            return NO;
    }
    return YES;
}

Bool _saveObjects(BPTR filehandle) {
	LONG objectSpaceLength = 0;
	LONG storedObjectTableLength = ObjectMemory_lastUsedObjectPointer() + 2;

	if(!_saveSnapshotHeader(filehandle)) {
		return NO;
	}
	
	if(!_padToPage(filehandle)) {
		return NO;
	}
	objectSpaceLength = _writeObjects(filehandle, storedObjectTableLength);
	if(objectSpaceLength == 0) {
		return NO;
	}	

	if(!_padToPage(filehandle)) {
		return NO;
	}
	
	if(!_writeObjectTable(filehandle, storedObjectTableLength)) {
		return NO;
	}
    
    /* Now we can go back fill in the values for the image header. */
    Seek(filehandle, 0, OFFSET_BEGINNING);
    Write( filehandle, (APTR)&objectSpaceLength, sizeof(objectSpaceLength) );
    Write( filehandle, (APTR)&storedObjectTableLength, sizeof(storedObjectTableLength) );
	
	return YES;
}

Bool ObjectMemory_saveSnapshot(CONST_STRPTR filename) {
	BPTR snapshotFile = Open(filename, MODE_READWRITE);
	Bool result = NO;
	if( snapshotFile != NULL ) {;
		result = _saveObjects(snapshotFile);
		Close( snapshotFile );
	}
	return result;
}




/*
 * -- LOAD -------------------------------------------------------------------------------
 */
Bool ObjectMemory_loadObjectTable(BPTR filehandle) {
    /* First two 32-bit values have the object space length and object table lengths in words */
    LONG objectTableLength = -1;
    LONG fileSize = -1;
    ObjectPointer objectPointer = NilPointer;
    
    if (Seek(filehandle, 4, OFFSET_BEGINNING) == -1) /* Skip over object space length */
        return NO;
        
    if (Read(filehandle, (APTR)&objectTableLength, sizeof(objectTableLength)) != sizeof(objectTableLength))
        return NO;
            
    if (Seek(filehandle, objectTableLength * -2, OFFSET_END) == -1) /* Reposition to start of object table */
        return NO;

    for(objectPointer = 0; objectPointer < objectTableLength; objectPointer+=2)
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
    for(objectPointer = ObjectTableSize-2; objectPointer >= 2; objectPointer -= 2)
        if (ObjectMemory_freeBitOf(objectPointer))
            ObjectMemory_toFreePointerListAdd(objectPointer);
    
    
    return YES;
}

Bool ObjectMemory_loadObjects(BPTR filehandle) {
#ifdef NOPE
   static const int SegmentHeapSpaceSize = HeapSpaceStop + 1;
    
    // Track amount of free space available for objects in each segment
    int heapSpaceRemaining[HeapSegmentCount];
    
    for(int segment = FirstHeapSegment; segment <= LastHeapSegment; segment++)
        heapSpaceRemaining[segment - FirstHeapSegment] = SegmentHeapSpaceSize;
    
    // Load objects from the virtual image into the heap segments
    // being careful to not split an object across a segment boundary
    int destinationSegment = FirstHeapSegment, destinationWord = 0;
    
    for(int objectPointer = 2; objectPointer < ObjectTableSize; objectPointer += 2)
    {
        if (freeBitOf(objectPointer)) continue;
        // A free chunk has it's COUNT field set to zero but the free bit is clear
        assert (countBitsOf(objectPointer) != 0); // SANITY Make sure a freeChunk wasn't saved!
        
        // On disk objects are stored contiguously as if a large 20-bit WORD addressed space
        // In this scheme, the OT segment and locations combine to form a WORD address
        const int objectImageWordAddress = (segmentBitsOf(objectPointer) << 16)
        + locationBitsOf(objectPointer);
        
        fileSystem->seek_to(fd, ObjectSpaceBaseInImage + objectImageWordAddress * sizeof(std::uint16_t));
        
        std::uint16_t objectSize;
        fileSystem->read(fd, (char *) &objectSize, sizeof(objectSize));
        
        
        // Account for the extra word used by HugeSize objects
        int extraSpace = objectSize < HugeSize || pointerBitOf(objectPointer) == 0 ? 0 : 1;
        int space = objectSize + extraSpace; // space in memory
        
        if (space > heapSpaceRemaining[destinationSegment - FirstHeapSegment])
        {
            // No room left in the current segment, move to next
            destinationSegment++;
            if (destinationSegment == HeapSegmentCount) return NO; // Full
            destinationWord = 0;
        }
        
        // Update OT entry so that it references the object location in ObjectMemory vs the disk image
        segmentBitsOf_put(objectPointer, destinationSegment);
        locationBitsOf_put(objectPointer, destinationWord);
        
        // Store the object in the image into word memory
        // First comes the size...
        
        sizeBitsOf_put(objectPointer, objectSize );
        
        // Next is the class...
        std::uint16_t classBits;
        fileSystem->read(fd,(char *) &classBits, sizeof(classBits));
        
        classBitsOf_put(objectPointer, classBits);
        
        // Followed by the fields...
        for(int wordIndex = 0; wordIndex < objectSize-HeaderSize; wordIndex++)
        {
            std::uint16_t word;
            fileSystem->read(fd,(char *) &word, sizeof(word));
            // use heap chunk
            storeWord_ofObject_withValue(wordIndex, objectPointer, word);
        }
        
        destinationWord += space;
        
        heapSpaceRemaining[destinationSegment - FirstHeapSegment] -= space;
    }
    
    // Initialize the free chunk lists for each heap segment with the sentinel
    for(int segment = FirstHeapSegment; segment <= LastHeapSegment; segment++)
    {
        for(int size = HeaderSize; size <= BigSize; size++)
            resetFreeChunkList_inSegment(size, segment);
    }
    
    /*
    //freeWords = 0;
    // Place any remaining space in each segment onto it's free chunk list, which is
    // is a linked list of object pointers.
    // The chunks are linked using the class field of an object. The size field of
    // the object contains the actual size of the free chunk.*/
    for(int segment = FirstHeapSegment; segment <= LastHeapSegment; segment++)
    {
        int freeChunkSize = heapSpaceRemaining[segment - FirstHeapSegment];
        freeWords += freeChunkSize;
        if (freeChunkSize >= HeaderSize)
        {
            int freeChunkLocation = SegmentHeapSpaceSize - freeChunkSize;
            // G&R pg 665 - each free chunk has an OT entry
            currentSegment = segment; // Set special segment register
            int objectPointer = obtainPointer_location(freeChunkSize, freeChunkLocation);
            toFreeChunkList_add(std::min(freeChunkSize, (int) BigSize), objectPointer);
        }
    }
    
    currentSegment = FirstHeapSegment;
#endif
    return YES;
}

Bool ObjectMemory_loadSnapshot(CONST_STRPTR filename) {
	BPTR snapshotfile = Open(filename, MODE_OLDFILE);
	Bool result = NO;
	if( snapshotfile != NULL ) {
		result = ObjectMemory_loadObjectTable(snapshotfile) && ObjectMemory_loadObjects(snapshotfile);
		Close(snapshotfile);
	}
	return result;
}


