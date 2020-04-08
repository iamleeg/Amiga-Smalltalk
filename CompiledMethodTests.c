#include "CompiledMethodTests.h"

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

Test(FindHeaderOfCompiledMethod) {
  ObjectPointer methodHeader, compiledMethod = dummyCompiledMethod();

  methodHeader = Interpreter_headerOf(compiledMethod);
  Expect(methodHeader == OnePointer);
}

Test(FindFirstLiteralInCompiledMethod) {
  ObjectPointer literal, compiledMethod = dummyCompiledMethod();

  literal = Interpreter_literal_ofMethod(0, compiledMethod);
  
}

void CompiledMethodTests(struct TestResult *tr) {
  RunTest(FindHeaderOfCompiledMethod);
  RunTest(FindFirstLiteralInCompiledMethod);
}
