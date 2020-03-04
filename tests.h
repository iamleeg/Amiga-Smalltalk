#ifndef AMIGA_SMALLTALK_TESTS_H
#define AMIGA_SMALLTALK_TESTS_H

#include <stdio.h>

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
    printf("PASS " #t "\n"); \
  } else { \
    tr->failed++; \
    printf("FAIL " #t "\n"); \
  } \
} while(0)

#endif