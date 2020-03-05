#include "ObjectMemory_Allocation.h"
#include "ObjectMemory_Constants.h"
#include "ObjectMemory_Storage.h"

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
  return NilPointer;
}