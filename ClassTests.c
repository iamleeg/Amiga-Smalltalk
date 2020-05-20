#include "ClassTests.h"
#include "Interpreter.h"

Test(TestHashOfDifferentObjectsIsDifferent) {
  /* honestly I find it hard to think of a non-exhaustive but meaningful test for a hash function. */
  Word hash1, hash2;
  hash1 = Interpreter_hash(NilPointer);
  hash2 = Interpreter_hash(0x1000);
  Expect(hash1 != hash2);
}

void ClassTests(struct TestResult *tr) {
  RunTest(TestHashOfDifferentObjectsIsDifferent);
}
