#ifndef AMIGA_SMALLTALK_OBJECTMEMORY_MARKINGCOLLECTOR_H
#define AMIGA_SMALLTALK_OBJECTMEMORY_MARKINGCOLLECTOR_H

#include "Types.h"

/**
 * Put the root object pointers in this list.
 * (And decide whether to change the data structure)
 */
ObjectPointer rootObjectPointers[0];

/**
 * The mark-and-sweep garbage collector collaborates with the reference counting collector.
 * It sets all reference counts to zero, marks all objects accessible from a root (the interpreter
 * stacks and the global Smalltalk dictionary), frees every unmarked object, then fixes up
 * the reference counts.
 */
void ObjectMemory_reclaimInaccessibleObjects(void);

/**
 * Set every object's reference count to zero. It is quicker to do this than to test whether
 * a particular object should be treated differently.
 */
void ObjectMemory_zeroReferenceCounts(void);

/**
 * Mark every object accessible from one of the root objects.
 */
void ObjectMemory_markAccessibleObjects(void);

/**
 * Mark every object accessible from a particular root object.
 */
void ObjectMemory_markObjectsAccessibleFrom(ObjectPointer rootObjectPointer);

/**
 * For each object, if it has been marked, decrement the count to unmark it, and increment
 * counts of all objects it references. If it is unmarked, free it.
 */
void ObjectMemory_rectifyCountsAndDeallocateGarbage(void);

#endif
