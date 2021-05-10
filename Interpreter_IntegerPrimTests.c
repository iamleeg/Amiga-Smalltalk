#include "Interpreter_IntegerPrimTests.h"
#include "Interpreter.h"
#include "Interpreter_PrimArith.h"

extern ObjectPointer stubBlockContext(void);
void putTwoNumbersOnStack(ObjectPointer receiver, ObjectPointer argument);

Test(PrimitiveBitAndBasic) {
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(0xF);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(0x1);
    Expect(Interpreter_success() == YES );
    
/* call AND */

	Interpreter_initPrimitive();
    Interpreter_primitiveBitAnd();
	Expect( Interpreter_success() == YES );
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
/* 0xF & 0x1 => 0x1 */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 0x1 == ObjectMemory_integerValueOf( resultShortInteger) ); 

}

Test(PrimitiveBitOrBasic) {
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(0x6); /* 110 */
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(0x3); /* 011 */
    Expect(Interpreter_success() == YES );
    
/* call AND */

	Interpreter_initPrimitive();
    Interpreter_primitiveBitOr();
	Expect( Interpreter_success() == YES );
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
/* 110 | 011 => 111 (7)*/
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 0x7 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveBitXorBasic) {
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(0x6); /* 110 */
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(0x3); /* 011 */
    Expect(Interpreter_success() == YES );
    
/* call XOR */

	Interpreter_initPrimitive();
    Interpreter_primitiveBitXor();
	Expect( Interpreter_success() == YES );
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
/* 110 ^ 011 => 101 (5) */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 0x5 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveBitShiftBasic) {
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(0x2); /* 0010 */
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(1); 
    Expect(Interpreter_success() == YES );
    
/* call SHIFT*/

	Interpreter_initPrimitive();
    Interpreter_primitiveBitShift();
	Expect( Interpreter_success() == YES );
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
/* 0010 << 1  => 0100 (4) */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 0x4 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveMakePointBasic) {
	ObjectPointer result = NilPointer;
	
	ObjectPointer xResult = NilPointer;
	ObjectPointer yResult = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(2);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(1);
    Expect(Interpreter_success() == YES );
    
/* call makepoint */
/* 2 @ 1 => (2,1) */

	Interpreter_initPrimitive();
    Interpreter_primitiveMakePoint();
	Expect( Interpreter_success() == YES );
	result = Interpreter_popStack(); 
	
/* assert  result == (2,1) */
	Expect( result != NilPointer );
	Expect( ObjectMemory_fetchClassOf(result) == ClassPointPointer );
	xResult = ObjectMemory_fetchPointer_ofObject(XIndex, result); 
	Expect( xResult != NilPointer );
	yResult = ObjectMemory_fetchPointer_ofObject(YIndex, result); 
	Expect( yResult != NilPointer );
	Expect( 2 == ObjectMemory_integerValueOf(xResult) );
	Expect( 1 == ObjectMemory_integerValueOf(yResult) );
}


void putTwoNumbersOnStack(ObjectPointer receiver, ObjectPointer argument) {
	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
	Interpreter_initPrimitive();
    Interpreter_push(receiver);

	Interpreter_initPrimitive();
    Interpreter_push(argument);
}


Test(PrimitiveBitAndFailIfReceiverNonInteger) {
	ObjectPointer first = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer second = ObjectMemory_integerObjectOf(10);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    Interpreter_primitiveBitAnd();
    /* assert fail */
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveBitAndFailIfArgumentNonInteger) {
	ObjectPointer first = ObjectMemory_integerObjectOf(10);
	ObjectPointer second = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    Interpreter_primitiveBitAnd();
    /* assert fail */
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveBitOrFailIfReceiverNonInteger) {
	ObjectPointer first = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer second = ObjectMemory_integerObjectOf(10);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    Interpreter_primitiveBitOr();
    /* assert fail */
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveBitOrFailIfArgumentNonInteger) {
	ObjectPointer first = ObjectMemory_integerObjectOf(10);
	ObjectPointer second = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    Interpreter_primitiveBitOr();
    /* assert fail */
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveBitXorFailIfReceiverNonInteger) {
	ObjectPointer first = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer second = ObjectMemory_integerObjectOf(10);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    Interpreter_primitiveBitXor();
    /* assert fail */
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveBitXorFailIfArgumentNonInteger) {
	ObjectPointer first = ObjectMemory_integerObjectOf(10);
	ObjectPointer second = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    Interpreter_primitiveBitXor();
    /* assert fail */
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveBitShiftFailIfReceiverNonInteger) {
	ObjectPointer first = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer second = ObjectMemory_integerObjectOf(2);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    Interpreter_primitiveBitShift();
    /* assert fail */
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveBitShiftFailIfArgumentNonInteger) {
	ObjectPointer first = ObjectMemory_integerObjectOf(2);
	ObjectPointer second = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    Interpreter_primitiveBitShift();
    /* assert fail */
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveMakePointFailIfReceiverNonInteger) {
	ObjectPointer first = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer second = ObjectMemory_integerObjectOf(2);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    Interpreter_primitiveBitShift();
    /* assert fail */
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveMakePointFailIfArgumentNonInteger) {
	ObjectPointer first = ObjectMemory_integerObjectOf(2);
	ObjectPointer second = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    Interpreter_primitiveBitShift();
    /* assert fail */
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveAsFloatBasic) {
	ObjectPointer floatPointer = NilPointer;
	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
	Interpreter_initPrimitive();
    Interpreter_pushInteger(123);
	Expect( Interpreter_success() == YES );
	
    Interpreter_primitiveAsFloat();
    Expect( Interpreter_success() == YES );
    floatPointer = Interpreter_stackTop();
    Expect( ObjectMemory_fetchClassOf(floatPointer) == ClassFloatPointer );
    Expect( 123.0 == Interpreter_popFloat() );
}

Test(PrimitiveAsFloatFailsIfReceiverNotInteger) {
	ObjectPointer resultPointer = NilPointer;
	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
	Interpreter_initPrimitive();
    Interpreter_push(Interpreter_positive16BitIntegerFor(16500));
	Expect( Interpreter_success() == YES );
	
    Interpreter_primitiveAsFloat();
    Expect( Interpreter_success() == NO );
    /* Expect Stack unchanged */
    resultPointer = Interpreter_stackTop();
    Expect( ObjectMemory_fetchClassOf(resultPointer) == ClassLargePositiveIntegerPointer );
    Expect( 16500 == Interpreter_positive16BitValueOf( resultPointer ) );
}


void InterpreterIntegerPrimitiveTests(struct TestResult *tr) {	
	RunTest(PrimitiveBitAndBasic);
	RunTest(PrimitiveBitOrBasic);
	RunTest(PrimitiveBitXorBasic);
	RunTest(PrimitiveBitShiftBasic);
	RunTest(PrimitiveMakePointBasic);
	RunTest(PrimitiveBitAndFailIfReceiverNonInteger);
	RunTest(PrimitiveBitAndFailIfArgumentNonInteger);
	RunTest(PrimitiveBitOrFailIfReceiverNonInteger);
	RunTest(PrimitiveBitOrFailIfArgumentNonInteger);
	RunTest(PrimitiveBitXorFailIfReceiverNonInteger);
	RunTest(PrimitiveBitXorFailIfArgumentNonInteger);
	RunTest(PrimitiveBitShiftFailIfReceiverNonInteger);
	RunTest(PrimitiveBitShiftFailIfArgumentNonInteger);
	RunTest(PrimitiveMakePointFailIfReceiverNonInteger);
	RunTest(PrimitiveMakePointFailIfArgumentNonInteger);
	RunTest(PrimitiveAsFloatBasic);
	RunTest(PrimitiveAsFloatFailsIfReceiverNotInteger);
}
