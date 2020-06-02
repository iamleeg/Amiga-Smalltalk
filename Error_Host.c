#include "Interpreter_Error.h"

int main(int argc, const char *argv[])
{
  /*
   * I'm writing the string in here because libaxrt doesn't let me use argc/argv yet
   */
  Interpreter_error("Recursive not understood error encountered");
  return 0;
}