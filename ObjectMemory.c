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

void ObjectMemory_ot_put(ObjectPointer objectPointer, short value) {
  RealWordMemory_segment_word_put(ObjectTableSegment,
    ObjectTableStart + objectPointer,
    value);
}

static short *segmentPointers[HeapSegmentCount];

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

void RealWordMemory_bit_indices_should_be_in_word_range(short firstBitIndex, short lastBitIndex) {
  assert(firstBitIndex >= 0 && firstBitIndex <= 15);
  assert(lastBitIndex >= firstBitIndex && lastBitIndex <= 15);
}

short RealWordMemory_mask_bits(short firstBitIndex, short lastBitIndex) {
  short i,j, mask = 0;
  for(i = 0, j = 15; i < 16; i++, j--) {
    if (i >= firstBitIndex && i <= lastBitIndex) {
      mask |= (1 << j);
    }
  }
  return mask;
}

void RealWordMemory_segment_word_bits_to_put(short s, short w, short firstBitIndex, short lastBitIndex, short value) {
  RealWordMemory_bit_indices_should_be_in_word_range(firstBitIndex, lastBitIndex);
  short mask, inverseMask, shiftedValue, currentStore, newStore;
  mask = RealWordMemory_mask_bits(firstBitIndex, lastBitIndex);
  inverseMask = ~mask;
  currentStore = RealWordMemory_segment_word(s, w);
  newStore = currentStore & inverseMask;
  shiftedValue = (value << (15 - lastBitIndex)) & mask;
  newStore |= shiftedValue;
  RealWordMemory_segment_word_put(s, w, newStore);
}

short RealWordMemory_segment_word_bits_to(short s, short w, short firstBitIndex, short lastBitIndex) {
  RealWordMemory_bit_indices_should_be_in_word_range(firstBitIndex, lastBitIndex);

  short mask, wordValue, maskedValue, shiftedValue;
  mask = RealWordMemory_mask_bits(firstBitIndex, lastBitIndex);
  wordValue = RealWordMemory_segment_word(s, w);
  maskedValue = wordValue & mask;
  shiftedValue = maskedValue >> (15 - lastBitIndex);
  return shiftedValue;
}
