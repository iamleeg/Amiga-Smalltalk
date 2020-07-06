#include "Interpreter.h"
#include "Interpreter_Error.h"
#include "ObjectMemory.h"

/**
 * Page 616.
 * Interpreter must keep track of primitive success or failure independently of routine 
 * call structure.  Part of the book specification therefore is a register called success
 * that is initialized to true when the primitive is started and may be set
 * to false if the routine fails.
 */
Bool success = NO;

ObjectPointer activeContext = NilPointer;
ObjectPointer homeContext = NilPointer;
ObjectPointer method = NilPointer;
ObjectPointer receiver = NilPointer;
Word instructionPointer = 0;
Word stackPointer = 0;
ObjectPointer messageSelector = NilPointer;
Word argumentCount = 0;
ObjectPointer newMethod = NilPointer;
Word primitiveIndex = 0;

/**
 * Page 616.
 * The following two routines set and test the state of the primitive success register
 */
void Interpreter_success_(Bool successValue) {
  success = successValue && success;
}

Bool Interpreter_success(void) {
  return success;
}

/**
 * Page 616.
 * The following  routines set the state of the success flag in the two common cases of 
 * initialization before a primitive routine runs and discovery by a primitive 
 * routine that it cannot complete
 */
void Interpreter_initPrimitive(void) {
  success = YES;
}

void Interpreter_primitiveFail(void) {
  success = NO;
}

/** 
 * Page 617
 * Many of the primitives manipulate integer quantities so the interpreter includes 
 * several routines that perform common functions
 */

/**
 * Page 617.  
 * Used when a primitive expects a SmallInteger on top of the stack, if it is a 
 * SmallInteger it's value is returned, if not a primitive failure is signalled.
 */
 
short Interpreter_popInteger(void) {
	ObjectPointer result = Interpreter_popStack();
	Interpreter_success_(ObjectMemory_isIntegerObject(result));
	if( Interpreter_success() ) {
		return ObjectMemory_integerValueOf(result);
	} else {
		Interpreter_primitiveFail();
		return 0; /* caller shouldn't rely on this */
	}
}

void Interpreter_pushInteger(short integerValue) {
	Interpreter_push(ObjectMemory_integerObjectOf(integerValue));
	Interpreter_success_(YES);
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

Word Interpreter_hash(ObjectPointer objectPointer) {
  return (objectPointer >> 1);
}

Bool Interpreter_lookupMethodInDictionary(ObjectPointer dictionary) {
  Word length, index, mask;
  ObjectPointer nextSelector, methodArray;
  Bool wrapAround;

  length = ObjectMemory_fetchWordLengthOf(dictionary);
  mask = length - SelectorStart - 1;
  index = (mask & Interpreter_hash(messageSelector)) + SelectorStart;
  wrapAround = NO;

  while(YES) {
    nextSelector = ObjectMemory_fetchPointer_ofObject(index, dictionary);
    if (nextSelector == NilPointer) {
      return NO;
    }
    if (nextSelector == messageSelector) {
      methodArray = ObjectMemory_fetchPointer_ofObject(MethodArrayIndex, dictionary);
      newMethod = ObjectMemory_fetchPointer_ofObject(index - SelectorStart, methodArray);
      primitiveIndex = Interpreter_primitiveIndexOf(newMethod);
      return YES;
    }
    index = index + 1;
    if (index = length) {
      if (wrapAround != NO) {
        return NO;
      }
      wrapAround = YES;
      index = SelectorStart;
    }
  }
}

Bool Interpreter_lookupMethodInClass(ObjectPointer class) {
  ObjectPointer currentClass, dictionary;
  currentClass = class;
  while(currentClass != NilPointer) {
    dictionary = ObjectMemory_fetchPointer_ofObject(MessageDictionaryIndex, currentClass);
    if (Interpreter_lookupMethodInDictionary(dictionary)) {
      return YES;
    }
    currentClass = Interpreter_superclassOf(currentClass);
  }
  if (messageSelector == DoesNotUnderstandSelector) {
    Interpreter_error("Recursive not understood error encountered");
  }
  Interpreter_createActualMessage();
  messageSelector = DoesNotUnderstandSelector;
  return Interpreter_lookupMethodInClass(class);
}

ObjectPointer Interpreter_superclassOf(ObjectPointer classPointer) {
  return ObjectMemory_fetchPointer_ofObject(SuperClassIndex, classPointer);
}

void Interpreter_createActualMessage() {
  ObjectPointer argumentArray, message;
  argumentArray = ObjectMemory_instantiateClass_withPointers(ClassArrayPointer, argumentCount);
  /*
   * On p589, the next line calls Interpreter_messageSize(), which isn't given. But it's evident
   * that a Message instance has constant size and no indexed variables.
   */
  message = ObjectMemory_instantiateClass_withPointers(ClassMessagePointer, MessageSize);
  ObjectMemory_storePointer_ofObject_withValue(MessageSelectorIndex, message, messageSelector);
  ObjectMemory_storePointer_ofObject_withValue(MessageArgumentsIndex, message, argumentArray);
  Interpreter_transfer_fromIndex_ofObject_toIndex_ofObject(argumentCount, stackPointer - (argumentCount - 1), activeContext, 0, argumentArray);
  Interpreter_pop(argumentCount);
  Interpreter_push(message);
  argumentCount = 1;
}

ObjectPointer Interpreter_instanceSpecificationOf(ObjectPointer classPointer) {
  return ObjectMemory_fetchPointer_ofObject(InstanceSpecificationIndex, classPointer);
}

Bool Interpreter_isPointers(ObjectPointer classPointer) {
  Word pointersFlag = Interpreter_extractBits_to_of(0, 0, Interpreter_instanceSpecificationOf(classPointer));
  return pointersFlag == 1;
}

Bool Interpreter_isWords(ObjectPointer classPointer) {
  Word wordsFlag = Interpreter_extractBits_to_of(1, 1, Interpreter_instanceSpecificationOf(classPointer));
  return wordsFlag == 1;  
}

Bool Interpreter_isIndexable(ObjectPointer classPointer) {
  Word indexableFlag = Interpreter_extractBits_to_of(2, 2, Interpreter_instanceSpecificationOf(classPointer));
  return indexableFlag == 1;
}

Word Interpreter_fixedFieldsOf(ObjectPointer classPointer) {
  return Interpreter_extractBits_to_of(4, 14, Interpreter_instanceSpecificationOf(classPointer));
}
