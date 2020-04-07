#ifndef AMIGA_SMALLTALK_INTERPRETER_H
#define AMIGA_SMALLTALK_INTERPRETER_H

#include "Types.h"

/**
 * This register indicates whether a primitive method succeeded.
 */
extern Bool success;

/**
 * Set the success register.
 * @note Actually ANDs the existing and new value, so if you previously failed, you will never un-fail.
 */
void Interpreter_Success_(Bool successValue);

/**
 * Retrieve the success register.
 */
Bool Interpreter_Success(void);

/**
 * Get ready to enter a primitive method.
 */
void Interpreter_initPrimitive(void);

/**
 * Indicate that execution of a primitive method failed.
 */
void Interpreter_failPrimitive(void);

#endif
