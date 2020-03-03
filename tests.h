#ifndef AMIGA_SMALLTALK_TESTS_H
#define AMIGA_SMALLTALK_TESTS_H

#include <stdio.h>

struct TestResult {
  int ran;
  int passed;
  int failed;
};

#define RunTest(t) do { \
  short result = 0; \
  tr->ran++; \
  result = t(); \
  if (result) { \
    tr->passed++; \
    printf("PASS " #t "\n"); \
  } else { \
    tr->failed++; \
    printf("FAIL " #t "\n"); \
  } \
} while(0)

#endif