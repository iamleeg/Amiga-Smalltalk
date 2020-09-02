#ifndef AMIGA_SMALLTALK_INTERPRETERPRIMARITH_H
#define AMIGA_SMALLTALK_INTERPRETERPRIMARITH_H

#include "Types.h"

Bool Interpreter_dispatchArithmeticPrimitives( int primitiveIndex );
Bool Interpreter_dispatchIntegerPrimitives( int primitiveIndex );

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

#endif
