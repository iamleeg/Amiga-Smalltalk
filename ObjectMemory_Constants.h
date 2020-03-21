#ifndef AMIGA_SMALLTALK_OBJECTMEMORY_CONSTANTS_H
#define AMIGA_SMALLTALK_OBJECTMEMORY_CONSTANTS_H

/**
 * The segment in RealWordMemory that contains the object table.
 */
#define ObjectTableSegment 0
/**
 * The first word of the object table in ObjectTableSegment.
 */
#define ObjectTableStart 0
/**
 * The length of the object table.
 */
#define ObjectTableSize 0x7FFF
/**
 * A size that's too big to fit in a single byte.
 */
#define HugeSize 0x0100

/**
 * The entry in the object table for the nil object.
 */
#define NilPointer 2
/**
 * The number of words in an object header.
 */
#define HeaderSize 2
/**
 * A size that is too big to appear in one of the small free chunk lists.
 */
#define BigSize 0x2e

#endif
