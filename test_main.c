#include <stdio.h>

#include "AllocationTests.h"
#include "FreeListTests.h"
#include "ObjectMemoryTests.h"
#include "RealWordMemoryTests.h"
#include "RefCountingTests.h"
#include "PrimitiveMethodTests.h"
#include "InterpreterMemoryTests.h"
#include "Interpreter_PrimArithTests.h"
#include "CompiledMethodTests.h"
#include "ContextTests.h"
#include "ClassTests.h"

#include "tests.h"

int main(int argc, const char *argv[]) {
  struct TestResult result;

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
  RunSuite(InterpreterArithmeticPrimitiveTests);
  RunSuite(CompiledMethodTests);
  RunSuite(ContextTests);
  RunSuite(ClassTests);
  
  printf("Tests completed.\n");
  printf("%d tests ran.\n", result.ran);
  printf("%d tests passed.\n", result.passed);
  printf("%d tests failed.\n", result.failed);
  
  return (result.failed != 0);
}
