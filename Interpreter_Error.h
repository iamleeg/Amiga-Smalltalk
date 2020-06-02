#ifndef AMIGA_SMALLTALK_INTERPRETER_ERROR_H
#define AMIGA_SMALLTALK_INTERPRETER_ERROR_H

/**
 * Display an error to the user. The error is (currently) shown in the form of a requester,
 * giving the choice between carrying on, or quitting Smalltalk. Because this was an error
 * detected in the virtual machine, it doesn't make a lot of sense to jump into the Smalltalk
 * debugger - the VM might have already been running it when it encountered the error!
 */
void Interpreter_error(char *message);

#endif
