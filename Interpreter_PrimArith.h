#ifndef AMIGA_SMALLTALK_INTERPRETERPRIMARITH_H
#define AMIGA_SMALLTALK_INTERPRETERPRIMARITH_H

#include "Types.h"


Bool Interpreter_dispatchArithmeticPrimitives( int primitiveIndex );
Bool Interpreter_dispatchIntegerPrimitives( int primitiveIndex );
Bool Interpreter_dispatchLargeIntegerPrimitives( int primitiveIndex );
Bool Interpreter_dispatchFloatPrimitives( int primitiveIndex );

Bool Interpreter_primitiveAdd(void);
Bool Interpreter_primitiveSubtract(void);
Bool Interpreter_primitiveLessThan(void);
Bool Interpreter_primitiveGreaterThan(void);
Bool Interpreter_primitiveLessOrEqual(void);
Bool Interpreter_primitiveGreaterOrEqual(void);
Bool Interpreter_primitiveEqual(void);
Bool Interpreter_primitiveNotEqual(void);
Bool Interpreter_primitiveMultiply(void);
Bool Interpreter_primitiveDivide(void);
Bool Interpreter_primitiveMod(void);
Bool Interpreter_primitiveDiv(void);
Bool Interpreter_primitiveQuo(void);
Bool Interpreter_primitiveBitAnd(void);
Bool Interpreter_primitiveBitOr(void);
Bool Interpreter_primitiveBitXor(void);
Bool Interpreter_primitiveBitShift(void);
Bool Interpreter_primitiveMakePoint(void);

Bool Interpreter_primitiveAsFloat(void);

Bool Interpreter_primitiveFloatAdd(void);
Bool Interpreter_primitiveFloatSubtract(void);
Bool Interpreter_primitiveFloatLessThan(void);
Bool Interpreter_primitiveFloatGreaterThan(void);
Bool Interpreter_primitiveFloatLessOrEqual(void);
Bool Interpreter_primitiveFloatGreaterOrEqual(void);
Bool Interpreter_primitiveFloatEqual(void);
Bool Interpreter_primitiveFloatNotEqual(void);
Bool Interpreter_primitiveFloatMultiply(void);
Bool Interpreter_primitiveFloatDivide(void);
Bool Interpreter_primitiveTruncated(void);
Bool Interpreter_primitiveFractionalPart(void);
Bool Interpreter_primitiveExponent(void);
Bool Interpreter_primitiveTimesTwoPower(void);

#endif
