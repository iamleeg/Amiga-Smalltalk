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
  short value = 0x000f, segment = (0 % HeapSegmentCount), expected = 0xf;
  ObjectMemory_ot_put(objectPointer, value);
  segment = ObjectMemory_segmentBitsOf(objectPointer);
  Expect(segment == expected);
}

Test(SegmentBitsStoredInLowestNybbleOfObjectHeader) {
  ObjectPointer objectPointer = 0x0;
  short value = 0x0000, segment = (0xf % HeapSegmentCount), result = 0, expected = 0x000f;
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
  short value = 0xb33f, result = 0;
  ObjectMemory_locationBitsOf_put(objectPointer, value);
  result = RealWordMemory_segment_word(ObjectTableSegment,
    ObjectTableStart + objectPointer + 1);
  Expect(result == value);
}

Test(GrabbingHeapChunkOfObjectGetsWordDeeperIntoMemory) {
  ObjectPointer objectPointer = 0x1000;
  short segment = (1 % HeapSegmentCount), location = 0x0f0f, offset = 4, value = 0x4321, result = 0;
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
  short segment = (3 % HeapSegmentCount), location = 0xcccc, offset = 0xaa, value = 0x4321, result = 0;
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_heapChunkOf_word_put(objectPointer, offset, value);

  result = RealWordMemory_segment_word(segment, location+offset);
  Expect(result == value);
}

Test(GetHeapChunkOfObjectMemoryByByte) {
  ObjectPointer objectPointer = 0x2222;
  short segment = (5 % HeapSegmentCount), location = 0x0202, wordOffset = 0x40, wordValue = 0xfedc, byteOffset = (wordOffset * 2) + 1;
  char lowByte = 0, expected = 0xdc;
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_heapChunkOf_word_put(objectPointer, wordOffset, wordValue);
  lowByte = ObjectMemory_heapChunkOf_byte(objectPointer, byteOffset);
  Expect(lowByte == expected);
}

Test(StoreHeapChunkOfObjectMemoryByByte) {
  ObjectPointer objectPointer = 0x3434;
  short segment = (2 % HeapSegmentCount), location = 0x099a, wordOffset = 0x72, byteOffset = (wordOffset * 2), retrieved = 0, expected = 0xb400;
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
  short segment = (2 % HeapSegmentCount), location = 0x1212, wordOffset = 0, size = 0xfeca, retrievedWord = 0;
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_heapChunkOf_word_put(objectPointer, wordOffset, size);
  retrievedWord = ObjectMemory_sizeBitsOf(objectPointer);
  Expect(retrievedWord == size);
}

Test(StoreSizeBitsOfObjectInFirstWordOfItsHeap) {
  ObjectPointer objectPointer = 0xa428;
  short segment = (1 % HeapSegmentCount), location = 0xb79a, wordOffset = 0, word = 0xacee, size = 0;
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_sizeBitsOf_put(objectPointer, word);
  size = ObjectMemory_heapChunkOf_word(objectPointer, wordOffset);
  Expect(size == word);
}

Test(ClassBitsOfObjectAreInSecondWordOfHeap) {
  ObjectPointer objectPointer = 0x0864;
  short segment = (2 % HeapSegmentCount), location = 0x1212, wordOffset = 1, size = 0xfeca, retrievedWord = 0;
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_heapChunkOf_word_put(objectPointer, wordOffset, size);
  retrievedWord = ObjectMemory_classBitsOf(objectPointer);
  Expect(retrievedWord == size);
}

Test(StoreClassBitsOfObjectInSecondWordOfItsHeap) {
  ObjectPointer objectPointer = 0xa428;
  short segment = (1 % HeapSegmentCount), location = 0xb79a, wordOffset = 1, word = 0xacee, size = 0;
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_classBitsOf_put(objectPointer, word);
  size = ObjectMemory_heapChunkOf_word(objectPointer, wordOffset);
  Expect(size == word);
}

Test(FetchPointerByRetrievingAppropriateWordInMemory) {
  ObjectPointer objectPointer = 0x1234, retrievedPointer;
  short segment = (3 % HeapSegmentCount), offset = 4, location = 0x2468, fieldIndex = 2;
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_heapChunkOf_word_put(objectPointer, offset, NilPointer);

  retrievedPointer = ObjectMemory_fetchPointer_ofObject(fieldIndex, objectPointer);

  Expect(retrievedPointer == NilPointer);
}

Test(StorePointerSavesInCorrectLocationAndAdjustsCounts) {
  ObjectPointer objectPointer = 0x1234, otherPointer = 0x1236, thirdPointer = 0x1238, retrievedPointer;
  short segment = (3 % HeapSegmentCount), offset = 4, location = 0x2468, fieldIndex = 2, otherCount = 4, thirdCount = 7;
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

Test(FetchWordFromAppropriateLocationInMemory) {
  ObjectPointer objectPointer = 0x1234;
  short segment = (3 % HeapSegmentCount), offset = 4, location = 0x2468, wordIndex = 2, value = 0x3579, retrievedWord;
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_heapChunkOf_word_put(objectPointer, offset, value);

  retrievedWord = ObjectMemory_fetchWord_ofObject(wordIndex, objectPointer);

  Expect(retrievedWord == value);
}

Test(StoreWordInObjectField) {
  ObjectPointer objectPointer = 0x1234;
  short segment = (3 % HeapSegmentCount), location = 0x2468, wordIndex = 2, value = 0x3579, retrievedWord;
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_locationBitsOf_put(objectPointer, location);

  ObjectMemory_storeWord_ofObject_withValue(wordIndex, objectPointer, value);

  retrievedWord = ObjectMemory_fetchWord_ofObject(wordIndex, objectPointer);
  Expect(retrievedWord == value);
}

Test(FetchLowByteOfWord) {
  ObjectPointer objectPointer = 0x1234;
  short segment = (3 % HeapSegmentCount), location = 0x2468, wordIndex = 2, byteIndex = 5, value = 0x3579;
  char retrievedByte, expectedByte = 0x79;
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_locationBitsOf_put(objectPointer, location);

  ObjectMemory_storeWord_ofObject_withValue(wordIndex, objectPointer, value);

  retrievedByte = ObjectMemory_fetchByte_ofObject(byteIndex, objectPointer);
  Expect(retrievedByte == expectedByte);
}

Test(FetchHighByteOfWord) {
  ObjectPointer objectPointer = 0x1234;
  short segment = (3 % HeapSegmentCount), location = 0x2468, wordIndex = 2, byteIndex = 4, value = 0x3579;
  char retrievedByte, expectedByte = 0x35;
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_locationBitsOf_put(objectPointer, location);

  ObjectMemory_storeWord_ofObject_withValue(wordIndex, objectPointer, value);

  retrievedByte = ObjectMemory_fetchByte_ofObject(byteIndex, objectPointer);
  Expect(retrievedByte == expectedByte);
}

Test(StoreBytesOfWord) {
  ObjectPointer objectPointer = 0x1234;
  short segment = (3 % HeapSegmentCount), location = 0x2468, wordIndex = 2, lowByteIndex = 5, highByteIndex = 4, expectedWord = 0x3579, retrievedWord;
  char lowValue = 0x79, highValue = 0x35;
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_locationBitsOf_put(objectPointer, location);

  ObjectMemory_storeByte_ofObject_withValue(lowByteIndex, objectPointer, lowValue);
  ObjectMemory_storeByte_ofObject_withValue(highByteIndex, objectPointer, highValue);

  retrievedWord = ObjectMemory_fetchWord_ofObject(wordIndex, objectPointer);
  Expect(retrievedWord == expectedWord);
}

Test(IncrementRefCount) {
  ObjectPointer objectPointer = 0x1234;
  short count = 12, newCount;
  ObjectMemory_countBitsOf_put(objectPointer, count);
  
  ObjectMemory_increaseReferencesTo(objectPointer);

  newCount = ObjectMemory_countBitsOf(objectPointer);
  Expect(newCount == count + 1);
}

Test(UpperBoundOfRefCount) {
  ObjectPointer objectPointer = 0x1234;
  short count = 129, newCount;
  ObjectMemory_countBitsOf_put(objectPointer, count);
  
  ObjectMemory_increaseReferencesTo(objectPointer);

  newCount = ObjectMemory_countBitsOf(objectPointer);
  Expect(newCount == count);
}

Test(DecrementRefCount) {
  ObjectPointer objectPointer = 0x1234;
  short count = 12, newCount;
  ObjectMemory_countBitsOf_put(objectPointer, count);
  
  ObjectMemory_decreaseReferencesTo(objectPointer);

  newCount = ObjectMemory_countBitsOf(objectPointer);
  Expect(newCount == count - 1);
}

Test(DoNotDecrementCountForMaximallyReferencedObject) {
  ObjectPointer objectPointer = 0x1234;
  short count = 128, newCount;
  ObjectMemory_countBitsOf_put(objectPointer, count);
  
  ObjectMemory_decreaseReferencesTo(objectPointer);

  newCount = ObjectMemory_countBitsOf(objectPointer);
  Expect(newCount == count);
}

Test(DeallocateObjectWhenFinalReferenceRemoved) {
  ObjectPointer objectPointer = 0x1234, freeHead;
  short count = 1, newCount, segment = (6 % HeapSegmentCount), size = 2;
  ObjectMemory_countBitsOf_put(objectPointer, count);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_headOfFreeChunkList_inSegment_put(size, segment, NonPointer);
  ObjectMemory_sizeBitsOf_put(objectPointer, size);
  ObjectMemory_decreaseReferencesTo(objectPointer);

  newCount = ObjectMemory_countBitsOf(objectPointer);
  Expect(newCount == 0);
  freeHead = ObjectMemory_headOfFreeChunkList_inSegment(size, segment);
  Expect(freeHead == objectPointer);
}

Test(FetchClassOfRegularObject) {
  ObjectPointer objectPointer = 0x1234, classPointer = 0x1230, reportedClass;
  ObjectMemory_classBitsOf_put(objectPointer, classPointer);

  reportedClass = ObjectMemory_fetchClassOf(objectPointer);
  Expect(reportedClass == classPointer);
}

Test(FetchClassOfImmediateInteger) {
  ObjectPointer zeroPointer = 0x1, reportedClass;

  reportedClass = ObjectMemory_fetchClassOf(zeroPointer);
  Expect(reportedClass == IntegerClass);
}

Test(DiscoverWordLengthOfObject) {
  ObjectPointer objectPointer = 0x1234;
  Word size = 12, wordLength;
  ObjectMemory_sizeBitsOf_put(objectPointer, size);

  wordLength = ObjectMemory_fetchWordLengthOf(objectPointer);
  Expect(wordLength == size - 2);
}

Test(DiscoverByteLengthOfObjectWithEvenSize) {
  ObjectPointer objectPointer = 0x1234;
  Word size = 12, byteLength;
  ObjectMemory_sizeBitsOf_put(objectPointer, size);
  ObjectMemory_oddBitOf_put(objectPointer, NO);

  byteLength = ObjectMemory_fetchByteLengthOf(objectPointer);
  Expect(byteLength == (size - 2)*2);
}

Test(DiscoverByteLengthOfObjectWithOddSize) {
  ObjectPointer objectPointer = 0x1234;
  Word size = 12, byteLength;
  ObjectMemory_sizeBitsOf_put(objectPointer, size);
  ObjectMemory_oddBitOf_put(objectPointer, YES);

  byteLength = ObjectMemory_fetchByteLengthOf(objectPointer);
  Expect(byteLength == (size - 2)*2 - 1);
}

Test(AllocateSmallObjectByReclaimingRightSizedSpace) {
  ObjectPointer objectPointer, anObject = 0x2300, classPointer = 0x2468, reportedClass, location = 0x3200;
  Word size, wordLength = 10;
  Bool pointerBit;
  /* this test allocates an object, so let's ensure there's a free space exactly equal to the object size */
  Word segment = (1 % HeapSegmentCount);
  currentSegment = segment;
  /* add a valid pointer to the free chunk list */
  ObjectMemory_locationBitsOf_put(anObject, location);
  ObjectMemory_segmentBitsOf_put(anObject, segment);
  ObjectMemory_headOfFreeChunkList_inSegment_put(wordLength + HeaderSize, currentSegment, anObject);

  objectPointer = ObjectMemory_instantiateClass_withPointers(classPointer, wordLength);

  Expect(objectPointer != NilPointer);

  size = ObjectMemory_sizeBitsOf(objectPointer);
  Expect(size == wordLength + HeaderSize);

  reportedClass = ObjectMemory_fetchClassOf(objectPointer);
  Expect(reportedClass == classPointer);

  pointerBit = ObjectMemory_pointerBitOf(objectPointer);
  Expect(pointerBit == YES);
}

Test(AllocateSmallObjectBySplittingLargeChunk) {
  ObjectPointer objectPointer, classPointer = 0x2468, reportedClass;
  Word size, wordLength = 10;
  Bool pointerBit;
  /* don't add a valid pointer to the free chunk list - rely on a BigSize chunk already existing */
  objectPointer = ObjectMemory_instantiateClass_withPointers(classPointer, wordLength);

  Expect(objectPointer != NilPointer);

  size = ObjectMemory_sizeBitsOf(objectPointer);
  Expect(size == wordLength + HeaderSize);

  reportedClass = ObjectMemory_fetchClassOf(objectPointer);
  Expect(reportedClass == classPointer);

  pointerBit = ObjectMemory_pointerBitOf(objectPointer);
  Expect(pointerBit == YES);
}

Test(AllocateHugeObject) {
  ObjectPointer objectPointer, classPointer = 0x2468, reportedClass, allocatedPointer = 0x2300;
  Word size, wordLength = HugeSize;
  Bool pointerBit;
  /* this test actually allocates an object, so let's ensure there's a free space */
  Word segment = (1 % HeapSegmentCount);
  currentSegment = segment;
  /* add a valid pointer to the free chunk list */

  ObjectMemory_headOfFreeChunkList_inSegment_put(LastFreeChunkList, currentSegment, allocatedPointer);
  /* +1 because an extra word is needed to handle the spilled size */
  ObjectMemory_sizeBitsOf_put(allocatedPointer, wordLength + HeaderSize + 1);

  objectPointer = ObjectMemory_instantiateClass_withPointers(classPointer, wordLength);

  Expect(objectPointer != NilPointer);

  /* The extra word is not reported here */
  size = ObjectMemory_sizeBitsOf(objectPointer);
  Expect(size == wordLength + HeaderSize );

  reportedClass = ObjectMemory_fetchClassOf(objectPointer);
  Expect(reportedClass == classPointer);

  pointerBit = ObjectMemory_pointerBitOf(objectPointer);
  Expect(pointerBit == YES);
}

Test(AllocateSmallObjectWithWordStorage) {
  ObjectPointer objectPointer, classPointer = 0x2468;
  Word wordLength = 10;
  Bool pointerBit;
  /* this test actually allocates an object, so let's ensure there's a free space */
  Word segment = (1 % HeapSegmentCount);
  currentSegment = segment;
  /* add a valid pointer to the free chunk list */
  ObjectMemory_headOfFreeChunkList_inSegment_put(wordLength + HeaderSize, currentSegment, 0x2300);

  objectPointer = ObjectMemory_instantiateClass_withWords(classPointer, wordLength);

  pointerBit = ObjectMemory_pointerBitOf(objectPointer);
  Expect(pointerBit == NO);
}

Test(AllocateSmallObjectWithByteStorage) {
  ObjectPointer objectPointer, classPointer = 0x2468;
  Word byteLength = 10, wordLength;
  Bool pointerBit, oddBit;
  /* this test actually allocates an object, so let's ensure there's a free space */
  Word segment = (1 % HeapSegmentCount);
  currentSegment = segment;
  /* add a valid pointer to the free chunk list */
  ObjectMemory_headOfFreeChunkList_inSegment_put(byteLength/2 + HeaderSize, currentSegment, 0x2300);

  objectPointer = ObjectMemory_instantiateClass_withBytes(classPointer, byteLength);

  pointerBit = ObjectMemory_pointerBitOf(objectPointer);
  Expect(pointerBit == NO);

  wordLength = ObjectMemory_fetchWordLengthOf(objectPointer);
  Expect(wordLength == (byteLength / 2));

  oddBit = ObjectMemory_oddBitOf(objectPointer);
  Expect(oddBit == NO);
}

Test(FindFirstInstanceOfClass) {
  ObjectPointer classPointer = 0x1234, objectPointer, expectedFirstInstance = 0x8;
  Word segment = (3 % HeapSegmentCount), location = 0x1000;

  ObjectMemory_segmentBitsOf_put(expectedFirstInstance, segment);
  ObjectMemory_locationBitsOf_put(expectedFirstInstance, location);
  ObjectMemory_classBitsOf_put(expectedFirstInstance, classPointer);
  objectPointer = ObjectMemory_initialInstanceOf(classPointer);

  Expect(objectPointer == expectedFirstInstance);
}

Test(FindFirstLiveInstanceOfClass) {
  ObjectPointer classPointer = 0x1234, objectPointer, expectedFirstInstance = 0xa, freedEarlierInstance = 0x8;
  Word segment = (3 % HeapSegmentCount), location = 0x1000;
  Word otherSegment = (3 % HeapSegmentCount), otherLocation = 0x1004;

  ObjectMemory_segmentBitsOf_put(expectedFirstInstance, segment);
  ObjectMemory_locationBitsOf_put(expectedFirstInstance, location);
  ObjectMemory_classBitsOf_put(expectedFirstInstance, classPointer);

  ObjectMemory_segmentBitsOf_put(freedEarlierInstance, otherSegment);
  ObjectMemory_locationBitsOf_put(freedEarlierInstance, otherLocation);
  ObjectMemory_classBitsOf_put(freedEarlierInstance, classPointer);
  ObjectMemory_freeBitOf_put(freedEarlierInstance, YES);

  objectPointer = ObjectMemory_initialInstanceOf(classPointer);

  Expect(objectPointer == expectedFirstInstance);
}

Test(FindNextInstanceOfClass) {
  ObjectPointer classPointer = 0x1234, objectPointer, expectedFoundInstance = 0xa, firstInstance = 0x8;
  Word segment = (3 % HeapSegmentCount), location = 0x1000;
  Word otherSegment = (3 % HeapSegmentCount), otherLocation = 0x1004;

  ObjectMemory_segmentBitsOf_put(expectedFoundInstance, segment);
  ObjectMemory_locationBitsOf_put(expectedFoundInstance, location);
  ObjectMemory_classBitsOf_put(expectedFoundInstance, classPointer);
  ObjectMemory_freeBitOf_put(expectedFoundInstance, NO);

  ObjectMemory_segmentBitsOf_put(firstInstance, otherSegment);
  ObjectMemory_locationBitsOf_put(firstInstance, otherLocation);
  ObjectMemory_classBitsOf_put(firstInstance, classPointer);
  ObjectMemory_freeBitOf_put(firstInstance, NO);

  objectPointer = ObjectMemory_initialInstanceOf(classPointer);
  objectPointer = ObjectMemory_instanceAfter(objectPointer);

  Expect(objectPointer == expectedFoundInstance);
}

Test(DoNotFindAnyInstanceOfClassThatWasNeverInstantiated) {
  ObjectPointer classPointer = 0x888a, objectPointer;

  objectPointer = ObjectMemory_initialInstanceOf(classPointer);

  Expect(objectPointer == NilPointer);
}

Test(SwapPointers) {
  ObjectPointer object1 = 0x1234, object2 = 0x1236, location1 = 0x1238, location2 = 0x123a;
  Word segment1 = 1, segment2 = 2;
  Bool odd1 = YES, odd2 = NO, pointer1 = NO, pointer2 = YES;

  ObjectMemory_segmentBitsOf_put(object1, segment1);
  ObjectMemory_locationBitsOf_put(object1, location1);
  ObjectMemory_oddBitOf_put(object1, odd1);
  ObjectMemory_pointerBitOf_put(object1, pointer1);

  ObjectMemory_segmentBitsOf_put(object2, segment2);
  ObjectMemory_locationBitsOf_put(object2, location2);
  ObjectMemory_oddBitOf_put(object2, odd2);
  ObjectMemory_pointerBitOf_put(object2, pointer2);

  ObjectMemory_swapPointersOf_and(object1, object2);

  Expect(ObjectMemory_segmentBitsOf(object1) == segment2);
  Expect(ObjectMemory_segmentBitsOf(object2) == segment1);

  Expect(ObjectMemory_locationBitsOf(object1) == location2);
  Expect(ObjectMemory_locationBitsOf(object2) == location1);

  Expect(ObjectMemory_oddBitOf(object1) == odd2);
  Expect(ObjectMemory_oddBitOf(object2) == odd1);

  Expect(ObjectMemory_pointerBitOf(object1) == pointer2);
  Expect(ObjectMemory_pointerBitOf(object2) == pointer1);
}

Test(GetIntegerValueOfObject) {
  ObjectPointer seven = 0x0f;
  short value = ObjectMemory_integerValueOf(seven);
  Expect(value == 7);
}

Test(GetObjectForIntegerValue) {
  ObjectPointer six = 0x0d, objectPointer;
  objectPointer = ObjectMemory_integerObjectOf(6);
  Expect(objectPointer == six);
}

Test(IntegerValueBelowRange) {
  short value = -16385;
  Bool isIntegerValue = ObjectMemory_isIntegerValue(value);
  Expect(isIntegerValue == NO);
}

Test(IntegerValueAboveRange) {
  short value = 16385;
  Bool isIntegerValue = ObjectMemory_isIntegerValue(value);
  Expect(isIntegerValue == NO);
}

Test(IntegerValueWithinRange) {
  short value = 0;
  Bool isIntegerValue = ObjectMemory_isIntegerValue(value);
  Expect(isIntegerValue == YES);
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
  RunTest(FetchWordFromAppropriateLocationInMemory);
  RunTest(StoreWordInObjectField);
  RunTest(FetchLowByteOfWord);
  RunTest(FetchHighByteOfWord);
  RunTest(StoreBytesOfWord);
  RunTest(IncrementRefCount);
  RunTest(UpperBoundOfRefCount);
  RunTest(DecrementRefCount);
  RunTest(DoNotDecrementCountForMaximallyReferencedObject);
  RunTest(DeallocateObjectWhenFinalReferenceRemoved);
  RunTest(FetchClassOfRegularObject);
  RunTest(FetchClassOfImmediateInteger);
  RunTest(DiscoverWordLengthOfObject);
  RunTest(DiscoverByteLengthOfObjectWithEvenSize);
  RunTest(DiscoverByteLengthOfObjectWithOddSize);
  RunTest(AllocateSmallObjectByReclaimingRightSizedSpace);
  RunIsolatedTest(AllocateSmallObjectBySplittingLargeChunk);
  RunTest(AllocateHugeObject);
  RunTest(AllocateSmallObjectWithWordStorage);
  RunTest(AllocateSmallObjectWithByteStorage);
  RunTest(FindFirstInstanceOfClass);
  RunTest(FindFirstLiveInstanceOfClass);
  RunTest(FindNextInstanceOfClass);
  RunTest(DoNotFindAnyInstanceOfClassThatWasNeverInstantiated);
  RunTest(SwapPointers);
  RunTest(GetIntegerValueOfObject);
  RunTest(GetObjectForIntegerValue);
  RunTest(IntegerValueBelowRange);
  RunTest(IntegerValueAboveRange);
  RunTest(IntegerValueWithinRange);
}
