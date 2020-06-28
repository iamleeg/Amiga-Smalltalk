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
 * The number of words in an object header.
 */
#define HeaderSize 2
/**
 * A size that is too big to appear in one of the small free chunk lists.
 */
#define BigSize 0x2e
/**
 * A run of object pointers that can be used to hold the free memory when the VM starts.
 */
#define FirstFreeObject 0x7F00

/**
 * The location in the object table of the head of the free pointer list.
 */
#define FreePointerList 0xffce

/**
 * The location in each segment of the first free chunk list.
 */
#define FirstFreeChunkList 0xffd0
/**
 * The location in each segment of the last free chunk list.
 * @note The last free chunk list contains big chunks. All other free chunk lists
 *       contain chunks of a small size, given by
 *       size = (location of list head) - (FirstFreeChunkList).
 */
#define LastFreeChunkList 0xfffe

/**
 * A pointer value that cannot be an object pointer.
 */
#define NonPointer 0xffff


/**
 * @note the ObjectMemory also needs to know constants published by the Interpreter.
 */
#include "Interpreter_Constants.h"

#endif
