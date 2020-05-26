#include "ClassTests.h"
#include "Interpreter.h"
#include "ObjectMemory.h"
#include "RealWordMemory.h"

Test(TestHashOfDifferentObjectsIsDifferent) {
  /* honestly I find it hard to think of a non-exhaustive but meaningful test for a hash function. */
  Word hash1, hash2;
  hash1 = Interpreter_hash(NilPointer);
  hash2 = Interpreter_hash(0x1000);
  Expect(hash1 != hash2);
}

Test(FindingMethodInEmptyDictionaryFails) {
  ObjectPointer dictionary = 0x2400, location = 0x3006;
  Word dictionarySize = SelectorStart + HeaderSize, segment = RealWordMemory_bestSegmentFor(3);
  Bool foundMethod;

  ObjectMemory_locationBitsOf_put(dictionary, location);
  ObjectMemory_segmentBitsOf_put(dictionary, segment);
  ObjectMemory_sizeBitsOf_put(dictionary, dictionarySize);

  messageSelector = DoesNotUnderstandSelector;

  foundMethod = Interpreter_lookupMethodInDictionary(dictionary);

  Expect(foundMethod == NO);
}

void ClassTests(struct TestResult *tr) {
  RunTest(TestHashOfDifferentObjectsIsDifferent);
  RunTest(FindingMethodInEmptyDictionaryFails);
}
