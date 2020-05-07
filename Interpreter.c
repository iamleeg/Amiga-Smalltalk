#include "Interpreter.h"
#include "ObjectMemory.h"

Bool success = NO;
ObjectPointer activeContext = NilPointer;
ObjectPointer homeContext = NilPointer;
ObjectPointer method = NilPointer;
ObjectPointer receiver = NilPointer;
Word instructionPointer = 0;
Word stackPointer = 0;

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

Byte Interpreter_primitiveIndexOf(ObjectPointer methodPointer) {
  Byte flagValue = Interpreter_flagValueOf(methodPointer);
  if (flagValue < 7) return 0;
  return Interpreter_extractBits_to_of(7,14, Interpreter_headerExtensionOf(methodPointer));
}

ObjectPointer Interpreter_methodClassOf(ObjectPointer methodPointer) {
  Byte literalCount;
  ObjectPointer association;
  literalCount = Interpreter_literalCountOf(methodPointer);
  association = Interpreter_literal_ofMethod(literalCount - 1, methodPointer);
  return ObjectMemory_fetchPointer_ofObject(ValueIndex, association);
}

Word Interpreter_instructionPointerOfContext(ObjectPointer contextPointer) {
  return Interpreter_fetchInteger_ofObject(InstructionPointerIndex, contextPointer);
}

void Interpreter_storeInstructionPointerValue_inContext(Word value, ObjectPointer contextPointer) {
  Interpreter_storeInteger_ofObject_withValue(InstructionPointerIndex, contextPointer, value);
}

Word Interpreter_stackPointerOfContext(ObjectPointer contextPointer) {
  return Interpreter_fetchInteger_ofObject(StackPointerIndex, contextPointer);
}

void Interpreter_storeStackPointerValue_inContext(Word value, ObjectPointer contextPointer) {
  Interpreter_storeInteger_ofObject_withValue(StackPointerIndex, contextPointer, value);
}

Word Interpreter_argumentCountOfBlock(ObjectPointer blockPointer) {
  return Interpreter_fetchInteger_ofObject(BlockArgumentCountIndex, blockPointer);
}

Bool Interpreter_isBlockContext(ObjectPointer contextPointer) {
  ObjectPointer methodOrArguments = ObjectMemory_fetchPointer_ofObject(MethodIndex, contextPointer);
  return ObjectMemory_isIntegerObject(methodOrArguments);
}

void Interpreter_fetchContextRegisters(void) {
  if (Interpreter_isBlockContext(activeContext)) {
    homeContext = ObjectMemory_fetchPointer_ofObject(HomeIndex, activeContext);
  } else {
    homeContext = activeContext;
  }
  receiver = ObjectMemory_fetchPointer_ofObject(ReceiverIndex, homeContext);
  method = ObjectMemory_fetchPointer_ofObject(MethodIndex, homeContext);
  instructionPointer = Interpreter_instructionPointerOfContext(activeContext) - 1;
  stackPointer = Interpreter_stackPointerOfContext(activeContext) + TempFrameStart - 1;
}

void Interpreter_storeContextRegisters(void) {
  Interpreter_storeInstructionPointerValue_inContext(instructionPointer + 1, activeContext);
  Interpreter_storeStackPointerValue_inContext(stackPointer - TempFrameStart + 1, activeContext);
}

void Interpreter_push(ObjectPointer object) {
  stackPointer = stackPointer + 1;
  ObjectMemory_storePointer_ofObject_withValue(stackPointer, activeContext, object);
}

ObjectPointer Interpreter_popStack(void) {
  ObjectPointer stackTop = ObjectMemory_fetchPointer_ofObject(stackPointer, activeContext);
  stackPointer = stackPointer - 1;
  return stackTop;
}

ObjectPointer Interpreter_stackTop(void) {
  return ObjectMemory_fetchPointer_ofObject(stackPointer, activeContext);
}

ObjectPointer Interpreter_stackValue(Word offset) {
  return ObjectMemory_fetchPointer_ofObject(stackPointer - offset, activeContext);
}

void Interpreter_pop(Word number) {
  stackPointer = stackPointer - number;
}

void Interpreter_unPop(Word number) {
  stackPointer = stackPointer + number;
}

void Interpreter_newActiveContext(ObjectPointer aContext) {
  Interpreter_storeContextRegisters();
  ObjectMemory_decreaseReferencesTo(activeContext);
  activeContext = aContext;
  ObjectMemory_increaseReferencesTo(activeContext);
  Interpreter_fetchContextRegisters();
}

ObjectPointer Interpreter_sender(void) {
  return ObjectMemory_fetchPointer_ofObject(SenderIndex, activeContext);
}

ObjectPointer Interpreter_caller(void) {
  /*
   * Note in p586 the constant SenderIndex is used, which has the same value
   * as CallerIndex but is supposed to relate to method contexts so I've
   * changed that here.
   */
  return ObjectMemory_fetchPointer_ofObject(CallerIndex, activeContext);
}

ObjectPointer Interpreter_temporary(Word offset) {
  return ObjectMemory_fetchPointer_ofObject(offset + TempFrameStart, homeContext);
}

ObjectPointer Interpreter_literal(Word offset) {
  return Interpreter_literal_ofMethod(offset, method);
}