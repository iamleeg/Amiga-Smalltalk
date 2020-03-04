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

void FreeListTests(struct TestResult *tr) {
  RunTest(test_RetrieveHeadOfFreePointerListFromObjectTable);
  RunTest(test_SetHeadOfFreePointerListInObjectTable);
}
