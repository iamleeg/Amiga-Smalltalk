# Amiga-Smalltalk

It's Smalltalk. For Amiga. It was made by following the instructions in the Blue Book:
Smalltalk-80: The Language and Its Implementation.

## A note on comments

Doxygen comments were added by the Amiga-Smalltalk authors, as were all /* block comments */.
/*ST:  line comments are transcribed from the Blue Book. */

Page numbers, if not otherwise clarified, refer to pages in the single printed edition of the Blue Book.

## Building the tests

### In AROS/AmigaDOS

`make check` will build and run the tests.

### Building with AROS metamake

1. Check this folder out somewhere in the AROS source tree (e.g. `/path/to/aros-src/local/AmigaSmalltalk`).
2. `cd /path/to/aros-src`
3. `./configure`
4. `make local-amigasmalltalktests`
5. The tests are available in AROS as `:local/AmigaSmalltalk/AmigaSmalltalkTests`.


## Copying

Amiga-Smalltalk is made available under the terms of the European Union Public License, version 1.2. See LICENSE.txt.