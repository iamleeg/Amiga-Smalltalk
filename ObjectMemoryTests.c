#include "ObjectMemoryTests.h"
#include "ObjectMemory.h"
#include "RealWordMemory.h"

Test(NonIntegerObjectIsNotIntegerObject) {
  short result = ObjectMemory_isIntegerObject(0);
  Expect(!result);
}

Test(IntegerObjectIsIntegerObject) {
  short result = ObjectMemory_isIntegerObject(1);
  Expect(result);
}

Test(ObjectTableLookupFindsWordInRealWordMemory) {
  ObjectPointer objectPointer = 0x2468;
  short value = 0xbeef, result = 0;
  RealWordMemory_segment_word_put(ObjectTableSegment,
    ObjectTableStart + objectPointer,
    value);
  result = ObjectMemory_ot(objectPointer);
  Expect(result == value);
}

Test(ObjectTableStorageSetsWordInRealWordMemory) {
  ObjectPointer objectPointer = 0x1234;
  short value = 0xcafe, result = 0;
  ObjectMemory_ot_put(objectPointer, value);
  result = RealWordMemory_segment_word(ObjectTableSegment,
    ObjectTableStart + objectPointer);
  Expect(result == value);
}

Test(ObjectTableStorageRetrievesSpecificBitsInRealWordMemory) {
  ObjectPointer objectPointer = 0x369C;
  short value = 0xAAAA, firstBitIndex = 0, lastBitIndex = 3, result = 0, expected = 0xA;
  ObjectMemory_ot_put(objectPointer, value);
  result = ObjectMemory_ot_bits_to(objectPointer, firstBitIndex, lastBitIndex);
  Expect(result == expected);
}

Test(ObjectTableStorageSetsSpecificBitsInRealWordMemory) {
  ObjectPointer objectPointer = 0xfda0;
  short value = 0, firstBitIndex = 11, lastBitIndex = 14, setBits = 0xf, result = 0, expected = 0x1e;
  ObjectMemory_ot_put(objectPointer, value);
  ObjectMemory_ot_bits_to_put(objectPointer, firstBitIndex, lastBitIndex, setBits);
  result = ObjectMemory_ot(objectPointer);
  Expect(result == expected);
}

Test(CountBitsInObjectStorageAreMostSignificantByteOfObjectHeader) {
  ObjectPointer objectPointer = 0x1234;
  short value = 0xbbcc, countBits = 0, expected = 0xbb;
  ObjectMemory_ot_put(objectPointer, value);
  countBits = ObjectMemory_countBitsOf(objectPointer);
  Expect(countBits == expected);
}

Test(CountBitsStoredInMostSignificantByteOfObjectHeader) {
  ObjectPointer objectPointer = 0xfffe;
  short value = 0x1122, countBits = 0x35, result = 0, expected = 0x3522;
  ObjectMemory_ot_put(objectPointer, value);
  ObjectMemory_countBitsOf_put(objectPointer, countBits);
  result = ObjectMemory_ot(objectPointer);
  Expect(result == expected);
}

Test(OddBitIsAtBitEightOfObjectHeader) {
  ObjectPointer objectPointer = 0x0;
  short value = 1<<7, result = 0, expected = 1;
  ObjectMemory_ot_put(objectPointer, value);
  result = ObjectMemory_oddBitOf(objectPointer);
  Expect(result == expected);
}

Test(OddBitIsSetAtBitEightOfObjectHeader) {
  ObjectPointer objectPointer = 0x0;
  short value = 0, bit = 1, result = 0, expected = 1<<7;
  ObjectMemory_ot_put(objectPointer, value);
  ObjectMemory_oddBitOf_put(objectPointer, bit);
  result = ObjectMemory_ot(objectPointer);
  Expect(result == expected);
}

Test(PointerBitIsAtBitNineOfObjectHeader) {
  ObjectPointer objectPointer = 0x0;
  short value = 1<<6, result = 0, expected = 1;
  ObjectMemory_ot_put(objectPointer, value);
  result = ObjectMemory_pointerBitOf(objectPointer);
  Expect(result == expected);
}

Test(PointerBitIsSetAtBitNineOfObjectHeader) {
  ObjectPointer objectPointer = 0x0;
  short value = 0, bit = 1, result = 0, expected = 1<<6;
  ObjectMemory_ot_put(objectPointer, value);
  ObjectMemory_pointerBitOf_put(objectPointer, bit);
  result = ObjectMemory_ot(objectPointer);
  Expect(result == expected);
}

Test(FreeBitIsAtBitTenOfObjectHeader) {
  ObjectPointer objectPointer = 0x0;
  short value = 1<<5, result = 0, expected = 1;
  ObjectMemory_ot_put(objectPointer, value);
  result = ObjectMemory_freeBitOf(objectPointer);
  Expect(result == expected);
}

Test(FreeBitIsSetAtBitTenOfObjectHeader) {
  ObjectPointer objectPointer = 0x0;
  short value = 0, bit = 1, result = 0, expected = 1<<5;
  ObjectMemory_ot_put(objectPointer, value);
  ObjectMemory_freeBitOf_put(objectPointer, bit);
  result = ObjectMemory_ot(objectPointer);
  Expect(result == expected);
}

Test(SegmentBitsAtLowestNybbleOfObjectHeader) {
  ObjectPointer objectPointer = 0x0;
  short value = 0x000f, segment = 0, expected = 0xf;
  ObjectMemory_ot_put(objectPointer, value);
  segment = ObjectMemory_segmentBitsOf(objectPointer);
  Expect(segment == expected);
}

Test(SegmentBitsStoredInLowestNybbleOfObjectHeader) {
  ObjectPointer objectPointer = 0x0;
  short value = 0x0000, segment = 0xf, result = 0, expected = 0x000f;
  ObjectMemory_ot_put(objectPointer, value);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  result = ObjectMemory_ot(objectPointer);
  Expect(result == expected);
}

Test(LocationBitsAreInTheSecondWordOfObjectHeader) {
  ObjectPointer objectPointer = 0x0;
  short locationWord = objectPointer + 1, value = 0xcafe, result = 0;
  RealWordMemory_segment_word_put(ObjectTableSegment,
    ObjectTableStart + locationWord,
    value);
  result = ObjectMemory_locationBitsOf(objectPointer);
  Expect(result == value);
}

Test(LocationBitsStoredInSecondWordOfObjectHeader) {
  ObjectPointer objectPointer = 0x0;
  short locationWord = objectPointer + 1, value = 0xb33f, result = 0;
  ObjectMemory_locationBitsOf_put(objectPointer, value);
  result = RealWordMemory_segment_word(ObjectTableSegment,
    ObjectTableStart + objectPointer + 1);
  Expect(result == value);
}

Test(GrabbingHeapChunkOfObjectGetsWordDeeperIntoMemory) {
  ObjectPointer objectPointer = 0x1000;
  short segment = 1, location = 0x0f0f, offset = 4, value = 0x4321, result = 0;
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  RealWordMemory_segment_word_put(segment,
    location + offset,
    value);
    
  result = ObjectMemory_heapChunkOf_word(objectPointer, offset);
  Expect(result == value);
}

Test(StoringHeapChunkOfObjectWritesWordDeeperIntoMemory) {
  ObjectPointer objectPointer = 0x100e;
  short segment = 3, location = 0xcccc, offset = 0xaa, value = 0x4321, result = 0;
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_heapChunkOf_word_put(objectPointer, offset, value);

  result = RealWordMemory_segment_word(segment, location+offset);
  Expect(result == value);
}

Test(GetHeapChunkOfObjectMemoryByByte) {
  ObjectPointer objectPointer = 0x2222;
  short segment = 5, location = 0x0202, wordOffset = 0x40, wordValue = 0xfedc, byteOffset = (wordOffset * 2) + 1;
  char lowByte = 0, expected = 0xdc;
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_heapChunkOf_word_put(objectPointer, wordOffset, wordValue);
  lowByte = ObjectMemory_heapChunkOf_byte(objectPointer, byteOffset);
  Expect(lowByte == expected);
}

Test(StoreHeapChunkOfObjectMemoryByByte) {
  ObjectPointer objectPointer = 0x3434;
  short segment = 2, location = 0x099a, wordOffset = 0x72, byteOffset = (wordOffset * 2), retrieved = 0, expected = 0xb400;
  char byteValue = 0xb4;
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_heapChunkOf_word_put(objectPointer, wordOffset, 0x0);
  ObjectMemory_heapChunkOf_byte_put(objectPointer, byteOffset, byteValue);
  retrieved = ObjectMemory_heapChunkOf_word(objectPointer, wordOffset);
  Expect(retrieved == expected);
}

Test(SizeBitsOfObjectAreInFirstWordOfHeap) {
  ObjectPointer objectPointer = 0x0864;
  short segment = 2, location = 0x1212, wordOffset = 0, size = 0xfeca, retrievedWord = 0;
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_heapChunkOf_word_put(objectPointer, wordOffset, size);
  retrievedWord = ObjectMemory_sizeBitsOf(objectPointer);
  Expect(retrievedWord == size);
}

Test(StoreSizeBitsOfObjectInFirstWordOfItsHeap) {
  ObjectPointer objectPointer = 0xa428;
  short segment = 1, location = 0xb79a, wordOffset = 0, word = 0xacee, size = 0;
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_sizeBitsOf_put(objectPointer, word);
  size = ObjectMemory_heapChunkOf_word(objectPointer, wordOffset);
  Expect(size == word);
}

Test(ClassBitsOfObjectAreInSecondWordOfHeap) {
  ObjectPointer objectPointer = 0x0864;
  short segment = 2, location = 0x1212, wordOffset = 1, size = 0xfeca, retrievedWord = 0;
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_heapChunkOf_word_put(objectPointer, wordOffset, size);
  retrievedWord = ObjectMemory_classBitsOf(objectPointer);
  Expect(retrievedWord == size);
}

Test(StoreClassBitsOfObjectInSecondWordOfItsHeap) {
  ObjectPointer objectPointer = 0xa428;
  short segment = 1, location = 0xb79a, wordOffset = 1, word = 0xacee, size = 0;
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_classBitsOf_put(objectPointer, word);
  size = ObjectMemory_heapChunkOf_word(objectPointer, wordOffset);
  Expect(size == word);
}

Test(FetchPointerByRetrievingAppropriateWordInMemory) {
  ObjectPointer objectPointer = 0x1234, retrievedPointer;
  short segment = 3, offset = 4, location = 0x2468, fieldIndex = 2;
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_heapChunkOf_word_put(objectPointer, offset, NilPointer);

  retrievedPointer = ObjectMemory_fetchPointer_ofObject(fieldIndex, objectPointer);

  Expect(retrievedPointer == NilPointer);
}

Test(StorePointerSavesInCorrectLocationAndAdjustsCounts) {
  ObjectPointer objectPointer = 0x1234, otherPointer = 0x1236, thirdPointer = 0x1238, retrievedPointer;
  short segment = 3, offset = 4, location = 0x2468, fieldIndex = 2, otherCount = 4, thirdCount = 7;
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_heapChunkOf_word_put(objectPointer, offset, otherPointer);
  ObjectMemory_countBitsOf_put(otherPointer, otherCount);
  ObjectMemory_countBitsOf_put(thirdPointer, thirdCount);

  ObjectMemory_storePointer_ofObject_withValue(fieldIndex, objectPointer, thirdPointer);

  retrievedPointer = ObjectMemory_fetchPointer_ofObject(fieldIndex, objectPointer);
  Expect(retrievedPointer == thirdPointer);
  Expect(ObjectMemory_countBitsOf(otherPointer) == (otherCount - 1));
  Expect(ObjectMemory_countBitsOf(thirdPointer) == (thirdCount + 1));
}

void ObjectMemoryTests(struct TestResult *tr) {
  RunTest(NonIntegerObjectIsNotIntegerObject);
  RunTest(IntegerObjectIsIntegerObject);
  RunTest(ObjectTableLookupFindsWordInRealWordMemory);
  RunTest(ObjectTableStorageSetsWordInRealWordMemory);
  RunTest(ObjectTableStorageRetrievesSpecificBitsInRealWordMemory);
  RunTest(ObjectTableStorageSetsSpecificBitsInRealWordMemory);
  RunTest(CountBitsInObjectStorageAreMostSignificantByteOfObjectHeader);
  RunTest(CountBitsStoredInMostSignificantByteOfObjectHeader);
  RunTest(OddBitIsAtBitEightOfObjectHeader);
  RunTest(OddBitIsSetAtBitEightOfObjectHeader);
  RunTest(PointerBitIsAtBitNineOfObjectHeader);
  RunTest(PointerBitIsSetAtBitNineOfObjectHeader);
  RunTest(FreeBitIsAtBitTenOfObjectHeader);
  RunTest(FreeBitIsSetAtBitTenOfObjectHeader);
  RunTest(SegmentBitsAtLowestNybbleOfObjectHeader);
  RunTest(SegmentBitsStoredInLowestNybbleOfObjectHeader);
  RunTest(LocationBitsAreInTheSecondWordOfObjectHeader);
  RunTest(LocationBitsStoredInSecondWordOfObjectHeader);
  RunTest(GrabbingHeapChunkOfObjectGetsWordDeeperIntoMemory);
  RunTest(StoringHeapChunkOfObjectWritesWordDeeperIntoMemory);
  RunTest(GetHeapChunkOfObjectMemoryByByte);
  RunTest(StoreHeapChunkOfObjectMemoryByByte);
  RunTest(SizeBitsOfObjectAreInFirstWordOfHeap);
  RunTest(StoreSizeBitsOfObjectInFirstWordOfItsHeap);
  RunTest(ClassBitsOfObjectAreInSecondWordOfHeap);
  RunTest(StoreClassBitsOfObjectInSecondWordOfItsHeap);
  RunTest(FetchPointerByRetrievingAppropriateWordInMemory);
  RunTest(StorePointerSavesInCorrectLocationAndAdjustsCounts);
}
