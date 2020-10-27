#include <stdio.h>
#include "ObjectMemory.h"
#include "Snapshot.h"


int main(WORD argc, STRPTR argv[]) {
	ObjectPointer anObject = NonPointer;

	if (!ObjectMemory_new()) { 
    	fprintf(stderr, "Unable to allocate heap space for the VM\n"); 
    	return 1; 
  	}  else {
    	fprintf(stderr, "ObjectMemory Allocated\n"); 
  	}
  	
  	if( argc < 3 ) {
  		fprintf(stderr, "usage:  ast load <imagefile>\n");
  		fprintf(stderr, "        ast save <imagefile>\n");
  		exit(1);
  	}
  	
  	if( strcmp( argv[1], "load"  ) == 0 ) {
  		printf("LETS LOAD %s\n", argv[2]);
  		
  		ObjectMemory_loadSnapshot(argv[2]);
  		
  	} else if( strcmp( argv[1], "save"  ) == 0 ) {  	
  		printf("LETS SAVE %s\n", argv[2]);
		anObject = ObjectMemory_instantiateClass_withBytes(ClassLargePositiveIntegerPointer, 2);
		ObjectMemory_storeByte_ofObject_withValue(0, anObject, Interpreter_lowByteOf(17000));
		ObjectMemory_storeByte_ofObject_withValue(1, anObject, Interpreter_highByteOf(17000));
		ObjectMemory_increaseReferencesTo(anObject);
		
		ObjectMemory_saveSnapshot(argv[2]);
		
  	} else {
  		fprintf(stderr, "unknown command %s\n", argv[1] );
  		exit(1);
  	}
  	
  	ObjectMemory_delete();
	exit(0);
	
}
