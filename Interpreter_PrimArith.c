#include "Interpreter.h"
#include "Interpreter_PrimArith.h"
#include "ObjectMemory.h"
#include "ObjectMemory_Allocation.h"

/**
 * Page 621
 */
Bool Interpreter_dispatchArithmeticPrimitives( int primitiveIndex ) {
	if( primitiveIndex < 20 ) {
		return Interpreter_dispatchIntegerPrimitives( primitiveIndex );
	}
	if( primitiveIndex < 40 ) {
		return Interpreter_dispatchLargeIntegerPrimitives( primitiveIndex );
	}
	if( primitiveIndex < 60 ) {
		return Interpreter_dispatchFloatPrimitives( primitiveIndex );
	}
	return NO;
}

/**
 * Page 621
 */
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

/** 
 *  Should be implemented in Smalltalk instead  page 625
 */
Bool Interpreter_dispatchLargeIntegerPrimitives( int primitiveIndex ) {
	Interpreter_primitiveFail();
	return NO;
}
/**
 * Page 626
 */
Bool Interpreter_dispatchFloatPrimitives( int primitiveIndex ) {
	switch( primitiveIndex ) {
		case 40: return Interpreter_primitiveAsFloat();
	}
	return NO;
}


/**
 * page 622
 */
Bool Interpreter_primitiveAdd(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	short integerResult = 0;
	if( Interpreter_success() == YES ) {
		integerResult = integerReceiver + integerArgument;
		Interpreter_success_( ObjectMemory_isIntegerValue( integerResult ) );
	}
	if( Interpreter_success() == YES ) {
		Interpreter_pushInteger(integerResult);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

/**
 * page 622
 */
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

/**
 * page 624
 */
Bool Interpreter_primitiveLessThan(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	Bool boolResult = NO;
	if( Interpreter_success() == YES ) {
		boolResult = (integerReceiver < integerArgument);
	}
	if( Interpreter_success() == YES ) {
		Interpreter_push(boolResult ? TruePointer : FalsePointer);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

/**
 * page 624
 */
Bool Interpreter_primitiveGreaterThan(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	Bool boolResult = NO;
	if( Interpreter_success() == YES ) {
		boolResult = (integerReceiver > integerArgument);
	}
	if( Interpreter_success() == YES ) {
		Interpreter_push(boolResult ? TruePointer : FalsePointer);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

/**
 * page 624
 */
Bool Interpreter_primitiveLessOrEqual(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	Bool boolResult = NO;
	if( Interpreter_success() == YES ) {
		boolResult = (integerReceiver <= integerArgument);
	}
	if( Interpreter_success() == YES ) {
		Interpreter_push(boolResult ? TruePointer : FalsePointer);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

/**
 * page 624
 */
Bool Interpreter_primitiveGreaterOrEqual(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	Bool boolResult = NO;
	if( Interpreter_success() == YES ) {
		boolResult = (integerReceiver >= integerArgument);
	}
	if( Interpreter_success() == YES ) {
		Interpreter_push(boolResult ? TruePointer : FalsePointer);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

/**
 * page 624
 */
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

/**
 * page 624
 */
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

/**
 * page 622
 */
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

/**
 * page 622
 */
Bool Interpreter_primitiveDivide(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	short integerResult = 0;
	short integerRemainder = 0;

	/* cant divide by 0 */
	Interpreter_success_(integerArgument != 0);
	
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


/**
 * page 623
 */

/* always rounded toward negative infinity */
/* not sure how this works for negative numbers */
Bool Interpreter_primitiveMod(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	short integerResult = 0;
	short integerRemainder = 0;

	/* cant divide by 0 */
	Interpreter_success_(integerArgument != 0);
	
	/* We only care about the remainder */
	if( Interpreter_success() == YES ) {	
		integerResult = integerReceiver % integerArgument;
/*		printf( "received = %d, arg = %d, result = %d\n", integerReceiver, integerArgument, integerResult);*/
		/* always rounded toward negative infinity */
		if( integerReceiver < 0 ) {
			if( integerArgument > 0 ) {	
				integerResult += integerArgument;
/*				printf("modified to %d\n", integerResult); */
			}
		} else {
			if( integerArgument < 0 ) {
				integerResult += integerArgument;
/*				printf("modified to %d\n", integerResult);  */
			}
		}
		
		Interpreter_success_( ObjectMemory_isIntegerValue( integerResult ) );
	}
	
	if( Interpreter_success() == YES ) {
		Interpreter_pushInteger(integerResult);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

/**
 * page 623
 */

/* always rounded toward negative infinity */
/* not sure how this works for negative numbers */
Bool Interpreter_primitiveDiv(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	short integerResult = 0;
	short integerRemainder = 0;

	/* cant divide by 0 */
	Interpreter_success_(integerArgument != 0);
	
	/* We ignore the remainder */
	if( Interpreter_success() == YES ) {	
		integerResult = integerReceiver / integerArgument;
		if( integerResult < 0 ) {
			integerResult--; /* smalltalk rounds down always */
		}
		Interpreter_success_( ObjectMemory_isIntegerValue(integerResult ) );
	}
	
	if( Interpreter_success() == YES ) {
		Interpreter_pushInteger(integerResult);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

/**
 * page 623
 */
/* rounded to zero, like C, so an answer of -3.5 becomes -3, and +3.5 becomes +3 */
Bool Interpreter_primitiveQuo(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	short integerResult = 0;
	short integerRemainder = 0;
	

	/* cant divide by 0 */
	Interpreter_success_(integerArgument != 0);
	
	/* We ignore the remainder */
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

/**
 * page 624
 */
Bool Interpreter_primitiveBitAnd(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	short integerResult = 0;
	
	if( Interpreter_success() == YES ) {
		integerResult = integerReceiver & integerArgument;	
	}
	
	if( Interpreter_success() == YES ) {
		Interpreter_pushInteger(integerResult);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

/**
 * page 624
 */
Bool Interpreter_primitiveBitOr(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	short integerResult = 0;
	
	if( Interpreter_success() == YES ) {
		integerResult = integerReceiver | integerArgument;	
	}
	
	if( Interpreter_success() == YES ) {
		Interpreter_pushInteger(integerResult);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

/**
 * page 624
 */
Bool Interpreter_primitiveBitXor(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	short integerResult = 0;
	
	if( Interpreter_success() == YES ) {
		integerResult = integerReceiver ^ integerArgument;	
	}
	
	if( Interpreter_success() == YES ) {
		Interpreter_pushInteger(integerResult);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}

/**
 * page 624
 */
Bool Interpreter_primitiveBitShift(void) {
	short integerArgument = Interpreter_popInteger();
	short integerReceiver = Interpreter_popInteger();
	short integerResult = 0;
	
	if( Interpreter_success() == YES ) {
		if( integerArgument >= 0 ) {
			integerResult = integerReceiver << integerArgument;
		} else {
			integerResult = integerReceiver >> (-1 * integerArgument);
		}
	}
	
	if( Interpreter_success() == YES ) {
		Interpreter_pushInteger(integerResult);
	} else {
		Interpreter_unPop(2);
	}
	return Interpreter_success();
}


/* initializePointIndices - see page 625 - these seem to be "well known" but its not clear
 *  in the existing code where to declare them...thoughts welcome... I put them in 
 *  Interpreter_Constants.h */
Bool Interpreter_primitiveMakePoint(void) {
	ObjectPointer argument = Interpreter_popStack();
	ObjectPointer receiver = Interpreter_popStack();
	ObjectPointer result = NilPointer;
	
	Interpreter_success_( ObjectMemory_isIntegerValue(receiver));
	Interpreter_success_( ObjectMemory_isIntegerValue(argument));
	
	if( Interpreter_success() ) {
		result = ObjectMemory_instantiateClass_withPointers(ClassPointPointer, ClassPointSize);
		ObjectMemory_storePointer_ofObject_withValue(XIndex, result, receiver);
		ObjectMemory_storePointer_ofObject_withValue(YIndex, result, argument); 
		Interpreter_push(result);
	} else {
		Interpreter_unPop(2);
	}

	return Interpreter_success();
}

/**
 * Page 626
 */
Bool Interpreter_primitiveAsFloat(void) {
	ObjectPointer receiver = Interpreter_popStack();
	short integerReceiver = 0;
	float floatReceiver = 0.0;
	
	Interpreter_success_( ObjectMemory_isIntegerValue(receiver));
	if( Interpreter_success() ) {
		integerReceiver = ObjectMemory_integerValueOf(receiver);
		floatReceiver = integerReceiver * 1.0;
		Interpreter_pushFloat(floatReceiver);
	} else {
		Interpreter_unPop(1);
	}
	return Interpreter_success();
}

