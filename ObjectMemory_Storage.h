#ifndef AMIGA_SMALLTALK_OBJECTMEMORY_STORAGE_H
#define AMIGA_SMALLTALK_OBJECTMEMORY_STORAGE_H

#include "Types.h"

/**
 * Test whether a pointer is a tagged integer.
 */
Bool ObjectMemory_isIntegerObject(ObjectPointer objectPointer);

/**
 * Does an ObjectPointer point to a valid Object (freebit == 0 , countBits > 0)
 */
Bool ObjectMemory_hasObject(ObjectPointer objectPointer);

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
 * Return the object's reference count.
 */
Word ObjectMemory_countBitsOf(ObjectPointer objectPointer);

/**
 * Update the object's reference count.
 */
void ObjectMemory_countBitsOf_put(ObjectPointer objectPointer, Word value);

/**
 * Return the object's odd-length flag.
 */
Word ObjectMemory_oddBitOf(ObjectPointer objectPointer);

/**
 * Set the odd-length flag of the object.
 */
void ObjectMemory_oddBitOf_put(ObjectPointer objectPointer, Word value);

/**
 * Return the object's is-a-pointer flag.
 */
Word ObjectMemory_pointerBitOf(ObjectPointer objectPointer);

/**
 * Set the is-a-pointer flag of the object.
 */
void ObjectMemory_pointerBitOf_put(ObjectPointer objectPointer, Word value);

/**
 * Return the object's free flag.
 */
Word ObjectMemory_freeBitOf(ObjectPointer objectPointer);

/**
 * Set the free flag of the object.
 */
void ObjectMemory_freeBitOf_put(ObjectPointer objectPointer, Word value);

/**
 * Return the object's memory segment.
 */
Word ObjectMemory_segmentBitsOf(ObjectPointer objectPointer);

/**
 * Set the memory segment of the object.
 */
void ObjectMemory_segmentBitsOf_put(ObjectPointer objectPointer, Word value);

/**
 * The location of an object.
 */
Word ObjectMemory_locationBitsOf(ObjectPointer objectPointer);

/**
 * Set the location of an object.
 */
void ObjectMemory_locationBitsOf_put(ObjectPointer objectPointer, Word value);

/**
 * Get the word in the object's heap storage at the given offset.
 * Offset is counted in words (2 bytes).
 */
Word ObjectMemory_heapChunkOf_word(ObjectPointer objectPointer, Word offset);

/**
 * Store the word in the object's heap storage at the given offset.
 * Offset is counted in words (2 bytes).
 */
void ObjectMemory_heapChunkOf_word_put(ObjectPointer objectPointer, Word offset, Word value);

/**
 * Get a byte in the object's heap storage at the given offset.
 * Offset is counted in bytes.
 */
Byte ObjectMemory_heapChunkOf_byte(ObjectPointer objectPointer, Word offset);

/**
 * Store a byte in the object's heap storage at the given offset.
 * Offset is counted in bytes.
 */
void ObjectMemory_heapChunkOf_byte_put(ObjectPointer objectPointer, Word offset, Byte value);

/**
 * Retrieve this object's size on the heap.
 */
Word ObjectMemory_sizeBitsOf(ObjectPointer objectPointer);

/**
 * Store this object's size on the heap.
 */
void ObjectMemory_sizeBitsOf_put(ObjectPointer objectPointer, Word value);

/**
 * Retrieve this object's class.
 */
Word ObjectMemory_classBitsOf(ObjectPointer objectPointer);

/**
 * Store this object's class.
 */
void ObjectMemory_classBitsOf_put(ObjectPointer objectPointer, Word value);

/**
 * The last pointer of an object.
 * @note this implementation is preliminary and untested.
 */
Word ObjectMemory_lastPointerOf(ObjectPointer objectPointer);

/**
 * The space occupied by an object.
 * @note this implementation is preliminary and untested.
 */
Word ObjectMemory_spaceOccupiedBy(ObjectPointer objectPointer);

#endif
