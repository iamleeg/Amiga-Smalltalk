#include "ContextTests.h"

#include "Interpreter.h"
#include "ObjectMemory.h"

ObjectPointer stubMethodContext() {
  ObjectPointer methodContext = 0x1220, location = 0x0ffe;
  Word segment = 2, instructionPointer = 0x1000, stackPointer = 0x1008;

  ObjectMemory_segmentBitsOf_put(methodContext, segment);
  ObjectMemory_locationBitsOf_put(methodContext, location);
  Interpreter_storeInteger_ofObject_withValue(InstructionPointerIndex, methodContext, instructionPointer);
  Interpreter_storeInteger_ofObject_withValue(StackPointerIndex, methodContext, stackPointer);
  ObjectMemory_storePointer_ofObject_withValue(MethodIndex, methodContext, NilPointer);

  return methodContext;
}

ObjectPointer stubBlockContext() {
  ObjectPointer context = stubMethodContext();
  Interpreter_storeInteger_ofObject_withValue(BlockArgumentCountIndex, context, 2);

  return context;
}

Test(FetchInstructionPointerFromContext) {
  ObjectPointer methodContext = stubMethodContext();

  Word fetchedIP = Interpreter_instructionPointerOfContext(methodContext);

  Expect(fetchedIP == 0x1000);
}

Test(UpdateInstructionPointerInContext) {
  ObjectPointer methodContext = stubMethodContext();

  Word newIP = 0x1001;

  Interpreter_storeInstructionPointerValue_inContext(newIP, methodContext);

  Expect(Interpreter_instructionPointerOfContext(methodContext) == newIP);
}

Test(FetchStackPointerFromContext) {
  ObjectPointer methodContext = stubMethodContext();

  Word fetchedSP = Interpreter_stackPointerOfContext(methodContext);

  Expect(fetchedSP == 0x1008);
}

Test(StoreStackPointerInContext) {
  ObjectPointer methodContext = stubMethodContext();

  Word newSP = 0x100a;

  Interpreter_storeStackPointerValue_inContext(newSP, methodContext);

  Expect(Interpreter_stackPointerOfContext(methodContext) == newSP);
}

Test(FetchArgumentCountOfBlockContext) {
  ObjectPointer blockContext = stubBlockContext();
  Word argumentCount = Interpreter_argumentCountOfBlock(blockContext);
  Expect(argumentCount == 2);
}

Test(PositiveTestForBlockContext) {
  ObjectPointer blockContext = stubBlockContext();
  Bool isBlock = Interpreter_isBlockContext(blockContext);
  Expect(isBlock == YES);
}

Test(NegativeTestForBlockContext) {
  ObjectPointer methodContext = stubMethodContext();
  Bool isBlock = Interpreter_isBlockContext(methodContext);
  Expect(isBlock == NO);
}

void ContextTests(struct TestResult *tr) {
  RunTest(FetchInstructionPointerFromContext);
  RunTest(UpdateInstructionPointerInContext);
  RunTest(FetchStackPointerFromContext);
  RunTest(StoreStackPointerInContext);
  RunTest(FetchArgumentCountOfBlockContext);
  RunTest(PositiveTestForBlockContext);
  RunTest(NegativeTestForBlockContext);
}
