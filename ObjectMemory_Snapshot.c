#include <proto/dos.h>
#include <dos/dos.h>

#include "ObjectMemory_Snapshot.h"


BOOL save_snapshot(STRPTR name) {
	BOOL result = TRUE;
	BPTR current_dir = CurrentDir( 0 );
	BPTR file = Open( name, MODE_NEWFILE );
	result = result && write_placeholder( file );
	result = result && write_interchange( file );
	Close( file );
	UnLock( current_dir );
	return result;
}

BOOL write_placeholder(BPTR filehandle) {
	LONG header[2];
	header[0] = (LONG)0xDEADBEEF;
	header[1] = (LONG)0xDEADBEEF;
	Write( filehandle, header, sizeof(header) );
	return TRUE;
}

BOOL write_interchange(BPTR filehandle) {
	BYTE interchange[2];
	interchange[0] = (BYTE)0x00;
	interchange[1] = (BYTE)0x00;
	Write( filehandle, interchange, sizeof(interchange) );
	return TRUE;
	
}


int main(int argc, char **argv) {
	save_snapshot( "image.st" );
	return 0;
}