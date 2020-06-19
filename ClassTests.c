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

Test(FindingMethodInDictionaryWithNilKeyFails) {
  ObjectPointer dictionary = 0x2600, location = 0x3006, array = 0x2610, arrayLocation = 0x3016, compiledMethod = 0xf002;
  Word dictionarySize = SelectorStart + HeaderSize + 1, segment = RealWordMemory_bestSegmentFor(3);
  Bool foundMethod;

  ObjectMemory_locationBitsOf_put(dictionary, location);
  ObjectMemory_segmentBitsOf_put(dictionary, segment);
  ObjectMemory_sizeBitsOf_put(dictionary, dictionarySize);
  ObjectMemory_storeWord_ofObject_withValue(SelectorStart, dictionary, NilPointer);

  ObjectMemory_locationBitsOf_put(array, arrayLocation);
  ObjectMemory_segmentBitsOf_put(array, segment);
  ObjectMemory_sizeBitsOf_put(array, HeaderSize + 1);
  ObjectMemory_storeWord_ofObject_withValue(0, array, compiledMethod);

  messageSelector = DoesNotUnderstandSelector;

  foundMethod = Interpreter_lookupMethodInDictionary(dictionary);

  Expect(foundMethod == NO);
}


Test(FindingMethodInDictionaryWithWrongKeyFails) {
  ObjectPointer dictionary = 0x2600, location = 0x3006, array = 0x2610, arrayLocation = 0x3016, compiledMethod = 0xf002, symbol = DoesNotUnderstandSelector;
  Word dictionarySize = SelectorStart + HeaderSize + 1, segment = RealWordMemory_bestSegmentFor(3);
  Bool foundMethod;

  ObjectMemory_locationBitsOf_put(dictionary, location);
  ObjectMemory_segmentBitsOf_put(dictionary, segment);
  ObjectMemory_sizeBitsOf_put(dictionary, dictionarySize);
  ObjectMemory_storeWord_ofObject_withValue(SelectorStart, dictionary, symbol);

  ObjectMemory_locationBitsOf_put(array, arrayLocation);
  ObjectMemory_segmentBitsOf_put(array, segment);
  ObjectMemory_sizeBitsOf_put(array, HeaderSize + 1);
  ObjectMemory_storeWord_ofObject_withValue(0, array, compiledMethod);
  ObjectMemory_storeWord_ofObject_withValue(MethodArrayIndex, dictionary, array);

  messageSelector = symbol + 2;

  foundMethod = Interpreter_lookupMethodInDictionary(dictionary);

  Expect(foundMethod == NO);
}

Test(FindingMethodInDictionaryWithCorrectKeySucceeds) {
  ObjectPointer dictionary = 0x2600, location = 0x3006, array = 0x2610, arrayLocation = 0x3016, compiledMethod = 0xf002, symbol = DoesNotUnderstandSelector;
  Word dictionarySize = SelectorStart + HeaderSize + 1, segment = RealWordMemory_bestSegmentFor(3);
  Bool foundMethod;

  ObjectMemory_locationBitsOf_put(dictionary, location);
  ObjectMemory_segmentBitsOf_put(dictionary, segment);
  ObjectMemory_sizeBitsOf_put(dictionary, dictionarySize);
  ObjectMemory_storeWord_ofObject_withValue(SelectorStart, dictionary, symbol);

  ObjectMemory_locationBitsOf_put(array, arrayLocation);
  ObjectMemory_segmentBitsOf_put(array, segment);
  ObjectMemory_sizeBitsOf_put(array, HeaderSize + 1);
  ObjectMemory_storeWord_ofObject_withValue(0, array, compiledMethod);
  ObjectMemory_storeWord_ofObject_withValue(MethodArrayIndex, dictionary, array);

  messageSelector = symbol;

  foundMethod = Interpreter_lookupMethodInDictionary(dictionary);

  Expect(foundMethod == YES);
  Expect(newMethod == compiledMethod);
}

Test(FindingMethodInDictionaryWithMultipleWrongKeysFails) {
  ObjectPointer dictionary = 0x2600, location = 0x3006, array = 0x2610, arrayLocation = 0x3016, compiledMethod = 0xf002, symbol = DoesNotUnderstandSelector, otherSymbol = 0x1030, otherMethod = 0xf008;
  Word dictionarySize = SelectorStart + HeaderSize + 1, segment = RealWordMemory_bestSegmentFor(3);
  Bool foundMethod;

  ObjectMemory_locationBitsOf_put(dictionary, location);
  ObjectMemory_segmentBitsOf_put(dictionary, segment);
  ObjectMemory_sizeBitsOf_put(dictionary, dictionarySize);
  ObjectMemory_storeWord_ofObject_withValue(SelectorStart, dictionary, symbol);
  ObjectMemory_storeWord_ofObject_withValue(SelectorStart + 1, dictionary, otherSymbol);

  ObjectMemory_locationBitsOf_put(array, arrayLocation);
  ObjectMemory_segmentBitsOf_put(array, segment);
  ObjectMemory_sizeBitsOf_put(array, HeaderSize + 2);
  ObjectMemory_storeWord_ofObject_withValue(0, array, compiledMethod);
  ObjectMemory_storeWord_ofObject_withValue(1, array, otherMethod);
  ObjectMemory_storeWord_ofObject_withValue(MethodArrayIndex, dictionary, array);

  messageSelector = symbol + 2;

  foundMethod = Interpreter_lookupMethodInDictionary(dictionary);

  Expect(foundMethod == NO);
}

Test(LookingUpMethodInClassFindsItInDictionary) {
  ObjectPointer dictionary = 0x2600, location = 0x3006, array = 0x2610, arrayLocation = 0x3016, compiledMethod = 0xf002, symbol = 0x2468;
  ObjectPointer class = 0x2620, classLocation = 0x3030;
  Word dictionarySize = SelectorStart + HeaderSize + 1, segment = RealWordMemory_bestSegmentFor(3);
  Bool foundMethod;

  ObjectMemory_locationBitsOf_put(dictionary, location);
  ObjectMemory_segmentBitsOf_put(dictionary, segment);
  ObjectMemory_sizeBitsOf_put(dictionary, dictionarySize);
  ObjectMemory_storeWord_ofObject_withValue(SelectorStart, dictionary, symbol);

  ObjectMemory_locationBitsOf_put(array, arrayLocation);
  ObjectMemory_segmentBitsOf_put(array, segment);
  ObjectMemory_sizeBitsOf_put(array, HeaderSize + 1);
  ObjectMemory_storeWord_ofObject_withValue(0, array, compiledMethod);
  ObjectMemory_storeWord_ofObject_withValue(MethodArrayIndex, dictionary, array);

  ObjectMemory_locationBitsOf_put(class, classLocation);
  ObjectMemory_segmentBitsOf_put(class, segment);
  ObjectMemory_sizeBitsOf_put(class, 3);
  ObjectMemory_storeWord_ofObject_withValue(MessageDictionaryIndex, class, dictionary);

  messageSelector = symbol;

  foundMethod = Interpreter_lookupMethodInClass(class);

  Expect(foundMethod == YES);
  Expect(newMethod == compiledMethod);
}

void ClassTests(struct TestResult *tr) {
  RunTest(TestHashOfDifferentObjectsIsDifferent);
  RunTest(FindingMethodInEmptyDictionaryFails);
  RunTest(FindingMethodInDictionaryWithNilKeyFails);
  RunTest(FindingMethodInDictionaryWithWrongKeyFails);
  RunTest(FindingMethodInDictionaryWithCorrectKeySucceeds);
  RunTest(FindingMethodInDictionaryWithMultipleWrongKeysFails);
  RunTest(LookingUpMethodInClassFindsItInDictionary);
}
