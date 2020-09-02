#include "Interpreter_IntegerPrimTests.h"
#include "Interpreter.h"
#include "Interpreter_PrimArith.h"

extern ObjectPointer stubBlockContext(void);
void putTwoNumbersOnStack(ObjectPointer receiver, ObjectPointer argument);

Test(PrimitiveBitAndBasic) {
	Bool localSuccess = NO;
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
    localSuccess = Interpreter_primitiveBitAnd();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
/* 0xF & 0x1 => 0x1 */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 0x1 == ObjectMemory_integerValueOf( resultShortInteger) ); 

}

Test(PrimitiveBitOrBasic) {
	Bool localSuccess = NO;
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
    localSuccess = Interpreter_primitiveBitOr();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
/* 110 | 011 => 111 (7)*/
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 0x7 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveBitXorBasic) {
	Bool localSuccess = NO;
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
    localSuccess = Interpreter_primitiveBitXor();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
/* 110 ^ 011 => 101 (5) */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 0x5 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveBitShiftBasic) {
	Bool localSuccess = NO;
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
    localSuccess = Interpreter_primitiveBitShift();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
/* 0010 << 1  => 0100 (4) */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 0x4 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveMakePointBasic) {
	Bool localSuccess = NO;
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
    localSuccess = Interpreter_primitiveMakePoint();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
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
	Bool localSuccess = NO;
	ObjectPointer first = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer second = ObjectMemory_integerObjectOf(10);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveBitAnd();
    /* assert fail */
	Expect( localSuccess == NO );
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveBitAndFailIfArgumentNonInteger) {
	Bool localSuccess = NO;
	ObjectPointer first = ObjectMemory_integerObjectOf(10);
	ObjectPointer second = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveBitAnd();
    /* assert fail */
	Expect( localSuccess == NO );
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveBitOrFailIfReceiverNonInteger) {
	Bool localSuccess = NO;
	ObjectPointer first = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer second = ObjectMemory_integerObjectOf(10);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveBitOr();
    /* assert fail */
	Expect( localSuccess == NO );
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveBitOrFailIfArgumentNonInteger) {
	Bool localSuccess = NO;
	ObjectPointer first = ObjectMemory_integerObjectOf(10);
	ObjectPointer second = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveBitOr();
    /* assert fail */
	Expect( localSuccess == NO );
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveBitXorFailIfReceiverNonInteger) {
	Bool localSuccess = NO;
	ObjectPointer first = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer second = ObjectMemory_integerObjectOf(10);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveBitXor();
    /* assert fail */
	Expect( localSuccess == NO );
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveBitXorFailIfArgumentNonInteger) {
	Bool localSuccess = NO;
	ObjectPointer first = ObjectMemory_integerObjectOf(10);
	ObjectPointer second = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveBitXor();
    /* assert fail */
	Expect( localSuccess == NO );
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveBitShiftFailIfReceiverNonInteger) {
	Bool localSuccess = NO;
	ObjectPointer first = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer second = ObjectMemory_integerObjectOf(2);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveBitShift();
    /* assert fail */
	Expect( localSuccess == NO );
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveBitShiftFailIfArgumentNonInteger) {
	Bool localSuccess = NO;
	ObjectPointer first = ObjectMemory_integerObjectOf(2);
	ObjectPointer second = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveBitShift();
    /* assert fail */
	Expect( localSuccess == NO );
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveMakePointFailIfReceiverNonInteger) {
	Bool localSuccess = NO;
	ObjectPointer first = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer second = ObjectMemory_integerObjectOf(2);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveBitShift();
    /* assert fail */
	Expect( localSuccess == NO );
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
}

Test(PrimitiveMakePointFailIfArgumentNonInteger) {
	Bool localSuccess = NO;
	ObjectPointer first = ObjectMemory_integerObjectOf(2);
	ObjectPointer second = Interpreter_positive16BitIntegerFor(16500);
	ObjectPointer resultObject = NilPointer;
	putTwoNumbersOnStack(first, second);

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveBitShift();
    /* assert fail */
	Expect( localSuccess == NO );
	Expect( Interpreter_success() == NO );
	
	/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == second);
	
	resultObject = Interpreter_popStack(); 
	Expect( resultObject == first);
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
}
