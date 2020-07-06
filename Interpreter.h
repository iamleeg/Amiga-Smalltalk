#ifndef AMIGA_SMALLTALK_INTERPRETER_H
#define AMIGA_SMALLTALK_INTERPRETER_H

#include "Types.h"

#include "Interpreter_Constants.h"

/**
 * This register indicates whether a primitive method succeeded.
 * @note if the interpreter were written in Smalltalk, it would be possible to tell that failure
 *       has been returned from a method rather than a real value. But it isn't, and because most
 *       methods return object pointers (which are 16-bit words) or integers (same), there's no
 *       sentinel value that can be used. Additionally, we don't want to use longjmp() or a
 *       similar fake exception mechanism, because we need Smalltalk, not C, to recover from
 *       primitive method failures. We could use an out error parameter, but that would have a pervasive
 *       impact on the signatures of routines across the system. Therefore the official, supported way in
 *       this Smalltalk to test whether a primitive method failed is to check the value of the success
 *       register on return. If it's NO, then you shouldn't rely on the returned value.
 */
extern Bool success;

/* The register comments here _are_ quotes from p583 hence line comments. */

/**ST: This is the active context itself. It is either a MethodContext or a BlockContext. */
extern ObjectPointer activeContext;

/**ST: If the active context is a MethodContext, the home context is the same context. If the active context is */
/**ST: a BlockContext, the home context is the contents of the home field of the active context. This will */
/**ST: always be a MethodContext. */
extern ObjectPointer homeContext;

/**ST: This is the CompiledMethod that contains the bytecodes the interpreter is executing. */
extern ObjectPointer method;

/**ST: This is the object that received the message that invoked the home context's method. */
extern ObjectPointer receiver;

/**ST: This is the byte index of the next bytecode of the method to be executed. */
extern Word instructionPointer;

/**ST: This is the index of the field of the active context containing the top of the stack. */
extern Word stackPointer;

/* The register comments here are quotes from p587. */

/**ST: This is the selector of the message being sent. It is always a Symbol. */
extern ObjectPointer messageSelector;

/**ST: This is the number of arguments in the message currently being sent. It indicates where the message */
/**ST: receiver can be found on the stack since it is below the arguments. */
extern Word argumentCount;

/**ST: This is the method associated with the messageSelector. */
extern ObjectPointer newMethod;

/**ST: This is the index of a primitive routine associated with newMethod if one exists. */
extern Word primitiveIndex;

/**
 * Set the success register.
 * @note Actually ANDs the existing and new value, so if you previously failed, you will never un-fail.
 */
void Interpreter_success_(Bool successValue);

/**
 * Retrieve the success register.
 */
Bool Interpreter_success(void);

/**
 * Get ready to enter a primitive method.
 */
void Interpreter_initPrimitive(void);

/**
 * Indicate that execution of a primitive method failed.
 */
void Interpreter_primitiveFail(void);

/**
 * Convert the supplied integer into a SmallInteger object and store it in the object's field.
 * Fails if the integer cannot be stored in a SmallInteger (i.e. is higher magnitude than ±2^14)
 */
void Interpreter_storeInteger_ofObject_withValue(Word fieldIndex, ObjectPointer objectPointer, Word integerValue);

/**
 * Convert the SmallInteger object in the object's field to an integer value and return it.
 * Fails if the object pointer retrieved isn't an immediate integer.
 */
Word Interpreter_fetchInteger_ofObject(Word fieldIndex, ObjectPointer objectPointer);

/**
 * Transfer ownership of a collection of pointers between objects, setting the source fields to nil.
 */
void Interpreter_transfer_fromIndex_ofObject_toIndex_ofObject(
  Word count,
  Word firstFrom,
  ObjectPointer fromOop,
  Word firstTo,
  ObjectPointer toOop
);

/**
 * Extract a bit field from an integer value (not a pointer to an immediate integer, or other integer object).
 * The bits in a word are numbered 0 (most significant) to 15 (least significant), consistent with p575.
 */
Word Interpreter_extractBits_to_of(Word firstBitIndex, Word lastBitIndex, Word anInteger);

/**
 * Extract bits 0-7 of an integer word.
 */
Byte Interpreter_highByteOf(Word anInteger);

/**
 * Extract bits 8-15 of an integer word.
 */
Byte Interpreter_lowByteOf(Word anInteger);

/**
 * Find the header of a CompiledMethod. This is a SmallInteger used as a bitfield to describe the method.
 */
ObjectPointer Interpreter_headerOf(ObjectPointer methodPointer);

/**
 * Find the object literal at a given index in this CompiledMethod's literal frame.
 */
ObjectPointer Interpreter_literal_ofMethod(Word offset, ObjectPointer methodPointer);

/**
 * Extract the count of temporary object pointers from the CompiledMethod header.
 */
Byte Interpreter_temporaryCountOf(ObjectPointer methodPointer);

/**
 * Report whether a CompiledMethod needs a large method context.
 */
Bool Interpreter_largeContextFlagOf(ObjectPointer methodPointer);

/**
 * Extract the size of this CompiledMethod's literal frame.
 */
Byte Interpreter_literalCountOf(ObjectPointer methodPointer);

/**
 * Extract the size of a CompiledMethod's literal frame from an immediate integer representing its header.
 */
Byte Interpreter_literalCountOfHeader(ObjectPointer headerPointer);

/**
 * Extract the flag value of a CompiledMethod (see the discussion in Interpreter_Constants.h).
 * @see struct CompiledMethodHeader
 */
Byte Interpreter_flagValueOf(ObjectPointer methodPointer);

/**
 * Extract the field index of the ivar to return from a primitive CompiledMethod.
 * @note this value only makes sense if the CompiledMethod represents a primitive method that returns an
 *       instance variable from the receiver, i.e. if the CompiledMethod's flagValue is 6.
 */
Byte Interpreter_fieldIndexOf(ObjectPointer methodPointer);

/**
 * Read the header extension from a CompiledMethod that has one.
 * @note this value only makes sense when the CompiledMethod's flagValue is 7.
 */
ObjectPointer Interpreter_headerExtensionOf(ObjectPointer methodPointer);

/**
 * Return the number of arguments required by the CompiledMethod.
 */
Byte Interpreter_argumentCountOf(ObjectPointer methodPointer);

/**
 * Return the index in the primitive method table of the associated primitive method.
 * @note most methods don't have an associated primitive method, so this routine returns 0
 *       in the common case.
 */
Byte Interpreter_primitiveIndexOf(ObjectPointer methodPointer);

/**
 * Methods that call super, and methods that have an associated primitive method, also have
 * an associated class on which this method was defined. Return that class.
 */
ObjectPointer Interpreter_methodClassOf(ObjectPointer methodPointer);

/**
 * Given a method or block context, fetch its instruction pointer.
 */
Word Interpreter_instructionPointerOfContext(ObjectPointer contextPointer);

/**
 * Update the instruction pointer in a method or block context.
 */
void Interpreter_storeInstructionPointerValue_inContext(Word value, ObjectPointer contextPointer);

/**
 * Fetch the stack pointer from a method or block context.
 */
Word Interpreter_stackPointerOfContext(ObjectPointer contextPointer);

/**
 * Update the stack pointer of a method or block context.
 */
void Interpreter_storeStackPointerValue_inContext(Word value, ObjectPointer contextPointer);

/**
 * Retrieve the number of arguments to the block.
 * @note The answer is invalid if you try to use this method to get the argument count from a method context.
 */
Word Interpreter_argumentCountOfBlock(ObjectPointer blockPointer);

/**
 * Test whether this context is a block context.
 */
Bool Interpreter_isBlockContext(ObjectPointer contextPointer);

/**
 * Populate the context registers (stack pointer, instruction pointer, method, receiver, home context) from
 * the active context. The receiver and method are always populated from the home context, the SP and IP
 * always from the active context.
 * @note In practice the IP is stored with an offset of 1 because Smalltalk's at: method uses one-based indexes,
 *       and the SP is stored with an offset of 1 + the temporary frame because Smalltalk's indexed ivar
 *       access takes the length of the fixed fields into account, and is one-based too.
 */
void Interpreter_fetchContextRegisters(void);

/**
 * Store the SP and IP in the active context, to prepare for a context change.
 */
void Interpreter_storeContextRegisters(void);

/**
 * Push an object onto the active context's stack.
 */
void Interpreter_push(ObjectPointer object);

/**
 * Pop the active context's stack and return the popped object.
 * @note There is no safety check here, bad things will happen if you pop past the bottom of the stack.
 */
ObjectPointer Interpreter_popStack(void);

/**
 * See the object at the top of the active context's stack without popping it.
 */
ObjectPointer Interpreter_stackTop(void);

/**
 * See the object at a particular depth on the active context's stack without modifying the stack.
 * @note The offset here uses zero-based indexing, so Interpreter_stackValue(0) == Interpreter_stackTop().
 * @see Interpreter_stackTop
 */
ObjectPointer Interpreter_stackValue(Word offset);

/**
 * Adjust the stack pointer to remove the top number elements from the stack.
 * @note This operation doesn't remove the pointer references from the "popped" elements, so
 *       the garbage collector will not think that they have been relinquished. The operation
 *       is potentially unsafe; it is up to the caller to ensure that the SP still points to
 *       a location within the stack.
 */
void Interpreter_pop(Word number);

/**
 * Adjust the stack pointer to unpop a number of elements back into the stack.
 * @note This operation is potentially unsafe; it's up to the caller to ensure that the SP
 *       still points to a location within the stack.
 */
void Interpreter_unPop(Word number);

/**
 * Change active contexts. This stores the IP and SP in the context we're leaving, and retrieves
 * the register values from the context we're activating.
 */
void Interpreter_newActiveContext(ObjectPointer aContext);

/**
 * Fetch the sender - the context object that caused the current context to be activated (and thus
 * the context to return to).
 */
ObjectPointer Interpreter_sender(void);

/**
 * Fetch the caller - the context object that caused the current block context to be activated (and thus
 * the context to return to).
 */
ObjectPointer Interpreter_caller(void);

/**
 * Fetch the temporary variable at the given offset in the home context (because a block has access to
 * the same temporaries as its enclosing method).
 * @note There is no safety check that offset is within the bounds of the temporary frame.
 */
ObjectPointer Interpreter_temporary(Word offset);

/**
 * Fetch the literal value at the given offset in the active method.
 * @note There is no safety check that offset is within the bounds of the method literals.
 */
ObjectPointer Interpreter_literal(Word offset);

/**
 * Calculate a prehash for a Symbol object to choose its location in a dictionary.
 */
Word Interpreter_hash(ObjectPointer objectPointer);

/**
 * Find a method in a class's method dictionary. This routine reads the message selector from
 * the messageSelector register. On success, it sets the newMethod register to the found
 * CompiledMethod instance, and the primitiveIndex register to that method's primitive index.
 * On failure, it does nothing.
 * @return A Boolean indicating whether it found the method.
 */
Bool Interpreter_lookupMethodInDictionary(ObjectPointer dictionary);

/**
 * Find the method implementation that the given class associates with a selector.
 * The selector is read from the messageSelector register, and the method is stored in
 * newMethod (and primitiveIndex is set if necessary). If the requested method doesn't
 * exist, then the interpreter looks through the chain of superclasses. If no parent class
 * implements the method, then the interpreter restarts the search, for the doesNotUnderstand:
 * selector. A functioning Smalltalk-80 MUST provide an implementation in the root
 * (Object) class for doesNotUnderstand: and the interpreter will report an error if it
 * cannot find doesNotUnderstand:.
 * @return YES
 */
Bool Interpreter_lookupMethodInClass(ObjectPointer class);

/**
 * Return the current class's superclass. The root class's superclass is NilPointer, but that's
 * up to your Smalltalk-80 implementation to arrange, not the interpreter.
 */
ObjectPointer Interpreter_superclassOf(ObjectPointer classPointer);

/**
 * If the interpreter can't find a method on a class (or any of its superclasses), then it sends
 * doesNotUnderstand: to the receiver. It needs to package up the original method selector and
 * arguments in a Message object, which becomes the single argument to doesNotUnderstand:.
 * This routine creates that Message, and pushes it onto the active context's stack.
 */
void Interpreter_createActualMessage();

/**
 * A class's instance specification describes what fixed fields instances of that class have, and
 * how they are addressed. They are stored in an immediate integer object.
 */
ObjectPointer Interpreter_instanceSpecificationOf(ObjectPointer classPointer);

/**
 * Whether this class's instance fields are object pointers.
 * @note If they are, then this class's instance fields must also be word- not byte-addressed.
 */
Bool Interpreter_isPointers(ObjectPointer classPointer);

/**
 * Whether this class's instance fields are addressed by word indices.
 * If not, they are addressed by bytes.
 * @note ST (p591) the instance specification of CompiledMethod does not accurate reflect the
 *       structure of its instances since CompiledMethods are not homogeneous. The instance
 *       specification says that the instances do not contain pointers and are addressed by
 *       bytes. This is true of the bytecode section of a CompiledMethod only. The storage
 *       manager needs to know that CompiledMethods are special and actually contain some
 *       pointers. For all other classes, the instance specification is accurate.
 */
Bool Interpreter_isWords(ObjectPointer classPointer);

/**
 * Whether this class's instances have indexable fields beyond their fixed fields.
 * @note if you wanted to implement a collection like Set or OrderedCollection, you might track
 *       the count in a fixed field, then use indexed fields to refer to the content, via
 *       the at: and at:put: messages.
 */
Bool Interpreter_isIndexable(ObjectPointer classPointer);

/**
 * The number of fixed fields owned by instances of this class.
 */
Word Interpreter_fixedFieldsOf(ObjectPointer classPointer);

#endif
