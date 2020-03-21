#ifndef AMIGA_SMALLTALK_OBJECTMEMORY_ALLOCATION_H
#define AMIGA_SMALLTALK_OBJECTMEMORY_ALLOCATION_H

#include "Types.h"

/**
 * A preliminary algorithm for allocating an object of a given size and class.
 * As this will get replaced, this algorithm (from p679) is not tested.
 * 
 * @param size The space needed for the object, in words.
 * @param extraWord Whether to reserve extra space that can be used by the GC.
 * @param classPointer The new object's class.
 */
ObjectPointer ObjectMemory_allocate_extra_class(Word size, Bool extraWord, ObjectPointer classPointer);

/**
 * A preliminary algorithm to allocate the memory needed to store an object of given size.
 * As this will get replaced, this algorithm (from p668) is not tested.
 */
ObjectPointer ObjectMemory_allocateChunk(Word size);

/**
 *  Find, allocate and return a free chunk of a given size on the heap.
 */
ObjectPointer ObjectMemory_attemptToAllocateChunk(Word size);

/**
 * Find a sufficiently-large chunk of free memory in the current segment.
 * If the available chunk is too big, return a pointer to a right-sized chunk and return the rest to the free list.
 * If there is no sufficiently large chunk in this segment, return NilPointer.
 */
ObjectPointer ObjectMemory_attemptToAllocateChunkInCurrentSegment(Word size);

/**
 * Remove a pointer from the free pointer list and initialise it to represent an allocated object.
 */
ObjectPointer ObjectMemory_obtainPointer_location(Word size, ObjectPointer location);

/**
 * 
 */
void ObjectMemory_compactCurrentSegment(void);

/**
 * Preliminary version of the deallocation routine from p670.
 */
void ObjectMemory_deallocate(ObjectPointer objectPointer);

/**
 * Find the first free chunk in a segment, and recycle deallocated object table entries.
 * Return the "low water mark", the lowest free chunk in the segment.
 */
ObjectPointer ObjectMemory_abandonFreeChunksInSegment(Word segment);

/**
 * Put an object back on the free-pointer list.
 */
void ObjectMemory_releasePointer(ObjectPointer objectPointer);

/**
 * Prepare to compact the heap memory by reversing the association between the object table
 * and the heap. After this operation, heap entries will point to their object table
 * references rather than the other way around. Then, once the compaction process has moved
 * all of the objects, the object table can be fixed up.
 */
void ObjectMemory_reverseHeapPointersAbove(ObjectPointer lowWaterMark);

/**
 * Sweep objects in the current segment, putting in-use objects as low into the memory
 * as possible so that the segment contains a single, contiguous, block of unallocated space.
 * Return a pointer to the unused space.
 */
ObjectPointer ObjectMemory_sweepCurrentSegmentFrom(ObjectPointer lowWaterMark);

/**
 * Move all objects above the current segment's low water mark into a single contiguous
 * region, then mark all the remaining memory as free. If there are no free chunks in
 * the current segment, then this routine does nothing.
 */
void ObjectMemory_compactCurrentSegment(void);

#endif
