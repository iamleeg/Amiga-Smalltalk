#ifndef AMIGA_SMALLTALK_REALWORDMEMORY_H
#define AMIGA_SMALLTALK_REALWORDMEMORY_H

#include "Types.h"

/**
 * The number of memory segments comprising the heap.
 */
#define HeapSegmentCount 16
/**
 * The index of the first heap segment.
*/
#define FirstHeapSegment 0
/**
 * The index of the last heap segment.
 */
#define LastHeapSegment 15
/**
 * The last address used in any heap segment.
 */
#define HeapSpaceStop 0xffca

/**
 * Initialize the VM's Memory. This MUST be done once before
 * using the VM.
 */
void RealWordMemory_new(void);

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

/**
 * Retrieve the value of the memory in the specified byte of the given word and segment address.
 * The byte must be 0 (MSB) or 1 (LSB)
 */
Byte RealWordMemory_segment_word_byte(Word s, Word w, Byte b);

/**
 * Set the byte in the specified segment and word to the given value.
 * The byte must be 0 (MSB) or 1 (LSB)
 */
void RealWordMemory_segment_word_byte_put(Word s, Word w, Byte b, Byte value);

#endif
