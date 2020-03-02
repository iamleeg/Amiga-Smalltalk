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

short test_ObjectTableStorageSetsWordInRealWordMemory() {
  ObjectPointer objectPointer = 0x1234;
  short value = 0xcafe, result = 0;
  ObjectMemory_ot_put(objectPointer, value);
  result = RealWordMemory_segment_word(ObjectTableSegment,
    ObjectTableStart + objectPointer);
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
  RunTest(test_ObjectTableStorageSetsWordInRealWordMemory);
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

short test_PutSpecificBitsInAWord() {
  short s = 5;
  short w = 123;
  short firstBit = 3;
  short lastBit = 9;
  short sevenOnBits = 0x007F;
  short retrievedValue = 0;
  short expectedValue = 0x1FC0;
  RealWordMemory_segment_word_put(s,w,0x0000);
  RealWordMemory_segment_word_bits_to_put(s,w, firstBit, lastBit, sevenOnBits);
  retrievedValue = RealWordMemory_segment_word(s,w);
  return (retrievedValue == expectedValue);
}

short test_RetrieveSpecificBitsFromAWord() {
  short s = 2;
  short w = 97;
  short firstBit = 12;
  short lastBit = 12;
  short allBitsOn = 0xFFFF;
  short retrievedValue = 0;
  short expectedValue = 1;
  RealWordMemory_segment_word_put(s, w, allBitsOn);
  retrievedValue = RealWordMemory_segment_word_bits_to(s, w, firstBit, lastBit);
  return (retrievedValue == expectedValue);
}

void RealWordMemoryTests(struct TestResult *tr) {
  RunTest(test_PutAndRetrieveWordInSegment);
  RunTest(test_PutSpecificBitsInAWord);
  RunTest(test_RetrieveSpecificBitsFromAWord);
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
