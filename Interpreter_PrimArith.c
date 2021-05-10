#include "Interpreter.h"
#include "Interpreter_PrimArith.h"
#include "ObjectMemory.h"
#include "ObjectMemory_Allocation.h"



/**
 * Page 621
 */
void Interpreter_dispatchArithmeticPrimitives( int primitiveIndex ) {
	if( primitiveIndex < 20 ) {
		Interpreter_dispatchIntegerPrimitives( primitiveIndex );
	}
	if( primitiveIndex < 40 ) {
		Interpreter_dispatchLargeIntegerPrimitives( primitiveIndex );
	}
	if( primitiveIndex < 60 ) {
		Interpreter_dispatchFloatPrimitives( primitiveIndex );
	}
}

/**
 * Page 621
 */
 void Interpreter_dispatchIntegerPrimitives( int primitiveIndex ) {
	switch( primitiveIndex ) {
		case 1:  Interpreter_primitiveAdd();
		case 2:  Interpreter_primitiveSubtract();
		case 3:  Interpreter_primitiveLessThan();
		case 4:  Interpreter_primitiveGreaterThan();
		case 5:  Interpreter_primitiveLessOrEqual();
		case 6:  Interpreter_primitiveGreaterOrEqual();
		case 7:  Interpreter_primitiveEqual();
		case 8:  Interpreter_primitiveNotEqual();
		case 9:  Interpreter_primitiveMultiply();
		case 10:  Interpreter_primitiveDivide();
		case 11:  Interpreter_primitiveMod();
		case 12:  Interpreter_primitiveDiv();
		case 13:  Interpreter_primitiveQuo();
		case 14:  Interpreter_primitiveBitAnd();
		case 15:  Interpreter_primitiveBitOr();
		case 16:  Interpreter_primitiveBitXor();
		case 17:  Interpreter_primitiveBitShift();	
		case 18:  Interpreter_primitiveMakePoint();	
	}
}

/** 
 *  Should be implemented in Smalltalk instead  page 625
 */
void Interpreter_dispatchLargeIntegerPrimitives( int primitiveIndex ) {
	Interpreter_primitiveFail();
}
/**
 * Page 626
 */
void Interpreter_dispatchFloatPrimitives( int primitiveIndex ) {
	switch( primitiveIndex ) {
		case 40:  Interpreter_primitiveAsFloat();
		case 41:  Interpreter_primitiveFloatAdd();
		case 42:  Interpreter_primitiveFloatSubtract();
		case 43:  Interpreter_primitiveFloatLessThan();
		case 44:  Interpreter_primitiveFloatGreaterThan();
		case 45:  Interpreter_primitiveFloatLessOrEqual();
		case 46:  Interpreter_primitiveFloatGreaterOrEqual();
		case 47:  Interpreter_primitiveFloatEqual();
		case 48:  Interpreter_primitiveFloatNotEqual();
		case 49:  Interpreter_primitiveFloatMultiply();
		case 50:  Interpreter_primitiveFloatDivide();
		case 51:  Interpreter_primitiveTruncated();
		case 52:  Interpreter_primitiveFractionalPart();
		case 53:  Interpreter_primitiveExponent();
		case 54:  Interpreter_primitiveTimesTwoPower();
	}
}


/**
 * page 622
 */
void Interpreter_primitiveAdd(void) {
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

}

/**
 * page 622
 */
void Interpreter_primitiveSubtract(void) {
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

}

/**
 * page 624
 */
void Interpreter_primitiveLessThan(void) {
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

}

/**
 * page 624
 */
void Interpreter_primitiveGreaterThan(void) {
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

}

/**
 * page 624
 */
void Interpreter_primitiveLessOrEqual(void) {
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

}

/**
 * page 624
 */
void Interpreter_primitiveGreaterOrEqual(void) {
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

}

/**
 * page 624
 */
void Interpreter_primitiveEqual(void) {
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

}

/**
 * page 624
 */
void Interpreter_primitiveNotEqual(void) {
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

}

/**
 * page 622
 */
void Interpreter_primitiveMultiply(void) {
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

}

/**
 * page 622
 */
void Interpreter_primitiveDivide(void) {
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

}


/**
 * page 623
 */

/* always rounded toward negative infinity */
/* not sure how this works for negative numbers */
void Interpreter_primitiveMod(void) {
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

}

/**
 * page 623
 */

/* always rounded toward negative infinity */
/* not sure how this works for negative numbers */
void Interpreter_primitiveDiv(void) {
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

}

/**
 * page 623
 */
/* rounded to zero, like C, so an answer of -3.5 becomes -3, and +3.5 becomes +3 */
void Interpreter_primitiveQuo(void) {
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

}

/**
 * page 624
 */
void Interpreter_primitiveBitAnd(void) {
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

}

/**
 * page 624
 */
void Interpreter_primitiveBitOr(void) {
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

}

/**
 * page 624
 */
void Interpreter_primitiveBitXor(void) {
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

}

/**
 * page 624
 */
void Interpreter_primitiveBitShift(void) {
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

}


/* initializePointIndices - see page 625 - these seem to be "well known" but its not clear
 *  in the existing code where to declare them...thoughts welcome... I put them in 
 *  Interpreter_Constants.h */
void Interpreter_primitiveMakePoint(void) {
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


}

/**
 * Page 626
 */
void Interpreter_primitiveAsFloat(void) {
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

}

void Interpreter_primitiveFloatAdd(void) {
	float argument = Interpreter_popFloat();
	float receiver = Interpreter_popFloat();
	float result = 0.0;
	if( Interpreter_success() ) {
		result = receiver + argument;
		Interpreter_pushFloat( result );
	} else {
		Interpreter_unPop(2);
	}

}

void Interpreter_primitiveFloatSubtract(void) {
	float argument = Interpreter_popFloat();
	float receiver = Interpreter_popFloat();
	float result = 0.0;
	if( Interpreter_success() ) {
		result = receiver - argument;
		Interpreter_pushFloat( result );
	} else {
		Interpreter_unPop(2);
	}

}

void Interpreter_primitiveFloatLessThan(void) {
	float argument = Interpreter_popFloat();
	float receiver = Interpreter_popFloat();
	if( Interpreter_success() ) {
		if( receiver < argument ) {
			Interpreter_push( TruePointer );
		} else {
			Interpreter_push( FalsePointer );
		}
	} else {
		Interpreter_unPop(2);
	}

}

void Interpreter_primitiveFloatGreaterThan(void) {
	float argument = Interpreter_popFloat();
	float receiver = Interpreter_popFloat();
	if( Interpreter_success() ) {
		if( receiver > argument ) {
			Interpreter_push( TruePointer );
		} else {
			Interpreter_push( FalsePointer );
		}
	} else {
		Interpreter_unPop(2);
	}

}

void Interpreter_primitiveFloatLessOrEqual(void) {
	float argument = Interpreter_popFloat();
	float receiver = Interpreter_popFloat();
	if( Interpreter_success() ) {
		if( receiver <= argument ) {
			Interpreter_push( TruePointer );
		} else {
			Interpreter_push( FalsePointer );
		}
	} else {
		Interpreter_unPop(2);
	}


}

void Interpreter_primitiveFloatGreaterOrEqual(void) {
	float argument = Interpreter_popFloat();
	float receiver = Interpreter_popFloat();
	if( Interpreter_success() ) {
		if( receiver >= argument ) {
			Interpreter_push( TruePointer );
		} else {
			Interpreter_push( FalsePointer );
		}
	} else {
		Interpreter_unPop(2);
	}


}

void Interpreter_primitiveFloatEqual(void) {
	float argument = Interpreter_popFloat();
	float receiver = Interpreter_popFloat();
	if( Interpreter_success() ) {
		if( receiver == argument ) {
			Interpreter_push( TruePointer );
		} else {
			Interpreter_push( FalsePointer );
		}
	} else {
		Interpreter_unPop(2);
	}

}

void Interpreter_primitiveFloatNotEqual(void) {
	float argument = Interpreter_popFloat();
	float receiver = Interpreter_popFloat();
	if( Interpreter_success() ) {
		if( receiver != argument ) {
			Interpreter_push( TruePointer );
		} else {
			Interpreter_push( FalsePointer );
		}
	} else {
		Interpreter_unPop(2);
	}


}

void Interpreter_primitiveFloatMultiply(void) {
	float argument = Interpreter_popFloat();
	float receiver = Interpreter_popFloat();
	float result = 0.0;
	if( Interpreter_success() ) {
		result = receiver * argument;
		Interpreter_pushFloat( result );
	} else {
		Interpreter_unPop(2);
	}

}

void Interpreter_primitiveFloatDivide(void) {
	float argument = 99.0;
	float receiver = 99.0;
	float result = 0.0;
	
	argument = Interpreter_popFloat();
	receiver = Interpreter_popFloat();
	
	if( Interpreter_success() ) {
		Interpreter_success_(argument > 0.0 || argument < 0.0);
	}
	
	if( Interpreter_success() ) {
		result = receiver / argument;
		Interpreter_pushFloat( result );
	} else {
		Interpreter_unPop(2);
	}

}

void Interpreter_primitiveTruncated(void) {
	short result = 0;
	float receiver = Interpreter_popFloat();
	
	if( Interpreter_success() ) {
		result = (short)receiver;
		Interpreter_success_( ObjectMemory_isIntegerValue(result) );
	}
		
	if( Interpreter_success() ) {
		Interpreter_pushInteger(result);
	} else {
		Interpreter_unPop(1);
	}
}

void Interpreter_primitiveFractionalPart(void) {
	float result = 0;
	float receiver = Interpreter_popFloat();
	if( Interpreter_success() ) {
		result = receiver - (short)receiver;
		Interpreter_pushFloat( result );
	} else {
		Interpreter_unPop(1);
	}		
}

/** optional */
void Interpreter_primitiveExponent(void) {
	Interpreter_primitiveFail();
}

/** optional */
void Interpreter_primitiveTimesTwoPower(void) {
	Interpreter_primitiveFail();
}

