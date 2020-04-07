#include "InterpreterMemoryTests.h"

#include "Interpreter.h"
#include "ObjectMemory.h"

Test(RoundTripIntegerThroughObjectMemory) {
  Word integerValue = 12, fetchedInteger, fieldIndex = 2, segment = 3;
  ObjectPointer objectPointer = 0x1000, location = 0x2000;

  Interpreter_initPrimitive(); /* clear the success register before test */
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_locationBitsOf_put(objectPointer, location);

  Interpreter_storeInteger_ofObject_withValue(fieldIndex, objectPointer, integerValue);

  Expect(Interpreter_success() == YES);

  fetchedInteger = Interpreter_fetchInteger_ofObject(fieldIndex, objectPointer);
  Expect(Interpreter_success() == YES);
  Expect(fetchedInteger == integerValue);
}

Test(FailToStoreOutOfRangeInteger) {
  Word integerValue = 32768, fieldIndex = 1, segment = 2, sentinelValue = 0x4141;
  ObjectPointer objectPointer = 0x1000, location = 0x2000;

  Interpreter_initPrimitive();
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_heapChunkOf_word_put(objectPointer, fieldIndex + HeaderSize, sentinelValue);

  Interpreter_storeInteger_ofObject_withValue(fieldIndex, objectPointer, integerValue);
  Expect(Interpreter_success() == NO);
  Expect(ObjectMemory_heapChunkOf_word(objectPointer, HeaderSize + fieldIndex) == sentinelValue);
}

Test(FailToFetchNonIntegerValue) {
  Word fieldIndex = 1, segment = 2, pointerWord = 0x2468;
  ObjectPointer objectPointer = 0x1000, location = 0x2000;

  Interpreter_initPrimitive();
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_heapChunkOf_word_put(objectPointer, fieldIndex + HeaderSize, pointerWord);

  Interpreter_fetchInteger_ofObject(fieldIndex, objectPointer);
  Expect(Interpreter_success() == NO);
}

void InterpreterMemoryTests(struct TestResult *tr) {
  RunTest(RoundTripIntegerThroughObjectMemory);
  RunTest(FailToStoreOutOfRangeInteger);
  RunTest(FailToFetchNonIntegerValue);
}