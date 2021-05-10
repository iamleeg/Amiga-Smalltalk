#include "Interpreter_IntegerArithTests.h"
#include "Interpreter.h"
#include "Interpreter_PrimArith.h"

extern ObjectPointer stubBlockContext(void);

/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE ADD --------------------------------------------------------------------- */ 
/*  ----------------------------------------------------------------------------------- */ 

Test(PrimitiveAddBasic) {
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
    Interpreter_primitiveAdd();
	Expect( Interpreter_success() == YES );

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* 3+4 == 7 */
	Expect( 7 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveAddFailsIfAlreadyFailed) {
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
    Interpreter_primitiveAdd();
	Expect( Interpreter_success() == NO );

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
    Interpreter_primitiveAdd();
	Expect( Interpreter_success() == NO );
	
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
    Interpreter_primitiveAdd();
	Expect( Interpreter_success() == NO );
	
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
    Interpreter_primitiveAdd();
	Expect( Interpreter_success() == NO );

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

/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE SUBTRACT ---------------------------------------------------------------- */ 
/*  ----------------------------------------------------------------------------------- */ 

Test(PrimitiveSubtractBasic) {
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
    Interpreter_primitiveSubtract();
	Expect( Interpreter_success() == YES );

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* 7-4 == 3 */
	Expect( 3 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveSubtractFailsIfAlreadyFailed) {
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
    Interpreter_primitiveSubtract();
	Expect( Interpreter_success() == NO );

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
    Interpreter_primitiveSubtract();
	Expect( Interpreter_success() == NO );

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
    Interpreter_primitiveSubtract();
	Expect( Interpreter_success() == NO );

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
    Interpreter_primitiveSubtract();
	Expect( Interpreter_success() == NO );

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

/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE MULTIPLY ---------------------------------------------------------------- */ 
/*  ----------------------------------------------------------------------------------- */ 

Test(PrimitiveMultiplyBasic) {
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
    Interpreter_primitiveMultiply();
	Expect( Interpreter_success() == YES );

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* 3*4 == 12 */
	Expect( 12 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveMultiplyFailsIfAlreadyFailed) {
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
    Interpreter_primitiveMultiply();
	Expect( Interpreter_success() == NO );
	
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
    Interpreter_primitiveAdd();
	Expect( Interpreter_success() == NO );

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
    Interpreter_primitiveAdd();
	Expect( Interpreter_success() == NO );

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
    Interpreter_primitiveAdd();
	Expect( Interpreter_success() == NO );

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

/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE DIVIDE ------------------------------------------------------------------ */ 
/*  ----------------------------------------------------------------------------------- */ 

Test(PrimitiveDivideBasic) {
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
    Interpreter_primitiveDivide();
	Expect( Interpreter_success() == YES );

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* 10 / 2 == 5 */
	Expect( 5 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveDivideFailsIfArgumentZero) {
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(0);
    Expect(Interpreter_success() == YES );
    
/* call divide  10 / 0 */
	Interpreter_initPrimitive();
    Interpreter_primitiveDivide();
	Expect( Interpreter_success() == NO );

/* assert stack unchanged */
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 0 == ObjectMemory_integerValueOf( resultShortInteger) ); 

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 10 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}


Test(PrimitiveDivideFailsIfWontDivideCleanly) {
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
    Interpreter_primitiveDivide();
	Expect( Interpreter_success() == NO );

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
    Interpreter_primitiveDivide();
	Expect( Interpreter_success() == NO );

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
    Interpreter_primitiveDivide();
	Expect( Interpreter_success() == NO );

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
    Interpreter_primitiveDivide();
	Expect( Interpreter_success() == NO );

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

/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE DIV --------------------------------------------------------------------- */
/*  ----------------------------------------------------------------------------------- */ 
 
Test(PrimitiveDivBasic) {
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
    
/* call div */
	Interpreter_initPrimitive();
    Interpreter_primitiveDiv();
	Expect( Interpreter_success() == YES );

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* 10 / 2 == 5 */
	Expect( 5 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveDivFailsIfArgumentZero) {
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(0);
    Expect(Interpreter_success() == YES );
    
/* call div  10 / 0 */
	Interpreter_initPrimitive();
    Interpreter_primitiveDiv();
	Expect( Interpreter_success() == NO );

/* assert stack unchanged */
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 0 == ObjectMemory_integerValueOf( resultShortInteger) ); 

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 10 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveDivFailsIfAlreadyFailed) {
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
    
/* call div */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    Interpreter_primitiveDiv();
	Expect( Interpreter_success() == NO );

	
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
Test(PrimitiveDivFailsIfAnswerNotInteger) {
}
*/

Test(PrimitiveDivFailsIfReceiverNotInteger) {
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
    
/* call div */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    Interpreter_primitiveDiv();
	Expect( Interpreter_success() == NO );

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

Test(PrimitiveDivFailsIfArgumentNotInteger) {
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
    
/* call div */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    Interpreter_primitiveDiv();
	Expect( Interpreter_success() == NO );

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

Test(PrimitiveDivRoundsPositiveDown) {
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
    
/* call div  10 // 3 == 3.333 == ROUND DOWN TO 3 */
	Interpreter_initPrimitive();
    Interpreter_primitiveDiv();
	Expect( Interpreter_success() == YES );

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* 10 // 3 == 3 */
	Expect( 3 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}


Test(PrimitiveDivRoundsNegativeToNegativeInfinity) {
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(-10);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(3);
    Expect(Interpreter_success() == YES );
    
/* call div  -10 // 3 == -3.333 == ROUND DOWN TO -4 */
	Interpreter_initPrimitive();
    Interpreter_primitiveDiv();
	Expect( Interpreter_success() == YES );

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* -10 // 3 == -4 */
	Expect( -4 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE MOD --------------------------------------------------------------------- */ 
/*  ----------------------------------------------------------------------------------- */ 

Test(PrimitiveModBasic) {
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
    
/* call mod */
	Interpreter_initPrimitive();
    Interpreter_primitiveMod();
	Expect( Interpreter_success() == YES );

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* 10 \\ 3 == 1 */
	Expect( 1 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveModFailsIfArgumentZero) {
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(0);
    Expect(Interpreter_success() == YES );
    
/* call mod  10 / 0 */
	Interpreter_initPrimitive();
    Interpreter_primitiveMod();
	Expect( Interpreter_success() == NO );

/* assert stack unchanged */
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 0 == ObjectMemory_integerValueOf( resultShortInteger) ); 

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 10 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveModFailsIfAlreadyFailed) {
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
    
/* call mod */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    Interpreter_primitiveMod();
	Expect( Interpreter_success() == NO );

	
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
Test(PrimitiveModFailsIfAnswerNotInteger) {
}
*/

Test(PrimitiveModFailsIfReceiverNotInteger) {
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
    
/* call mod */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    Interpreter_primitiveMod();
	Expect( Interpreter_success() == NO );

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

Test(PrimitiveModFailsIfArgumentNotInteger) {
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
    
/* call mod */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    Interpreter_primitiveMod();
	Expect( Interpreter_success() == NO );

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

/* THIS AND PRIMITIVE DIV ARE MADNESS */
/* -100 divided by 30 in this case would normally give -3 and a remainder of 10 */
/* you can go down 30 3 times and are left with -10 to go */
/*                                                         */
/* with this craxy round toward infinity method */
/* we go down 4 times, and the remainder is -20 */
/* because we go down further than we have to and the remainder says how much */
/* further we have  gone */

Test(PrimitiveModRoundsToNegativeInfinity) {
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(100);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(-30);
    Expect(Interpreter_success() == YES );
    
/* call mod */
	Interpreter_initPrimitive();
    Interpreter_primitiveMod();
	Expect( Interpreter_success() == YES );

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* 100 \\ -30 == -20 */
	Expect( -20 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}
/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE QUO --------------------------------------------------------------------- */ 
/*  ----------------------------------------------------------------------------------- */ 

Test(PrimitiveQuoBasic) {
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
    
/* call quo */
	Interpreter_initPrimitive();
    Interpreter_primitiveQuo();
	Expect( Interpreter_success() == YES );

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result (quo truncates) */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* 10 quo 2 == 5 */
	Expect( 5 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveQuoFailsIfArgumentZero) {
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(0);
    Expect(Interpreter_success() == YES );
    
/* call quo */
	Interpreter_initPrimitive();
    Interpreter_primitiveQuo();
	Expect( Interpreter_success() == NO );

/* assert stack unchanged */
	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 0 == ObjectMemory_integerValueOf( resultShortInteger) ); 

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	Expect( 10 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}

Test(PrimitiveQuoFailsIfAlreadyFailed) {
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
    
/* call quo */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    Interpreter_primitiveQuo();
	Expect( Interpreter_success() == NO );

	
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
Test(PrimitiveQuoFailsIfAnswerNotInteger) {
}
*/

Test(PrimitiveQuoFailsIfReceiverNotInteger) {
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
    
/* call mod */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    Interpreter_primitiveQuo();
	Expect( Interpreter_success() == NO );

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

Test(PrimitiveQuoFailsIfArgumentNotInteger) {
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
    
/* call mod */
	Interpreter_initPrimitive();
	Interpreter_primitiveFail();
    Interpreter_primitiveQuo();
	Expect( Interpreter_success() == NO );

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

Test(PrimitiveQuoRoundsPositiveDown) {
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
    
/* call   10 quo 3  */
	Interpreter_initPrimitive();
    Interpreter_primitiveQuo();
	Expect( Interpreter_success() == YES );

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* 10 quo 3 == 3.333 == ROUND DOWN TO 3 */
	Expect( 3 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}


Test(PrimitiveQuoRoundsNegativeUpTowardZero) {
	ObjectPointer resultShortInteger = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(-10);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(3);
    Expect(Interpreter_success() == YES );
    
/* call   -10 quo 3 */
	Interpreter_initPrimitive();
    Interpreter_primitiveQuo();
	Expect( Interpreter_success() == YES );

	resultShortInteger = Interpreter_popStack(); /* could just call popInteger, but lets be clear */
	
/* assert correct result */
	Expect( resultShortInteger != NilPointer );
	Expect( ObjectMemory_isIntegerObject( resultShortInteger ) );
	/* -10 // 3 == -3.333 == ROUND UP TO -3 */
	Expect( -3 == ObjectMemory_integerValueOf( resultShortInteger) ); 
}


void InterpreterIntegerPrimitiveArithmeticTests(struct TestResult *tr) {
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
	RunTest(PrimitiveDivideFailsIfArgumentZero);
	RunTest(PrimitiveDivideFailsIfWontDivideCleanly);
	RunTest(PrimitiveDivideFailsIfAlreadyFailed);
/*	RunTest(PrimitiveDivideFailsIfAnswerNotInteger);*/
	RunTest(PrimitiveDivideFailsIfReceiverNotInteger);
	RunTest(PrimitiveDivideFailsIfArgumentNotInteger);

	RunTest(PrimitiveDivBasic);
	RunTest(PrimitiveDivFailsIfArgumentZero);
	RunTest(PrimitiveDivFailsIfAlreadyFailed);
/*	RunTest(PrimitiveDivFailsIfAnswerNotInteger);*/
	RunTest(PrimitiveDivFailsIfReceiverNotInteger);
	RunTest(PrimitiveDivFailsIfArgumentNotInteger);
	RunTest(PrimitiveDivRoundsNegativeToNegativeInfinity);
	RunTest(PrimitiveDivRoundsPositiveDown);

	RunTest(PrimitiveModBasic); 
	RunTest(PrimitiveModFailsIfArgumentZero);
	RunTest(PrimitiveModFailsIfAlreadyFailed);
/*	RunTest(PrimitiveModFailsIfAnswerNotInteger);*/
	RunTest(PrimitiveModFailsIfReceiverNotInteger);
	RunTest(PrimitiveModFailsIfArgumentNotInteger);
	RunTest(PrimitiveModRoundsToNegativeInfinity);

	RunTest(PrimitiveQuoBasic);
	RunTest(PrimitiveQuoFailsIfArgumentZero);
	RunTest(PrimitiveQuoFailsIfAlreadyFailed);
/*	RunTest(PrimitiveQuoFailsIfAnswerNotInteger);*/
	RunTest(PrimitiveQuoFailsIfReceiverNotInteger);
	RunTest(PrimitiveQuoFailsIfArgumentNotInteger);
	RunTest(PrimitiveQuoRoundsPositiveDown);
	RunTest(PrimitiveQuoRoundsNegativeUpTowardZero); 
}
