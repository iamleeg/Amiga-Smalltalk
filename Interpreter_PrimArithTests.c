#include "Interpreter_PrimArithTests.h"
#include "Interpreter.h"
#include "Interpreter_PrimArith.h"

extern ObjectPointer stubBlockContext(void);

/*  PRIMITIVE ADD --------------------------------------------------------------------- */ 
Test(PrimitiveAddBasic) {
	Bool localSuccess = NO;
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(3);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(4);
    Expect(Interpreter_success() == YES );
    
/* call add */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveAdd();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* 3+4 == 7 */
	Expect( 7 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveAddFailsIfAlreadyFailed) {
	Bool localSuccess = NO;
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(3);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(4);
    Expect(Interpreter_success() == YES );
    
/* call add */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    localSuccess = Interpreter_primitiveAdd();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
	
/* assert stack unchanged */
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 4 == ObjectMemory_integerValueOf( resultShortInteger) ); 

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 3 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveAddFailsIfAnswerNotInteger) {
	Bool localSuccess = NO;
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10000);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(10000);
    Expect(Interpreter_success() == YES );
    
/* call add */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    localSuccess = Interpreter_primitiveAdd();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
	
/* assert stack unchanged */
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 10000 == ObjectMemory_integerValueOf( resultShortInteger) ); 

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 10000 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveAddFailsIfReceiverNotInteger) {
	Bool localSuccess = NO;
	ObjectPointer resultObject = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_push(Interpreter_positive16BitIntegerFor(16500));
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(10000);
    Expect(Interpreter_success() == YES );
    
/* call add */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    localSuccess = Interpreter_primitiveAdd();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
/* assert stack unchanged */
	resultObject = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultObject != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 10000 == ObjectMemory_integerValueOf( resultObject) ); 

	resultObject = Interpreter_popStack(); 
	Expect( resultObject != NilPointer );
	Expect( !ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 16500 == Interpreter_positive16BitValueOf( resultObject) ); 
}

Test(PrimitiveAddFailsIfArgumentNotInteger) {
	Bool localSuccess = NO;
	ObjectPointer resultObject = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10000);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_push(Interpreter_positive16BitIntegerFor(16500));
    Expect(Interpreter_success() == YES );
    
/* call add */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    localSuccess = Interpreter_primitiveAdd();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject != NilPointer );
	Expect( !ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 16500 == Interpreter_positive16BitValueOf( resultObject) ); 

	resultObject = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultObject != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 10000 == ObjectMemory_integerValueOf( resultObject) ); 

}

/*  PRIMITIVE SUBTRACT ---------------------------------------------------------------- */ 
Test(PrimitiveSubtractBasic) {
	Bool localSuccess = NO;
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(7);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(4);
    Expect(Interpreter_success() == YES );
    
/* call subtract */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveSubtract();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* 7-4 == 3 */
	Expect( 3 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveSubtractFailsIfAlreadyFailed) {
	Bool localSuccess = NO;
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(7);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(4);
    Expect(Interpreter_success() == YES );
    
/* call subtract */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    localSuccess = Interpreter_primitiveSubtract();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
	
/* assert stack unchanged */
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 4 == ObjectMemory_integerValueOf( resultShortInteger) ); 

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 7 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveSubtractFailsIfAnswerNotInteger) {
	Bool localSuccess = NO;
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack   (-16000) - 1000 == -17000 (not a small integer)*/
	Interpreter_initPrimitive();
    Interpreter_pushInteger(-16000);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(1000);
    Expect(Interpreter_success() == YES );
    
/* call subtract */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    localSuccess = Interpreter_primitiveSubtract();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
	
/* assert stack unchanged */
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 1000 == ObjectMemory_integerValueOf( resultShortInteger) ); 

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( -16000 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveSubtractFailsIfReceiverNotInteger) {
	Bool localSuccess = NO;
	ObjectPointer resultObject = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_push(Interpreter_positive16BitIntegerFor(16500));
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(10000);
    Expect(Interpreter_success() == YES );
    
/* call subtract */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    localSuccess = Interpreter_primitiveSubtract();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
/* assert stack unchanged */
	resultObject = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultObject != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 10000 == ObjectMemory_integerValueOf( resultObject) ); 

	resultObject = Interpreter_popStack(); 
	Expect( resultObject != NilPointer );
	Expect( !ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 16500 == Interpreter_positive16BitValueOf( resultObject) ); 
}

Test(PrimitiveSubtractFailsIfArgumentNotInteger) {
	Bool localSuccess = NO;
	ObjectPointer resultObject = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10000);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_push(Interpreter_positive16BitIntegerFor(16500));
    Expect(Interpreter_success() == YES );
    
/* call add */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    localSuccess = Interpreter_primitiveSubtract();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject != NilPointer );
	Expect( !ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 16500 == Interpreter_positive16BitValueOf( resultObject) ); 

	resultObject = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultObject != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 10000 == ObjectMemory_integerValueOf( resultObject) ); 

}

/*  PRIMITIVE MULTIPLY ---------------------------------------------------------------- */ 
Test(PrimitiveMultiplyBasic) {
	Bool localSuccess = NO;
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(3);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(4);
    Expect(Interpreter_success() == YES );
    
/* call multiply */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveMultiply();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* 3*4 == 12 */
	Expect( 12 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveMultiplyFailsIfAlreadyFailed) {
	Bool localSuccess = NO;
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(3);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(4);
    Expect(Interpreter_success() == YES );
    
/* call multiply */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    localSuccess = Interpreter_primitiveMultiply();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
	
/* assert stack unchanged */
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 4 == ObjectMemory_integerValueOf( resultShortInteger) ); 

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 3 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveMultiplyFailsIfAnswerNotInteger) {
	Bool localSuccess = NO;
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10000);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(10000);
    Expect(Interpreter_success() == YES );
    
/* call multiply */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    localSuccess = Interpreter_primitiveAdd();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
	
/* assert stack unchanged */
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 10000 == ObjectMemory_integerValueOf( resultShortInteger) ); 

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 10000 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveMultiplyFailsIfReceiverNotInteger) {
	Bool localSuccess = NO;
	ObjectPointer resultObject = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_push(Interpreter_positive16BitIntegerFor(16500));
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(10000);
    Expect(Interpreter_success() == YES );
    
/* call multiply */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    localSuccess = Interpreter_primitiveAdd();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
/* assert stack unchanged */
	resultObject = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultObject != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 10000 == ObjectMemory_integerValueOf( resultObject) ); 

	resultObject = Interpreter_popStack(); 
	Expect( resultObject != NilPointer );
	Expect( !ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 16500 == Interpreter_positive16BitValueOf( resultObject) ); 
}

Test(PrimitiveMultiplyFailsIfArgumentNotInteger) {
	Bool localSuccess = NO;
	ObjectPointer resultObject = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10000);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_push(Interpreter_positive16BitIntegerFor(16500));
    Expect(Interpreter_success() == YES );
    
/* call multiply */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    localSuccess = Interpreter_primitiveAdd();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject != NilPointer );
	Expect( !ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 16500 == Interpreter_positive16BitValueOf( resultObject) ); 

	resultObject = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultObject != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 10000 == ObjectMemory_integerValueOf( resultObject) ); 
}

/*  PRIMITIVE DIVIDE ---------------------------------------------------------------- */ 
Test(PrimitiveDivideBasic) {
	Bool localSuccess = NO;
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(2);
    Expect(Interpreter_success() == YES );
    
/* call divide */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveDivide();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* 10 / 2 == 5 */
	Expect( 5 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveDivideFailsIfWontDivideCleanly) {
	Bool localSuccess = NO;
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(3);
    Expect(Interpreter_success() == YES );
    
/* call divide */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveDivide();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
/* assert stack unchanged */
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 3 == ObjectMemory_integerValueOf( resultShortInteger) ); 

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 10 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveDivideFailsIfAlreadyFailed) {
	Bool localSuccess = NO;
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(2);
    Expect(Interpreter_success() == YES );
    
/* call divide */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    localSuccess = Interpreter_primitiveDivide();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
	
/* assert stack unchanged */
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 2 == ObjectMemory_integerValueOf( resultShortInteger) ); 

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 10 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

/* Technically  true, but cant think of a way to get an out of range answer
   from two in range  integers */
/*  
Test(PrimitiveDivideFailsIfAnswerNotInteger) {
}
*/

Test(PrimitiveDivideFailsIfReceiverNotInteger) {
	Bool localSuccess = NO;
	ObjectPointer resultObject = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_push(Interpreter_positive16BitIntegerFor(16500));
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(100);
    Expect(Interpreter_success() == YES );
    
/* call divide */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    localSuccess = Interpreter_primitiveDivide();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
/* assert stack unchanged */
	resultObject = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultObject != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 100 == ObjectMemory_integerValueOf( resultObject) ); 

	resultObject = Interpreter_popStack(); 
	Expect( resultObject != NilPointer );
	Expect( !ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 16500 == Interpreter_positive16BitValueOf( resultObject) ); 
}

Test(PrimitiveDivideFailsIfArgumentNotInteger) {
	Bool localSuccess = NO;
	ObjectPointer resultObject = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(100);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_push(Interpreter_positive16BitIntegerFor(16500));
    Expect(Interpreter_success() == YES );
    
/* call divide */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    localSuccess = Interpreter_primitiveDivide();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject != NilPointer );
	Expect( !ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 16500 == Interpreter_positive16BitValueOf( resultObject) ); 

	resultObject = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultObject != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 100 == ObjectMemory_integerValueOf( resultObject) ); 

}

/*  PRIMITIVE EQUALS ---------------------------------------------------------------- */ 
Test(PrimitiveEqualTrueWorks) {
	Bool localSuccess = NO;
	ObjectPointer resultBool = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );
    
/* call equals */
/* 10 == 10 */

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveEqual();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
	resultBool = Interpreter_popStack(); 
	
/* assert  result == TRUE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == TruePointer );
}

Test(PrimitiveEqualFalseWorks) {
	Bool localSuccess = NO;
	ObjectPointer resultBool = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(20);
    Expect(Interpreter_success() == YES );
    
/* call equals */
/* 10 == 20 */

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveEqual();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
	resultBool = Interpreter_popStack();
	
/* assert  result == FALSE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == FalsePointer );
}

Test(PrimitiveEqualFailsIfReceiverNotInteger) {
	Bool localSuccess = NO;
	ObjectPointer resultObject = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_push(Interpreter_positive16BitIntegerFor(16500));
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );
    
/* call equals */
/* 16500 == 10 */

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveEqual();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
/* assert stack unchanged */
	resultObject = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultObject != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 10 == ObjectMemory_integerValueOf( resultObject) ); 

	resultObject = Interpreter_popStack(); 
	Expect( resultObject != NilPointer );
	Expect( !ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 16500 == Interpreter_positive16BitValueOf( resultObject) ); 

}

Test(PrimitiveEqualFailsIfArgumentNotInteger) {
	Bool localSuccess = NO;
	ObjectPointer resultObject = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );
    
	Interpreter_initPrimitive();
    Interpreter_push(Interpreter_positive16BitIntegerFor(16500));
    Expect(Interpreter_success() == YES );

/* call equals */
/* 10 == 16500 */

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveEqual();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject != NilPointer );
	Expect( !ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 16500 == Interpreter_positive16BitValueOf( resultObject) ); 

	resultObject = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultObject != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 10 == ObjectMemory_integerValueOf( resultObject) ); 
}


/*  PRIMITIVE NOTEQUAL ---------------------------------------------------------------- */ 
Test(PrimitiveNotEqualTrueWorks) {
	Bool localSuccess = NO;
	ObjectPointer resultBool = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(20);
    Expect(Interpreter_success() == YES );
    
/* call notequal */
/* 10 != 20 */

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveNotEqual();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
	resultBool = Interpreter_popStack(); 
	
/* assert  result == TRUE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == TruePointer );
}

Test(PrimitiveNotEqualFalseWorks) {
	Bool localSuccess = NO;
	ObjectPointer resultBool = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );
    
/* call note1ual */
/* 10 != 10 */

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveNotEqual();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
	resultBool = Interpreter_popStack(); 
	
/* assert  result == FALSE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == FalsePointer );
}

Test(PrimitiveNotEqualFailsIfReceiverNotInteger) {
	Bool localSuccess = NO;
	ObjectPointer resultObject = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_push(Interpreter_positive16BitIntegerFor(16500));
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );
    
/* call notequal */
/* 16500 != 10 */

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveNotEqual();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
/* assert stack unchanged */
	resultObject = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultObject != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 10 == ObjectMemory_integerValueOf( resultObject) ); 

	resultObject = Interpreter_popStack(); 
	Expect( resultObject != NilPointer );
	Expect( !ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 16500 == Interpreter_positive16BitValueOf( resultObject) ); 

}

Test(PrimitiveNotEqualFailsIfArgumentNotInteger) {
	Bool localSuccess = NO;
	ObjectPointer resultObject = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );
    
	Interpreter_initPrimitive();
    Interpreter_push(Interpreter_positive16BitIntegerFor(16500));
    Expect(Interpreter_success() == YES );

/* call notequal */
/* 10 != 16500 */

	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveNotEqual();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
/* assert stack unchanged */
	resultObject = Interpreter_popStack(); 
	Expect( resultObject != NilPointer );
	Expect( !ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 16500 == Interpreter_positive16BitValueOf( resultObject) ); 

	resultObject = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultObject != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultObject ) );
	Expect( 10 == ObjectMemory_integerValueOf( resultObject) ); 
}



void InterpreterArithmeticPrimitiveTests(struct TestResult *tr) {
	RunTest(PrimitiveAddBasic);
	RunTest(PrimitiveAddFailsIfAlreadyFailed);
	RunTest(PrimitiveAddFailsIfAnswerNotInteger);
	RunTest(PrimitiveAddFailsIfReceiverNotInteger);
	RunTest(PrimitiveAddFailsIfArgumentNotInteger);
	RunTest(PrimitiveSubtractBasic);
	RunTest(PrimitiveSubtractFailsIfAlreadyFailed);
	RunTest(PrimitiveSubtractFailsIfAnswerNotInteger);
	RunTest(PrimitiveSubtractFailsIfReceiverNotInteger);
	RunTest(PrimitiveSubtractFailsIfArgumentNotInteger);
	RunTest(PrimitiveMultiplyBasic);
	RunTest(PrimitiveMultiplyFailsIfAlreadyFailed);
	RunTest(PrimitiveMultiplyFailsIfAnswerNotInteger);
	RunTest(PrimitiveMultiplyFailsIfReceiverNotInteger);
	RunTest(PrimitiveMultiplyFailsIfArgumentNotInteger);
	RunTest(PrimitiveDivideBasic);
	RunTest(PrimitiveDivideFailsIfWontDivideCleanly);
	RunTest(PrimitiveDivideFailsIfAlreadyFailed);
/*	RunTest(PrimitiveDivideFailsIfAnswerNotInteger);*/
	RunTest(PrimitiveDivideFailsIfReceiverNotInteger);
	RunTest(PrimitiveDivideFailsIfArgumentNotInteger);
	RunTest(PrimitiveEqualTrueWorks);
	RunTest(PrimitiveEqualFalseWorks);
	RunTest(PrimitiveEqualFailsIfReceiverNotInteger);
	RunTest(PrimitiveEqualFailsIfArgumentNotInteger);
	RunTest(PrimitiveNotEqualTrueWorks);
	RunTest(PrimitiveNotEqualFalseWorks);
	RunTest(PrimitiveNotEqualFailsIfReceiverNotInteger);
	RunTest(PrimitiveNotEqualFailsIfArgumentNotInteger);
}