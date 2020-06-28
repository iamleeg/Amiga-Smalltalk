#include "InterpreterMemoryTests.h"

#include "Interpreter.h"
#include "ObjectMemory.h"
#include "RealWordMemory.h"

Test(RoundTripIntegerThroughObjectMemory) {
  Word integerValue = 12, fetchedInteger, fieldIndex = 2, segment = (3 % HeapSegmentCount);
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
  Word integerValue = 32768, fieldIndex = 1, segment = (2 % HeapSegmentCount), sentinelValue = 0x4141;
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
  Word fieldIndex = 1, segment = (2 % HeapSegmentCount), pointerWord = 0x2468;
  ObjectPointer objectPointer = 0x1000, location = 0x2000;

  Interpreter_initPrimitive();
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_heapChunkOf_word_put(objectPointer, fieldIndex + HeaderSize, pointerWord);

  Interpreter_fetchInteger_ofObject(fieldIndex, objectPointer);
  Expect(Interpreter_success() == NO);
}

Test(BlockTransferOfFieldsBetweenObjects) {
  ObjectPointer fromObject = 0x1234, toObject = 0x1236, fromLocation = 0x1000, toLocation = 0x2000;
  ObjectPointer field1 = 0x2300, field2 = 0x2302, field3 = 0x2304;
  Word segment = (1 % HeapSegmentCount), fromIndex = 2, toIndex = 3;

  ObjectMemory_segmentBitsOf_put(fromObject, segment);
  ObjectMemory_locationBitsOf_put(fromObject, fromLocation);
  ObjectMemory_storeWord_ofObject_withValue(fromIndex, fromObject, field1);
  ObjectMemory_storeWord_ofObject_withValue(fromIndex + 1, fromObject, field2);
  ObjectMemory_storeWord_ofObject_withValue(fromIndex + 2, fromObject, field3);

  ObjectMemory_segmentBitsOf_put(toObject, segment);
  ObjectMemory_locationBitsOf_put(toObject, toLocation);

  Interpreter_transfer_fromIndex_ofObject_toIndex_ofObject(3, fromIndex, fromObject, toIndex, toObject);

  Expect(ObjectMemory_fetchPointer_ofObject(fromIndex, fromObject) == NilPointer);
  Expect(ObjectMemory_fetchPointer_ofObject(fromIndex + 1, fromObject) == NilPointer);
  Expect(ObjectMemory_fetchPointer_ofObject(fromIndex + 2, fromObject) == NilPointer);

  Expect(ObjectMemory_fetchPointer_ofObject(toIndex, toObject) == field1);
  Expect(ObjectMemory_fetchPointer_ofObject(toIndex + 1, toObject) == field2);
  Expect(ObjectMemory_fetchPointer_ofObject(toIndex + 2, toObject) == field3);
}

Test(ExtractBitsFromInteger) {
  Word pattern = 0xaaaa, masked, firstBitIndex = 1, lastBitIndex = 6, expected = 0x15;

  masked = Interpreter_extractBits_to_of(firstBitIndex, lastBitIndex, pattern);

  Expect(masked == expected);
}

Test(ExtractHighByteOfInteger) {
  Word pattern = 0xaa55;
  Byte highByte, expected = 0xaa;

  highByte = Interpreter_highByteOf(pattern);
  Expect(highByte == expected);
}

Test(ExtractLowByteOfInteger) {
  Word pattern = 0xaa55;
  Byte lowByte, expected = 0x55;

  lowByte = Interpreter_lowByteOf(pattern);
  Expect(lowByte == expected);
}


Test(PushAndPop) {
	ObjectPointer anObject = 0x1234;
	ObjectPointer aResult = 0;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
	Interpreter_push(anObject);
	aResult = Interpreter_popStack();
	Expect(anObject == aResult);
}

Test(PushAndPopInteger) {
	short integerValue=1;
	short actual = 0;
	
	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();

	Interpreter_initPrimitive();
	Interpreter_pushInteger(integerValue);
	Expect(Interpreter_success() == YES);
	actual = Interpreter_popInteger();
	Expect(Interpreter_success() == YES);
	Expect(integerValue == actual);
}

void InterpreterMemoryTests(struct TestResult *tr) {
  RunTest(PushAndPop);
  RunTest(PushAndPopInteger);
  RunTest(RoundTripIntegerThroughObjectMemory);
  RunTest(FailToStoreOutOfRangeInteger);
  RunTest(FailToFetchNonIntegerValue);
  RunTest(BlockTransferOfFieldsBetweenObjects);
  RunTest(ExtractBitsFromInteger);
  RunTest(ExtractHighByteOfInteger);
  RunTest(ExtractLowByteOfInteger);
}
