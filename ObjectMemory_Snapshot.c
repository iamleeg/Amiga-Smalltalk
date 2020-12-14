#include <proto/dos.h>
#include <proto/exec.h>

#include "ObjectMemory_Snapshot.h"
#include "ObjectMemory.h"

#include <stdio.h>


BOOL write_placeholder(BPTR filehandle);
BOOL write_interchange(BPTR filehandle);
BOOL pad_to_512(BPTR filehandle);
LONG write_objects(BPTR filehandle);
ULONG write_object(BPTR filehandle, ObjectPointer objectPointer);
LONG write_object_table(BPTR filehandle);
void write_object_table_entry(BPTR filehandle, ObjectPointer objectPointer, ULONG objectImageWordAddress);
ObjectPointer last_used_objectpointer(void);

BOOL save_snapshot(STRPTR name) {
	BOOL result = TRUE;
	BPTR current_dir = CurrentDir( 0 );
	BPTR file = Open( name, MODE_NEWFILE );
	
	result = result && write_header( file, 0xDEADBEEF, 0xDEADBEEF ); /* placeholder */
	result = result && write_interchange( file );

	result = result && pad_to_512( file );
	
	LONG object_size = write_objects( file );
	result = result && (object_size >= 0);

	result = result && pad_to_512( file );

	LONG object_table_size = write_object_table( file );
	result = result && (object_table_size >= 0);
	
	write_header( file, object_size, object_table_size ); /* real values */
	
	Close( file );
	UnLock( current_dir );
	return result;
}

BOOL write_header(BPTR filehandle, LONG objectSize, LONG tableSize) {
	LONG header[2];
	header[0] = objectSize;
	header[1] = tableSize;
	ULONG oldpos = Seek( filehandle, 0, OFFSET_CURRENT );
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
	// get current position
	ULONG pos = Seek( filehandle, 0, OFFSET_CURRENT );
	// desired  = ((pos + 512 - 1) / 512) * 512;
	ULONG desired = ((pos + 512 - 1) / 512) * 512;
	// count desired - current_pos
	ULONG count = desired - pos;
	// write $COUNT zerobytes
	APTR bytes = (APTR) AllocMem(count, MEMF_CLEAR);	
	Write( filehandle, bytes, count );
	FreeMem( bytes, count );
	return TRUE;
}

LONG write_objects(BPTR filehandle) {
	ObjectPointer iterator = NilPointer;
	
	// FOR EACH OBJECT
	LONG total_size = 0;
    printf("\n\n");
	for(iterator = 2; iterator < ObjectTableSize; iterator += 2) {
		if( ObjectMemory_hasObject( iterator ) ) {
			printf("%ul ", iterator);
			total_size += write_object( filehandle, iterator );
		}
    }
    printf("\n total size (%ld) in WORDS\n", total_size);

	return total_size;
}

ObjectPointer last_used_objectpointer(void) {
	ObjectPointer lastUsedObjectPointer = NilPointer;
    for(int objectPointer = 2; objectPointer < ObjectTableSize; objectPointer += 2)
    {
         if( ObjectMemory_hasObject( objectPointer ) )
            lastUsedObjectPointer = objectPointer;
    }
    return lastUsedObjectPointer;
}
    

ULONG write_object(BPTR filehandle, ObjectPointer objectPointer) {
     
	WORD size = ObjectMemory_sizeBitsOf( objectPointer );
	Write( filehandle, &size, sizeof(size) );
	
	UWORD classPointer = ObjectMemory_fetchClassOf( objectPointer );
	Write( filehandle, &classPointer, sizeof(classPointer) );
	
	UWORD wordLengthOfObject = ObjectMemory_fetchWordLengthOf(objectPointer);
	for(UWORD wordIndex = 0; wordIndex < wordLengthOfObject; wordIndex++ )
	{
		UWORD word = (UWORD)ObjectMemory_fetchWord_ofObject(wordIndex, objectPointer);
		Write( filehandle, &word, sizeof(word) );
	}
	
	/* return the number of WORDs written (not bytes) */
	return size;
}

LONG write_object_table(BPTR filehandle) {
	ULONG objectImageWordAddress = 0;
	ObjectPointer iterator = NilPointer;
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
	
	if( objectPointer >= 2) {
		if (!ObjectMemory_freeBitOf(objectPointer) && ObjectMemory_countBitsOf(objectPointer) == 0) {
			/* This entry was for a free chunk of memory, but we don't save free
			   space in the image. Store as a an available OT entry by setting free bit */
			ObjectMemory_freeBitOf_put(objectPointer, 1);
        }
        
        if (ObjectMemory_freeBitOf(objectPointer)) {
			/* manual.pdf - page 3: free entries have freeBit set and other bits 
			   in both words are 0. */
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
	UWORD words[2];
	words[0] = ObjectMemory_ot(objectPointer);
	words[1] = ObjectMemory_locationBitsOf(objectPointer);
	
	Write( filehandle, words, sizeof(words) );

	// Restore OT entry
	ObjectMemory_ot_put(objectPointer, old_ot_value);
	ObjectMemory_locationBitsOf_put(objectPointer, old_ot_location);
}

int main(int argc, char **argv) {
    ObjectMemory_new();
	save_snapshot( "image.st" );
	ObjectMemory_delete();
	return 0;
}