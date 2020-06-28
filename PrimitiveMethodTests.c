#include "PrimitiveMethodTests.h"
#include "Interpreter.h"

Test(SetSuccessRegister) {
  success = YES;
  Interpreter_success_(YES);
  Expect(success == YES);
  Interpreter_success_(NO);
  Expect(success == NO);
  /* p616: success-setting is logical AND, so once it's failed, you can't un-fail. */
  Interpreter_success_(YES);
  Expect(success == NO);
}

Test(FetchSuccessRegister) {
  success = NO;
  Expect(Interpreter_success() == NO);
  success = YES;
  Expect(Interpreter_success() == YES);
}

Test(InitializingPrimitiveSetsSuccess) {
  success = NO;
  Interpreter_initPrimitive();
  Expect(Interpreter_success() == YES);
}

Test(FailingPrimitiveClearsSuccess) {
  success = YES;
  Interpreter_primitiveFail();
  Expect(Interpreter_success() == NO);
}

void PrimitiveMethodTests(struct TestResult *tr) {
  RunIsolatedTest(SetSuccessRegister);
  RunIsolatedTest(FetchSuccessRegister);
  RunIsolatedTest(InitializingPrimitiveSetsSuccess);
  RunIsolatedTest(FailingPrimitiveClearsSuccess);
}
