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

void ObjectMemory_increaseReferencesTo(ObjectPointer objectPointer) {
  ObjectMemory_countUp(objectPointer);
}

void ObjectMemory_decreaseReferencesTo(ObjectPointer objectPointer) {
  ObjectMemory_countDown(objectPointer);
}

ObjectPointer ObjectMemory_fetchClassOf(ObjectPointer objectPointer) {
  if (ObjectMemory_isIntegerObject(objectPointer)) {
    return IntegerClass;
  } else {
    return ObjectMemory_classBitsOf(objectPointer);
  }
}

Word ObjectMemory_fetchWordLengthOf(ObjectPointer objectPointer) {
  return ObjectMemory_sizeBitsOf(objectPointer) - HeaderSize;
}

Word ObjectMemory_fetchByteLengthOf(ObjectPointer objectPointer) {
  return (ObjectMemory_fetchWordLengthOf(objectPointer) * 2 - ObjectMemory_oddBitOf(objectPointer));
}