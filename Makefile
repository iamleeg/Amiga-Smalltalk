check: ast_tests
	./ast_tests

.PHONY: check

CFLAGS = -ggdb -D__AROS__ -I/usr/include/axrt
LDFLAGS = -lexec -lintuition -lmieee
OBJ = -specs=/usr/lib/x86_64-linux-gnu/axrt.specs /usr/lib/x86_64-linux-gnu/startup.o /usr/lib/x86_64-linux-gnu/axrt.ld

SOURCE_FILES = ObjectMemory.c ObjectMemory_Storage.c ObjectMemory_FreeList.c ObjectMemory_Allocation.c ObjectMemory_RefCounting.c ObjectMemory_GC.c RealWordMemory.c Interpreter.c Interpreter_Error.c Interpreter_PrimArith.c 
HEADER_FILES = ObjectMemory.h ObjectMemory_Constants.h ObjectMemory_Storage.h ObjectMemory_FreeList.h ObjectMemory_Allocation.h ObjectMemory_RefCounting.h ObjectMemory_GC.h RealWordMemory.h Types.h Interpreter.h Interpreter_Constants.h Interpreter_Error.h Interpreter_PrimArith.h

TEST_SOURCES = test_main.c FreeListTests.c ObjectMemoryTests.c RealWordMemoryTests.c AllocationTests.c RefCountingTests.c PrimitiveMethodTests.c InterpreterMemoryTests.c CompiledMethodTests.c ContextTests.c ClassTests.c Interpreter_IntegerPrimTests.c Interpreter_IntegerArithTests.c Interpreter_IntegerCmpTests.c
TEST_HEADERS = tests.h FreeListTests.h ObjectMemoryTests.h RealWordMemoryTests.h AllocationTests.h RefCountingTests.h PrimitiveMethodTests.h InterpreterMemoryTests.h CompiledMethodTests.h ContextTests.h ClassTests.h Interpreter_IntegerPrimTests.h Interpreter_IntegerArithTests.h Interpreter_IntegerCmpTests.h

ast_tests: $(SOURCE_FILES) $(HEADER_FILES) $(TEST_SOURCES) $(TEST_HEADERS)
	gcc $(CFLAGS) $(SOURCE_FILES) $(TEST_SOURCES) $(OBJ) -o ast_tests $(LDFLAGS)

error_host: Interpreter_Error.c Interpreter_Error.h Error_Host.c
	gcc $(CFLAGS) Interpreter_Error.c Error_Host.c $(OBJ) -o error_host $(LDFLAGS)
