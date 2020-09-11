#include <exec/types.h>
#include "types.h"
Bool ObjectMemory_saveSnapshot(CONST_STRPTR filename);
Bool ObjectMemory_loadSnapshot(CONST_STRPTR filename);
ObjectPointer ObjectMemory_lastUsedObjectPointer();
