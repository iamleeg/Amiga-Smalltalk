#include "InterpreterMemoryTests.h"

#include "Interpreter.h"
#include "ObjectMemory.h"

Test(RoundTripIntegerThroughObjectMemory) {
  Word integerValue = 12, fetchedInteger, fieldIndex = 2, segment = 3;
  ObjectPointer objectPointer = 0x1000, location = 0x2000;

  Interpreter_initPrimitive(); /* clear the success register before test */
  ObjectMemory_segmentBitsOf_put(objectPointer, segment);
  ObjectMemory_locationBitsOf_put(objectPointer, location);

  Interpreter_storeInteger_ofObject_withValue(fieldIndex, objectPointer, integerValue);

  Expect(Interpreter_success() == YES);

  fetchedInteger = Interpreter_fetchInteger_ofObject(fieldIndex, objectPointer);
  Expect(Interpreter_success() == YES);
  Expect(fetchedInteger == integerValue);
}

void InterpreterMemoryTests(struct TestResult *tr) {
  RunTest(RoundTripIntegerThroughObjectMemory);
}