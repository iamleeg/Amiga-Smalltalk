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
  Word value = 0x37b5, size = 0x24, segment = 3, result = 0;
  RealWordMemory_segment_word_put(segment,
    FirstFreeChunkList + size,
    value);
  result = ObjectMemory_headOfFreeChunkList_inSegment(size, segment);
  Expect(result == value);
}

Test(SetHeadOfFreeChunkListInRealWordMemory) {
  Word value = 0x37b5, size = 0x24, segment = 3, result = 0;
  ObjectMemory_headOfFreeChunkList_inSegment_put(size, segment, value);
  result = RealWordMemory_segment_word(segment,
    FirstFreeChunkList + size);
  Expect(result == value);
}

void FreeListTests(struct TestResult *tr) {
  RunTest(RetrieveHeadOfFreePointerListFromObjectTable);
  RunTest(SetHeadOfFreePointerListInObjectTable);
  RunTest(AppendingEntryToFreePointerList);
  RunTest(RemovingEntryFromFreePointerList);
  RunTest(GettingNilWhenTryingToRemoveFromEmptyFreePointerList);
  RunTest(RetrieveHeadOfFreeChunkListFromRealWordMemory);
  RunTest(SetHeadOfFreeChunkListInRealWordMemory);
}
