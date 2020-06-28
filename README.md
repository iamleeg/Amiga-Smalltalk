# Amiga-Smalltalk

It's Smalltalk. For Amiga. It was made by following the instructions in the Blue Book:
Smalltalk-80: The Language and Its Implementation.

![VM Tests](https://github.com/iamleeg/Amiga-Smalltalk/workflows/VM%20Tests/badge.svg)

## A note on comments

Doxygen comments were added by the Amiga-Smalltalk authors, as were all `/* block comments */`.

`/*ST:  line comments are transcribed from the Blue Book. */` These were originally BCPL-style
`// comments`, but those aren't in the C89 standard and many Amiga compilers won't accept them.

Page numbers, if not otherwise clarified, refer to pages in the single printed edition of the Blue Book.

## Building the tests

### In Debianish Linux

You need the AxRuntime, which is included in `vendor/`. Note that once you've installed the packages you (currently) need to link against the included version of the library, which addresses a crash on process quit that made successful tests look like failures.

`make check` will build and run the tests.

### Building with AROS metamake

1. Check this folder out somewhere in the AROS source tree (e.g. `/path/to/aros-src/local/AmigaSmalltalk`).
2. `cd /path/to/aros-src`
3. `./configure`
4. `make local-amigasmalltalktests`
5. The tests are available in AROS as `:local/AmigaSmalltalk/AmigaSmalltalkTests`.

### `error_host`

`error_host` is a tool that simulates a VM error, to make sure that it displays the UI correctly. Just build and run to see an uninspiring error message :).

## A note on the tests

While these tests mostly look like unit tests, they're actually integration tests because they use the real `RealWordMemory` even when testing the `Interpreter`. A test suite gets a new, blank `RealWordMemory` on each run. You should be aware of these gotchas in your test design:

 - assigning object pointers to variables will change the retain counts on the pointers, and potentially trigger deallocation. For this reason, I often design tests to write the pointers into memory as _words_ to avoid triggering unexpected behaviour.
 - re-using objects or obejct pointers between tests _may_ cause problems if you don't explicitly clean up.
 - allocating a new object, either in test code or in the SUT, relies on having free memory available from which to allocate the object. If sufficient memory isn't immediately available, the VM will trigger a mark-sweep garbage collection which may both take time and fail, if the memory is not in a consistent state.
 - if the test runner launches, then you have exactly one segment (128kB, minus space for the free lists and object table) of memory available to use. You may have more, but can't rely on that. Once I realised this I introduced the `RealWordMemory_bestSegmentFor` method to support "legacy" tests that wrote to arbitrarily-chosen segments (it picks a segment number based on the requested one, and the number actually available). This introduces some non-determinism into the tests, so isn't recommended. For new tests, use segment 0 wherever possible.

## Copying

Amiga-Smalltalk is made available under the terms of the European Union Public License, version 1.2. See LICENSE.txt.
