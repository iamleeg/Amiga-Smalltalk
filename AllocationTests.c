#include "AllocationTests.h"
#include "ObjectMemory.h"

Test(ReleasePointer) {
  ObjectPointer objectPointer = 0x3580, freeHead = NonPointer;
  ObjectMemory_headOfFreePointerList_put(freeHead);
  ObjectMemory_freeBitOf_put(objectPointer, 0);

  ObjectMemory_releasePointer(objectPointer);

  Expect(ObjectMemory_freeBitOf(objectPointer) == 1);
  Expect(ObjectMemory_headOfFreePointerList() == objectPointer);
}

Test(LowWaterMarkCalculation) {
  Word NonEmptyListSize = 3, segment = 4, size;
  ObjectPointer freeRegion = 0xabc0;
  ObjectPointer location = 0x4242, lowWaterMark;
  ObjectMemory_locationBitsOf_put(freeRegion, location);
  ObjectMemory_segmentBitsOf_put(freeRegion, segment);
  for (size = HeaderSize; size <= BigSize; size++) {
    ObjectMemory_headOfFreeChunkList_inSegment_put(size, segment, NonPointer);
    if (size == NonEmptyListSize) {
      ObjectMemory_toFreeChunkList_add(size, freeRegion);
    }
  }

  lowWaterMark = ObjectMemory_abandonFreeChunksInSegment(segment);
  Expect(lowWaterMark == location);
  Expect(ObjectMemory_freeBitOf(freeRegion) == 1);
}

Test(ReverseHeapPointerAboveLowWaterMark) {
  Word segment = 2, size = 0x0a, retrievedSize = 0x0;
  ObjectPointer lowWaterMark = 0x1000,
    location = 0x2000,
    objectPointer = 0x1010;
  currentSegment = segment;
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_freeBitOf_put(objectPointer, 0);
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_sizeBitsOf_put(objectPointer, size);

  ObjectMemory_reverseHeapPointersAbove(lowWaterMark);

  retrievedSize = RealWordMemory_segment_word(segment, location);
  Expect(ObjectMemory_locationBitsOf(objectPointer) == size);
  Expect(retrievedSize == objectPointer);
}

void AllocationTests(struct TestResult *tr) {
  RunTest(ReleasePointer);
  RunTest(LowWaterMarkCalculation);
  RunTest(ReverseHeapPointerAboveLowWaterMark);
}
