#include <proto/dos.h>
#include <dos/dos.h>
#include <exec/types.h>

BOOL write_header(BPTR filehandle, LONG objectSize, LONG tableSize);
BOOL write_interchange(BPTR filehandle);
BOOL save_snapshot(STRPTR name);

