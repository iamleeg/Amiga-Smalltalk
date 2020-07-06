#include "ClassTests.h"
#include "Interpreter.h"
#include "ObjectMemory.h"
#include "RealWordMemory.h"

static Word segment = 0; /* The only segment we know to exist */

void IdentityDictionaryAtPointer_key_value(ObjectPointer dictionary, ObjectPointer key, ObjectPointer value) {
  Word size = HeaderSize + SelectorStart + 1;
  ObjectPointer location = dictionary + 0x10, array = dictionary + 0x20, arrayLocation = dictionary + 0x30;

  ObjectMemory_locationBitsOf_put(array, arrayLocation);
  ObjectMemory_segmentBitsOf_put(array, segment);
  ObjectMemory_sizeBitsOf_put(array, HeaderSize + 1);
  ObjectMemory_storePointer_ofObject_withValue(0, array, value);

  ObjectMemory_locationBitsOf_put(dictionary, location);
  ObjectMemory_segmentBitsOf_put(dictionary, segment);
  ObjectMemory_sizeBitsOf_put(dictionary, size);
  ObjectMemory_storePointer_ofObject_withValue(SelectorStart, dictionary, key);
  ObjectMemory_storePointer_ofObject_withValue(MethodArrayIndex, dictionary, array);
}

void ClassAtPointer_super_methods(ObjectPointer class, ObjectPointer superclass, ObjectPointer messageDictionary) {
  Word size = 3;
  ObjectPointer location = class + 0x10;

  ObjectMemory_locationBitsOf_put(class, location);
  ObjectMemory_segmentBitsOf_put(class, segment);
  ObjectMemory_sizeBitsOf_put(class, size);
  ObjectMemory_storePointer_ofObject_withValue(SuperClassIndex, class, superclass);
  ObjectMemory_storePointer_ofObject_withValue(MessageDictionaryIndex, class, messageDictionary);
}

Test(TestHashOfDifferentObjectsIsDifferent) {
  /* honestly I find it hard to think of a non-exhaustive but meaningful test for a hash function. */
  Word hash1, hash2;
  hash1 = Interpreter_hash(NilPointer);
  hash2 = Interpreter_hash(0x1000);
  Expect(hash1 != hash2);
}

Test(FindingMethodInEmptyDictionaryFails) {
  ObjectPointer dictionary = 0x2400, location = 0x3006;
  Word dictionarySize = SelectorStart + HeaderSize;
  Bool foundMethod;

  ObjectMemory_locationBitsOf_put(dictionary, location);
  ObjectMemory_segmentBitsOf_put(dictionary, segment);
  ObjectMemory_sizeBitsOf_put(dictionary, dictionarySize);

  messageSelector = DoesNotUnderstandSelector;

  foundMethod = Interpreter_lookupMethodInDictionary(dictionary);

  Expect(foundMethod == NO);
}

Test(FindingMethodInDictionaryWithNilKeyFails) {
  ObjectPointer dictionary = 0x2600, compiledMethod = 0xf002;
  Bool foundMethod;

  IdentityDictionaryAtPointer_key_value(dictionary, NilPointer, compiledMethod);

  messageSelector = DoesNotUnderstandSelector;

  foundMethod = Interpreter_lookupMethodInDictionary(dictionary);

  Expect(foundMethod == NO);
}


Test(FindingMethodInDictionaryWithWrongKeyFails) {
  ObjectPointer dictionary = 0x2600, compiledMethod = 0xf002, symbol = DoesNotUnderstandSelector;
  Bool foundMethod;

  IdentityDictionaryAtPointer_key_value(dictionary, symbol, compiledMethod);

  messageSelector = symbol + 2;

  foundMethod = Interpreter_lookupMethodInDictionary(dictionary);

  Expect(foundMethod == NO);
}

Test(FindingMethodInDictionaryWithCorrectKeySucceeds) {
  ObjectPointer dictionary = 0x2600, compiledMethod = 0xf002, symbol = DoesNotUnderstandSelector;
  Bool foundMethod;

  IdentityDictionaryAtPointer_key_value(dictionary, symbol, compiledMethod);

  messageSelector = symbol;

  foundMethod = Interpreter_lookupMethodInDictionary(dictionary);

  Expect(foundMethod == YES);
  Expect(newMethod == compiledMethod);
}

Test(FindingMethodInDictionaryWithMultipleWrongKeysFails) {
  ObjectPointer dictionary = 0x2600, location = 0x3006, array = 0x2610, arrayLocation = 0x3016, compiledMethod = 0xf002, symbol = DoesNotUnderstandSelector, otherSymbol = 0x1030, otherMethod = 0xf008;
  Word dictionarySize = SelectorStart + HeaderSize + 2;
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
  ObjectPointer dictionary = 0x2600, compiledMethod = 0xf002, symbol = 0x2468;
  ObjectPointer class = 0x3000;
  Bool foundMethod;

  IdentityDictionaryAtPointer_key_value(dictionary, symbol, compiledMethod);
  ClassAtPointer_super_methods(class, NilPointer, dictionary);

  messageSelector = symbol;

  foundMethod = Interpreter_lookupMethodInClass(class);

  Expect(foundMethod == YES);
  Expect(newMethod == compiledMethod);
}

Test(LookingUpMethodInSuperclassDictionary) {
  ObjectPointer dictionary = 0x2600, compiledMethod = 0xf002, symbol = 0x2468;
  ObjectPointer class = 0x3000, classLocation = 0x3030, subclass = 0x2820, subclassLocation = 0x3230, subdictionary = 0x2800;
  ObjectPointer wrongSymbol = 0x2668, wrongMethod = 0x3430;
  Word dictionarySize = SelectorStart + HeaderSize + 1;
  Bool foundMethod;

  IdentityDictionaryAtPointer_key_value(dictionary, symbol, compiledMethod);
  ClassAtPointer_super_methods(class, NilPointer, dictionary);

  IdentityDictionaryAtPointer_key_value(subdictionary, wrongSymbol, wrongMethod);
  ClassAtPointer_super_methods(subclass, class, subdictionary);

  messageSelector = symbol;

  foundMethod = Interpreter_lookupMethodInClass(subclass);

  Expect(foundMethod == YES);
  Expect(newMethod == compiledMethod);
}

Test(GetClassInstanceSpecification) {
  ObjectPointer class = 0x3008, location = 0x200c, instanceSpecification;
  Word segment = 0, integer = MinusOnePointer, size = 5;

  ObjectMemory_locationBitsOf_put(class, location);
  ObjectMemory_segmentBitsOf_put(class, segment);
  ObjectMemory_sizeBitsOf_put(class, size);
  ObjectMemory_storeWord_ofObject_withValue(InstanceSpecificationIndex, class, integer);

  instanceSpecification = Interpreter_instanceSpecificationOf(class);

  Expect(instanceSpecification == MinusOnePointer);
}

void ClassTests(struct TestResult *tr) {
  RunTest(TestHashOfDifferentObjectsIsDifferent);
  RunTest(FindingMethodInEmptyDictionaryFails);
  RunTest(FindingMethodInDictionaryWithNilKeyFails);
  RunTest(FindingMethodInDictionaryWithWrongKeyFails);
  RunTest(FindingMethodInDictionaryWithCorrectKeySucceeds);
  RunTest(FindingMethodInDictionaryWithMultipleWrongKeysFails);
  RunTest(LookingUpMethodInClassFindsItInDictionary);
  RunTest(LookingUpMethodInSuperclassDictionary);
  RunTest(GetClassInstanceSpecification);
}
