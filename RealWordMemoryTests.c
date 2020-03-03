#include "RealWordMemoryTests.h"
#include "RealWordMemory.h"

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

short test_PutAndRetrieveByteInSegment() {
  short s = 7;
  short w = 23;
  short b = 1;
  short wordValue = 0x0, expected = 0x007d;
  char value = 0x7d, retrieved = 0;
  RealWordMemory_segment_word_byte_put(s, w, b, value);
  retrieved = RealWordMemory_segment_word_byte(s, w, b);
  wordValue = RealWordMemory_segment_word(s, w);

  return (retrieved == value && wordValue == expected);
}

void RealWordMemoryTests(struct TestResult *tr) {
  RunTest(test_PutAndRetrieveWordInSegment);
  RunTest(test_PutSpecificBitsInAWord);
  RunTest(test_RetrieveSpecificBitsFromAWord);
  RunTest(test_PutAndRetrieveByteInSegment);
}
