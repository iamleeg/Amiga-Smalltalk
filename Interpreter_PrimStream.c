#include "Interpreter.h"
#include "Interpreter_PrimStream.h"
#include "ObjectMemory.h"

void Interpreter_dispatchSubscriptAndStreamPrimitives( int primitiveIndex ) {
	switch( primitiveIndex ) {
		case 60: Interpreter_primitiveAt();
		case 61: Interpreter_primitiveAtPut();
		case 62: Interpreter_primitiveSize();
		case 63: Interpreter_primitiveStringAt();
		case 64: Interpreter_primitiveStringAtPut();
		case 65: Interpreter_primitiveNext();
		case 66: Interpreter_primitiveNextPut();
		case 67: Interpreter_primitiveAtEnd();
	}
}

Word Interpreter_lengthOf(ObjectPointer array) {
	if( Interpreter_isWords(ObjectMemory_fetchClassOf(array)) ) {
		return ObjectMemory_fetchWordLengthOf(array);
	} else {
		return ObjectMemory_fetchByteLengthOf(array);
	}
}

void Interpreter_checkIndexableBoundsOf_in(Word index, ObjectPointer array) {
    Interpreter_success_(index >= 1);
    Interpreter_success_(index <= (Word)Interpreter_lengthOf(array));
}

ObjectPointer Interpreter_subscript_with(ObjectPointer array, Word index){
	ObjectPointer class = ObjectMemory_fetchClassOf(array);
	Word value = 0;
    if (Interpreter_isWords(class)) {
        if (Interpreter_isPointers(class)) {
            return ObjectMemory_fetchPointer_ofObject(index - 1, array);
        } else {
            value = ObjectMemory_fetchWord_ofObject(index - 1, array);
            return Interpreter_positive16BitIntegerFor(value);
        }
    }
    
    value = ObjectMemory_fetchByte_ofObject(index - 1, array);
    return ObjectMemory_integerObjectOf(value);
}

void Interpreter_subscript_with_storing(ObjectPointer array, Word index, ObjectPointer value) {
    ObjectPointer class = ObjectMemory_fetchClassOf(array);
    if (Interpreter_isWords(class)) {
        if (Interpreter_isPointers(class)) {
            ObjectMemory_storePointer_ofObject_withValue(index - 1, array, value);
        } else {
            Interpreter_success_(ObjectMemory_isIntegerObject(value) || ObjectMemory_fetchClassOf(value) == ClassLargePositiveIntegerPointer);
            if (Interpreter_success()) {
                ObjectMemory_storeWord_ofObject_withValue(index - 1, array, Interpreter_positive16BitValueOf(value));
            }
        }
    } else {
        Interpreter_success_(ObjectMemory_isIntegerObject(value));
        if(Interpreter_success())   {
            ObjectMemory_storeByte_ofObject_withValue(index - 1, array, Interpreter_lowByteOf(ObjectMemory_integerValueOf(value)));
        }
    }
}

void Interpreter_primitiveAt(void) {
	Word index = Interpreter_positive16BitValueOf(Interpreter_popStack());
    ObjectPointer array = Interpreter_popStack();
    ObjectPointer arrayClass = ObjectMemory_fetchClassOf(array);
    ObjectPointer result = NilPointer;
    Interpreter_checkIndexableBoundsOf_in(index, array);
    
    if(Interpreter_success()) {
        index = index + Interpreter_fixedFieldsOf(arrayClass);
        result = Interpreter_subscript_with(array, index);
    }
    
    if (Interpreter_success()) {
        Interpreter_push(result);
    } else {
        Interpreter_unPop(2);
    }
}

void Interpreter_primitiveAtPut(void) {
	ObjectPointer value = Interpreter_popStack();
	Word index = Interpreter_positive16BitValueOf(Interpreter_popStack());
    ObjectPointer array = Interpreter_popStack();
    ObjectPointer arrayClass = ObjectMemory_fetchClassOf(array);
    Interpreter_checkIndexableBoundsOf_in(index, array);
    if(Interpreter_success()) {
        index = index + Interpreter_fixedFieldsOf(arrayClass);
        Interpreter_subscript_with_storing(array, index, value);
    }
    
    if (Interpreter_success()) {
        Interpreter_push(value);
    } else {
        Interpreter_unPop(3);
    }
}

void Interpreter_primitiveSize(void){
	ObjectPointer array = Interpreter_popStack();
	ObjectPointer class = NilPointer;
	ObjectPointer length = NilPointer;
	
    Interpreter_success_(!ObjectMemory_isIntegerObject(array));
    if (Interpreter_success()) {
        class = ObjectMemory_fetchClassOf(array);
        length = Interpreter_positive16BitIntegerFor(Interpreter_lengthOf(array) - Interpreter_fixedFieldsOf(class));
    }
    if (Interpreter_success()) {
        Interpreter_push(length);
    } else {
        Interpreter_unPop(1);
    }
}

void Interpreter_primitiveStringAt(void){
	ObjectPointer character = NilPointer;

    Word index = Interpreter_positive16BitValueOf(Interpreter_popStack());
    ObjectPointer array = Interpreter_popStack();
    Interpreter_checkIndexableBoundsOf_in(index, array);
    if (Interpreter_success()) {
        Word ascii  = ObjectMemory_integerValueOf(Interpreter_subscript_with(array, index));
        character = ObjectMemory_fetchPointer_ofObject(ascii, CharacterTablePointer);
    }
    
    if (Interpreter_success()) {
        Interpreter_push(character);
    } else {
        Interpreter_unPop(2);
    }
}

void Interpreter_primitiveStringAtPut(void){
	ObjectPointer character = Interpreter_popStack();
    Word index = Interpreter_positive16BitValueOf(Interpreter_popStack());
    ObjectPointer array = Interpreter_popStack();
    Interpreter_checkIndexableBoundsOf_in(index, array);
    Interpreter_success_(ObjectMemory_fetchClassOf(character) == ClassCharacterPointer);
    if (Interpreter_success()) {
        Word ascii  = ObjectMemory_fetchPointer_ofObject(characterValueIndex, character);
        Interpreter_subscript_with_storing(array, index, ascii);
    }
    if (Interpreter_success()) {
        Interpreter_push(character);
    } else {
        Interpreter_unPop(3);
    }
}

void Interpreter_primitiveNext(void){
    ObjectPointer result = NilPointer;

    ObjectPointer stream = Interpreter_popStack();
    ObjectPointer array = ObjectMemory_fetchPointer_ofObject(streamArrayIndex, stream);
    ObjectPointer arrayClass = ObjectMemory_fetchClassOf(array);
    Word index = Interpreter_fetchInteger_ofObject(streamIndexIndex, stream);
    Word limit = Interpreter_fetchInteger_ofObject(streamReadLimitIndex, stream);
    Interpreter_success_(index < limit);
    Interpreter_success_(arrayClass == ClassArrayPointer || arrayClass == ClassStringPointer);
    Interpreter_checkIndexableBoundsOf_in(index + 1, array);
    if (Interpreter_success()) {
        index = index + 1;
        result = Interpreter_subscript_with(array, index);
    }
    if (Interpreter_success()){
        Interpreter_storeInteger_ofObject_withValue(streamIndexIndex, stream, index);
    }
    if (Interpreter_success()){
        if (arrayClass == ClassArrayPointer){
            Interpreter_push(result);
        } else {
            Word ascii = ObjectMemory_integerValueOf(result);
            Interpreter_push(ObjectMemory_fetchPointer_ofObject(ascii, CharacterTablePointer));
        }
    }
    else {
        Interpreter_unPop(1);
    }
}

void Interpreter_primitiveNextPut(void){
    ObjectPointer value = Interpreter_popStack();
    ObjectPointer stream = Interpreter_popStack();
    ObjectPointer array = ObjectMemory_fetchPointer_ofObject(streamArrayIndex, stream);
    ObjectPointer arrayClass = ObjectMemory_fetchClassOf(array);
    Word index = Interpreter_fetchInteger_ofObject(streamIndexIndex, stream);
    Word limit = Interpreter_fetchInteger_ofObject(streamWriteLimitIndex,stream);
    Interpreter_success_(index < limit);
    Interpreter_success_(arrayClass==ClassArrayPointer || arrayClass==ClassStringPointer);
    Interpreter_checkIndexableBoundsOf_in(index + 1, array);
    if(Interpreter_success()){
        index = index + 1;
        if(arrayClass == ClassArrayPointer){
            Interpreter_subscript_with_storing(array, index, value);
        } else {
            ObjectPointer ascii = ObjectMemory_fetchPointer_ofObject(characterValueIndex, value);
            Interpreter_subscript_with_storing(array, index, ascii);
        }
    }
     
    if (Interpreter_success()) {
        Interpreter_storeInteger_ofObject_withValue(streamIndexIndex, stream, index);
    }

    if (Interpreter_success()) {
        Interpreter_push(value);
    } else {
        Interpreter_unPop(2);
    }
        
}

void Interpreter_primitiveAtEnd(void){
    ObjectPointer stream = Interpreter_popStack();
    ObjectPointer array = ObjectMemory_fetchPointer_ofObject(streamArrayIndex, stream);
    ObjectPointer arrayClass = ObjectMemory_fetchClassOf(array);
    Word length = Interpreter_lengthOf(array);
    Word index = Interpreter_fetchInteger_ofObject(streamIndexIndex, stream);
    Word limit = Interpreter_fetchInteger_ofObject(streamReadLimitIndex, stream);
    Interpreter_success_(arrayClass == ClassArrayPointer || arrayClass == ClassStringPointer);
    if(Interpreter_success()) {
        if (index >= limit || index >= length) {
            Interpreter_push(TruePointer);
        } else {
            Interpreter_push(FalsePointer);
        }
    } else {
        Interpreter_unPop(1);
    }
}

