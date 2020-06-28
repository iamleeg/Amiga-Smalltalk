#include "FreeListTests.h"
#include "ObjectMemory.h"
#include "RealWordMemory.h"
#include "Types.h"

Test(RetrieveHeadOfFreePointerListFromObjectTable) {
  Word value = 0x37b5, result = 0;
  RealWordMemory_segment_word_put(ObjectTableSegment,
    FreePointerList,
    value);
  result = ObjectMemory_headOfFreePointerList();
  Expect(result == value);
}

Test(SetHeadOfFreePointerListInObjectTable) {
  Word value = 0x57b3, result = 0;
  ObjectMemory_headOfFreePointerList_put(value);
  result = RealWordMemory_segment_word(ObjectTableSegment,
    FreePointerList);
  Expect(result == value);
}

Test(AppendingEntryToFreePointerList) {
  ObjectPointer object1 = 0x0002, object2 = 0x0004, retrievedHead = 0, retrievedNext = 0;
  ObjectMemory_headOfFreePointerList_put(object1);
  ObjectMemory_toFreePointerListAdd(object2);
  retrievedHead = ObjectMemory_headOfFreePointerList();
  retrievedNext = ObjectMemory_locationBitsOf(retrievedHead);
  Expect(retrievedHead == object2);
  Expect(retrievedNext == object1);
}

Test(RemovingEntryFromFreePointerList) {
  ObjectPointer object1 = 0x0002, object2 = 0x0004, retrievedHead = 0, returnedObjectPointer = 0;
  ObjectMemory_headOfFreePointerList_put(object1);
  ObjectMemory_toFreePointerListAdd(object2);
  returnedObjectPointer = ObjectMemory_removeFromFreePointerList();
  Expect(returnedObjectPointer == object2);
  retrievedHead = ObjectMemory_headOfFreePointerList();
  Expect(retrievedHead == object1);
}

Test(GettingNilWhenTryingToRemoveFromEmptyFreePointerList) {
  ObjectPointer removedObject = 0;
  ObjectMemory_headOfFreePointerList_put(NonPointer);
  removedObject = ObjectMemory_removeFromFreePointerList();
  Expect(removedObject == NilPointer);
}

Test(RetrieveHeadOfFreeChunkListFromRealWordMemory) {
  Word value = 0x37b5, size = 0x24, segment = (3 % HeapSegmentCount), result = 0;
  RealWordMemory_segment_word_put(segment,
    FirstFreeChunkList + size,
    value);
  result = ObjectMemory_headOfFreeChunkList_inSegment(size, segment);
  Expect(result == value);
}

Test(SetHeadOfFreeChunkListInRealWordMemory) {
  Word value = 0x37b5, size = 0x24, segment = (3 % HeapSegmentCount), result = 0;
  ObjectMemory_headOfFreeChunkList_inSegment_put(size, segment, value);
  result = RealWordMemory_segment_word(segment,
    FirstFreeChunkList + size);
  Expect(result == value);
}

Test(AddObjectToFreeChunkListInItsSegment) {
  ObjectPointer objectPointer = 0x2468, otherObject = 0x1238, head = 0, next = 0;
  Word size = 0x10, segment = (8 % HeapSegmentCount);
  ObjectMemory_headOfFreeChunkList_inSegment_put(size, segment, otherObject);
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_toFreeChunkList_add(size, objectPointer);

  head = ObjectMemory_headOfFreeChunkList_inSegment(size, segment);
  Expect(head == objectPointer);
  next = ObjectMemory_classBitsOf(objectPointer);
  Expect(next == otherObject);
}

Test(RemoveEntryFromCurrentSegmentFreeChunkList) {
  ObjectPointer object1 = 0x1234, object2 = 0x1236, removedObject = 0, newHead = 0;
  Word segment = (4 % HeapSegmentCount), size = 2;
  ObjectMemory_segmentBitsOf_put(object1, segment);
  ObjectMemory_segmentBitsOf_put(object2, segment);
  ObjectMemory_headOfFreeChunkList_inSegment_put(size, segment, object1);
  ObjectMemory_toFreeChunkList_add(size, object2);
  currentSegment = segment;

  removedObject = ObjectMemory_removeFromFreeChunkList(size);
  Expect(removedObject == object2);
  newHead = ObjectMemory_headOfFreeChunkList_inSegment(size, segment);
  Expect(newHead == object1);
}

Test(GetNilWhenTryingToRemoveFromEmptyFreeChunkList) {
  ObjectPointer removed = 0;
  Word segment = (4 % HeapSegmentCount), size = 0xc;
  currentSegment = segment;
  ObjectMemory_headOfFreeChunkList_inSegment_put(size, segment, NonPointer);

  removed = ObjectMemory_removeFromFreeChunkList(size);
  Expect(removed == NilPointer);
}

Test(ResetAFreeChunkListByWritingANonPointerToItsHead) {
  Word segment = (2 % HeapSegmentCount), size = 4;

  ObjectMemory_resetFreeChunkList_inSegment(size, segment);

  Expect(ObjectMemory_headOfFreeChunkList_inSegment(size, segment) == NonPointer);
}

void FreeListTests(struct TestResult *tr) {
  RunIsolatedTest(RetrieveHeadOfFreePointerListFromObjectTable);
  RunIsolatedTest(SetHeadOfFreePointerListInObjectTable);
  RunIsolatedTest(AppendingEntryToFreePointerList);
  RunIsolatedTest(RemovingEntryFromFreePointerList);
  RunIsolatedTest(GettingNilWhenTryingToRemoveFromEmptyFreePointerList);
  RunIsolatedTest(RetrieveHeadOfFreeChunkListFromRealWordMemory);
  RunIsolatedTest(SetHeadOfFreeChunkListInRealWordMemory);
  RunIsolatedTest(AddObjectToFreeChunkListInItsSegment);
  RunIsolatedTest(RemoveEntryFromCurrentSegmentFreeChunkList);
  RunIsolatedTest(GetNilWhenTryingToRemoveFromEmptyFreeChunkList);
  RunIsolatedTest(ResetAFreeChunkListByWritingANonPointerToItsHead);
}
