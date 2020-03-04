#include "ObjectMemory.h"
#include "RealWordMemory.h"

Word ObjectMemory_headOfFreePointerList(void) {
  return RealWordMemory_segment_word(ObjectTableSegment,
    FreePointerList);
}

void ObjectMemory_headOfFreePointerList_put(Word value) {
  RealWordMemory_segment_word_put(ObjectTableSegment,
    FreePointerList,
    value);
}