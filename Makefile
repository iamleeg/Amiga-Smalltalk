check: ast_tests
	./ast_tests

.PHONY: check

SOURCE_FILES = ObjectMemory_Storage.c ObjectMemory_FreeList.c ObjectMemory_Allocation.c RealWordMemory.c
HEADER_FILES = ObjectMemory.h ObjectMemory_Constants.h ObjectMemory_Storage.h ObjectMemory_FreeList.h ObjectMemory_Allocation.h RealWordMemory.h Types.h

TEST_SOURCES = test_main.c FreeListTests.c ObjectMemoryTests.c RealWordMemoryTests.c AllocationTests.c
TEST_HEADERS = tests.h FreeListTests.h ObjectMemoryTests.h RealWordMemoryTests.h AllocationTests.h

ast_tests: $(SOURCE_FILES) $(HEADER_FILES) $(TEST_SOURCES) $(TEST_HEADERS)
	gcc -ggdb $(SOURCE_FILES) $(TEST_SOURCES) -o ast_tests
