#include "ObjectMemory.h"
#include "RealWordMemory.h"
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

Word ObjectMemory_countBitsOf(ObjectPointer objectPointer) {
  ObjectMemory_ot_bits_to(objectPointer, 0, 7);
}

void ObjectMemory_countBitsOf_put(ObjectPointer objectPointer, Word value) {
  ObjectMemory_ot_bits_to_put(objectPointer, 0, 7, value);
}

Word ObjectMemory_oddBitOf(ObjectPointer objectPointer) {
  ObjectMemory_ot_bits_to(objectPointer, 8, 8);
}

void ObjectMemory_oddBitOf_put(ObjectPointer objectPointer, Word value) {
  ObjectMemory_ot_bits_to_put(objectPointer, 8, 8, value);
}
