#include "AllocationTests.h"
#include "ObjectMemory.h"
#include "RealWordMemory.h"

Test(ReleasePointer) {
  ObjectPointer objectPointer = 0x3580, freeHead = NonPointer;
  ObjectMemory_headOfFreePointerList_put(freeHead);
  ObjectMemory_freeBitOf_put(objectPointer, 0);

  ObjectMemory_releasePointer(objectPointer);

  Expect(ObjectMemory_freeBitOf(objectPointer) == 1);
  Expect(ObjectMemory_headOfFreePointerList() == objectPointer);
}

Test(LowWaterMarkCalculation) {
  Word NonEmptyListSize = 3, segment = (4 % HeapSegmentCount), size;
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
  Word segment = (2 % HeapSegmentCount), size = 0x0a, retrievedSize = 0x0;
  ObjectPointer lowWaterMark = 0x1000,
    location = 0x2000,
    objectPointer = 0x1010;
  currentSegment = segment;
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_freeBitOf_put(objectPointer, 0);
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_sizeBitsOf_put(objectPointer, size);

  ObjectMemory_reverseHeapPointersAbove(lowWaterMark);

  /*
   * Don't use ObjectMemory_sizeBitsOf() here. If the pointer was
   * reversed, then the location bits point to the wrong place on
   * the heap.
   */
  retrievedSize = RealWordMemory_segment_word(segment, location);
  Expect(ObjectMemory_locationBitsOf(objectPointer) == size);
  Expect(retrievedSize == objectPointer);
}

Test(DoNotReverseHeapPointerBelowLowWaterMark) {
  Word segment = (2 % HeapSegmentCount), size = 0x0a, retrievedSize = 0x0;
  ObjectPointer lowWaterMark = 0x3000,
    location = 0x2000,
    objectPointer = 0x1010;
  currentSegment = segment;
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_freeBitOf_put(objectPointer, 0);
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_sizeBitsOf_put(objectPointer, size);

  ObjectMemory_reverseHeapPointersAbove(lowWaterMark);

  retrievedSize = RealWordMemory_segment_word(segment, location);
  Expect(ObjectMemory_locationBitsOf(objectPointer) == location);
  Expect(retrievedSize == size);
}

Test(DoNotReverseHeapPointerInWrongSegment) {
  Word segment = (2 % HeapSegmentCount), size = 0x0a, retrievedSize = 0x0;
  ObjectPointer lowWaterMark = 0x1000,
    location = 0x2000,
    objectPointer = 0x1010;
  currentSegment = segment + 1;
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_freeBitOf_put(objectPointer, 0);
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_sizeBitsOf_put(objectPointer, size);

  ObjectMemory_reverseHeapPointersAbove(lowWaterMark);

  retrievedSize = RealWordMemory_segment_word(segment, location);
  Expect(ObjectMemory_locationBitsOf(objectPointer) == location);
  Expect(retrievedSize == size);
}

Test(DoNotReverseHeapPointerOfUnusedSpace) {
  Word segment = (2 % HeapSegmentCount), size = 0x0a, retrievedSize = 0x0;
  ObjectPointer lowWaterMark = 0x1000,
    location = 0x2000,
    objectPointer = 0x1010;
  currentSegment = segment;
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_freeBitOf_put(objectPointer, 1);
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_sizeBitsOf_put(objectPointer, size);

  ObjectMemory_reverseHeapPointersAbove(lowWaterMark);

  retrievedSize = RealWordMemory_segment_word(segment, location);
  Expect(ObjectMemory_locationBitsOf(objectPointer) == location);
  Expect(retrievedSize == size);
}

void AllocationTests(struct TestResult *tr) {
  RunTest(ReleasePointer);
  RunTest(LowWaterMarkCalculation);
  RunTest(ReverseHeapPointerAboveLowWaterMark);
  RunTest(DoNotReverseHeapPointerBelowLowWaterMark);
  RunTest(DoNotReverseHeapPointerInWrongSegment);
  RunTest(DoNotReverseHeapPointerOfUnusedSpace);
}
