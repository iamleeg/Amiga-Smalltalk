#ifndef AMIGA_SMALLTALK_INTERPRETER_CONSTANTS_H
#define AMIGA_SMALLTALK_INTERPRETER_CONSTANTS_H

/**
 * @file Constants defined in the initialization routines (e.g. p575-576) of the interpreter.
 */

#include "Types.h"

/**
 * Small Integer constants.
 */
enum {
  MinusOnePointer = 65535,
  ZeroPointer = 1,
  OnePointer = 3,
  TwoPointer = 5
};

/** 
 * Point Indices (p625)
 */
enum {
	XIndex = 0,
	YIndex = 1,
	ClassPointSize = 2
};

/**
 * Guaranteed Pointers.
 */
enum {
  /*ST:  UndefinedObject and Booleans */
  NilPointer = 2,
  FalsePointer = 4,
  TruePointer = 6,
  /*ST:  Root */
  SchedulerAssociationPointer = 8,
  /*ST:  Classes */
  /**
   * The entry in the object table for the CompiledMethod class.
   * @note I've put 1.5 and 1.5 together and come up with 4 here. On p686,
   *       there's a constant called MethodClass which is the class for compiled methods.
   *       Meanwhile, on p576 there's a definition of the "guaranteed pointers", which
   *       doesn't include CompiledMethod. I have taken the liberty of defining it.
   *       Even if you could write CompiledMethod entirely in Smalltalk, the ObjectMemory
   *       needs to know the class entry for CompiledMethods. If we can construct a
   *       GetClassByName() routine in both C and Smalltalk, this isn't needed.
   */
  MethodClass = 10,
  /**
   * The entry in the object table for the SmallInteger class.
   * Again, I've inferred the necessity of this constant.
   */
  ClassSmallInteger = 12,
  ClassStringPointer = 14,
  ClassArrayPointer = 16,
  ClassFloatPointer = 20,
  ClassMethodContextPointer = 22,
  ClassBlockContextPointer = 24,
  ClassPointPointer = 26,
  ClassLargePositiveIntegerPointer = 28,
  ClassMessagePointer = 32,
/*  ClassSelectorPointer = 32,  Could not find this mentioned anywhere */
  ClassCharacterPointer = 40,
  /*ST:  Selectors */
  DoesNotUnderstandSelector = 42,
  CannotReturnSelector = 44,
  MustBeBooleanSelector = 52,
  /*ST:  Tables */
  SpecialSelectorsPointer = 48,
  CharacterTablePointer = 50,
  /**
   * The ObjectMemory needs to know not to add the above pointers to the free pointer list on initialisation.
   */
  FirstUnusedObjectPointer = 52
};

/**
 * These constants are used to find information in the fields of a CompiledMethod.
 * While CompiledMethods are stored as objects in the object table with class MethodClass,
 * the interpreter treats them as unencapsulated data structures.
 */
enum {
  HeaderIndex = 0,
  LiteralStart = 1
};

/**
 * The layout of a CompiledMethod header. This is just for documentation, as the Interpreter
 * bit-twiddling methods are actually used for accessing the fields, and the C compiler
 * might not align everything in the expected way. Adapted from Figure 27.2.
 */
struct CompiledMethodHeader {
  /**
   * The number of arguments, and whether the method has an associated primitive routine.
   * Adapted from the table on pp578-579, the values are:
   * 0-4 no primitive, 0-4 args.
   * 5 0 args, method does nothing but returns self.
   * 6 0 args, method does nothing but returns an ivar. The field index of the ivar to return is stored
   *   in the temporaryCount bits, because the method has no temporaries.
   * 7 see the header extension.
   */
  unsigned int flagValue:3;
  /**
   * The number of temporary variables used by the method, including the arguments.
   */
  unsigned int temporaryCount: 5;
  /**
   * The total context space needed to execute a method is the sum of the maximum stack depth
   * and the number of temporaries. Small contexts have space for 12 object pointers, while
   * large ones have space for 32. This flag is YES if the method needs a large context.
   */
  unsigned int largeContextFlag: 1;
  /**
   * The number of literals used by the method. These are all stored in the literal frame,
   * which is directly after the header in the method's fields. The bytecodes start in the
   * field immediately after the last literal.
   */
  unsigned int literalCount: 6;
  unsigned int isImmediateInteger: 1; /**< always true */
};

/**
 * The layout of a CompiledMethod header extension. This will be found in the last-but-one
 * literal of the CompiledMethod if the flagValue in the method's header is 7. Again, this
 * bitmap is only written out for documentation purposes. Based on figure 27.4.
 */
struct CompiledMethodHeaderExtension {
  /**
   * What would you like me to tell you?
   */
  unsigned int unused:2;
  /**
   * The number of arguments to the primitive method.
   */
  unsigned int argumentCount:5;
  /**
   * The index of the primitive method in the primitives table.
   */
  unsigned int primitiveIndex:8;
  unsigned int isImmediateInteger: 1; /**< always true */
};

/**
 * An Association is an indirect reference to a shared object. Their value field contains
 * the pointer to the shared object. As an example, pool variables are stored in Associations,
 * and all objects in the pool have access to those Associations. If the pool variable changes,
 * the Association gets updated and the pool members will find the new value next time they
 * look in the Association.
 */
enum {
  ValueIndex = 1
};

/**
 * The state of execution of the interpreter is tracked in contexts, either MethodContext or
 * BlockContext depending on what is being executed. They track the instruction pointer,
 * stack variables, temporary variables, and so on. Each has seven fixed fields relevant to
 * the activation frame (though MethodContext only uses six of them) and indexed fields corresponding
 * to the temporaries and stack, but the two contexts use the fixed fields in different ways.
 */
enum {
  /*ST:  Class MethodContext */
  /**
   * The context that sent the message executed by this context. We need it to
   * return a value.
   */
  SenderIndex = 0,
  /**
   * A cursor into the bytecode at the current execution position.
   */
  InstructionPointerIndex = 1,
  /**
   * The top of the stack.
   */
  StackPointerIndex = 2,
  /**
   * The CompiledMethod being executed.
   */
  MethodIndex = 3,
  /* 4 is unused. */
  /**
   * The instance that received this message.
   */
  ReceiverIndex = 5,
  /**
   * The pointer index to the first temporary variable.
   */
  TempFrameStart = 6,

  /*ST:  Class BlockContext */
  /**
   * The context to return to when the block exits.
   */
  CallerIndex = 0,
  /* I.P. and S.P. as MethodContext */
  /**
   * Number of arguments to the block. Block#value, Block#value: and so on
   * supply different numbers of arguments.
   */
  BlockArgumentCountIndex = 3,
  /**
   * The initial instruction pointer value.
   * @see HomeIndex
   */
  InitialIPIndex = 4,
  /**
   * The "home" context is the MethodContext whose CompiledMethod contains this
   * context's block. A block's instructions are compiled inline in their home
   * method, so the home context and the initial I.P. are needed to find the
   * block's bytecode.
   */
  HomeIndex = 5
  /* TempFrameStart as MethodContext */
};

/*ST: Class Class */
enum ClassIndices {
  SuperClassIndex = 0,
  MessageDictionaryIndex = 1,
  InstanceSpecificationIndex = 2,
  /*ST: Fields of a message dictionary */
  MethodArrayIndex = 1,
  SelectorStart = 2
};

/**
 * The indices used to access fields of a Message.
 */
enum MessageIndices {
  /**
   * The message selector.
   */
  MessageSelectorIndex = 0,
  /**
   * The list of arguments to the message.
   */
  MessageArgumentsIndex = 1,
  /**
   * The number of fields in a message.
   */
  MessageSize = 2
};

/**
 * The interpretation of a class instance specification.
 * Adapted from figure 27.8 on p590
 */
struct InstanceSpecificationField {
  /* isPointers implies isWords */
  unsigned int isPointers:1;
  unsigned int isWords:1;
  unsigned int isIndexable:1;
  unsigned int unused:1; /* documented as 0 */
  unsigned int fixedFieldsCount:11;
  unsigned int isImmediateInteger: 1; /* always 1 */
};

#endif
