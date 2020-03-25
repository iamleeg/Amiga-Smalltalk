#include "ObjectMemory.h"

ObjectPointer ObjectMemory_fetchPointer_ofObject(Word fieldIndex, ObjectPointer objectPointer) {
  return ObjectMemory_heapChunkOf_word(objectPointer, HeaderSize + fieldIndex);
}

void ObjectMemory_storePointer_ofObject_withValue(Word fieldIndex, ObjectPointer objectPointer, ObjectPointer valuePointer) {
  Word chunkIndex = HeaderSize + fieldIndex;
  ObjectMemory_countUp(valuePointer);
  ObjectMemory_countDown(ObjectMemory_heapChunkOf_word(objectPointer, chunkIndex));
  ObjectMemory_heapChunkOf_word_put(objectPointer, chunkIndex, valuePointer);
}

Word ObjectMemory_fetchWord_ofObject(Word wordIndex, ObjectPointer objectPointer) {
  return ObjectMemory_heapChunkOf_word(objectPointer, HeaderSize + wordIndex);
}