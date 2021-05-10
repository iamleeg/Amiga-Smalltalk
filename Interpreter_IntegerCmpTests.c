#include "Interpreter_IntegerCmpTests.h"
#include "Interpreter.h"
#include "Interpreter_PrimArith.h"

extern ObjectPointer stubBlockContext(void);

/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE EQUALS ------------------------------------------------------------------ */ 
/*  ----------------------------------------------------------------------------------- */ 

Test(PrimitiveEqualTrueWorks) {
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
    Interpreter_primitiveEqual();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack(); 
	
/* assert  result == TRUE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == TruePointer );
}

Test(PrimitiveEqualFalseWorks) {
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
    Interpreter_primitiveEqual();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack();
	
/* assert  result == FALSE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == FalsePointer );
}

Test(PrimitiveEqualFailsIfReceiverNotInteger) {
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
    Interpreter_primitiveEqual();
	Expect( Interpreter_success() == NO );

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
    Interpreter_primitiveEqual();
	Expect( Interpreter_success() == NO );

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


/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE NOTEQUAL ---------------------------------------------------------------- */ 
/*  ----------------------------------------------------------------------------------- */ 

Test(PrimitiveNotEqualTrueWorks) {
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
    Interpreter_primitiveNotEqual();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack(); 
	
/* assert  result == TRUE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == TruePointer );
}

Test(PrimitiveNotEqualFalseWorks) {
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
    Interpreter_primitiveNotEqual();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack(); 
	
/* assert  result == FALSE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == FalsePointer );
}

Test(PrimitiveNotEqualFailsIfReceiverNotInteger) {
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
    Interpreter_primitiveNotEqual();
	Expect( Interpreter_success() == NO );

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
    Interpreter_primitiveNotEqual();
	Expect( Interpreter_success() == NO );

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

/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE GREATERTHAN ------------------------------------------------------------- */
/*  ----------------------------------------------------------------------------------- */ 
 
Test(PrimitiveGreaterThanTrueWorks) {
	ObjectPointer resultBool = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(20);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );
    
/* call greater */
/* 20 > 10 */

	Interpreter_initPrimitive();
    Interpreter_primitiveGreaterThan();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack(); 
	
/* assert  result == TRUE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == TruePointer );
}

Test(PrimitiveGreaterThanFalseWorks) {
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
    
/* call greater */
/* 10 > 20 */

	Interpreter_initPrimitive();
    Interpreter_primitiveGreaterThan();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack(); 
	
/* assert  result == FALSE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == FalsePointer );
}

Test(PrimitiveGreaterThanFalseIfEqualWorks) {
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
    
/* call greater */
/* 10 > 10 */

	Interpreter_initPrimitive();
    Interpreter_primitiveGreaterThan();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack(); 
	
/* assert  result == FALSE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == FalsePointer );
}

Test(PrimitiveGreaterThanFailsIfReceiverNotInteger) {
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
    
/* call greater */
/* 16500 > 10 */

	Interpreter_initPrimitive();
    Interpreter_primitiveGreaterThan();
	Expect( Interpreter_success() == NO );

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

Test(PrimitiveGreaterThanFailsIfArgumentNotInteger) {
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

/* call greater */
/* 10 > 16500 */

	Interpreter_initPrimitive();
    Interpreter_primitiveGreaterThan();
	Expect( Interpreter_success() == NO );

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

/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE LESSTHAN ---------------------------------------------------------------- */ 
/*  ----------------------------------------------------------------------------------- */ 

Test(PrimitiveLessThanTrueWorks) {
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
    
/* call less */
/* 10 < 20 */

	Interpreter_initPrimitive();
    Interpreter_primitiveLessThan();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack(); 
	
/* assert  result == TRUE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == TruePointer );
}

Test(PrimitiveLessThanFalseWorks) {
	ObjectPointer resultBool = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(20);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );
    
/* call less */
/* 20 < 10 */

	Interpreter_initPrimitive();
    Interpreter_primitiveLessThan();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack(); 
	
/* assert  result == FALSE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == FalsePointer );
}

Test(PrimitiveLessThanFalseIfEqualWorks) {
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
    
/* call greater */
/* 10 < 10 */

	Interpreter_initPrimitive();
    Interpreter_primitiveLessThan();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack(); 
	
/* assert  result == FALSE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == FalsePointer );
}

Test(PrimitiveLessThanFailsIfReceiverNotInteger) {
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
    
/* call less */
/* 16500 < 10 */

	Interpreter_initPrimitive();
    Interpreter_primitiveLessThan();
	Expect( Interpreter_success() == NO );

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

Test(PrimitiveLessThanFailsIfArgumentNotInteger) {
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

/* call less */
/* 10 < 16500 */

	Interpreter_initPrimitive();
    Interpreter_primitiveLessThan();
	Expect( Interpreter_success() == NO );

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

/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE LESSOREQUAL ------------------------------------------------------------- */ 
/*  ----------------------------------------------------------------------------------- */ 

Test(PrimitiveLessOrEqualThanTrueWorks) {
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
    
/* call lessorequal */
/* 10 <= 20 */

	Interpreter_initPrimitive();
    Interpreter_primitiveLessOrEqual();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack(); 
	
/* assert  result == TRUE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == TruePointer );
}

Test(PrimitiveLessOrEqualThanFalseWorks) {
	ObjectPointer resultBool = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(20);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );
    
/* call lessorequal */
/* 20 <= 10 */

	Interpreter_initPrimitive();
    Interpreter_primitiveLessOrEqual();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack(); 
	
/* assert  result == FALSE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == FalsePointer );
}

Test(PrimitiveLessOrEqualThanTrueIfEqualWorks) {
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
    
/* call lessorequal */
/* 10 <= 10 */

	Interpreter_initPrimitive();
    Interpreter_primitiveLessOrEqual();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack(); 
	
/* assert  result == TRUE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == TruePointer );
}

Test(PrimitiveLessOrEqualThanFailsIfReceiverNotInteger) {
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
    
/* call lessorequal */
/* 16500 <= 10 */

	Interpreter_initPrimitive();
    Interpreter_primitiveLessOrEqual();
	Expect( Interpreter_success() == NO );

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

Test(PrimitiveLessOrEqualThanFailsIfArgumentNotInteger) {
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

/* call lessorequal */
/* 10 <= 16500 */

	Interpreter_initPrimitive();
    Interpreter_primitiveLessOrEqual();
	Expect( Interpreter_success() == NO );

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


/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE GREATERTHANOREQUAL ------------------------------------------------------ */ 
/*  ----------------------------------------------------------------------------------- */ 

Test(PrimitiveGreaterOrEqualThanTrueWorks) {
	ObjectPointer resultBool = NilPointer;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushInteger(20);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushInteger(10);
    Expect(Interpreter_success() == YES );
    
/* call greaterorequal */
/* 20 >= 10 */

	Interpreter_initPrimitive();
    Interpreter_primitiveGreaterOrEqual();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack(); 
	
/* assert  result == TRUE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == TruePointer );
}

Test(PrimitiveGreaterOrEqualThanFalseWorks) {
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
    
/* call greaterorequal */
/* 10 >= 20 */

	Interpreter_initPrimitive();
    Interpreter_primitiveGreaterOrEqual();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack(); 
	
/* assert  result == FALSE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == FalsePointer );
}

Test(PrimitiveGreaterOrEqualThanTrueIfEqualWorks) {
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
    
/* call greaterorequal */
/* 10 >= 10 */

	Interpreter_initPrimitive();
    Interpreter_primitiveGreaterOrEqual();
	Expect( Interpreter_success() == YES );

	resultBool = Interpreter_popStack(); 
	
/* assert  result == TRUE */
	Expect( resultBool != NilPointer );
	Expect( resultBool == TruePointer );
}

Test(PrimitiveGreaterOrEqualThanFailsIfReceiverNotInteger) {
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
    
/* call greaterorequal */
/* 16500 >= 10 */

	Interpreter_initPrimitive();
    Interpreter_primitiveGreaterOrEqual();
	Expect( Interpreter_success() == NO );

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

Test(PrimitiveGreaterOrEqualThanFailsIfArgumentNotInteger) {
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

/* call greaterorequal */
/* 10 >= 16500 */

	Interpreter_initPrimitive();
    Interpreter_primitiveGreaterOrEqual();
	Expect( Interpreter_success() == NO );

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


void InterpreterIntegerPrimitiveComparisonTests(struct TestResult *tr) {
	RunTest(PrimitiveEqualTrueWorks);
	RunTest(PrimitiveEqualFalseWorks);
	RunTest(PrimitiveEqualFailsIfReceiverNotInteger);
	RunTest(PrimitiveEqualFailsIfArgumentNotInteger);

	RunTest(PrimitiveNotEqualTrueWorks);
	RunTest(PrimitiveNotEqualFalseWorks);
	RunTest(PrimitiveNotEqualFailsIfReceiverNotInteger);
	RunTest(PrimitiveNotEqualFailsIfArgumentNotInteger);

	RunTest(PrimitiveGreaterThanTrueWorks);
	RunTest(PrimitiveGreaterThanFalseWorks);
	RunTest(PrimitiveGreaterThanFalseIfEqualWorks);
	RunTest(PrimitiveGreaterThanFailsIfReceiverNotInteger);
	RunTest(PrimitiveGreaterThanFailsIfArgumentNotInteger);

	RunTest(PrimitiveLessThanTrueWorks);
	RunTest(PrimitiveLessThanFalseWorks);
	RunTest(PrimitiveLessThanFalseIfEqualWorks);
	RunTest(PrimitiveLessThanFailsIfReceiverNotInteger);
	RunTest(PrimitiveLessThanFailsIfArgumentNotInteger);

	RunTest(PrimitiveLessOrEqualThanTrueWorks);
	RunTest(PrimitiveLessOrEqualThanTrueIfEqualWorks);
	RunTest(PrimitiveLessOrEqualThanFalseWorks);
	RunTest(PrimitiveLessOrEqualThanFailsIfReceiverNotInteger);
	RunTest(PrimitiveLessOrEqualThanFailsIfArgumentNotInteger);

	RunTest(PrimitiveGreaterOrEqualThanTrueWorks);
	RunTest(PrimitiveGreaterOrEqualThanTrueIfEqualWorks);
	RunTest(PrimitiveGreaterOrEqualThanFalseWorks);
	RunTest(PrimitiveGreaterOrEqualThanFailsIfReceiverNotInteger);
	RunTest(PrimitiveGreaterOrEqualThanFailsIfArgumentNotInteger);
}
