#ifndef AMIGA_SMALLTALK_OBJECTMEMORY_H
#define AMIGA_SMALLTALK_OBJECTMEMORY_H

typedef unsigned short Word;
typedef unsigned short ObjectPointer;

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
Word ObjectMemory_isIntegerObject(Word objectPointer);

/**
 * Retrieve the first word of an object table entry.
 */
Word ObjectMemory_ot(ObjectPointer objectPointer);

/**
 * Set the first word of an object table entry.
 */
void ObjectMemory_ot_put(ObjectPointer objectPointer, Word value);

/**
 * Retrieve the value of the specified bit range in an object table entry.
 * The bits are numbered 0 (MSB) to 15 (LSB). The range is inclusive of the
 * two index values.
 */
Word ObjectMemory_ot_bits_to(ObjectPointer objectPointer, Word firstBitIndex, Word lastBitIndex);

/**
 * Set the value of the specified bit range in an object table entry.
 * @see ObjectMemory_ot_bits_to
 */
void ObjectMemory_ot_bits_to_put(ObjectPointer objectPointer, Word firstBitIndex, Word lastBitIndex, Word value);

/**
 * Initialize the ObjectMemory. This MUST be done once before
 * using the VM.
 */
void ObjectMemory_new(void);

/**
 * Store a value at a given word in the specified segment in
 * RealWordMemory. No check is done: this just overwrites the value.
 */
void RealWordMemory_segment_word_put(Word s, Word w, Word value);

/**
 * Retrieve a value from the specified segment and word in
 * RealWordMemory.
 */
Word RealWordMemory_segment_word(Word s, Word w);

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
void RealWordMemory_segment_word_bits_to_put(Word s, Word w, Word firstBitIndex, Word lastBitIndex, Word value);

/**
 * Retrieve the value of memory at the specified segment and word between the given bit indices.
 * @see RealWordMemory_segment_word_bits_to_put
 */
Word RealWordMemory_segment_word_bits_to(Word s, Word w, Word firstBitIndex, Word lastBitIndex);

#endif
