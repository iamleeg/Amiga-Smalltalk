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
  return ObjectMemory_ot_bits_to(objectPointer, 0, 7);
}

void ObjectMemory_countBitsOf_put(ObjectPointer objectPointer, Word value) {
  ObjectMemory_ot_bits_to_put(objectPointer, 0, 7, value);
}

Word ObjectMemory_oddBitOf(ObjectPointer objectPointer) {
  return ObjectMemory_ot_bits_to(objectPointer, 8, 8);
}

void ObjectMemory_oddBitOf_put(ObjectPointer objectPointer, Word value) {
  ObjectMemory_ot_bits_to_put(objectPointer, 8, 8, value);
}

Word ObjectMemory_pointerBitOf(ObjectPointer objectPointer) {
  return ObjectMemory_ot_bits_to(objectPointer, 9, 9);
}

void ObjectMemory_pointerBitOf_put(ObjectPointer objectPointer, Word value) {
  ObjectMemory_ot_bits_to_put(objectPointer, 9, 9, value);
}

Word ObjectMemory_freeBitOf(ObjectPointer objectPointer) {
  return ObjectMemory_ot_bits_to(objectPointer, 10, 10);
}

void ObjectMemory_freeBitOf_put(ObjectPointer objectPointer, Word value) {
  ObjectMemory_ot_bits_to_put(objectPointer, 10, 10, value);
}

Word ObjectMemory_segmentBitsOf(ObjectPointer objectPointer) {
  return ObjectMemory_ot_bits_to(objectPointer, 12, 15);
}

void ObjectMemory_segmentBitsOf_put(ObjectPointer objectPointer, Word value) {
  ObjectMemory_ot_bits_to_put(objectPointer, 12, 15, value);
}

Word ObjectMemory_locationBitsOf(ObjectPointer objectPointer) {
  ObjectMemory_cantBeIntegerObject(objectPointer);
  return RealWordMemory_segment_word(ObjectTableSegment,
    ObjectTableStart + objectPointer + 1);
}

void ObjectMemory_locationBitsOf_put(ObjectPointer objectPointer, Word value) {
  ObjectMemory_cantBeIntegerObject(objectPointer);
  RealWordMemory_segment_word_put(ObjectTableSegment,
    ObjectTableStart + objectPointer + 1,
    value);
}

Word ObjectMemory_heapChunkOf_word(ObjectPointer objectPointer, Word offset) {
  return RealWordMemory_segment_word(ObjectMemory_segmentBitsOf(objectPointer),
    ObjectMemory_locationBitsOf(objectPointer) + offset);
}

void ObjectMemory_heapChunkOf_word_put(ObjectPointer objectPointer, Word offset, Word value) {
  RealWordMemory_segment_word_put(ObjectMemory_segmentBitsOf(objectPointer),
    ObjectMemory_locationBitsOf(objectPointer) + offset,
    value);
}

Byte ObjectMemory_heapChunkOf_byte(ObjectPointer objectPointer, Word offset) {
  Word wordLocation = ObjectMemory_locationBitsOf(objectPointer) + (offset / 2);
  return RealWordMemory_segment_word_byte(ObjectMemory_segmentBitsOf(objectPointer),
    wordLocation,
    offset % 2);
}

void ObjectMemory_heapChunkOf_byte_put(ObjectPointer objectPointer, Word offset, Byte value) {
  Word wordLocation = ObjectMemory_locationBitsOf(objectPointer) + (offset / 2);
  RealWordMemory_segment_word_byte_put(ObjectMemory_segmentBitsOf(objectPointer),
    wordLocation,
    offset % 2,
    value);
}

Word ObjectMemory_sizeBitsOf(ObjectPointer objectPointer) {
  return ObjectMemory_heapChunkOf_word(objectPointer, 0);
}

void ObjectMemory_sizeBitsOf_put(ObjectPointer objectPointer, Word value) {
  ObjectMemory_heapChunkOf_word_put(objectPointer, 0, value);
}

Word ObjectMemory_classBitsOf(ObjectPointer objectPointer) {
  return ObjectMemory_heapChunkOf_word(objectPointer, 1);
}

void ObjectMemory_classBitsOf_put(ObjectPointer objectPointer, Word value) {
  ObjectMemory_heapChunkOf_word_put(objectPointer, 1, value);
}

Word ObjectMemory_lastPointerOf(ObjectPointer objectPointer) {
  return ObjectMemory_sizeBitsOf(objectPointer);
}

Word ObjectMemory_spaceOccupiedBy(ObjectPointer objectPointer) {
  Word size;
  size = ObjectMemory_sizeBitsOf(objectPointer);
  if (size < HugeSize) {
    return size;
  } else {
    return size + 1;
  }
}