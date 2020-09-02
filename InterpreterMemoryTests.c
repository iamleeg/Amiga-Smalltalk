#include "InterpreterMemoryTests.h"

#include "Interpreter.h"
#include "ObjectMemory.h"
#include "RealWordMemory.h"

extern ObjectPointer stubBlockContext(void);

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
  short pattern = 0xaaaa, masked, firstBitIndex = 1, lastBitIndex = 6, expected = 0x15;

  masked = Interpreter_extractBits_to_of(firstBitIndex, lastBitIndex, pattern);

  Expect(masked == expected);
}

Test(ExtractHighByteOfInteger) {
  short pattern = 0xaa55;
  Byte highByte, expected = 0xaa;

  highByte = Interpreter_highByteOf(pattern);
  Expect(highByte == expected);
}

Test(ExtractLowByteOfInteger) {
  short pattern = 0xaa55;
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

Test(PushAndPopFloat) {
	float floatValue = 1.23;
	float actual = 0.0;
	
	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();

	Interpreter_initPrimitive();
	Interpreter_pushFloat(floatValue);
	Expect(Interpreter_success() == YES);
	actual = Interpreter_popFloat();
	Expect(Interpreter_success() == YES);
	Expect(floatValue == actual);
}

Test(GetPositive16BitIntegerForValueFailsWithNegative) {
	short negativeIntegerValue=-123;
	short fetchedInteger = -1;
	ObjectPointer largePositiveIntegerPointer = NilPointer;
	
	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();

	Interpreter_initPrimitive();
	largePositiveIntegerPointer = Interpreter_positive16BitIntegerFor(negativeIntegerValue);
	Expect(Interpreter_success() == NO );
	Expect(largePositiveIntegerPointer == NilPointer);
}

Test(GetPositive16BitIntegerForValueWorksWithSmallInteger) {
	short smallIntegerValue=16383;
	short fetchedInteger = -1;
	ObjectPointer largePositiveIntegerPointer = NilPointer;
	
	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();

	Interpreter_initPrimitive();
	largePositiveIntegerPointer = Interpreter_positive16BitIntegerFor(smallIntegerValue);
	Expect(Interpreter_success() == YES);
	Expect(largePositiveIntegerPointer != NilPointer);
	fetchedInteger = ObjectMemory_integerValueOf(largePositiveIntegerPointer);
	Expect(fetchedInteger == smallIntegerValue);
}

Test(GetPositive16BitIntegerForValueWorksWithLargeInteger) {
	short largeIntegerValue=0x4001; /*16385*/
	short fetchedInteger = -1;
	ObjectPointer largePositiveIntegerPointer = NilPointer;
	
	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();

	Interpreter_initPrimitive();
	largePositiveIntegerPointer = Interpreter_positive16BitIntegerFor(largeIntegerValue);
	Expect(Interpreter_success() == YES);
	Expect(largePositiveIntegerPointer != NilPointer);
	Expect(ObjectMemory_fetchClassOf(largePositiveIntegerPointer) == ClassLargePositiveIntegerPointer);
	
	Expect( 0x01 == ObjectMemory_fetchByte_ofObject(0, largePositiveIntegerPointer)); 
	Expect( 0x40 == ObjectMemory_fetchByte_ofObject(1, largePositiveIntegerPointer)); 
}

Test(GetPositive16BitValueWorksWithSmallInteger) {
	short integerValue = 123;
	short resultValue = -1;
	ObjectPointer integerPointer = ObjectMemory_integerObjectOf(integerValue);

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();

	Interpreter_initPrimitive();

	resultValue = Interpreter_positive16BitValueOf(integerPointer);

	Expect(Interpreter_success() == YES);
	Expect(resultValue == integerValue);
}

Test(GetPositive16BitValueWorksWithLargeInteger) {
	short integerValue = 16385;
	short resultValue = -1;
	ObjectPointer integerPointer = Interpreter_positive16BitIntegerFor(integerValue);
	
	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();

	Interpreter_initPrimitive();

	resultValue = Interpreter_positive16BitValueOf(integerPointer);

	Expect(Interpreter_success() == YES);
	Expect(resultValue == integerValue);
}

Test(GetPositive16BitValueFailsWithNegative) {
	short integerValue = -123;
	short resultValue = -1;
	ObjectPointer integerPointer = ObjectMemory_integerObjectOf(integerValue);
	ObjectPointer integerPointer2 = ObjectMemory_integerObjectOf(123);
	
	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();

	Interpreter_initPrimitive();
	resultValue = Interpreter_positive16BitValueOf(integerPointer);
	
	Expect(Interpreter_success() == NO);
	Expect(resultValue == 0);
}

Test(GetPositive16BitValueFailsWithOtherClass) {
	short integerValue = 16385;
	short resultValue = -1;
	ObjectPointer integerPointer = NilPointer;
	
	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();

	/* make it a selector pointer instead */
	integerPointer = ObjectMemory_instantiateClass_withBytes(ClassMessagePointer, 2);

	Interpreter_initPrimitive();
	resultValue = Interpreter_positive16BitValueOf(integerPointer);
	Expect(Interpreter_success() == NO);
	Expect(resultValue == 0);
}

Test(GetPositive16BitValueFailsWithWrongBytes) {
	short integerValue = 16385;
	short resultValue = -1;
	ObjectPointer integerPointer = NilPointer;
	
	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();

	/* make it three bytes instead */
	integerPointer = ObjectMemory_instantiateClass_withBytes(ClassLargePositiveIntegerPointer, 3);
	ObjectMemory_storeByte_ofObject_withValue(0, integerPointer, 0);
	ObjectMemory_storeByte_ofObject_withValue(1, integerPointer, Interpreter_lowByteOf(integerValue));
	ObjectMemory_storeByte_ofObject_withValue(2, integerPointer, Interpreter_highByteOf(integerValue));

	Interpreter_initPrimitive();
	resultValue = Interpreter_positive16BitValueOf(integerPointer);
	Expect(Interpreter_success() == NO);
	Expect(resultValue == 0);
}

void InterpreterMemoryTests(struct TestResult *tr) {
  RunTest(PushAndPop);
  RunTest(PushAndPopInteger);
  RunTest(PushAndPopFloat);
  RunTest(GetPositive16BitIntegerForValueFailsWithNegative);
  RunTest(GetPositive16BitIntegerForValueWorksWithSmallInteger);
  RunTest(GetPositive16BitIntegerForValueWorksWithLargeInteger);
  RunTest(GetPositive16BitValueWorksWithSmallInteger);
  RunTest(GetPositive16BitValueFailsWithNegative);
  RunTest(GetPositive16BitValueWorksWithLargeInteger);
  RunTest(GetPositive16BitValueFailsWithOtherClass);
  RunTest(GetPositive16BitValueFailsWithWrongBytes); 
  RunTest(RoundTripIntegerThroughObjectMemory);
  RunTest(FailToStoreOutOfRangeInteger);
  RunTest(FailToFetchNonIntegerValue);
  RunTest(BlockTransferOfFieldsBetweenObjects);
  RunTest(ExtractBitsFromInteger);
  RunTest(ExtractHighByteOfInteger);
  RunTest(ExtractLowByteOfInteger);
}
