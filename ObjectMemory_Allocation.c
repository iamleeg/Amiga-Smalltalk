#include "ObjectMemory_Allocation.h"
#include "ObjectMemory_Constants.h"
#include "ObjectMemory_FreeList.h"
#include "ObjectMemory_RefCounting.h"
#include "ObjectMemory_Storage.h"
#include "RealWordMemory.h"

#include <stdio.h>
#include <stdlib.h>

ObjectPointer ObjectMemory_allocate_extra_class(Word size, Bool extraWord, ObjectPointer classPointer) {
  Word i;
  ObjectPointer objectPointer;
  ObjectMemory_countUp(classPointer); // increment the reference count of the class
  objectPointer = ObjectMemory_allocateChunk(size + extraWord); // allocate enough
  ObjectMemory_classBitsOf_put(objectPointer, classPointer);
  // initialize all fields to the object table index of the object nil /* p668 */
  for (i = HeaderSize; i < size; i++) {
    ObjectMemory_heapChunkOf_word_put(objectPointer, i, NilPointer);
  }
  // the next statement to correct the SIZE need only be executed if extraWord>0
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
  ObjectMemory_sizeBitsOf_put(objectPointer, space);
  ObjectMemory_toFreeChunkList_add(listSize, objectPointer);
}

ObjectPointer ObjectMemory_abandonFreeChunksInSegment(Word segment) {
  ObjectPointer lowWaterMark, objectPointer, nextPointer, location;
  Word size;
  lowWaterMark = HeapSpaceStop; // first assume that no chunk is free
  for (size = HeaderSize; size < BigSize; size ++) { // for each free-chunk list
    objectPointer = ObjectMemory_headOfFreeChunkList_inSegment(size, segment);
    while (objectPointer != NonPointer) {
      location = ObjectMemory_locationBitsOf(objectPointer);
      lowWaterMark = (lowWaterMark < location) ? lowWaterMark : location;
      nextPointer = ObjectMemory_classBitsOf(objectPointer); // link to next free chunk
      ObjectMemory_classBitsOf_put(objectPointer, NonPointer); // distinguish for sweep
      ObjectMemory_releasePointer(objectPointer); // add entry to free-pointer list
      objectPointer = nextPointer;
    }
    ObjectMemory_resetFreeChunkList_inSegment(size, segment);
  }
  return lowWaterMark;
}

void ObjectMemory_releasePointer(ObjectPointer objectPointer) {
  ObjectMemory_freeBitOf_put(objectPointer, 1);
  ObjectMemory_toFreePointerListAdd(objectPointer);
}

void ObjectMemory_reverseHeapPointersAbove(ObjectPointer lowWaterMark) {
  Word size;
  ObjectPointer objectPointer;
  for (objectPointer = 0; objectPointer < ObjectTableSize; objectPointer += 2) {
    if (ObjectMemory_freeBitOf(objectPointer) == 0) { // the object table entry is in use
      if (ObjectMemory_segmentBitsOf(objectPointer) == currentSegment) {
        // the object is in this segment
        if (ObjectMemory_locationBitsOf(objectPointer) >= lowWaterMark) {
          // the object will be swept
          size = ObjectMemory_sizeBitsOf(objectPointer); // rescue the size
          ObjectMemory_sizeBitsOf_put(objectPointer, objectPointer); // reverse the pointer
          ObjectMemory_locationBitsOf_put(objectPointer, size); // save the size
        }
      }
    }
  }
}

ObjectPointer ObjectMemory_sweepCurrentSegmentFrom(ObjectPointer lowWaterMark) {
  ObjectPointer si = lowWaterMark, di = lowWaterMark, objectPointer;
  Word size, i;
  while (si < HeapSpaceStop) { // for each object, si
    if (RealWordMemory_segment_word(currentSegment, si+1) == NonPointer) {
      // unallocated, so skip it
      size = RealWordMemory_segment_word(currentSegment, si);
      si += size;
    } else {
      // uallocated, so keep it, but move it to compact storage
      objectPointer = RealWordMemory_segment_word(currentSegment, si);
      size = ObjectMemory_locationBitsOf(objectPointer); // the reversed size
      ObjectMemory_locationBitsOf_put(objectPointer, di); // point object table at new location
      ObjectMemory_sizeBitsOf_put(objectPointer, size); // restore the size to its proper place
      si++; // skip the size
      di++; // skip the size
      for(i = 2; i <= ObjectMemory_spaceOccupiedBy(objectPointer); i++) {
        // move the rest of the object
        RealWordMemory_segment_word_put(currentSegment,
          di,
          RealWordMemory_segment_word(currentSegment,
            si));
        si++;
        di++;
      }
    }
  }
  return di;
}

void ObjectMemory_compactCurrentSegment() {
  ObjectPointer lowWaterMark, bigSpace;
  lowWaterMark = ObjectMemory_abandonFreeChunksInSegment(currentSegment);
  if (lowWaterMark < HeapSpaceStop) {
    ObjectMemory_reverseHeapPointersAbove(lowWaterMark);
    bigSpace = ObjectMemory_sweepCurrentSegmentFrom(lowWaterMark);
    ObjectMemory_deallocate(ObjectMemory_obtainPointer_location(HeapSpaceStop - bigSpace + 1,
      bigSpace));
  }
}
