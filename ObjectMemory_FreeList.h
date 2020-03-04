#ifndef AMIGA_SMALLTALK_OBJECTMEMORY_FREELIST_H
#define AMIGA_SMALLTALK_OBJECTMEMORY_FREELIST_H

#include "Types.h"

/**
 * The location in the object table of the head of the free pointer list.
 */
#define FreePointerList 0x1000

/**
 * Return the head of the free pointer list.
 */
Word ObjectMemory_headOfFreePointerList(void);

/**
 * Set the head of the free pointer list.
 */
void ObjectMemory_headOfFreePointerList_put(Word value);

#endif
