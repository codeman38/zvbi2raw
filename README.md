# zvbi2raw

## Overview

`zvbi2raw` is a quick-and-dirty tool for capturing raw data from the
vertical blanking interval of a video capture device, using the
[`libzvbi`](http://zapping.sourceforge.net/ZVBI/) library.

## Usage

By default, `zvbi2raw` will write unmodified VBI data from line 21 of
the video signal to standard output, using `/dev/vbi0` as the input device.
However, the following options are available:

* **-l**: Specify a different line of the video signal to use.
* **-d**: Specify a different input device to use.
* **-s**: Strip off the parity bit of each byte in the output; this makes
    the output somewhat more human-readable.
* **-m**: Include a 'magic number' of 0xFFFFFFFF at the beginning of the file.
    This makes the output file compatible with McPoodle's
    [SCC Tools](http://www.theneitherworld.com/mcpoodle/SCC_TOOLS/DOCS/SCC_TOOLS.HTML).

## Compiling

Assuming you already have `libzvbi` and its headers installed, run `make`
from the source directory and you should get an executable named `zvbi2raw`.
Then just copy that wherever you'd like it.
