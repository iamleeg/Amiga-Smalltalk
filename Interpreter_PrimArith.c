#include "Interpreter.h"
#include "Interpreter_PrimArith.h"
#include <stdio.h>

Bool Interpreter_dispatchArithmeticPrimitives( int primitiveIndex ) {
	if( primitiveIndex < 20 ) {
		return Interpreter_dispatchIntegerPrimitives( primitiveIndex );
	}
/*	if( primitiveIndex < 40 ) {
		return Interpreter_dispatchLargeIntegerPrimitives( primitiveIndex );
	}
	if( primitiveIndex < 60 ) {
		return Interpreter_dispatchFloatPrimitives( primitiveIndex );
	}*/
	return NO;
}

Bool Interpreter_dispatchIntegerPrimitives( int primitiveIndex ) {
	switch( primitiveIndex ) {
		case 1: return Interpreter_primitiveAdd();
		case 2: return Interpreter_primitiveSubtract();
		case 3: return Interpreter_primitiveLessThan();
		case 4: return Interpreter_primitiveGreaterThan();
		case 5: return Interpreter_primitiveLessOrEqual();
		case 6: return Interpreter_primitiveGreaterOrEqual();
		case 7: return Interpreter_primitiveEqual();
		case 8: return Interpreter_primitiveNotEqual();
		case 9: return Interpreter_primitiveMultiply();
		case 10: return Interpreter_primitiveDivide();
		case 11: return Interpreter_primitiveMod();
		case 12: return Interpreter_primitiveDiv();
		case 13: return Interpreter_primitiveQuo();
		case 14: return Interpreter_primitiveBitAnd();
		case 15: return Interpreter_primitiveBitOr();
		case 16: return Interpreter_primitiveBitXor();
		case 17: return Interpreter_primitiveBitShift();	
		case 18: return Interpreter_primitiveMakePoint();	
	}
	return NO;
}

Bool Interpreter_primitiveAdd(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	short integerResult = 0;
	if( Interpreter_success() == YES ) {
		integerResult = integerReceiver + integerArgument;
		Interpreter_success_( ObjectMemory_isIntegerValue(integerResult ) );
	}
	if( Interpreter_success() == YES ) {
		Interpreter_pushInteger(integerResult);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

Bool Interpreter_primitiveSubtract(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	short integerResult = 0;
	if( Interpreter_success() == YES ) {
		integerResult = integerReceiver - integerArgument;
		Interpreter_success_( ObjectMemory_isIntegerValue(integerResult ) );
	}
	if( Interpreter_success() == YES ) {
		Interpreter_pushInteger(integerResult);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

Bool Interpreter_primitiveLessThan(void) {
return NO;
}

Bool Interpreter_primitiveGreaterThan(void) {
return NO;
}

Bool Interpreter_primitiveLessOrEqual(void) {
return NO;
}

Bool Interpreter_primitiveGreaterOrEqual(void) {
return NO;
}

Bool Interpreter_primitiveEqual(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	Bool boolResult = NO;
	if( Interpreter_success() == YES ) {
		boolResult = (integerReceiver == integerArgument);
	}
	if( Interpreter_success() == YES ) {
		Interpreter_push(boolResult ? TruePointer : FalsePointer);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

Bool Interpreter_primitiveNotEqual(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	Bool boolResult = NO;
	if( Interpreter_success() == YES ) {
		boolResult = (integerReceiver != integerArgument);
	}
	if( Interpreter_success() == YES ) {
		Interpreter_push(boolResult ? TruePointer : FalsePointer);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

Bool Interpreter_primitiveMultiply(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	short integerResult = 0;
	if( Interpreter_success() == YES ) {
		integerResult = integerReceiver * integerArgument;
		Interpreter_success_( ObjectMemory_isIntegerValue(integerResult ) );
	}
	if( Interpreter_success() == YES ) {
		Interpreter_pushInteger(integerResult);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

Bool Interpreter_primitiveDivide(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	short integerResult = 0;
	short integerRemainder = 0;

	/* We only succeed of an integer division can happen cleanly */
	if( Interpreter_success() == YES ) {
		integerRemainder = integerReceiver % integerArgument;
		Interpreter_success_( integerRemainder == 0 );
	}
	
	if( Interpreter_success() == YES ) {	
		integerResult = integerReceiver / integerArgument;
		Interpreter_success_( ObjectMemory_isIntegerValue(integerResult ) );
	}
	
	if( Interpreter_success() == YES ) {
		Interpreter_pushInteger(integerResult);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

Bool Interpreter_primitiveMod(void) {
return NO;
}

Bool Interpreter_primitiveDiv(void) {
return NO;
}

Bool Interpreter_primitiveQuo(void) {
return NO;
}

Bool Interpreter_primitiveBitAnd(void) {
return NO;
}

Bool Interpreter_primitiveBitOr(void) {
return NO;
}

Bool Interpreter_primitiveBitXor(void) {
return NO;
}

Bool Interpreter_primitiveBitShift(void) {
return NO;
}

Bool Interpreter_primitiveMakePoint(void) {
return NO;
}

