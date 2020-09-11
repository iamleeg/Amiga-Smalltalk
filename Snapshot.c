#include "Snapshot.h"
#include "types.h"
#include "ObjectMemory.h"
#include <exec/types.h>
#include <proto/dos.h>
#include <dos/dos.h>

/*
 * -- SAVE -------------------------------------------------------------------------------
 */

/*
 * start at the current position and pad zeros up to a 512 word boundary
 */
Bool _padToPage(BPTR filehandle) {
	LONG pos = Seek(filehandle, 0, OFFSET_CURRENT);
	LONG desired = ((pos + 512 -1 ) / 512) * 512;
	LONG zero = 0L;
	LONG pads = (desired - pos) / sizeof(zero);
	while( pads-- > 0 ) {
		Write(filehandle, (APTR)&zero, sizeof(zero));
	}
	return YES;
}


ObjectPointer ObjectMemory_lastUsedObjectPointer() {
	ObjectPointer lastUsedObjectPointer = NilPointer;
	ObjectPointer iteratingPointer = NilPointer;
	
	for( iteratingPointer = 2; iteratingPointer < ObjectTableSize; iteratingPointer += 2) {
		if( ObjectMemory_hasObject(iteratingPointer) ) {
			lastUsedObjectPointer = iteratingPointer;
		}
	}
	return lastUsedObjectPointer;
}

Bool _saveSnapshotHeader(BPTR filehandle) {
	LONG placeholder[2] = {0};
	BYTE  interchange[2] = {0};
	
	/* We will fill these placeholder values in at the end once we calculate them */
	if( Write(filehandle, &placeholder, sizeof(placeholder)) != sizeof(placeholder) ) {
		return NO;
	}
	/* we leave this empty */
	
	if( Write(filehandle, &interchange, sizeof(interchange)) != sizeof(interchange) ) {
		return NO;
	}	
	return YES;
}

LONG _writeObjects(BPTR filehandle, WORD storedObjectTableLength) {
	LONG objectSpaceLength = 0;
	WORD objectSize = 0;
	ObjectPointer anObject = NilPointer;
	WORD header[2];
	WORD wordLengthOfObject = 0;
	WORD wordIndex = 0;
	Word word = 0;

	
	for( anObject = 2; anObject < storedObjectTableLength; anObject += 2) {
		if(!ObjectMemory_hasObject(anObject)) continue;
		
		objectSize = (WORD)ObjectMemory_sizeBitsOf(anObject);
		header[0] = objectSize;
		header[1] = (WORD)ObjectMemory_fetchClassOf(anObject);
		if( Write(filehandle, (APTR)&header, sizeof(header)) != sizeof(header) ) {
			return 0;
		}
		wordLengthOfObject = ObjectMemory_fetchWordLengthOf(anObject);
		for(wordIndex = 0; wordIndex < wordLengthOfObject; wordIndex++) {
			word = ObjectMemory_fetchWord_ofObject( wordIndex, anObject);
			if( Write(filehandle, (APTR)&word, sizeof(word)) != sizeof(word) ) {
				return 0;
			}
		}
		objectSpaceLength += objectSize;
	}
	
	return objectSpaceLength;
}

Bool _writeObjectTable(BPTR filehandle, WORD storedObjectTableLength) {
    int objectImageWordAddress = 0;
    ObjectPointer objectPointer = NilPointer;
    WORD oldOTValue = 0;
    WORD oldOTLocation = 0;
    WORD objectSize = 0;
    WORD words[2] = {0};
    for( objectPointer = 0; objectPointer < storedObjectTableLength; objectPointer += 2)
    {
        oldOTValue = ObjectMemory_ot(objectPointer);
        oldOTLocation = ObjectMemory_locationBitsOf(objectPointer);
        
        if (objectPointer >= 2)
        {
            if (!ObjectMemory_freeBitOf(objectPointer) && ObjectMemory_countBitsOf(objectPointer) == 0)
            {
               /*// This entry was for a free chunk of memory, but we don't save
                // free space in the image. Store as a
                // an available OT entry by setting free bit*/
                ObjectMemory_freeBitOf_put(objectPointer, 1);
            }
            
            if (ObjectMemory_freeBitOf(objectPointer))
            {
                /*// manual.pdf - page 3: free entries have freeBit set and other bits in both
                  // words are 0.*/
                ObjectMemory_ot_put(objectPointer, 0);
                ObjectMemory_freeBitOf_put(objectPointer, 1);
                ObjectMemory_locationBitsOf_put(objectPointer, 0);
            }
            else
            {
                objectSize = (WORD) ObjectMemory_sizeBitsOf(objectPointer);
            
                /*// Modify the location of the object table entry... we do this once we no longer
                  // process this object table entry*/
                ObjectMemory_segmentBitsOf_put(objectPointer, objectImageWordAddress >> 16);
                ObjectMemory_locationBitsOf_put(objectPointer, objectImageWordAddress & 0xffff);
                objectImageWordAddress += objectSize;
            }
        }
        
        /* Assemble object table entry */

        words[0] = ObjectMemory_ot(objectPointer);
        words[1] = ObjectMemory_locationBitsOf(objectPointer);
   
        /* Restore OT entry */
        ObjectMemory_ot_put(objectPointer, oldOTValue);
        ObjectMemory_locationBitsOf_put(objectPointer, oldOTLocation);
        
        /* Write this entry */
        if (Write(filehandle, (APTR) &words, sizeof(words)) != sizeof(words))
            return NO;
    }
    return YES;
}

Bool _saveObjects(BPTR filehandle) {
	LONG objectSpaceLength = 0;
	LONG storedObjectTableLength = ObjectMemory_lastUsedObjectPointer() + 2;

	if(!_saveSnapshotHeader(filehandle)) {
		return NO;
	}
	
	if(!_padToPage(filehandle)) {
		return NO;
	}
	objectSpaceLength = _writeObjects(filehandle, storedObjectTableLength);
	if(objectSpaceLength == 0) {
		return NO;
	}	

	if(!_padToPage(filehandle)) {
		return NO;
	}
	
	if(!_writeObjectTable(filehandle, storedObjectTableLength)) {
		return NO;
	}
    
    /* Now we can go back fill in the values for the image header. */
    Seek(filehandle, 0, OFFSET_BEGINNING);
    Write( filehandle, (APTR)&objectSpaceLength, sizeof(objectSpaceLength) );
    Write( filehandle, (APTR)&storedObjectTableLength, sizeof(storedObjectTableLength) );
	
	return YES;
}

Bool ObjectMemory_saveSnapshot(CONST_STRPTR filename) {
	BPTR snapshotFile = Open(filename, MODE_READWRITE);
	Bool result = NO;
	if( snapshotFile != NULL ) {;
		result = _saveObjects(snapshotFile);
		Close( snapshotFile );
	}
	return result;
}




/*
 * -- LOAD -------------------------------------------------------------------------------
 */
Bool ObjectMemory_loadObjectTable(BPTR filehandle) {
	return NO;
}

Bool ObjectMemory_loadObjects(BPTR filehandle) {
	return NO;
}

Bool ObjectMemory_loadSnapshot(CONST_STRPTR filename) {
	BPTR snapshotfile = Open(filename, MODE_OLDFILE);
	Bool result = NO;
	if( snapshotfile != NULL ) {
		result = ObjectMemory_loadObjectTable(snapshotfile) && ObjectMemory_loadObjects(snapshotfile);
		Close(snapshotfile);
	}
	return result;
}


