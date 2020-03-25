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

/* Include routines from other categories */
#include "ObjectMemory_Storage.h"
#include "ObjectMemory_FreeList.h"
#include "ObjectMemory_Allocation.h"
#include "ObjectMemory_RefCounting.h"
#include "ObjectMemory_MarkingCollector.h"

#endif
