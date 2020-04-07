#include <stdio.h>
#include "RealWordMemory.h"
#include "AllocationTests.h"
#include "FreeListTests.h"
#include "ObjectMemoryTests.h"
#include "RealWordMemoryTests.h"
#include "RefCountingTests.h"
#include "PrimitiveMethodTests.h"
#include "InterpreterMemoryTests.h"
#include "tests.h"

int main(int argc, const char *argv[]) {
  RealWordMemory_new();

  struct TestResult result;
  result.ran = 0;
  result.passed = 0;
  result.failed = 0;
  
  AllocationTests(&result);
  FreeListTests(&result);
  ObjectMemoryTests(&result);
  RealWordMemoryTests(&result);
  RefCountingTests(&result);
  PrimitiveMethodTests(&result);
  InterpreterMemoryTests(&result);
  
  printf("Tests completed.\n");
  printf("%d tests ran.\n", result.ran);
  printf("%d tests passed.\n", result.passed);
  printf("%d tests failed.\n", result.failed);
  
  return (result.failed != 0);
}
