#ifndef AMIGA_SMALLTALK_OBJECTMEMORY_FREELIST_H
#define AMIGA_SMALLTALK_OBJECTMEMORY_FREELIST_H

#include "Types.h"

/**
 * The location in the object table of the head of the free pointer list.
 */
#define FreePointerList 0x8000

/**
 * The location in each segment of the first free chunk list.
 */
#define FirstFreeChunkList 0xf000
/**
 * The location in each segment of the last free chunk list.
 */
#define LastFreeChunkList 0xfffe

/**
 * A pointer value that cannot be an object pointer.
 */
#define NonPointer 0xffff

/**
 * Return the head of the free pointer list.
 */
ObjectPointer ObjectMemory_headOfFreePointerList(void);

/**
 * Set the head of the free pointer list.
 */
void ObjectMemory_headOfFreePointerList_put(ObjectPointer objectPointer);

/**
 * Add another object pointer to the free pointer list.
 */
void ObjectMemory_toFreePointerListAdd(ObjectPointer objectPointer);

/**
 * Remove the most recent entry from the free pointer list and return it.
 * If the list is empty, then return nil.
 */
ObjectPointer ObjectMemory_removeFromFreePointerList();

/**
 * Return the head of the free chunk list of given size in the specified segment.
 */
ObjectPointer ObjectMemory_headOfFreeChunkList_inSegment(Word size, Word segment);

/**
 * Set the head of the free chunk list of given size in the specified segment.
 */
void ObjectMemory_headOfFreeChunkList_inSegment_put(Word size, Word segment, ObjectPointer objectPointer);

#endif
