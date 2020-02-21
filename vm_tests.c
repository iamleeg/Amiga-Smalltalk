#include <stdio.h>
#include "ObjectMemory.h"

struct TestResult {
  int ran;
  int passed;
  int failed;
};


short test_NonIntegerObjectIsNotIntegerObject() {
  short result = ObjectMemory_isIntegerObject(0);
  return !result;
}

short test_IntegerObjectIsIntegerObject() {
  short result = ObjectMemory_isIntegerObject(1);
  return result;
}

short test_ObjectTableLookupFindsWordInRealWordMemory() {
  ObjectPointer objectPointer = 0x2468;
  short value = 0xbeef, result = 0;
  RealWordMemory_segment_word_put(ObjectTableSegment,
    ObjectTableStart + objectPointer,
    value);
  result = ObjectMemory_ot(objectPointer);
  return (result == value);
}

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

void ObjectMemoryTests(struct TestResult *tr) {
  RunTest(test_NonIntegerObjectIsNotIntegerObject);
  RunTest(test_IntegerObjectIsIntegerObject);
  RunTest(test_ObjectTableLookupFindsWordInRealWordMemory);
}

short test_PutAndRetrieveWordInSegment() {
  short s = 3;
  short w = 7;
  short value = 0x1234;
  short retrieved = 0;
  
  RealWordMemory_segment_word_put(s,w,value);
  retrieved = RealWordMemory_segment_word(s,w);
  return (retrieved == value);
}

void RealWordMemoryTests(struct TestResult *tr) {
  RunTest(test_PutAndRetrieveWordInSegment);
}

int main(int argc, const char *argv[]) {
  ObjectMemory_new();

  struct TestResult result;
  result.ran = 0;
  result.passed = 0;
  result.failed = 0;
  
  ObjectMemoryTests(&result);
  RealWordMemoryTests(&result);  
  
  printf("Tests completed.\n");
  printf("%d tests ran.\n", result.ran);
  printf("%d tests passed.\n", result.passed);
  printf("%d tests failed.\n", result.failed);
  
  return (result.failed != 0);
}
