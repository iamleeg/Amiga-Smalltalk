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

void Interpreter_transfer_fromIndex_ofObject_toIndex_ofObject(
  Word count,
  Word firstFrom,
  ObjectPointer fromOop,
  Word firstTo,
  ObjectPointer toOop
) {
  Word fromIndex = firstFrom, toIndex = firstTo, lastFrom = firstFrom + count;
  ObjectPointer oop;
  while(fromIndex < lastFrom) {
    oop = ObjectMemory_fetchPointer_ofObject(fromIndex, fromOop);
    ObjectMemory_storePointer_ofObject_withValue(toIndex, toOop, oop);
    ObjectMemory_storePointer_ofObject_withValue(fromIndex, fromOop, NilPointer);
    fromIndex += 1;
    toIndex += 1;
  }
}

Word Interpreter_extractBits_to_of(Word firstBitIndex, Word lastBitIndex, Word anInteger) {
  Word shifted = (anInteger >> (15 - lastBitIndex));
  Word mask = ((1 << (lastBitIndex - firstBitIndex + 1)) - 1);
  return (shifted & mask);
}

Byte Interpreter_highByteOf(Word anInteger) {
  return Interpreter_extractBits_to_of(0, 7, anInteger);
}

Byte Interpreter_lowByteOf(Word anInteger) {
  return Interpreter_extractBits_to_of(8, 15, anInteger);
}

ObjectPointer Interpreter_headerOf(ObjectPointer methodPointer) {
  return ObjectMemory_fetchPointer_ofObject(HeaderIndex, methodPointer);
}

ObjectPointer Interpreter_literal_ofMethod(Word offset, ObjectPointer methodPointer) {
  return ObjectMemory_fetchPointer_ofObject(LiteralStart + offset, methodPointer);
}

Byte Interpreter_temporaryCountOf(ObjectPointer methodPointer) {
  return Interpreter_extractBits_to_of(3, 7, Interpreter_headerOf(methodPointer));
}

Bool Interpreter_largeContextFlagOf(ObjectPointer methodPointer) {
  return Interpreter_extractBits_to_of(8, 8, methodPointer);
}

Byte Interpreter_literalCountOf(ObjectPointer methodPointer) {
  return Interpreter_literalCountOfHeader(Interpreter_headerOf(methodPointer));
}

Byte Interpreter_literalCountOfHeader(ObjectPointer headerPointer) {
  return Interpreter_extractBits_to_of(9, 14, headerPointer);
}

Byte Interpreter_flagValueOf(ObjectPointer methodPointer) {
  return Interpreter_extractBits_to_of(0, 2, Interpreter_headerOf(methodPointer));
}

Byte Interpreter_fieldIndexOf(ObjectPointer methodPointer) {
  return Interpreter_extractBits_to_of(3, 7, Interpreter_headerOf(methodPointer));
}

ObjectPointer Interpreter_headerExtensionOf(ObjectPointer methodPointer) {
  Byte literalCount = Interpreter_literalCountOf(methodPointer);
  return Interpreter_literal_ofMethod(literalCount - 2, methodPointer);
}

Byte Interpreter_argumentCountOf(ObjectPointer methodPointer) {
  Byte flagValue = Interpreter_flagValueOf(methodPointer);
  if (flagValue < 5) return flagValue;
  if (flagValue < 7) return 0;
  return Interpreter_extractBits_to_of(2, 6, Interpreter_headerExtensionOf(methodPointer));
}
