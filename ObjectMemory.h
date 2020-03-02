#ifndef AMIGA_SMALLTALK_OBJECTMEMORY_H
#define AMIGA_SMALLTALK_OBJECTMEMORY_H

typedef short ObjectPointer;

#define HeapSegmentCount 16
#define FirstHeapSegment 0
#define LastHeapSegment 15
#define HeapSpaceStop 0xFFFF
#define HeaderSize 2 /* * sizeof(unsigned short) */

#define ObjectTableSegment 0
#define ObjectTableStart 0
#define ObjectTableSize 0xFFFF
#define HugeSize 0x0100
#define NilPointer 2

/**
 * Test whether a pointer is a tagged integer.
 */
short ObjectMemory_isIntegerObject(unsigned short objectPointer);

/**
 * Retrieve the first word of an object table entry.
 */
short ObjectMemory_ot(ObjectPointer objectPointer);

/**
 * Set the first word of an object table entry.
 */
void ObjectMemory_ot_put(ObjectPointer objectPointer, short value);

/**
 * Initialize the ObjectMemory. This MUST be done once before
 * using the VM.
 */
void ObjectMemory_new(void);

/**
 * Store a value at a given word in the specified segment in
 * RealWordMemory. No check is done: this just overwrites the value.
 */
void RealWordMemory_segment_word_put(short s, short w, short value);

/**
 * Retrieve a value from the specified segment and word in
 * RealWordMemory.
 */
short RealWordMemory_segment_word(short s, short w);

/**
 * Set the region of the specified segment and word between the given bit indices
 * to the supplied value.
 * Words are 16-bit, with the bits numbered from 0 (MSB) to 15 (LSB). Both indices
 * supplied here must be in that range, with the last bit index greater than or
 * equal to the first. The range is inclusive, so to set a single bit (e.g. bit 7)
 * supply firstBitIndex == lastBitIndex == 7.
 * 
 * This function does not check that the value supplied fits into the number of bits
 * implied by the range, but it does ensure that bits outside of the range are not
 * modified.
 */
void RealWordMemory_segment_word_bits_to_put(short s, short w, short firstBitIndex, short lastBitIndex, short value);

/**
 * Retrieve the value of memory at the specified segment and word between the given bit indices.
 * @see RealWordMemory_segment_word_bits_to_put
 */
short RealWordMemory_segment_word_bits_to(short s, short w, short firstBitIndex, short lastBitIndex);

#endif
