#ifndef AMIGA_SMALLTALK_INTERPRETERPRIMSYSTEM_H
#define AMIGA_SMALLTALK_INTERPRETERPRIMSYSTEM_H

#include "Types.h"

void Interpreter_dispatchSystemPrimitives( int primitiveIndex );

/** Page 653 - System primitive */
void Interpreter_primitiveEquivalent();
void Interpreter_primitiveClass();
void Interpreter_primitiveCoreLeft();
void Interpreter_primitiveQuit();
void Interpreter_primitiveExitToDebugger();
void Interpreter_primitiveOopsLeft();
void Interpreter_primitiveSignalAtOopsLeftWordsLeft();


#endif
