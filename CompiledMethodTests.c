#include "CompiledMethodTests.h"

#include "Interpreter.h"
#include "ObjectMemory.h"

ObjectPointer dummyCompiledMethod(void) {
  ObjectPointer compiledMethod = 0x1234, classPointer = MethodClass, header = OnePointer, firstLiteral = NilPointer;
  Word segment = 1, location = 0, size = 2;

  ObjectMemory_sizeBitsOf_put(compiledMethod, size);
  ObjectMemory_segmentBitsOf_put(compiledMethod, segment);
  ObjectMemory_locationBitsOf_put(compiledMethod, location);
  ObjectMemory_classBitsOf_put(compiledMethod, classPointer);
  ObjectMemory_storePointer_ofObject_withValue(0, compiledMethod, header);
  ObjectMemory_storePointer_ofObject_withValue(1, compiledMethod, firstLiteral);
  return compiledMethod;
}

ObjectPointer compiledMethodWithInterestingHeader(void) {
  ObjectPointer compiledMethod = dummyCompiledMethod();
  Word header = 0x0f5d;
  ObjectMemory_storePointer_ofObject_withValue(0, compiledMethod, header);

  return compiledMethod;
}

ObjectPointer compiledMethodWithExtension(void) {
  ObjectPointer compiledMethod = 0x1234, classPointer = MethodClass, header = 0xe107, firstLiteral = NilPointer;
  ObjectPointer extension = 0x0285;
  Word segment = 1, location = 0, size = 4;

  ObjectMemory_sizeBitsOf_put(compiledMethod, size);
  ObjectMemory_segmentBitsOf_put(compiledMethod, segment);
  ObjectMemory_locationBitsOf_put(compiledMethod, location);
  ObjectMemory_classBitsOf_put(compiledMethod, classPointer);
  ObjectMemory_storePointer_ofObject_withValue(0, compiledMethod, header);
  ObjectMemory_storePointer_ofObject_withValue(1, compiledMethod, firstLiteral);
  ObjectMemory_storePointer_ofObject_withValue(2, compiledMethod, extension);
  return compiledMethod;
}

Test(FindHeaderOfCompiledMethod) {
  ObjectPointer methodHeader, compiledMethod = dummyCompiledMethod();

  methodHeader = Interpreter_headerOf(compiledMethod);
  Expect(methodHeader == OnePointer);
}

Test(FindFirstLiteralInCompiledMethod) {
  ObjectPointer literal, compiledMethod = dummyCompiledMethod();

  literal = Interpreter_literal_ofMethod(0, compiledMethod);

  Expect(literal == NilPointer);
}

Test(ExtractTemporaryCountOfCompiledMethod) {
  ObjectPointer compiledMethod = compiledMethodWithInterestingHeader();
  Byte temporaryCount;

  temporaryCount = Interpreter_temporaryCountOf(compiledMethod);
  Expect(temporaryCount == 0x0f);
}

Test(ExtractLargeContextFlagFromCompiledMethod) {
  ObjectPointer compiledMethod = compiledMethodWithInterestingHeader();
  Bool largeContextFlag = Interpreter_largeContextFlagOf(compiledMethod);
  Expect(largeContextFlag == NO);
}

Test(ExtractLiteralCountFromCompiledMethod) {
  ObjectPointer compiledMethod = compiledMethodWithInterestingHeader();
  Byte literalCount = Interpreter_literalCountOf(compiledMethod);
  Byte expectedValue = 0x2e;
  Expect(literalCount == expectedValue);
}

Test(ExtractFlagValueFromCompiledMethod) {
  ObjectPointer compiledMethod = compiledMethodWithInterestingHeader();
  Byte flagValue = Interpreter_flagValueOf(compiledMethod);
  Expect(flagValue == 0);
}

Test(ExtractFieldIndexFromCompiledMethod) {
  ObjectPointer compiledMethod = compiledMethodWithInterestingHeader();
  Byte fieldIndex;

  fieldIndex = Interpreter_fieldIndexOf(compiledMethod);
  Expect(fieldIndex == 0x0f);
}

Test(ExtractHeaderExtensionFromCompiledMethod) {
  ObjectPointer compiledMethod = compiledMethodWithExtension(), headerExtension;
  headerExtension = Interpreter_headerExtensionOf(compiledMethod);
  Expect(headerExtension == 0x0285);
}

Test(ArgumentCountOfNonprimitiveMethod) {
  ObjectPointer compiledMethod = dummyCompiledMethod();
  Word header = 0x6f5b;
  Byte argumentCount;

  ObjectMemory_storePointer_ofObject_withValue(0, compiledMethod, header);

  argumentCount = Interpreter_argumentCountOf(compiledMethod);

  Expect(argumentCount == 3);
}

Test(ArgumentCountOfPrimitiveZeroArgMethod) {
  ObjectPointer compiledMethod = dummyCompiledMethod();
  Word header = 0xaf5b;
  Byte argumentCount;

  ObjectMemory_storePointer_ofObject_withValue(0, compiledMethod, header);

  argumentCount = Interpreter_argumentCountOf(compiledMethod);

  Expect(argumentCount == 0);
}

Test(ArgumentCountOfMethodWithHeaderExtension) {
  ObjectPointer compiledMethod = compiledMethodWithExtension();
  Byte argumentCount = Interpreter_argumentCountOf(compiledMethod);

  Expect(argumentCount == 1);
}

Test(PrimitiveIndexOfRegularMethodIsZero) {
  ObjectPointer compiledMethod = dummyCompiledMethod();
  Byte primitiveIndex = Interpreter_primitiveIndexOf(compiledMethod);

  Expect(primitiveIndex == 0);
}

Test(PrimitiveIndexOfMethodWithHeaderExtensionFoundInExtension) {
  ObjectPointer compiledMethod = compiledMethodWithExtension();
  Byte primitiveIndex = Interpreter_primitiveIndexOf(compiledMethod);

  Expect(primitiveIndex == 66);
}

Test(FindMethodClassOfMethodWithAssoication) {
  ObjectPointer compiledMethod = compiledMethodWithExtension(), classPointer = 0x3576, associationPointer = 0x3578, methodClassPointer;
  Word segment = 1, location = 0x4000, size = 3;
  ObjectMemory_segmentBitsOf_put(associationPointer, segment);
  ObjectMemory_locationBitsOf_put(associationPointer, location);
  ObjectMemory_sizeBitsOf_put(associationPointer, size);
  ObjectMemory_storePointer_ofObject_withValue(ValueIndex, associationPointer, classPointer);
  ObjectMemory_storePointer_ofObject_withValue(3, compiledMethod, associationPointer);

  methodClassPointer = Interpreter_methodClassOf(compiledMethod);

  Expect(methodClassPointer == classPointer);
}

void CompiledMethodTests(struct TestResult *tr) {
  RunTest(FindHeaderOfCompiledMethod);
  RunTest(FindFirstLiteralInCompiledMethod);
  RunTest(ExtractTemporaryCountOfCompiledMethod);
  RunTest(ExtractLargeContextFlagFromCompiledMethod);
  RunTest(ExtractLiteralCountFromCompiledMethod);
  RunTest(ExtractFlagValueFromCompiledMethod);
  RunTest(ExtractFieldIndexFromCompiledMethod);
  RunTest(ExtractHeaderExtensionFromCompiledMethod);
  RunTest(ArgumentCountOfNonprimitiveMethod);
  RunTest(ArgumentCountOfPrimitiveZeroArgMethod);
  RunTest(ArgumentCountOfMethodWithHeaderExtension);
  RunTest(PrimitiveIndexOfRegularMethodIsZero);
  RunTest(PrimitiveIndexOfMethodWithHeaderExtensionFoundInExtension);
  RunTest(FindMethodClassOfMethodWithAssoication);
}
