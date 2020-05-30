# Amiga-Smalltalk

It's Smalltalk. For Amiga. It was made by following the instructions in the Blue Book:
Smalltalk-80: The Language and Its Implementation.

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


## Copying

Amiga-Smalltalk is made available under the terms of the European Union Public License, version 1.2. See LICENSE.txt.
