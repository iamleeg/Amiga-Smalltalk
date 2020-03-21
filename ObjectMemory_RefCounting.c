#include "ObjectMemory_RefCounting.h"
#include "ObjectMemory_Allocation.h"
#include "ObjectMemory_Constants.h"
#include "ObjectMemory_FreeList.h"
#include "ObjectMemory_Storage.h"
#include "Types.h"

ObjectPointer ObjectMemory_countUp(ObjectPointer objectPointer) {
  Word count;
  if (!ObjectMemory_isIntegerObject(objectPointer)) {
    count = ObjectMemory_countBitsOf(objectPointer) + 1;
    if (count < 129) {
      ObjectMemory_countBitsOf_put(objectPointer, count);
    }
  }
  return objectPointer;
}

Word HELPER_decrementCountAndTestForZero(ObjectPointer objectPointer) {
  Word count = ObjectMemory_countBitsOf(objectPointer) - 1;
  if (count < 127) {
    ObjectMemory_countBitsOf_put(objectPointer, count);
  }
  return (count == 0);
}

void HELPER_zeroCountAndDeallocateObject(ObjectPointer objectPointer) {
  ObjectMemory_countBitsOf_put(objectPointer, 0);
  ObjectMemory_deallocate(objectPointer);
}

ObjectPointer ObjectMemory_countDown(ObjectPointer rootObjectPointer) {
  if (ObjectMemory_isIntegerObject(rootObjectPointer)) {
    return rootObjectPointer;
  } else {
    // this is a pointer, so decrement its reference count
    return ObjectMemory_forAllObjectsAccessibleFrom_suchThat_do(
      rootObjectPointer,
      HELPER_decrementCountAndTestForZero,
      HELPER_zeroCountAndDeallocateObject);
  }
}

ObjectPointer ObjectMemory_forAllObjectsAccessibleFrom_suchThat_do(ObjectPointer objectPointer,
  Word(*predicate)(ObjectPointer),
  void(*action)(ObjectPointer)) {
  if (predicate(objectPointer)) {
    return ObjectMemory_forAllOtherObjectsAccessibleFrom_suchThat_do(objectPointer,
      predicate,
      action);
  }
  /*
   * The specification (p678) doesn't indicate what to return if the predicate
   * is false, my best guess is to return objectPointer but do nothing.
   */
  return objectPointer;
}

/*
 * p681 has Smalltalk-style ranges 1 to n, this uses C-style 0 to n-1
 * which should explain differences in conditions and storage locations
 */
ObjectPointer ObjectMemory_forAllOtherObjectsAccessibleFrom_suchThat_do(ObjectPointer objectPointer,
  Word(*predicate)(ObjectPointer),
  void(*action)(ObjectPointer)) {
  ObjectPointer prior, current, next;
  Word offset, size;
  // compute prior, current, offset, and size to begin processing objectPointer
  prior = NonPointer;
  current = objectPointer;
  offset = size = ObjectMemory_lastPointerOf(objectPointer);
  while (YES) {
    offset = offset - 1; // decrement the field index
    if (offset > 1) { // the class field hasn't been passed yet
      next = ObjectMemory_heapChunkOf_word(current, offset); // one of the pointers
      if ((!ObjectMemory_isIntegerObject(next)) && predicate(next)) {
        // it's a non-immediate object and it should be processed
        // reverse the pointer chain
        ObjectMemory_heapChunkOf_word_put(current, offset, prior);
        // save the offset either in the count field or in the extra word
        if (size < HugeSize) {
          ObjectMemory_countBitsOf_put(current, offset);
        } else {
          ObjectMemory_heapChunkOf_word_put(current, size, offset);
        }
        // compute prior, current, offset, and size to begin processing next
        prior = current;
        current = next;
        offset = size = ObjectMemory_lastPointerOf(current);
      }
    } else {
      // all pointers have been followed; now perform the action
      action(current);
      // did we get here from another object?
      if (prior == NonPointer) {
        // this was the root object, so we are done
        return objectPointer;
      }
      // restore next, current, and size to resume processing prior
      next = current;
      current = prior;
      size = ObjectMemory_lastPointerOf(current);
      // restore offset either from the count field or from the extra word
      if (size < HugeSize) {
        offset = ObjectMemory_countBitsOf(current);
      } else {
        offset = ObjectMemory_heapChunkOf_word(current, size);
      }
      // restore prior from reversed pointer chain
      prior = ObjectMemory_heapChunkOf_word(current, offset);
      // restore (unreverse) the pointer chain
      ObjectMemory_heapChunkOf_word_put(current, offset, next);
    }
  }
}
