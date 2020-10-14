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
  
  /* create a simple object */
 /* anObject = ObjectMemory_instantiateClass_withBytes(ClassLargePositiveIntegerPointer, 2);
  ObjectMemory_storeByte_ofObject_withValue(0, anObject, Interpreter_lowByteOf(17000));
  ObjectMemory_storeByte_ofObject_withValue(1, anObject, Interpreter_highByteOf(17000));
  ObjectMemory_increaseReferencesTo(anObject);*/
  

  result.ran = 0;
  result.passed = 0;
  result.failed = 0;
    
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
  
 /* 
  snapresult = ObjectMemory_saveSnapshot("SYS:snapshot.img");
  printf("SNAPSHOT %u\n", snapresult);
  */
  
  
  
  printf("LAST OBJECT1 %lu", ObjectMemory_lastUsedObjectPointer());
  snapresult = ObjectMemory_loadSnapshot("SYS:snapshot.img");
  printf("SNAPSHOT %u\n", snapresult);
  printf("LAST OBJECT2 %lu", ObjectMemory_lastUsedObjectPointer()); 
  
  
  
/*  printf("Tests completed.\n");
  printf("%d tests ran.\n", result.ran);
  printf("%d tests passed.\n", result.passed);
  printf("%d tests failed.\n", result.failed);
  
  return (result.failed != 0);*/
  return !snapresult;
}
