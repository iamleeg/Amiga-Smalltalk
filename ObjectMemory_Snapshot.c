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

BOOL save_snapshot(STRPTR name) {
	BOOL result = TRUE;
	BPTR current_dir = CurrentDir( 0 );
	BPTR file = Open( name, MODE_NEWFILE );
	result = result && write_header( file, 0xDEADBEEF, 0xDEADBEEF ); /* placeholder */
	result = result && write_interchange( file );
	result = result && pad_to_512( file );
	
	LONG object_size = write_objects( file );
	
	result = result && (object_size >= 0);

	write_header( file, object_size, 0xCAFEBABE ); /* real values */
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
	interchange[0] = (UBYTE)0xAA;
	interchange[1] = (UBYTE)0xBB;
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
    printf("\n total size (%ld)\n", total_size);

	return total_size;
}

ULONG write_object(BPTR filehandle, ObjectPointer object) {
	WORDBITS size = ObjectMemory_sizeBitsOf( object );
	Write( filehandle, &size, sizeof(size) );
	
	UWORD classPointer = ObjectMemory_fetchClassOf( object );
	Write( filehandle, &classPointer, sizeof(classPointer) );
	
/*
//.  WRITE 16-bit sizeBitsOf
//   WRITE 16-bit fetchClassOf
//.  WRITE each 16-bit fetchWordOfObject
//    objectsLength += sizeBitsOf ?? (The should be same as 16-bit x 2 (header) + count of fetchWords
// objectLength goes inplaceholder[0]
*/

	return sizeof(size) + sizeof(classPointer);
}

int main(int argc, char **argv) {
    ObjectMemory_new();
	save_snapshot( "image.st" );
	ObjectMemory_delete();
	return 0;
}