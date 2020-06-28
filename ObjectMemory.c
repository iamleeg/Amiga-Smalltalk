#include "ObjectMemory.h"
#include "RealWordMemory.h"

Bool ObjectMemory_new(void) {
  Word thisSegment, chunkList, freeChunkSize, freeChunkStart;
  ObjectPointer objectPointer, freeObjectPointer;
  Bool hasMemory = RealWordMemory_new();
  if (!hasMemory) {
    return NO;
  }
  for (thisSegment = 0; thisSegment < HeapSegmentCount; thisSegment++) {
    /* Make sure chunk lists are empty */
    for (chunkList = HeaderSize; chunkList <= BigSize; chunkList++) {
      ObjectMemory_resetFreeChunkList_inSegment(chunkList, thisSegment);
    }
    /* create one object with all of the free memory in this segment */
    freeChunkSize = (thisSegment > 0) ? HeapSpaceStop - 1 : HeapSpaceStop - ObjectTableSize - 1;
    freeChunkStart = (thisSegment > 0) ? 0 : ObjectTableSize + 2;
    freeObjectPointer = FirstFreeObject + (2 * thisSegment);
    ObjectMemory_sizeBitsOf_put(freeObjectPointer, freeChunkSize);
    ObjectMemory_segmentBitsOf_put(freeObjectPointer, thisSegment);
    ObjectMemory_locationBitsOf_put(freeObjectPointer, freeChunkStart);
    ObjectMemory_toFreeChunkList_add(BigSize, freeObjectPointer);
  }
  /* All predefined objects get an initial reference count */
  for (objectPointer = 0; objectPointer < FirstUnusedObjectPointer; objectPointer += 2) {
    ObjectMemory_countBitsOf_put(objectPointer, 1);
  }
  /* All remaining object pointers are on the free pointer list */
  ObjectMemory_headOfFreePointerList_put(NonPointer);
  for (objectPointer = FirstUnusedObjectPointer; objectPointer < FirstFreeObject; objectPointer += 2) {
    ObjectMemory_freeBitOf_put(objectPointer, YES);
    ObjectMemory_toFreePointerListAdd(objectPointer);
  }
  /* Make sure we start in the first segment, in case something has already been happening */
  currentSegment = 0;
  return YES;
}

void ObjectMemory_delete(void) {
  RealWordMemory_delete();
}

ObjectPointer ObjectMemory_fetchPointer_ofObject(Word fieldIndex, ObjectPointer objectPointer) {
  return ObjectMemory_heapChunkOf_word(objectPointer, HeaderSize + fieldIndex);
}

void ObjectMemory_storePointer_ofObject_withValue(Word fieldIndex, ObjectPointer objectPointer, ObjectPointer valuePointer) {
  Word chunkIndex = HeaderSize + fieldIndex;
  ObjectMemory_countUp(valuePointer);
  ObjectMemory_countDown(ObjectMemory_heapChunkOf_word(objectPointer, chunkIndex));
  ObjectMemory_heapChunkOf_word_put(objectPointer, chunkIndex, valuePointer);
}

Word ObjectMemory_fetchWord_ofObject(Word wordIndex, ObjectPointer objectPointer) {
  return ObjectMemory_heapChunkOf_word(objectPointer, HeaderSize + wordIndex);
}

void ObjectMemory_storeWord_ofObject_withValue(Word wordIndex, ObjectPointer objectPointer, Word valueWord) {
  ObjectMemory_heapChunkOf_word_put(objectPointer, HeaderSize + wordIndex, valueWord);
}

Byte ObjectMemory_fetchByte_ofObject(Word byteIndex, ObjectPointer objectPointer) {
  return ObjectMemory_heapChunkOf_byte(objectPointer, (HeaderSize * 2) + byteIndex);
}

void ObjectMemory_storeByte_ofObject_withValue(Word byteIndex, ObjectPointer objectPointer, Byte valueByte) {
  ObjectMemory_heapChunkOf_byte_put(objectPointer, (HeaderSize * 2 + byteIndex), valueByte);
}

void ObjectMemory_increaseReferencesTo(ObjectPointer objectPointer) {
  ObjectMemory_countUp(objectPointer);
}

void ObjectMemory_decreaseReferencesTo(ObjectPointer objectPointer) {
  ObjectMemory_countDown(objectPointer);
}

ObjectPointer ObjectMemory_fetchClassOf(ObjectPointer objectPointer) {
  if (ObjectMemory_isIntegerObject(objectPointer)) {
    return IntegerClass;
  } else {
    return ObjectMemory_classBitsOf(objectPointer);
  }
}

Word ObjectMemory_fetchWordLengthOf(ObjectPointer objectPointer) {
  return ObjectMemory_sizeBitsOf(objectPointer) - HeaderSize;
}

Word ObjectMemory_fetchByteLengthOf(ObjectPointer objectPointer) {
  return (ObjectMemory_fetchWordLengthOf(objectPointer) * 2 - ObjectMemory_oddBitOf(objectPointer));
}

ObjectPointer ObjectMemory_instantiateClass_withPointers(ObjectPointer classPointer, Word length) {
  Word size;
  Bool extra;
  size = HeaderSize + length;
  extra = size >= HugeSize;
  return ObjectMemory_allocate_odd_pointer_extra_class(size, 0, 1, extra, classPointer);
}

ObjectPointer ObjectMemory_instantiateClass_withWords(ObjectPointer classPointer, Word length) {
  Word size = HeaderSize + length;
  return ObjectMemory_allocate_odd_pointer_extra_class(size, 0, 0, 0, classPointer);
}

ObjectPointer ObjectMemory_instantiateClass_withBytes(ObjectPointer classPointer, Word length) {
  Word size = HeaderSize + ((length + 1) / 2);
  return ObjectMemory_allocate_odd_pointer_extra_class(size, length % 2, 0, 0, classPointer);
}

ObjectPointer ObjectMemory_initialInstanceOf(ObjectPointer classPointer) {
  ObjectPointer pointer;
  for(pointer = 0; pointer < ObjectTableSize; pointer += 2) {
    if (ObjectMemory_freeBitOf(pointer) == NO) {
      if (ObjectMemory_fetchClassOf(pointer) == classPointer) {
        return pointer;
      }
    }
  }
  return NilPointer;
}

ObjectPointer ObjectMemory_instanceAfter(ObjectPointer objectPointer) {
  ObjectPointer classPointer = ObjectMemory_fetchClassOf(objectPointer), pointer;
  for(pointer = objectPointer + 2; pointer < ObjectTableSize; pointer += 2) {
    if (ObjectMemory_freeBitOf(pointer) == NO) {
      if (ObjectMemory_fetchClassOf(pointer) == classPointer) {
        return pointer;
      }
    }
  }
  return NilPointer;
}

void ObjectMemory_swapPointersOf_and(ObjectPointer firstPointer, ObjectPointer secondPointer) {
  Word firstSegment;
  ObjectPointer firstLocation;
  Bool firstPointerBit, firstOddBit; /* firstPointerBit is called firstPointer on p688 */

  firstSegment = ObjectMemory_segmentBitsOf(firstPointer);
  firstLocation = ObjectMemory_locationBitsOf(firstPointer);
  firstPointerBit = ObjectMemory_pointerBitOf(firstPointer);
  firstOddBit = ObjectMemory_oddBitOf(firstPointer);

  ObjectMemory_segmentBitsOf_put(firstPointer, ObjectMemory_segmentBitsOf(secondPointer));
  ObjectMemory_locationBitsOf_put(firstPointer, ObjectMemory_locationBitsOf(secondPointer));
  ObjectMemory_pointerBitOf_put(firstPointer, ObjectMemory_pointerBitOf(secondPointer));
  ObjectMemory_oddBitOf_put(firstPointer, ObjectMemory_oddBitOf(secondPointer));

  ObjectMemory_segmentBitsOf_put(secondPointer, firstSegment);
  ObjectMemory_locationBitsOf_put(secondPointer, firstLocation);
  ObjectMemory_pointerBitOf_put(secondPointer, firstPointerBit);
  ObjectMemory_oddBitOf_put(secondPointer, firstOddBit);
}

short ObjectMemory_integerValueOf(ObjectPointer objectPointer) {
  return objectPointer/2;
}

ObjectPointer ObjectMemory_integerObjectOf(short value) {
  return (value << 1) + 1;
}

Bool ObjectMemory_isIntegerValue(short valueWord) {
  return ((valueWord < 16384) && (valueWord >= -16384)) ? YES : NO;
}
