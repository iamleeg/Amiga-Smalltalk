#include "Interpreter.h"
#include "Interpreter_PrimSystem.h"
#include "ObjectMemory.h"
#include <stdlib.h> //abort

void Interpreter_dispatchSystemPrimitives( int primitiveIndex ) {
	switch( primitiveIndex ) {
		case 110: Interpreter_primitiveEquivalent();
		case 111: Interpreter_primitiveClass();
		case 112: Interpreter_primitiveCoreLeft();
		case 113: Interpreter_primitiveQuit();
		case 114: Interpreter_primitiveExitToDebugger();
		case 115: Interpreter_primitiveOopsLeft();
		case 116: Interpreter_primitiveSignalAtOopsLeftWordsLeft();
	}
}

void Interpreter_primitiveEquivalent() {
   ObjectPointer otherObject = Interpreter_popStack();
   ObjectPointer receiver = Interpreter_popStack();

    if (receiver == otherObject) {
        Interpreter_push(TruePointer);
    } else {
        Interpreter_push(FalsePointer);
    }
}

void Interpreter_primitiveClass() {
	ObjectPointer anObject = Interpreter_popStack();
    Interpreter_push(ObjectMemory_fetchClassOf(anObject));
}

void Interpreter_primitiveCoreLeft() {
	// Need ObjectMemory to track free words, maybe in GC ?
}

void Interpreter_primitiveQuit() {
	// somehow tell the event look we have asked to quit 
	// and let it stop gracefully
}

void Interpreter_primitiveExitToDebugger() {
	abort();
}

void Interpreter_primitiveOopsLeft() {
	// Need ObjectMemory to track free oops, maybe in GC ?
}


void Interpreter_primitiveSignalAtOopsLeftWordsLeft() {
/*
     (from the sources)
     Tell the object memory to signal the Semaphore when either the number
      of object pointers remaining drops below numOops, or the number of
     words in the object space remaining drops below numWords.  Fail if the
     first argument is neither a Semaphore nor nil.  Fail if numOops is not a
     16-bit Integer, or if numWords is not a 32-bit LargePositiveInteger.
*/
}