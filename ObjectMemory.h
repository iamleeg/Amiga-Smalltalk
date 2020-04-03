#ifndef AMIGA_SMALLTALK_OBJECTMEMORY_H
#define AMIGA_SMALLTALK_OBJECTMEMORY_H

#include "Types.h"

#include "ObjectMemory_Constants.h"

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

/* Include routines from other categories */
#include "ObjectMemory_Storage.h"
#include "ObjectMemory_FreeList.h"
#include "ObjectMemory_Allocation.h"
#include "ObjectMemory_RefCounting.h"
#include "ObjectMemory_MarkingCollector.h"

#endif
