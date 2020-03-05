#include "ObjectMemory_Allocation.h"
#include "ObjectMemory_Constants.h"
#include "ObjectMemory_FreeList.h"
#include "ObjectMemory_Storage.h"
#include "RealWordMemory.h"

#include <stdio.h>
#include <stdlib.h>

ObjectPointer ObjectMemory_allocate_class(Word size, ObjectPointer classPointer) {
  Word i;
  ObjectPointer objectPointer = ObjectMemory_allocateChunk(size); // Actually allocate
  ObjectMemory_classBitsOf_put(objectPointer, classPointer); // fill in class
  // initialize all fields to the object table index of the object nil
  for (i = HeaderSize; i < size; i++) {
    ObjectMemory_heapChunkOf_word_put(objectPointer, i, NilPointer);
  }
  ObjectMemory_sizeBitsOf_put(objectPointer, size);
  return objectPointer;
}

ObjectPointer ObjectMemory_allocateChunk(Word size) {
  ObjectPointer objectPointer = ObjectMemory_attemptToAllocateChunk(size);
  if (objectPointer != NilPointer) {
    return objectPointer;
  }
  fprintf(stderr, "VM ran out of heap space\n");
  abort();
}

ObjectPointer ObjectMemory_attemptToAllocateChunk(Word size) {
  ObjectPointer objectPointer = ObjectMemory_attemptToAllocateChunkInCurrentSegment(size);
  short i;
  if (objectPointer != NilPointer) {
    return objectPointer;
  }
  for (i = 0; i < HeapSegmentCount; i++) {
    currentSegment = currentSegment + 1;
    if (currentSegment > LastHeapSegment) {
      currentSegment = FirstHeapSegment;
    }
    ObjectMemory_compactCurrentSegment();
    objectPointer = ObjectMemory_attemptToAllocateChunkInCurrentSegment(size);
    if (objectPointer != NilPointer) {
      return objectPointer;
    }
  }
  return NilPointer;
}

ObjectPointer ObjectMemory_attemptToAllocateChunkInCurrentSegment(Word size) {
  ObjectPointer objectPointer, predecessor, next, newPointer;
  Word availableSize, excessSize;

  /*
   * According to p669, this should be the common case. Most objects are small,
   * and most allocations can be satisfied by recycling previously-freed objects.
   */
  if (size < BigSize) {
    objectPointer = ObjectMemory_removeFromFreeChunkList(size);
    if (objectPointer) {
      return objectPointer; // small chunk of exact size handy so use it
    }
  }

  /* we got here, so recycling an earlier allocation didn't work. Try to find a large chunk. */
  predecessor = NonPointer; // remember predecessor of chunk under consideration
  objectPointer = ObjectMemory_headOfFreeChunkList_inSegment(LastFreeChunkList, currentSegment);
  // the search loop stops when the end of the linked list is encountered
  while(objectPointer != NonPointer) {
    availableSize = ObjectMemory_sizeBitsOf(objectPointer);
    if (availableSize == size) { // exact fit - remove from free chunk list and return
      next = ObjectMemory_classBitsOf(objectPointer); // the link to the next chunk
      if (predecessor == NonPointer) {
        // it was the head of the list; make the next item the head
        ObjectMemory_headOfFreeChunkList_inSegment_put(LastFreeChunkList, currentSegment, next);
      } else {
        // it was in between two chunks; link them together
        ObjectMemory_classBitsOf_put(predecessor, next);
      }
      return objectPointer;
    }
    // this chunk was either too big or too small; inspect the amount of variance
    excessSize = availableSize - size;
    if (excessSize >= HeaderSize) {
      // can be broken into two usable parts; return the second part
      // obtain an object table entry for the second part
      newPointer = ObjectMemory_obtainPointer_location(size,
        ObjectMemory_locationBitsOf(objectPointer) + excessSize);
      if (newPointer == NilPointer) {
        return NilPointer;
      }
      // correct the size of the first part (which remains on the free list)
      ObjectMemory_sizeBitsOf_put(objectPointer, excessSize);
      return newPointer;
    } else {
      // not big enough to use; try the next chunk on the list
      predecessor = objectPointer;
      objectPointer = ObjectMemory_classBitsOf(objectPointer);
    }
  }
  return NilPointer; // the end of the linked list was reached and no fit was found
}

ObjectPointer ObjectMemory_obtainPointer_location(Word size, ObjectPointer location) {
  ObjectPointer objectPointer = ObjectMemory_removeFromFreePointerList();
  if (objectPointer == NilPointer) {
    return NilPointer;
  }
  ObjectMemory_ot_put(objectPointer, 0);
  ObjectMemory_segmentBitsOf_put(objectPointer, currentSegment);
  ObjectMemory_locationBitsOf_put(objectPointer, location);
  ObjectMemory_sizeBitsOf_put(objectPointer, size);
  return objectPointer;
}

void ObjectMemory_deallocate(ObjectPointer objectPointer) {
  Word space = ObjectMemory_spaceOccupiedBy(objectPointer);
  Word listSize = (space < BigSize) ? space : BigSize;
  ObjectMemory_toFreeChunkList_add(listSize, objectPointer);
}
