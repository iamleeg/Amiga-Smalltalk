#include "AllocationTests.h"
#include "ObjectMemory_Allocation.h"
#include "ObjectMemory_FreeList.h"
#include "ObjectMemory_Storage.h"

Test(ReleasePointer) {
  ObjectPointer objectPointer = 0x3580, freeHead = 0x8530;
  ObjectMemory_headOfFreePointerList_put(freeHead);
  ObjectMemory_freeBitOf_put(objectPointer, 0);

  ObjectMemory_releasePointer(objectPointer);

  Expect(ObjectMemory_freeBitOf(objectPointer) == 1);
  Expect(ObjectMemory_headOfFreePointerList() == objectPointer);
}

void AllocationTests(struct TestResult *tr) {
  RunTest(ReleasePointer);
}
