#ifndef AMIGA_SMALLTALK_OBJECTMEMORY_REFCOUNTING_H
#define AMIGA_SMALLTALK_OBJECTMEMORY_REFCOUNTING_H

#include "Types.h"

/**
 * Increment the reference count for an object and return that object.
 * The reference count overflows at 128; an object that attains that refcount
 * becomes permanent and its refcount never changes (which means it will
 * never be deallocated). This adds to the chance of memory leaking beyond the
 * presence of reference cycles; if this is a problem in practice then
 * periodic mark-and-sweep collection can be added to the system.
 */
ObjectPointer ObjectMemory_countUp(ObjectPointer objectPointer);

#endif
