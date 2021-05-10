#ifndef AMIGA_SMALLTALK_INTERPRETERPRIMSTREAM_H
#define AMIGA_SMALLTALK_INTERPRETERPRIMSTREAM_H

#include "Types.h"

void Interpreter_dispatchSubscriptAndStreamPrimitives(int primitiveIndex);

void Interpreter_primitiveAt(void);
void Interpreter_primitiveAtPut(void);
void Interpreter_primitiveSize(void);
void Interpreter_primitiveStringAt(void);
void Interpreter_primitiveStringAtPut(void);
void Interpreter_primitiveNext(void);
void Interpreter_primitiveNextPut(void);
void Interpreter_primitiveAtEnd(void);

#endif