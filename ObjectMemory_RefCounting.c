#include "ObjectMemory_RefCounting.h"
#include "ObjectMemory_Storage.h"

ObjectPointer ObjectMemory_countUp(ObjectPointer objectPointer) {
  Word count;
  if (!ObjectMemory_isIntegerObject(objectPointer)) {
    count = ObjectMemory_countBitsOf(objectPointer) + 1;
    if (count < 129) {
      ObjectMemory_countBitsOf_put(objectPointer, count);
    }
  }
  return objectPointer;
}
