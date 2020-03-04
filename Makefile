check: ast_tests
	./ast_tests

.PHONY: check

SOURCE_FILES = ObjectMemory.c ObjectMemory_FreeList.c RealWordMemory.c
HEADER_FILES = ObjectMemory.h ObjectMemory_FreeList.h RealWordMemory.h Types.h

TEST_SOURCES = test_main.c FreeListTests.c ObjectMemoryTests.c RealWordMemoryTests.c
TEST_HEADERS = tests.h FreeListTests.h ObjectMemoryTests.h RealWordMemoryTests.h

ast_tests: $(SOURCE_FILES) $(HEADER_FILES) $(TEST_SOURCES) $(TEST_HEADERS)
	gcc -ggdb $(SOURCE_FILES) $(TEST_SOURCES) -o ast_tests
