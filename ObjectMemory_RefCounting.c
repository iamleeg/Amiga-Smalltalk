#include "ObjectMemory_RefCounting.h"
#include "ObjectMemory_Allocation.h"
#include "ObjectMemory_Storage.h"

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

ObjectPointer ObjectMemory_forAllOtherObjectsAccessibleFrom_suchThat_do(ObjectPointer objectPointer,
  Word(*predicate)(ObjectPointer),
  void(*action)(ObjectPointer)) {
  ObjectPointer next;
  Word offset;
  for (offset = 0; offset < ObjectMemory_lastPointerOf(objectPointer); offset++) {
    next = ObjectMemory_heapChunkOf_word(objectPointer, offset);
    if ((ObjectMemory_isIntegerObject(next) == 0) && predicate(next)) {
      // It's a non-immediate object and it should be processed
      ObjectMemory_forAllOtherObjectsAccessibleFrom_suchThat_do(next,
        predicate,
	action);
    }
  }
  // all pointers have been followed; now perform the action
  action(objectPointer);
  return objectPointer;
}
