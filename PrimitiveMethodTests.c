#include "PrimitiveMethodTests.h"
#include "Interpreter.h"

Test(SetSuccessRegister) {
  success = YES;
  Interpreter_Success_(YES);
  Expect(success == YES);
  Interpreter_Success_(NO);
  Expect(success == NO);
  /* p616: success-setting is logical AND, so once it's failed, you can't un-fail. */
  Interpreter_Success_(YES);
  Expect(success == NO);
}

Test(FetchSuccessRegister) {
  success = NO;
  Expect(Interpreter_Success() == NO);
  success = YES;
  Expect(Interpreter_Success() == YES);
}

Test(InitializingPrimitiveSetsSuccess) {
  success = NO;
  Interpreter_initPrimitive();
  Expect(Interpreter_Success() == YES);
}

Test(FailingPrimitiveClearsSuccess) {
  success = YES;
  Interpreter_failPrimitive();
  Expect(Interpreter_Success() == NO);
}

void PrimitiveMethodTests(struct TestResult *tr) {
  RunTest(SetSuccessRegister);
  RunTest(FetchSuccessRegister);
  RunTest(InitializingPrimitiveSetsSuccess);
  RunTest(FailingPrimitiveClearsSuccess);
}
