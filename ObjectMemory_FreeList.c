#include "ObjectMemory.h"
#include "RealWordMemory.h"

ObjectPointer ObjectMemory_headOfFreePointerList(void) {
  return RealWordMemory_segment_word(ObjectTableSegment,
    FreePointerList);
}

void ObjectMemory_headOfFreePointerList_put(ObjectPointer value) {
  RealWordMemory_segment_word_put(ObjectTableSegment,
    FreePointerList,
    value);
}

void ObjectMemory_toFreePointerListAdd(ObjectPointer objectPointer) {
  ObjectMemory_locationBitsOf_put(objectPointer, ObjectMemory_headOfFreePointerList());
  ObjectMemory_headOfFreePointerList_put(objectPointer);
}

ObjectPointer ObjectMemory_removeFromFreePointerList() {
  ObjectPointer objectPointer = ObjectMemory_headOfFreePointerList();
  if (objectPointer == NonPointer) {
    return NilPointer;
  }
  ObjectMemory_headOfFreePointerList_put(ObjectMemory_locationBitsOf(objectPointer));
  return objectPointer;
}

ObjectPointer ObjectMemory_headOfFreeChunkList_inSegment(Word size, Word segment) {
  return RealWordMemory_segment_word(segment, FirstFreeChunkList + size);
}

void ObjectMemory_headOfFreeChunkList_inSegment_put(Word size, Word segment, ObjectPointer objectPointer) {
  RealWordMemory_segment_word_put(segment, FirstFreeChunkList + size, objectPointer);
}