#include "RefCountingTests.h"
#include "ObjectMemory.h"

Test(IncrementCountOfRegularObject) {
  ObjectPointer objectPointer = 0x1234, incremented = 0;
  Word count = 5;
  ObjectMemory_countBitsOf_put(objectPointer, count);

  incremented = ObjectMemory_countUp(objectPointer);

  Expect(incremented == objectPointer);
  Expect(ObjectMemory_countBitsOf(objectPointer) == count + 1);
}

Test(DoNotIncrementOverflownCount) {
  ObjectPointer objectPointer = 0x1234, incremented = 0;
  Word count = 128;
  ObjectMemory_countBitsOf_put(objectPointer, count);

  incremented = ObjectMemory_countUp(objectPointer);

  Expect(incremented == objectPointer);
  Expect(ObjectMemory_countBitsOf(objectPointer) == count);
}

Test(ReturnPointerToImmediateInteger) {
  ObjectPointer objectPointer = 0x1233, incremented = 0;

  incremented = ObjectMemory_countUp(objectPointer);

  Expect(incremented == objectPointer);
}

void RefCountingTests(struct TestResult *tr) {
  RunIsolatedTest(IncrementCountOfRegularObject);
  RunIsolatedTest(DoNotIncrementOverflownCount);
  RunIsolatedTest(ReturnPointerToImmediateInteger);
}
