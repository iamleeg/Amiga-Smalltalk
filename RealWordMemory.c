#include "RealWordMemory.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define HeapSegmentCount 16
#define FirstHeapSegment 0
#define LastHeapSegment 15
#define HeapSpaceStop 0xFFFF
#define HeaderSize 2 /* * sizeof(unsigned short) */

static Word *segmentPointers[HeapSegmentCount];

void RealWordMemory_new() {
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
