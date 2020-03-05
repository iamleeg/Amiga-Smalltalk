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
 * As this will get replaced, this algorithm (from p668) is not tested.
 */
ObjectPointer ObjectMemory_allocateChunk(Word size);

/**
 *  Find, allocate and return a free chunk of a given size on the heap.
 */
ObjectPointer ObjectMemory_attemptToAllocateChunk(Word size);

/**
 * Find a sufficiently-large chunk of free memory in the current segment.
 * If the available chunk is too big, return a pointer to a right-sized chunk and return the rest to the free list.
 * If there is no sufficiently large chunk in this segment, return NilPointer.
 */
ObjectPointer ObjectMemory_attemptToAllocateChunkInCurrentSegment(Word size);

/**
 * Remove a pointer from the free pointer list and initialise it to represent an allocated object.
 */
ObjectPointer ObjectMemory_obtainPointer_location(Word size, ObjectPointer location);

/**
 * 
 */
void ObjectMemory_compactCurrentSegment(void);

/**
 * Preliminary version of the deallocation routine from p670.
 */
void ObjectMemory_deallocate(ObjectPointer objectPointer);

#endif
