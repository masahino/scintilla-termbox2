# scintilla-termbox2

A [Scintilla](https://www.scintilla.org/) platform implementation using
[termbox2](https://github.com/termbox/termbox2).

termbox2 2.5.0 is included in `vendor/termbox2`.

## Build

Place this directory at `scintilla/termbox2` in a Scintilla source tree, then run:

```sh
make
```

This builds `scintilla/bin/scintilla.a`.

## Integration

Define the termbox2 implementation in one source file:

```c
#define TB_IMPL
#include "ScintillaTermbox2.h"
```

`ScintillaTermbox2.h` enables true color and extended grapheme cluster support
consistently for termbox2 and the Scintilla platform sources.

## Example

```sh
cd examples/semester
make
./semester
```
