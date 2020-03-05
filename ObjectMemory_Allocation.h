#ifndef AMIGA_SMALLTALK_OBJECTMEMORY_ALLOCATION_H
#define AMIGA_SMALLTALK_OBJECTMEMORY_ALLOCATION_H

#include "Types.h"

/**
 * A preliminary algorithm for allocating an object of a given size and class.
 * As this will get replaced, this algorithm (from p668) is not tested.
 */
ObjectPointer ObjectMemory_allocate_class(Word size, ObjectPointer classPointer);

/**
 * A preliminary algorithm to allocate the memory needed to store an object of given size.
 */
ObjectPointer ObjectMemory_allocateChunk(Word size);

#endif
