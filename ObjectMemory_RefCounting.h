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

/**
 * Decrement the reference count for an object and return that object.
 * If the object's count reaches zero, then deallocate it, after decreasing
 * the count of all of the objects it refers to (recursively if necessary).
 */
ObjectPointer ObjectMemory_countDown(ObjectPointer rootObjectPointer);

/**
 * Perform the specified action on the object if the predicate is true.
 */
ObjectPointer ObjectMemory_forAllObjectsAccessibleFrom_suchThat_do(ObjectPointer objectPointer,
  Word(*predicate)(ObjectPointer),
  void(*action)(ObjectPointer));

/**
 * The recursive helper for the above routine. This is the implementation from p680
 * that reverses the object->object pointer references to use as an in-place stack,
 * avoiding potentially unbounded call stack usage.
 */
ObjectPointer ObjectMemory_forAllOtherObjectsAccessibleFrom_suchThat_do(ObjectPointer objectPointer,
  Word(*predicate)(ObjectPointer),
  void(*action)(ObjectPointer));

#endif
