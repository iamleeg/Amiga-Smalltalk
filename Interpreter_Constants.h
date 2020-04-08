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
  TwoPointer = 5,
};

/**
 * Guaranteed Pointers.
 */
enum {
  // UndefinedObject and Booleans
  NilPointer = 2,
  FalsePointer = 4,
  TruePointer = 6,
  // Root
  SchedulerAssociationPointer = 8,
  // Classes
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
  IntegerClass = 12,
  ClassStringPointer = 14,
  ClassArrayPointer = 16,
  ClassMethodContextPointer = 22,
  ClassBlockContextPointer = 24,
  ClassPointPointer = 26,
  ClassLargePositiveIntegerPointer = 28,
  ClassMessagePointer = 30,
  ClassSelectorPointer = 32,
  ClassCharacterPointer = 40,
  // Selectors
  DoesNotUnderstandSelector = 42,
  CannotReturnSelector = 44,
  MustBeBooleanSelector = 52,
  // Tables
  SpecialSelectorsPointer = 48,
  CharacterTablePointer = 50,
};

/**
 * These constants are used to find information in the fields of a CompiledMethod.
 * While CompiledMethods are stored as objects in the object table with class MethodClass,
 * the interpreter treats them as unencapsulated data structures.
 */
enum {
  HeaderIndex = 0,
  LiteralStart = 1,
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
   * 5 0 args, primitive that returns self
   * 6 0 args, primitive that returns an ivar. The field index of the ivar to return is stored
   *   in the temporaryCount bits, because the method has no temporaries.
   * 7 see the header extension
   */
  Word flagValue:3;
  /**
   * The number of temporary variables used by the method, including the arguments.
   */
  Word temporaryCount: 5;
  /**
   * The total context space needed to execute a method is the sum of the maximum stack depth
   * and the number of temporaries. Small contexts have space for 12 object pointers, while
   * large ones have space for 32. This flag is YES if the method needs a large context.
   */
  Word largeContextFlag: 1;
  /**
   * The number of literals used by the method. These are all stored in the literal frame,
   * which is directly after the header in the method's fields. The bytecodes start in the
   * field immediately after the last literal.
   */
  Word literalCount: 6;
  Word isImmediateInteger: 1; /**< always true */
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
  Word unused:2;
  /**
   * The number of arguments to the primitive method.
   */
  Word argumentCount:5;
  /**
   * The index of the primitive method in the primitives table.
   */
  Word primitiveIndex:8;
  Word isImmediateInteger: 1; /**< always true */
};

#endif
