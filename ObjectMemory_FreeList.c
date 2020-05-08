#include "ObjectMemory_Constants.h"
#include "ObjectMemory_FreeList.h"
#include "ObjectMemory_Storage.h"
#include "RealWordMemory.h"

Word currentSegment = 0;

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

void ObjectMemory_toFreeChunkList_add(Word size, ObjectPointer objectPointer) {
  Word segment = ObjectMemory_segmentBitsOf(objectPointer);
  ObjectMemory_classBitsOf_put(objectPointer, ObjectMemory_headOfFreeChunkList_inSegment(size, segment));
  ObjectMemory_headOfFreeChunkList_inSegment_put(size, segment, objectPointer);
}

ObjectPointer ObjectMemory_removeFromFreeChunkList(Word size) {
  ObjectPointer objectPointer = ObjectMemory_headOfFreeChunkList_inSegment(size, currentSegment), secondChunk;
  if (objectPointer == NonPointer) {
    return NilPointer;
  }
  secondChunk = ObjectMemory_classBitsOf(objectPointer);
  ObjectMemory_headOfFreeChunkList_inSegment_put(size, currentSegment, secondChunk);
  return objectPointer;
}

void ObjectMemory_resetFreeChunkList_inSegment(Word size, Word segment) {
  ObjectMemory_headOfFreeChunkList_inSegment_put(size, segment, NonPointer);
}
