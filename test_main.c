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
  struct TestResult result;
  Bool snapresult = NO;

  result.ran = 0;
  result.passed = 0;
  result.failed = 0;
  
  RunSuite(AllocationTests);
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
  RunSuite(ClassTests);
  
  printf("LAST OBJECT = %u", ObjectMemory_lastUsedObjectPointer());
  snapresult = ObjectMemory_saveSnapshot("SYS:snapshot.img");
  printf("SNAPSHOT %u\n", snapresult);
  
  printf("Tests completed.\n");
  printf("%d tests ran.\n", result.ran);
  printf("%d tests passed.\n", result.passed);
  printf("%d tests failed.\n", result.failed);
  
  return (result.failed != 0);
}
