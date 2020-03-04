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