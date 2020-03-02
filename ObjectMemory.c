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

Word ObjectMemory_isIntegerObject(Word objectPointer) {
  return (objectPointer & 0x0001);
}

Word ObjectMemory_ot(ObjectPointer objectPointer) {
  ObjectMemory_cantBeIntegerObject(objectPointer);
  return RealWordMemory_segment_word(ObjectTableSegment,
    ObjectTableStart + objectPointer);
}

void ObjectMemory_ot_put(ObjectPointer objectPointer, Word value) {
  RealWordMemory_segment_word_put(ObjectTableSegment,
    ObjectTableStart + objectPointer,
    value);
}

Word ObjectMemory_ot_bits_to(ObjectPointer objectPointer, Word firstBitIndex, Word lastBitIndex) {
  return RealWordMemory_segment_word_bits_to(ObjectTableSegment,
    ObjectTableStart + objectPointer,
    firstBitIndex,
    lastBitIndex);
}

void ObjectMemory_ot_bits_to_put(ObjectPointer objectPointer, Word firstBitIndex, Word lastBitIndex, Word value) {
  RealWordMemory_segment_word_bits_to_put(ObjectTableSegment,
    ObjectTableStart + objectPointer,
    firstBitIndex,
    lastBitIndex,
    value);
}

static Word *segmentPointers[HeapSegmentCount];

void ObjectMemory_new() {
  short i;
  for(i = FirstHeapSegment; i <= LastHeapSegment; i++) {
    segmentPointers[i] = malloc(HeapSpaceStop*sizeof(Word));
  }
}

void RealWordMemory_segment_word_put(Word s, Word w, Word value) {
  segmentPointers[s][w] = value;
}

Word RealWordMemory_segment_word(Word s, Word w) {
  return segmentPointers[s][w];
}

void RealWordMemory_bit_indices_should_be_in_word_range(Word firstBitIndex, Word lastBitIndex) {
  assert(firstBitIndex >= 0 && firstBitIndex <= 15);
  assert(lastBitIndex >= firstBitIndex && lastBitIndex <= 15);
}

Word RealWordMemory_mask_bits(Word firstBitIndex, Word lastBitIndex) {
  Word i,j, mask = 0;
  for(i = 0, j = 15; i < 16; i++, j--) {
    if (i >= firstBitIndex && i <= lastBitIndex) {
      mask |= (1 << j);
    }
  }
  return mask;
}

void RealWordMemory_segment_word_bits_to_put(Word s, Word w, Word firstBitIndex, Word lastBitIndex, Word value) {
  RealWordMemory_bit_indices_should_be_in_word_range(firstBitIndex, lastBitIndex);
  Word mask, inverseMask, shiftedValue, currentStore, newStore;
  mask = RealWordMemory_mask_bits(firstBitIndex, lastBitIndex);
  inverseMask = ~mask;
  currentStore = RealWordMemory_segment_word(s, w);
  newStore = currentStore & inverseMask;
  shiftedValue = (value << (15 - lastBitIndex)) & mask;
  newStore |= shiftedValue;
  RealWordMemory_segment_word_put(s, w, newStore);
}

Word RealWordMemory_segment_word_bits_to(Word s, Word w, Word firstBitIndex, Word lastBitIndex) {
  RealWordMemory_bit_indices_should_be_in_word_range(firstBitIndex, lastBitIndex);

  Word mask, wordValue, maskedValue, shiftedValue;
  mask = RealWordMemory_mask_bits(firstBitIndex, lastBitIndex);
  wordValue = RealWordMemory_segment_word(s, w);
  maskedValue = wordValue & mask;
  shiftedValue = maskedValue >> (15 - lastBitIndex);
  return shiftedValue;
}
