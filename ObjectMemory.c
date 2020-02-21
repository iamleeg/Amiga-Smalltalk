#include "ObjectMemory.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void ObjectMemory_cantBeIntegerObject(ObjectPointer op) {
  if ((op & 0x0001) != 0) {
    fprintf(stderr, "Trying to use integer object %x", op);
  }
  assert((op & 0x0001) == 0);
}

short ObjectMemory_isIntegerObject(unsigned short objectPointer) {
  return (objectPointer & 0x0001);
}

short ObjectMemory_ot(ObjectPointer objectPointer) {
  ObjectMemory_cantBeIntegerObject(objectPointer);
  return RealWordMemory_segment_word(ObjectTableSegment,
    ObjectTableStart + objectPointer);
}

short *segmentPointers[HeapSegmentCount];

void ObjectMemory_new() {
  short i;
  for(i = FirstHeapSegment; i <= LastHeapSegment; i++) {
    segmentPointers[i] = malloc(HeapSpaceStop*sizeof(short));
  }
}

void RealWordMemory_segment_word_put(short s, short w, short value) {
  segmentPointers[s][w] = value;
}

short RealWordMemory_segment_word(short s, short w) {
  return segmentPointers[s][w];
}
