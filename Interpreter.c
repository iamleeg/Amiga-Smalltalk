#include "Interpreter.h"
#include "ObjectMemory.h"

Bool success = NO;

void Interpreter_success_(Bool successValue) {
  success = successValue && success;
}

Bool Interpreter_success(void) {
  return success;
}

void Interpreter_initPrimitive(void) {
  success = YES;
}

void Interpreter_primitiveFail(void) {
  success = NO;
}

void Interpreter_storeInteger_ofObject_withValue(Word fieldIndex, ObjectPointer objectPointer, Word integerValue) {
  ObjectPointer integerPointer;
  if (ObjectMemory_isIntegerValue(integerValue)) {
    integerPointer = ObjectMemory_integerObjectOf(integerValue);
    ObjectMemory_storePointer_ofObject_withValue(fieldIndex, objectPointer, integerPointer);
  } else {
    Interpreter_primitiveFail();
  }
}

Word Interpreter_fetchInteger_ofObject(Word fieldIndex, ObjectPointer objectPointer) {
  ObjectPointer integerPointer = ObjectMemory_fetchPointer_ofObject(fieldIndex, objectPointer);
  if (ObjectMemory_isIntegerObject(integerPointer)) {
    return ObjectMemory_integerValueOf(integerPointer);
  } else {
    Interpreter_primitiveFail();
    return 0; /* caller shouldn't rely on this */
  }
}