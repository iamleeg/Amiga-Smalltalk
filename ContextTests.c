#include "ContextTests.h"

#include "Interpreter.h"
#include "ObjectMemory.h"

ObjectPointer stubMethodContext() {
  ObjectPointer methodContext = 0x1220, location = 0x0ffe;
  Word segment = 2, instructionPointer = 0x1000;

  ObjectMemory_segmentBitsOf_put(methodContext, segment);
  ObjectMemory_locationBitsOf_put(methodContext, location);
  Interpreter_storeInteger_ofObject_withValue(InstructionPointerIndex, methodContext, instructionPointer);

  return methodContext;
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

void ContextTests(struct TestResult *tr) {
  RunTest(FetchInstructionPointerFromContext);
  RunTest(UpdateInstructionPointerInContext);
}
