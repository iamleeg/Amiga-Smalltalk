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

void ObjectMemory_storeWord_ofObject_withValue(Word wordIndex, ObjectPointer objectPointer, Word valueWord) {
  ObjectMemory_heapChunkOf_word_put(objectPointer, HeaderSize + wordIndex, valueWord);
}

Byte ObjectMemory_fetchByte_ofObject(Word byteIndex, ObjectPointer objectPointer) {
  return ObjectMemory_heapChunkOf_byte(objectPointer, (HeaderSize * 2) + byteIndex);
}

void ObjectMemory_storeByte_ofObject_withValue(Word byteIndex, ObjectPointer objectPointer, Byte valueByte) {
  ObjectMemory_heapChunkOf_byte_put(objectPointer, (HeaderSize * 2 + byteIndex), valueByte);
}
