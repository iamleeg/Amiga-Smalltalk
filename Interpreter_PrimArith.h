#ifndef AMIGA_SMALLTALK_INTERPRETERPRIMARITH_H
#define AMIGA_SMALLTALK_INTERPRETERPRIMARITH_H

#include "Types.h"


void Interpreter_dispatchArithmeticPrimitives( int primitiveIndex );
void Interpreter_dispatchIntegerPrimitives( int primitiveIndex );
void Interpreter_dispatchLargeIntegerPrimitives( int primitiveIndex );
void Interpreter_dispatchFloatPrimitives( int primitiveIndex );


void Interpreter_primitiveAdd(void);
void Interpreter_primitiveSubtract(void);
void Interpreter_primitiveLessThan(void);
void Interpreter_primitiveGreaterThan(void);
void Interpreter_primitiveLessOrEqual(void);
void Interpreter_primitiveGreaterOrEqual(void);
void Interpreter_primitiveEqual(void);
void Interpreter_primitiveNotEqual(void);
void Interpreter_primitiveMultiply(void);
void Interpreter_primitiveDivide(void);
void Interpreter_primitiveMod(void);
void Interpreter_primitiveDiv(void);
void Interpreter_primitiveQuo(void);
void Interpreter_primitiveBitAnd(void);
void Interpreter_primitiveBitOr(void);
void Interpreter_primitiveBitXor(void);
void Interpreter_primitiveBitShift(void);
void Interpreter_primitiveMakePoint(void);

void Interpreter_primitiveAsFloat(void);

void Interpreter_primitiveFloatAdd(void);
void Interpreter_primitiveFloatSubtract(void);
void Interpreter_primitiveFloatLessThan(void);
void Interpreter_primitiveFloatGreaterThan(void);
void Interpreter_primitiveFloatLessOrEqual(void);
void Interpreter_primitiveFloatGreaterOrEqual(void);
void Interpreter_primitiveFloatEqual(void);
void Interpreter_primitiveFloatNotEqual(void);
void Interpreter_primitiveFloatMultiply(void);
void Interpreter_primitiveFloatDivide(void);
void Interpreter_primitiveTruncated(void);
void Interpreter_primitiveFractionalPart(void);
void Interpreter_primitiveExponent(void);
void Interpreter_primitiveTimesTwoPower(void);

#endif
