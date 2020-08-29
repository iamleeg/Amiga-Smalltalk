#ifndef AMIGA_SMALLTALK_TESTS_H
#define AMIGA_SMALLTALK_TESTS_H

#include <stdio.h>

#include "ObjectMemory.h"

struct TestResult {
  int ran;
  int passed;
  int failed;
};

#define Test(t) void t (short *failures)

#define Expect(condition) do { \
  if(!(condition)) { \
    fprintf(stderr, "Expectation unsatisfied: " #condition "\n"); \
    *failures = *failures + 1; \
  } \
} while (0)

#define RunTest(t) do { \
  short failed = 0; \
  tr->ran++; \
  t(&failed); \
  if (failed == 0) { \
    tr->passed++; \
    /*printf("PASS " #t "\n");*/ \
  } else { \
    tr->failed++; \
    printf("FAIL " #t "\n"); \
  } \
} while(0)

/* If you think a test needs its own memory, use this. */
#define RunIsolatedTest(t) do { \
  ObjectMemory_delete(); \
  ObjectMemory_new(); \
  RunTest(t); \
  ObjectMemory_delete(); \
  ObjectMemory_new(); \
} while (0)

#define RunSuite(s) do { \
  Bool hasMemory = ObjectMemory_new(); \
  if (!hasMemory) { \
    fprintf(stderr, "Unable to allocate heap space for the VM\n"); \
    return 1; \
  } \
  s(&result); \
  ObjectMemory_delete(); \
} while (0)

#endif
