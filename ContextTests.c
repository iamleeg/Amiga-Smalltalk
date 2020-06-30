#include "ContextTests.h"

#include "Interpreter.h"
#include "ObjectMemory.h"
#include "RealWordMemory.h"

ObjectPointer stubMethodContext(void) {
  ObjectPointer methodContext = 0x1220, location = 0x0ffe, method = 0x200c, methodLocation = 0x0f00, literal = NilPointer;
  Word segment = (2 % HeapSegmentCount), instructionPointer = 0x1000, stackPointer = 0x1008;

  ObjectMemory_segmentBitsOf_put(methodContext, segment);
  ObjectMemory_locationBitsOf_put(methodContext, location);
  ObjectMemory_segmentBitsOf_put(method, segment);
  ObjectMemory_locationBitsOf_put(method, methodLocation);
  ObjectMemory_storePointer_ofObject_withValue(LiteralStart, method, literal);
  Interpreter_storeInteger_ofObject_withValue(InstructionPointerIndex, methodContext, instructionPointer);
  Interpreter_storeInteger_ofObject_withValue(StackPointerIndex, methodContext, stackPointer);
  ObjectMemory_storePointer_ofObject_withValue(MethodIndex, methodContext, method);
  ObjectMemory_storePointer_ofObject_withValue(ReceiverIndex, methodContext, ZeroPointer);
  ObjectMemory_storePointer_ofObject_withValue(SenderIndex, methodContext, TwoPointer);
  ObjectMemory_storePointer_ofObject_withValue(1 + TempFrameStart, methodContext, TruePointer);

  return methodContext;
}

ObjectPointer stubBlockContext(void) {
  ObjectPointer context = 0x1222, location = 0x1ffc, homeMethod = stubMethodContext();
  Word segment = (2 % HeapSegmentCount), instructionPointer = 0x1004, stackPointer = 0x100c;

  ObjectMemory_segmentBitsOf_put(context, segment);
  ObjectMemory_locationBitsOf_put(context, location);
  Interpreter_storeInteger_ofObject_withValue(InstructionPointerIndex, context, instructionPointer);
  Interpreter_storeInteger_ofObject_withValue(StackPointerIndex, context, stackPointer);
  Interpreter_storeInteger_ofObject_withValue(BlockArgumentCountIndex, context, 2);
  ObjectMemory_storePointer_ofObject_withValue(HomeIndex, context, homeMethod);
  ObjectMemory_storePointer_ofObject_withValue(CallerIndex, context, OnePointer);
  return context;
}

void activateContextWithThreeObjectsOnTheStack(void) {
  ObjectPointer object1 = NilPointer, object2 = OnePointer, object3 = TwoPointer, context = stubMethodContext();
  activeContext = context;
  Interpreter_fetchContextRegisters();
  Interpreter_push(object1);
  Interpreter_push(object2);
  Interpreter_push(object3);
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

Test(FetchContextRegistersFromBlockContext) {
  ObjectPointer expectedHome = stubMethodContext();
  activeContext = stubBlockContext();

  Interpreter_fetchContextRegisters();

  Expect(homeContext == expectedHome);
  Expect(receiver == ZeroPointer);
  Expect(method == 0x200c);
  Expect(instructionPointer == 0x1004 - 1);
  Expect(stackPointer == 0x100c + TempFrameStart - 1);
}

Test(FetchContextRegistersFromMethodContext) {
  ObjectPointer expectedHome = stubMethodContext();
  activeContext = expectedHome;

  Interpreter_fetchContextRegisters();

  Expect(homeContext == expectedHome);
  Expect(receiver == ZeroPointer);
  Expect(method == 0x200c);
  Expect(instructionPointer == 0x1000 - 1);
  Expect(stackPointer == 0x1008 + TempFrameStart - 1);
}

Test(StoreContextRegisters) {
  instructionPointer = 0x2000;
  stackPointer = 0x2010;
  activeContext = stubMethodContext();

  Interpreter_storeContextRegisters();

  Expect(Interpreter_instructionPointerOfContext(activeContext) == instructionPointer + 1);
  Expect(Interpreter_stackPointerOfContext(activeContext) == stackPointer - TempFrameStart + 1);
}

Test(PushObjectOntoContextStack) {
  ObjectPointer object = NilPointer, context = stubMethodContext();
  Word firstStackPointer, secondStackPointer;
  activeContext = context;
  Interpreter_fetchContextRegisters();
  firstStackPointer = stackPointer;

  Interpreter_push(object);
  secondStackPointer = stackPointer;

  Expect(secondStackPointer - firstStackPointer == 1);
  Expect(ObjectMemory_fetchPointer_ofObject(stackPointer, activeContext) == object);
}

Test(PopContextStack) {
  ObjectPointer object = NilPointer, context = stubMethodContext(), popped;
  Word firstStackPointer, secondStackPointer;
  activeContext = context;
  Interpreter_fetchContextRegisters();
  firstStackPointer = stackPointer;
  Interpreter_push(object);

  popped = Interpreter_popStack();
  secondStackPointer = stackPointer;

  Expect(secondStackPointer == firstStackPointer);
  Expect(popped == object);
}

Test(PeepStackTop) {
  ObjectPointer object = NilPointer, context = stubMethodContext(), peeped;
  activeContext = context;
  Interpreter_fetchContextRegisters();
  Interpreter_push(object);

  peeped = Interpreter_stackTop();
  Expect(peeped == object);
}

Test(ExamineValueOnStack) {
  ObjectPointer found;
  activateContextWithThreeObjectsOnTheStack();
  found = Interpreter_stackValue(2);
  Expect(found == NilPointer);
}

Test(PopStackByN) {
  ObjectPointer top;
  activateContextWithThreeObjectsOnTheStack();
  Interpreter_pop(2);
  top = Interpreter_stackTop();
  Expect(top == NilPointer);
}

Test(RoundTripPopAndUnPopByN) {
  ObjectPointer top;
  activateContextWithThreeObjectsOnTheStack();
  Interpreter_pop(2);
  Interpreter_unPop(2);
  top = Interpreter_stackTop();
  Expect(top == TwoPointer);
}

Test(ActivateContext) {
  ObjectPointer firstContext, secondContext = stubBlockContext(), currentSP = stackPointer;
  Word firstRefCount = 3, secondRefCount = 6, stackOffset = 2;

  activateContextWithThreeObjectsOnTheStack();
  firstContext = activeContext;
  ObjectMemory_countBitsOf_put(firstContext, firstRefCount);
  ObjectMemory_countBitsOf_put(secondContext, secondRefCount);
  Interpreter_pop(stackOffset);

  Interpreter_newActiveContext(secondContext);

  Expect(Interpreter_stackPointerOfContext(firstContext) == (currentSP - stackOffset - TempFrameStart + 1));
  Expect(ObjectMemory_countBitsOf(firstContext) == (firstRefCount - 1));
  Expect(ObjectMemory_countBitsOf(secondContext) == (secondRefCount + 1));
  Expect(activeContext == secondContext);
  Expect(instructionPointer == Interpreter_instructionPointerOfContext(secondContext) - 1);
  Expect(stackPointer == (Interpreter_stackPointerOfContext(secondContext) + TempFrameStart - 1));
}

Test(FetchSender) {
  ObjectPointer sender;
  activateContextWithThreeObjectsOnTheStack();
  sender = Interpreter_sender();
  Expect(sender == TwoPointer);
}

Test(FetchCaller) {
  ObjectPointer context = stubBlockContext(), caller;
  Interpreter_newActiveContext(context);
  caller = Interpreter_caller();
  Expect(caller == OnePointer);
}

Test(FetchTemporary) {
  ObjectPointer temp;
  activateContextWithThreeObjectsOnTheStack();
  temp = Interpreter_temporary(1);
  Expect(temp == TruePointer);
}

Test(FetchLiteral) {
  ObjectPointer literal;
  activateContextWithThreeObjectsOnTheStack();
  literal = Interpreter_literal(0);
  Expect(literal == NilPointer);
}

void ContextTests(struct TestResult *tr) {
  RunTest(FetchInstructionPointerFromContext);
  RunTest(UpdateInstructionPointerInContext);
  RunTest(FetchStackPointerFromContext);
  RunTest(StoreStackPointerInContext);
  RunTest(FetchArgumentCountOfBlockContext);
  RunTest(PositiveTestForBlockContext);
  RunTest(NegativeTestForBlockContext);
  RunTest(FetchContextRegistersFromBlockContext);
  RunTest(FetchContextRegistersFromMethodContext);
  RunTest(StoreContextRegisters);
  RunTest(PushObjectOntoContextStack);
  RunTest(PopContextStack);
  RunTest(PeepStackTop);
  RunTest(ExamineValueOnStack);
  RunTest(PopStackByN);
  RunTest(RoundTripPopAndUnPopByN);
  RunTest(ActivateContext);
  RunTest(FetchSender);
  RunTest(FetchCaller);
  RunTest(FetchTemporary);
  RunTest(FetchLiteral);
}
