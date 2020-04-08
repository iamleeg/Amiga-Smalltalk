#ifndef AMIGA_SMALLTALK_INTERPRETER_CONSTANTS_H
#define AMIGA_SMALLTALK_INTERPRETER_CONSTANTS_H

/**
 * @file Constants defined in the initialization routines (e.g. p575-576) of the interpreter.
 */

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

#endif
