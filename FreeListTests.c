#include "FreeListTests.h"
#include "ObjectMemory.h"
#include "RealWordMemory.h"
#include "Types.h"

short test_RetrieveHeadOfFreePointerListFromObjectTable() {
  Word value = 0x37b5, result = 0;
  RealWordMemory_segment_word_put(ObjectTableSegment,
    FreePointerList,
    value);
  result = ObjectMemory_headOfFreePointerList();
  return (result == value);
}

short test_SetHeadOfFreePointerListInObjectTable() {
  Word value = 0x57b3, result = 0;
  ObjectMemory_headOfFreePointerList_put(value);
  result = RealWordMemory_segment_word(ObjectTableSegment,
    FreePointerList);
  return (result == value);
}

short test_AppendingEntryToFreePointerList() {
  Word object1 = 0x0002, object2 = 0x0004, retrievedHead = 0, retrievedNext = 0;
  ObjectMemory_headOfFreePointerList_put(object1);
  ObjectMemory_toFreePointerListAdd(object2);
  retrievedHead = ObjectMemory_headOfFreePointerList();
  retrievedNext = ObjectMemory_locationBitsOf(retrievedHead);
  return ((retrievedHead == object2) && (retrievedNext == object1));
}

void FreeListTests(struct TestResult *tr) {
  RunTest(test_RetrieveHeadOfFreePointerListFromObjectTable);
  RunTest(test_SetHeadOfFreePointerListInObjectTable);
  RunTest(test_AppendingEntryToFreePointerList);
}
