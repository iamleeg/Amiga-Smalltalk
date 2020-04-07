#include "Interpreter.h"

Bool success = NO;

void Interpreter_Success_(Bool successValue) {
  success = successValue && success;
}

Bool Interpreter_Success(void) {
  return success;
}

void Interpreter_initPrimitive(void) {
  success = YES;
}

void Interpreter_failPrimitive(void) {
  success = NO;
}
