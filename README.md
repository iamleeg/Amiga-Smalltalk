# Amiga-Smalltalk

It's Smalltalk. For Amiga.

## Building the tests

### In AROS/AmigaDOS

`make check` will build and run the tests.

### Building with AROS metamake

1. Check this folder out somewhere in the AROS source tree (e.g. `/path/to/aros-src/local/AmigaSmalltalk`).
2. `cd /path/to/aros-src`
3. `./configure`
4. `make local-amigasmalltalktests`
5. The tests are available in AROS as `:local/AmigaSmalltalk/AmigaSmalltalkTests`.
