enum Bytecode {
/** 
	Page 618/619 arithmeticSelectorPrimitive 
*/
	FirstArithmeticSelectorPrimitive = 176,
	PrimitiveAdd = 176,
	PrimitiveSubtract = 177,
	PrimitiveLessThan = 178,
	PrimitiveGreaterThan = 179,
	PrimitiveLessOrEqual = 180,
	PrimitiveGreaterOrEqual = 181,
	PrimitiveEqual = 182,
	PrimitiveNotEqual = 183,
	PrimitiveMultiply = 184,
	PrimitiveDivide = 185,
	PrimitiveMod = 186,
	PrimitiveMakePoint = 187,
	PrimitiveBitShift = 188,
	PrimitiveDiv = 189,
	PrimitiveBitAnd = 190,
	PrimitiveBitOr = 191,
	LastArithmeticSelectorPrimitive = 191,
	
	/** 
		Page 681 routes codes 192-207 through  commonSelectorPrimitive but that method
		seems to only handle 198 - 202 and does the same thing  for 201 and 202.  Maybe
		it'll be clearer later but for now, just going what I see on 681 */	
	FirstCommonSelectorPrimitive = 192,
	/* 192, 193, 194, 195, 196, 197? */
	PrimitiveEquivalent = 198,
	PrimitiveClass = 199,
	PrimitiveBlockCopy = 200,
	PrimitiveValue1 = 201,
	PrimitiveValue2 = 202,
	/* 203, 204, 205, 206, 207 */
	LastCommonSelectorPrimitive = 207
};