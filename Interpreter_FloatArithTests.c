#include "Interpreter_FloatArithTests.h"
#include "Interpreter.h"
#include "Interpreter_PrimArith.h"

extern ObjectPointer stubBlockContext(void);

/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE ADD --------------------------------------------------------------------- */ 
/*  ----------------------------------------------------------------------------------- */ 


#define CMP(x) ((int)(x*1000))

Test(PrimitiveFloatAddBasic) {
	Bool localSuccess = NO;
	float result = 0.0;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.1);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(4.1);
    Expect(Interpreter_success() == YES );
    
/* call add */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatAdd();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );

	/* 3.1 + 4.1 == 7.2 */
	result = Interpreter_popFloat();
	Expect( CMP(7.2) == CMP(result) ); 
}




/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE SUBTRACT ---------------------------------------------------------------- */ 
/*  ----------------------------------------------------------------------------------- */ 

Test(PrimitiveFloatSubtractBasic) {
	Bool localSuccess = NO;
	float result = 0.0;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(7.5);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(4.2);
    Expect(Interpreter_success() == YES );
    
/* call subtract */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatSubtract();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
	/* 7.5 - 4.2 == 3.3 */
	result = Interpreter_popFloat();
	Expect( CMP(3.3) == CMP(result) ); 
}

/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE MULTIPLY ---------------------------------------------------------------- */ 
/*  ----------------------------------------------------------------------------------- */ 

Test(PrimitiveFloatMultiplyBasic) {
	Bool localSuccess = NO;
	float result = 0.0;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.0);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(1.1);
    Expect(Interpreter_success() == YES );
    
/* call multiply */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatMultiply();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
	/* 3.0 * 1.1 == 3.3 */
	result = Interpreter_popFloat();

	Expect( CMP(3.3) == CMP(result) ); 
}

/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE DIVIDE ------------------------------------------------------------------ */ 
/*  ----------------------------------------------------------------------------------- */ 

Test(PrimitiveFloatDivideBasic) {
	Bool localSuccess = NO;
	float result = 0.0;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(10.0);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(2.0);
    Expect(Interpreter_success() == YES );
    
/* call divide */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatDivide();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );
	
	/* 10.0 / 2.0 == 5.0 */
	result = Interpreter_popFloat();
	Expect( CMP(5.0) == CMP(result) ); 
}

Test(PrimitiveFloatDivideFailsIfArgumentZero) {
	Bool localSuccess = NO;
	ObjectPointer resultShortInteger = NilPointer;
	float pop1 = 99.0;
	float pop2 = 99.0;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(10.0);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(0.0);
    Expect(Interpreter_success() == YES );
    
/* call divide  10.0 / 0.0 */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatDivide();
	Expect( Interpreter_success() == NO );

/* assert success */
	Expect( localSuccess == NO );
	
/* assert stack unchanged */
	pop1 = Interpreter_popFloat();
	pop2 = Interpreter_popFloat();

	Expect( CMP(0.0) == CMP(pop1) );
	Expect( CMP(10.0) == CMP(pop2) );
}

/*  ----------------------------------------------------------------------------------- */ 
/*  PRIMITIVE COMPARE ----------------------------------------------------------------- */ 
/*  ----------------------------------------------------------------------------------- */ 

Test(PrimitiveFloatEqualsTrue) {
	Bool localSuccess = NO;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.1);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.1);
    Expect(Interpreter_success() == YES );
    
/* call primitive */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatEqual();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );

	/* 3.1 == 3.1 */
	Expect( TruePointer == Interpreter_popStack() ); 
}

Test(PrimitiveFloatEqualsFalse) {
	Bool localSuccess = NO;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.1);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(4.1);
    Expect(Interpreter_success() == YES );
    
/* call primitive */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatEqual();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );

	/* 3.1 == 4.1  */
	Expect( FalsePointer == Interpreter_popStack() ); 
}

Test(PrimitiveFloatNotEqualsTrue) {
	Bool localSuccess = NO;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.1);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(4.1);
    Expect(Interpreter_success() == YES );
    
/* call primitive */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatNotEqual();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );

	/* 3.1 != 4.1 */
	Expect( TruePointer == Interpreter_popStack() ); 
}

Test(PrimitiveFloatNotEqualsFalse) {
	Bool localSuccess = NO;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.1);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.1);
    Expect(Interpreter_success() == YES );
    
/* call primitive */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatNotEqual();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );

	/* 3.1 != 3.1  */
	Expect( FalsePointer == Interpreter_popStack() ); 
}

Test(PrimitiveFloatGreaterThanTrue) {
	Bool localSuccess = NO;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.1);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(2.1);
    Expect(Interpreter_success() == YES );
    
/* call primitive */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatGreaterThan();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );

	/* 3.1 > 2.1 */
	Expect( TruePointer == Interpreter_popStack() ); 
}

Test(PrimitiveFloatGreaterThanFalse) {
	Bool localSuccess = NO;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(2.1);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.1);
    Expect(Interpreter_success() == YES );
    
/* call primitive */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatGreaterThan();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );

	/* 2.1 > 3.1  */
	Expect( FalsePointer == Interpreter_popStack() ); 
}

Test(PrimitiveFloatLessThanTrue) {
	Bool localSuccess = NO;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.1);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(4.1);
    Expect(Interpreter_success() == YES );
    
/* call primitive */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatLessThan();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );

	/* 3.1 < 4.1 */
	Expect( TruePointer == Interpreter_popStack() ); 
}

Test(PrimitiveFloatLessThanFalse) {
	Bool localSuccess = NO;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(4.1);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.1);
    Expect(Interpreter_success() == YES );
    
/* call primitive */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatLessThan();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );

	/* 4.1 < 3.1  */
	Expect( FalsePointer == Interpreter_popStack() ); 
}

Test(PrimitiveFloatGreaterEqualTrue) {
	Bool localSuccess = NO;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(4.1);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(4.1);
    Expect(Interpreter_success() == YES );
    
/* call primitive */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatGreaterOrEqual();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );

	/* 4.1 >= 4.1 */
	Expect( TruePointer == Interpreter_popStack() ); 
}

Test(PrimitiveFloatGreaterEqualFalse) {
	Bool localSuccess = NO;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.1);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(4.1);
    Expect(Interpreter_success() == YES );
    
/* call primitive */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatGreaterOrEqual();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );

	/* 3.1 >= 4.1  */
	Expect( FalsePointer == Interpreter_popStack() ); 
}

Test(PrimitiveFloatLessEqualTrue) {
	Bool localSuccess = NO;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.1);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.1);
    Expect(Interpreter_success() == YES );
    
/* call primitive */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatLessOrEqual();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );

	/* 3.1 <= 3.1 */
	Expect( TruePointer == Interpreter_popStack() ); 
}

Test(PrimitiveFloatLessEqualFalse) {
	Bool localSuccess = NO;

	activeContext = stubBlockContext();
    Interpreter_fetchContextRegisters();
    
/* put two numbers on the stack */
	Interpreter_initPrimitive();
    Interpreter_pushFloat(4.1);
    Expect(Interpreter_success() == YES );

	Interpreter_initPrimitive();
    Interpreter_pushFloat(3.1);
    Expect(Interpreter_success() == YES );
    
/* call primitive */
	Interpreter_initPrimitive();
    localSuccess = Interpreter_primitiveFloatLessOrEqual();
	Expect( Interpreter_success() == YES );

/* assert success */
	Expect( localSuccess == YES );

	/* 4.1 <= 3.1  */
	Expect( FalsePointer == Interpreter_popStack() ); 
}



void InterpreterFloatPrimitiveArithmeticTests(struct TestResult *tr) {
	RunTest(PrimitiveFloatAddBasic);
	RunTest(PrimitiveFloatSubtractBasic);
	RunTest(PrimitiveFloatMultiplyBasic);
	RunTest(PrimitiveFloatDivideBasic);
	RunTest(PrimitiveFloatDivideFailsIfArgumentZero);
	
	RunTest(PrimitiveFloatEqualsTrue);
	RunTest(PrimitiveFloatEqualsFalse);
 	RunTest(PrimitiveFloatNotEqualsTrue);
	RunTest(PrimitiveFloatNotEqualsFalse);
	
	RunTest(PrimitiveFloatGreaterThanTrue);
	RunTest(PrimitiveFloatGreaterThanFalse);
	RunTest(PrimitiveFloatLessThanTrue);
	RunTest(PrimitiveFloatLessThanFalse);

	RunTest(PrimitiveFloatGreaterEqualTrue);
	RunTest(PrimitiveFloatGreaterEqualFalse);
	RunTest(PrimitiveFloatLessEqualTrue);
	RunTest(PrimitiveFloatLessEqualFalse);
}
