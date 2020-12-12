#include <proto/dos.h>
#include <proto/exec.h>
#include <clib/macros.h>

#include "ObjectMemory_Snapshot.h"
#include "ObjectMemory.h"
#include "RealWordMemory.h"

#include <stdio.h>
#include <assert.h>

// we write our header and then pad to 512 to start with Objects
#define StartOfImageObjectSpace 512L

/* SAVING PROTOS */
BOOL write_header(BPTR filehandle, LONG objectSize, LONG tableSize);
BOOL write_interchange(BPTR filehandle);
BOOL pad_to_512(BPTR filehandle);
LONG write_objects(BPTR filehandle);
ULONG write_object(BPTR filehandle, ObjectPointer objectPointer);
LONG write_object_table(BPTR filehandle);
void write_object_table_entry(BPTR filehandle, ObjectPointer objectPointer, ULONG objectImageWordAddress);
ObjectPointer last_used_objectpointer(void);

/* LOADING PROTOS */
BOOL load_object_table(BPTR file);
BOOL load_objects(BPTR file);

/*
 *   S A V I N G -------------------------------------------------------------------------
 */
BOOL save_snapshot(CONST_STRPTR name) {
	BOOL result = TRUE;
	BPTR current_dir = CurrentDir( 0 );
	BPTR file = Open( name, MODE_NEWFILE );
	LONG object_size = 0L;
	LONG object_table_size = 0L;
	
	LONG object_table_start_pos = 0L;
	LONG object_start_pos = 0L;
	
	result = result && write_header( file, 0xDEADBEEF, 0xDEADBEEF ); /* placeholder */
	result = result && write_interchange( file );

	result = result && pad_to_512( file );
	
	object_start_pos = Seek( file, 0, OFFSET_CURRENT );

	object_size = write_objects( file );
	result = result && (object_size >= 0);

	result = result && pad_to_512( file );

	object_table_start_pos = Seek( file, 0, OFFSET_CURRENT );
	object_table_size = write_object_table( file );
	result = result && (object_table_size >= 0);
	
	write_header( file, object_size, object_table_size ); /* real values */
	
	Close( file );
	UnLock( current_dir );
	return result;
}

BOOL write_header(BPTR filehandle, LONG objectSize, LONG tableSize) {
	LONG header[2];
	ULONG oldpos = 0L;

	header[0] = objectSize;
	header[1] = tableSize;
	oldpos = Seek( filehandle, 0, OFFSET_CURRENT );
	Seek( filehandle, 0, OFFSET_BEGINNING );
	Write( filehandle, header, sizeof(header) );
	if( oldpos > 0 ) {
		Seek( filehandle, oldpos, OFFSET_BEGINNING );
	}
	return TRUE;
}

BOOL write_interchange(BPTR filehandle) {
	UBYTE interchange[2];
	interchange[0] = (UBYTE)0xAA; /* should eventually be 0 but easier to see*/
	interchange[1] = (UBYTE)0xBB; /* should eventualty be 0 but easier to see*/
	Write( filehandle, interchange, sizeof(interchange) );
	return TRUE;	
}

BOOL pad_to_512(BPTR filehandle) {
	ULONG pos = 0L;
	ULONG desired = 0L;
	ULONG count = 0L;
	APTR bytes = NULL;
	
	/* get current position */
	pos = Seek( filehandle, 0, OFFSET_CURRENT );
	/* desired  = ((pos + 512 - 1) / 512) * 512; */
	desired = ((pos + 512 - 1) / 512) * 512;
	/* count desired - current_pos */
	count = desired - pos;
	/* write $COUNT zerobytes */
	bytes = (APTR) AllocMem(count, MEMF_CLEAR);	
	Write( filehandle, bytes, count );
	FreeMem( bytes, count );
	return TRUE;
}

LONG write_objects(BPTR filehandle) {
	ObjectPointer iterator = NonPointer;
	
	/* FOR EACH OBJECT */
	LONG total_size = 0;
	for(iterator = 2; iterator < ObjectTableSize; iterator += 2) {
		if( ObjectMemory_hasObject( iterator ) ) {
			total_size += write_object( filehandle, iterator );
		}
    }

	return total_size;
}

ObjectPointer last_used_objectpointer(void) {
	ObjectPointer lastUsedObjectPointer = NonPointer;
	ObjectPointer iterator = NonPointer;
    for(iterator = 2; iterator < ObjectTableSize; iterator += 2)
    {
         if( ObjectMemory_hasObject( iterator ) )
            lastUsedObjectPointer = iterator;
    }
    return lastUsedObjectPointer;
}
    

ULONG write_object(BPTR filehandle, ObjectPointer objectPointer) {
     
	WORD size = 0;
	UWORD classPointer = 0;
	UWORD wordLengthOfObject = 0;
	UWORD wordIndex = 0;
	
	size = ObjectMemory_sizeBitsOf( objectPointer );
	Write( filehandle, &size, sizeof(size) );
	
	classPointer = ObjectMemory_fetchClassOf( objectPointer );
	Write( filehandle, &classPointer, sizeof(classPointer) );
	
	wordLengthOfObject = ObjectMemory_fetchWordLengthOf(objectPointer);
	for( wordIndex = 0; wordIndex < wordLengthOfObject; wordIndex++ )
	{
		UWORD word = (UWORD)ObjectMemory_fetchWord_ofObject(wordIndex, objectPointer);
		Write( filehandle, &word, sizeof(word) );
	}
	printf("written object %u with %u WORDS\n", objectPointer, wordLengthOfObject);
	
	/* return the number of WORDs written (not bytes) */
	return size;
}

LONG write_object_table(BPTR filehandle) {
	ULONG objectImageWordAddress = 0;
	ObjectPointer iterator = NonPointer;
	UWORD storedObjectTableLength = (UWORD)last_used_objectpointer() + 2; 
	for( iterator = 0; iterator < storedObjectTableLength; iterator +=2 ) {
		write_object_table_entry( filehandle, iterator, objectImageWordAddress );
		objectImageWordAddress += ObjectMemory_sizeBitsOf(iterator);
	}
	
	return (LONG)storedObjectTableLength;
}

void write_object_table_entry( BPTR filehandle, ObjectPointer objectPointer, ULONG objectImageWordAddress ) {
	UWORD old_ot_value = ObjectMemory_ot(objectPointer);
	UWORD old_ot_location = ObjectMemory_locationBitsOf(objectPointer);
	UWORD tableentry[2];
	
	if( objectPointer >= 2) {
		if (!ObjectMemory_freeBitOf(objectPointer) && ObjectMemory_countBitsOf(objectPointer) == 0) {
			/* This entry was for a free chunk of memory, but we don't save free
			   space in the image. Store as a an available OT entry by setting free bit */
			ObjectMemory_freeBitOf_put(objectPointer, 1);
        }
        
        if (ObjectMemory_freeBitOf(objectPointer)) {
			/* http://www.wolczko.com/st80/ (booklet) - page 3: 
			   free entries have freeBit set and other bits in both words are 0. */
			ObjectMemory_ot_put(objectPointer, 0);
			ObjectMemory_freeBitOf_put(objectPointer, 1);
			ObjectMemory_locationBitsOf_put(objectPointer, 0);
        } else {
			/* Modify the location of the object table entry... we do this once 
			   we no longer process this object table entry */
			ObjectMemory_segmentBitsOf_put(objectPointer, objectImageWordAddress >> 16);
			ObjectMemory_locationBitsOf_put(objectPointer, objectImageWordAddress & 0xffff);
		}
	}
	
	/* Assemble object table entry record */
	tableentry[0] = ObjectMemory_ot(objectPointer);
	tableentry[1] = ObjectMemory_locationBitsOf(objectPointer);
	
	Write( filehandle, tableentry, sizeof(tableentry) );

	/* Restore OT entry */
	ObjectMemory_ot_put(objectPointer, old_ot_value);
	ObjectMemory_locationBitsOf_put(objectPointer, old_ot_location);
}

/*
 * L O A D I N G -------------------------------------------------------------------------
 */
BOOL load_snapshot(CONST_STRPTR name) { 
	BOOL result = TRUE;
	BPTR current_dir = CurrentDir( 0 );
	BPTR file = Open( name, MODE_OLDFILE );

	result = result && load_object_table( file );
	result = result && load_objects( file );
	
	Close( file );
    return result;
}

BOOL load_object_table(BPTR file) {
    /* First two 32-bit values have the object space length and object table lengths in words */
    LONG objectTableLengthInWords = 0L;
    LONG objectTableLengthInBytes = 0L;
    ObjectPointer iterator = NonPointer;
    UWORD tableentry[2] = {0,0};
    LONG objectTableStartPosition = 0L;

    
    Seek( file, 4, OFFSET_BEGINNING);
    Read( file, &objectTableLengthInWords, sizeof(objectTableLengthInWords) );
    objectTableLengthInBytes = objectTableLengthInWords*2;
        
    Seek( file, (objectTableLengthInBytes * -1), OFFSET_END );
    objectTableStartPosition = Seek( file, 0, OFFSET_CURRENT );
	
	    
    for(iterator = 0; iterator < objectTableLengthInWords; iterator+=2) {
        Read(file, &tableentry, sizeof(tableentry));
        ObjectMemory_ot_put(iterator, tableentry[0]);
        ObjectMemory_locationBitsOf_put(iterator, tableentry[1]);
    }

    ObjectMemory_headOfFreePointerList_put(NilPointer);
    
    /* Initialize the remaining entries as free */
    for(iterator = objectTableLengthInWords; iterator < ObjectTableSize; iterator += 2) {
        ObjectMemory_ot_put(iterator, 0);
        ObjectMemory_freeBitOf_put(iterator, 1);
        ObjectMemory_locationBitsOf_put(iterator, 0);
    }
    
    /* Build the OT entry free list, Go backwards so we have lower entries first on free list.
       Why? Makes the OT easier to see in debugger.
       Note we skip oop 0, which is considered reserved and invalid. (page 2, Xerox Virtual Image booklet) */
    for(iterator = 2; iterator < ObjectTableSize; iterator += 2) {
        if (ObjectMemory_freeBitOf(iterator) == 1) {
            ObjectMemory_toFreePointerListAdd(iterator);
        }
    }
    
	return TRUE;
}

BOOL load_objects(BPTR file) {
	LONG destinationSegment = FirstHeapSegment;
	UWORD destinationWord = 0;
/* track how much remains in each segment */
	static LONG SegmentHeapSpaceSize = HeapSpaceStop + 1;
	LONG heapSpaceRemaining[HeapSegmentCount];
	
	for(LONG segment = FirstHeapSegment; segment <= LastHeapSegment; segment++) {
		heapSpaceRemaining[segment-FirstHeapSegment] = SegmentHeapSpaceSize;
	}
	

	for( ObjectPointer iterator = 2; iterator < ObjectTableSize; iterator += 2 ) {
		// we set the freebit in load_object_table 
		// check we are not loading up a free chunk
		if( ObjectMemory_freeBitOf(iterator) ) continue;
		//assert( ObjectMemory_countBitsOf(iterator) != 0 );
				
		/* see write_object_table_entry.  This is like the location(in words) in the object section of thee file */
		UWORD objectImageWordAddress = (ObjectMemory_segmentBitsOf(iterator) << 16) + ObjectMemory_locationBitsOf(iterator);
		UWORD objectImageByteAddress = objectImageWordAddress * 2;
		Seek( file, StartOfImageObjectSpace + objectImageByteAddress, OFFSET_BEGINNING );

		/* OK we are at the right place in the file */
		/* how many words to read? */
		UWORD objectSize = 0;
		Read( file, &objectSize, sizeof(objectSize) );
		
		/* Account for the extra word used by HugeSize objects */
        UWORD extraSpace = objectSize < HugeSize || ObjectMemory_pointerBitOf(iterator) == 0 ? 0 : 1;
        UWORD memorySpace = objectSize + extraSpace; 
        
        /* check if need to start a new segment */
        if (memorySpace > heapSpaceRemaining[destinationSegment - FirstHeapSegment]) {
            // No room left in the current segment, move to next
            destinationSegment++;
            if (destinationSegment == HeapSegmentCount) return FALSE; /* Full */
            destinationWord = 0;
        }
        
        /* WARNING THIS SEEMS TO CORRUPT THE SIZE BITS OF OOP N-1 */
        
         /* Update OT entry so that it references the object location in ObjectMemory vs the disk image */
        ObjectMemory_segmentBitsOf_put(iterator, destinationSegment);
        ObjectMemory_locationBitsOf_put(iterator, destinationWord);
        printf("LOADED Object %u, length %d, stored at segment %ld, word %d\n", 
        	iterator, objectSize-HeaderSize, destinationSegment, destinationWord);
		
		/* Store the object in the image into word memory
           First comes the size... */
        ObjectMemory_sizeBitsOf_put(iterator, objectSize );
        
        /* Next is the class... */
        UWORD classBits = 0;
        Read( file, &classBits, sizeof(classBits) );
        ObjectMemory_classBitsOf_put(iterator, classBits);

        /* Now the fields */
        // Followed by the fields...
        for(UWORD wordIndex = 0; wordIndex < objectSize-HeaderSize; wordIndex++) {
            UWORD word = 0;
            Read( file, &word, sizeof(word) );
	        ObjectMemory_storeWord_ofObject_withValue(wordIndex, iterator, word);
        }
        
        destinationWord += memorySpace;
        
        heapSpaceRemaining[destinationSegment - FirstHeapSegment] -= memorySpace;  
	}
	
	/* Initialize the free chunk lists for each heap segment with the sentinel */
    for(LONG segment = FirstHeapSegment; segment <= LastHeapSegment; segment++)
    {
        for(LONG size = HeaderSize; size <= BigSize; size++) {
            ObjectMemory_resetFreeChunkList_inSegment(size, segment);
        }
    }
    
    /* Place any remaining space in each segment onto it's free chunk list, which is
       is a linked list of object pointers.
       The chunks are linked using the class field of an object. The size field of
       the object contains the actual size of the free chunk. */
       
    /* WARNING THIS SEEMS TO CORRUPT OOP50 AND GIVE IT A HUGE WORD LENGTH */
    for(LONG segment = FirstHeapSegment; segment <= LastHeapSegment; segment++)
    {
        LONG freeChunkSize = heapSpaceRemaining[segment - FirstHeapSegment];
        if (freeChunkSize >= HeaderSize)
        {
            LONG freeChunkLocation = SegmentHeapSpaceSize - freeChunkSize;
            // G&R pg 665 - each free chunk has an OT entry
            currentSegment = segment; // Set special segment register
            ObjectPointer objectPointer = ObjectMemory_obtainPointer_location(freeChunkSize, freeChunkLocation);
            ObjectMemory_toFreeChunkList_add(MIN(freeChunkSize, (LONG) BigSize), objectPointer);
        }
    }
    
    currentSegment = FirstHeapSegment;
	
	return TRUE;
}

/*
 * M A I N -------------------------------------------------------------------------------
 */
int main(int argc, char **argv) {
    ObjectMemory_new();
    load_snapshot( "SYS:image80.st" );
    save_snapshot( "SYS:image80-2.st");
	ObjectMemory_delete();
	
	return 0;
}