#include <stdio.h>

#include "AllocationTests.h"
#include "FreeListTests.h"
#include "ObjectMemoryTests.h"
#include "RealWordMemoryTests.h"
#include "RefCountingTests.h"
#include "PrimitiveMethodTests.h"
#include "InterpreterMemoryTests.h"
#include "Interpreter_IntegerPrimTests.h"
#include "Interpreter_IntegerArithTests.h"
#include "Interpreter_FloatArithTests.h"
#include "Interpreter_IntegerCmpTests.h"
#include "CompiledMethodTests.h"
#include "ContextTests.h"
#include "ClassTests.h"

#include "Snapshot.h"

#include "tests.h"

int main(int argc, const char *argv[]) {
  Bool snapresult = 126;
  struct TestResult result;
    ObjectPointer anObject;
  	if (!ObjectMemory_new()) { 
    	fprintf(stderr, "Unable to allocate heap space for the VM\n"); 
    	return 1; 
  	}  else {
    	fprintf(stderr, "ObjectMempry Allocatied\n"); 
  	}
  
  printf("LAST OBJECT1 = %u", ObjectMemory_lastUsedObjectPointer());
  /* create a simple object */
  anObject = ObjectMemory_instantiateClass_withBytes(ClassLargePositiveIntegerPointer, 2);
  printf("STORE1  %lu\n", anObject);
  ObjectMemory_storeByte_ofObject_withValue(0, anObject, Interpreter_lowByteOf(17000));
  printf("STORE2\n");
  ObjectMemory_storeByte_ofObject_withValue(1, anObject, Interpreter_highByteOf(17000));
  
  ObjectMemory_increaseReferencesTo(anObject);
  
  printf("HAS %d\n", ObjectMemory_hasObject(anObject) );
  printf("FREE(0) == %d", ObjectMemory_freeBitOf(anObject) == 0 );
  printf("COUNT(!0) == %d", ObjectMemory_countBitsOf(anObject) != 0 );

  result.ran = 0;
  result.passed = 0;
  result.failed = 0;
  
  printf("S4\n");
  
/*  RunSuite(AllocationTests);
  RunSuite(FreeListTests);
  RunSuite(ObjectMemoryTests);
  RunSuite(RealWordMemoryTests);
  RunSuite(RefCountingTests);
  RunSuite(PrimitiveMethodTests);
  RunSuite(InterpreterMemoryTests);
  RunSuite(InterpreterIntegerPrimitiveArithmeticTests);
  RunSuite(InterpreterFloatPrimitiveArithmeticTests);
  RunSuite(InterpreterIntegerPrimitiveComparisonTests);
  RunSuite(InterpreterIntegerPrimitiveTests);
  RunSuite(CompiledMethodTests);
  RunSuite(ContextTests);
  RunSuite(ClassTests);*/
  
  
  printf("LAST OBJECT2 = %u", ObjectMemory_lastUsedObjectPointer());
  printf("HELLO");
  snapresult = ObjectMemory_saveSnapshot("SYS:snapshot.img");
  printf("SNAPSHOT %u\n", snapresult);
  
/*  printf("Tests completed.\n");
  printf("%d tests ran.\n", result.ran);
  printf("%d tests passed.\n", result.passed);
  printf("%d tests failed.\n", result.failed);
  
  return (result.failed != 0);*/
  return snapresult;
}
