#ifndef AMIGA_SMALLTALK_OBJECTMEMORY_H
#define AMIGA_SMALLTALK_OBJECTMEMORY_H

#include "Types.h"

#include "ObjectMemory_Constants.h"

/**
 * Set the ObjectMemory up into a usable state:
 * - a clean heap with at least one 128kB segment
 * - object table in the first segment
 * - empty "small chunk" free lists in all segments
 * - all free memory in one "large chunk" object in each segment
 * - all unused object pointers are on the free pointer list
 * @return YES iff at least one heap segment is allocated and the memory is initialised.
 */
Bool ObjectMemory_new(void);

/**
 * Tear down ObjectMemory.
 */
void ObjectMemory_delete(void);

/* "Public" methods used by the interpreter. */

/**
 * Get the object in this object's fields at the given index.
 */
ObjectPointer ObjectMemory_fetchPointer_ofObject(Word fieldIndex, ObjectPointer objectPointer);

/**
 * Set the object in this object's fields at the given index.
 * This method adjusts the reference counts for each object.
 */
void ObjectMemory_storePointer_ofObject_withValue(Word fieldIndex, ObjectPointer objectPointer, ObjectPointer valuePointer);

/**
 * Get the word in this object's fields at the given index.
 */
Word ObjectMemory_fetchWord_ofObject(Word wordIndex, ObjectPointer objectPointer);

/**
 * Set the word in this object's fields at the given index.
 * Unlike ObjectMemory_storePointer_ofObject_withValue(), this method
 * doesn't adjust reference counts as the value is taken to be a bare value,
 * not an object pointer.
 */
void ObjectMemory_storeWord_ofObject_withValue(Word wordIndex, ObjectPointer objectPointer, Word valueWord);

/**
 * Get the byte in this object's fields at the given index.
 */
Byte ObjectMemory_fetchByte_ofObject(Word byteIndex, ObjectPointer objectPointer);

/**
 * Store a byte in this object's fields at the given index.
 */
void ObjectMemory_storeByte_ofObject_withValue(Word byteIndex, ObjectPointer objectPointer, Byte valueByte);

/**
 * Add a reference to this object.
 */
void ObjectMemory_increaseReferencesTo(ObjectPointer objectPointer);

/**
 * Remove a reference to this object.
 */
void ObjectMemory_decreaseReferencesTo(ObjectPointer objectPointer);

/**
 * Find this object's class.
 */
ObjectPointer ObjectMemory_fetchClassOf(ObjectPointer objectPointer);

/**
 * Get the length of this object's field storage in words.
 */
Word ObjectMemory_fetchWordLengthOf(ObjectPointer objectPointer);

/**
 * Get the length of this object's field storage in bytes.
 */
Word ObjectMemory_fetchByteLengthOf(ObjectPointer objectPointer);

/**
 * Allocate and return an instance of the specified class, with space for pointer storage.
 */
ObjectPointer ObjectMemory_instantiateClass_withPointers(ObjectPointer classPointer, Word length);

/**
 * Allocate and return an instance of the specified class, with space for word storage.
 */
ObjectPointer ObjectMemory_instantiateClass_withWords(ObjectPointer classPointer, Word length);

/**
 * Allocate and return an instance of the specified class, with space for byte storage.
 */
ObjectPointer ObjectMemory_instantiateClass_withBytes(ObjectPointer classPointer, Word length);

/**
 * Find the first live instance of a class in the object table.
 */
ObjectPointer ObjectMemory_initialInstanceOf(ObjectPointer classPointer);

/**
 * Find the next object in the object table that is an instance of the same class as this object.
 * @note the class test is strict equality, so objects that are instances of subclasses will be skipped.
 */
ObjectPointer ObjectMemory_instanceAfter(ObjectPointer objectPointer);

/**
 * Swap two references in the object table. After this operation, references to firstPointer will find fields
 * that were previously in secondPointer and vice versa.
 * @note this seems dangerous, in that the sizes of the objects and the classes are not patched up. It seems
 *       that you should only use this for two instances of the same class, with the same number of indexed variables.
 */
void ObjectMemory_swapPointersOf_and(ObjectPointer firstPointer, ObjectPointer secondPointer);

/**
 * Return the integer value of an immediate integer.
 * @note This method doesn't actually _check_ that you passed an immediate integer, but returns
 *       a nonsensical result if you don't.
 */
short ObjectMemory_integerValueOf(ObjectPointer objectPointer);

/**
 * Return the Integer object for a given integer value.
 */
ObjectPointer ObjectMemory_integerObjectOf(short value);

/**
 * Return whether the integer value supplied can be represented as an immediate integer.
 * @note I have implemented this routine based on the note on p573, and knowledge of how
 *       immediate integers are represented, not the typo-laden example on p688.
 */
Bool ObjectMemory_isIntegerValue(short valueWord);

/** 
 * Added by alan as convenience methods.  Not in the blue book directly
 */
ObjectPointer ObjectMemory_floatObjectOf(float floatValue);
float ObjectMemory_floatValueOf(ObjectPointer floatPointer);


/** 
 * Is objectPointer pointing to a valid object ?
 */
 Bool ObjectMemory_hasObject(ObjectPointer objectPointer);

/* Include routines from other categories */
#include "ObjectMemory_Storage.h"
#include "ObjectMemory_FreeList.h"
#include "ObjectMemory_Allocation.h"
#include "ObjectMemory_RefCounting.h"
#include "ObjectMemory_GC.h"

#endif
