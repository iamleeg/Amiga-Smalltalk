check: ast_tests
	./ast_tests

.PHONY: check

ast_tests: ObjectMemory.c ObjectMemory.h vm_tests.c
	gcc -ggdb ObjectMemory.c vm_tests.c -o ast_tests
