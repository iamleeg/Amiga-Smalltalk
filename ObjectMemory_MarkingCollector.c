#include "ObjectMemory_MarkingCollector.h"
#include "ObjectMemory_Allocation.h"
#include "ObjectMemory_Constants.h"
#include "ObjectMemory_FreeList.h"
#include "ObjectMemory_RefCounting.h"
#include "ObjectMemory_Storage.h"
#include "RealWordMemory.h"

void ObjectMemory_reclaimInaccessibleObjects(void) {
  ObjectMemory_zeroReferenceCounts();
  ObjectMemory_markAccessibleObjects();
  ObjectMemory_rectifyCountsAndDeallocateGarbage();
}

void ObjectMemory_zeroReferenceCounts(void) {
  ObjectPointer objectPointer;
  for (objectPointer = 0; objectPointer < ObjectTableSize; objectPointer += 2) {
    ObjectMemory_countBitsOf_put(objectPointer, 0);
  }
}

void ObjectMemory_markAccessibleObjects(void) {
  ObjectPointer rootObjectPointer;
  while(NO) { /* haven't defined any root objects yet */
    ObjectMemory_markObjectsAccessibleFrom(rootObjectPointer);
  }
}

Bool HELPER_mark_unmarked_objects(ObjectPointer objectPointer) {
  Bool unmarked;
  unmarked = ObjectMemory_countBitsOf(objectPointer) == 0;
  if (unmarked) {
    ObjectMemory_countBitsOf_put(objectPointer, 1);
  }
  return unmarked;
}

void HELPER_set_count_to_one(ObjectPointer objectPointer) {
  ObjectMemory_countBitsOf_put(objectPointer, 1);
}

void ObjectMemory_markObjectsAccessibleFrom(ObjectPointer rootObjectPointer) {
  ObjectMemory_forAllObjectsAccessibleFrom_suchThat_do(rootObjectPointer,
    HELPER_mark_unmarked_objects,
    HELPER_set_count_to_one);
}

void ObjectMemory_rectifyCountsAndDeallocateGarbage(void) {
  Word count, segment, size, offset;
  ObjectPointer objectPointer, rootObjectPointer;
  for (segment = FirstHeapSegment; segment <= LastHeapSegment; segment++) {
    for (size = HeaderSize; size < BigSize; size++) {
      // reset the list head
      ObjectMemory_resetFreeChunkList_inSegment(size, segment);
    }
  }
  // rectify counts, and deallocate garbage
  for (objectPointer = 0; objectPointer < ObjectTableSize; objectPointer += 2) {
    if (ObjectMemory_freeBitOf(objectPointer) == 0) {
      // if it is not a free entry
      if ((count = ObjectMemory_countBitsOf(objectPointer)) == 0) {
        // it is unmarked, so deallocate it
        ObjectMemory_deallocate(objectPointer);
      } else {
        // it is marked, so rectify reference counts
        if (count < 128) {
          // subtract 1 to compensate for the mark
          ObjectMemory_countBitsOf_put(objectPointer, count - 1);
        }
        for (offset = 1; offset < ObjectMemory_lastPointerOf(objectPointer); offset++) {
            ObjectMemory_countUp(ObjectMemory_heapChunkOf_word(objectPointer, offset));
        }
      }
    }
  }
  // be sure the root objects don't disappear
  while (NO) { // there aren't any root objects yet
    ObjectMemory_countUp(rootObjectPointer);
  }
  ObjectMemory_countBitsOf_put(NilPointer, 128);
}
