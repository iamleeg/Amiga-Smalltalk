#ifndef AMIGA_SMALLTALK_OBJECTMEMORY_H
#define AMIGA_SMALLTALK_OBJECTMEMORY_H

#include "Types.h"

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

#endif
