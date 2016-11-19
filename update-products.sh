#! /bin/bash
# Copyright 2016 Peter Williams
# Licensed under the MIT License.
#
# Copy the generated C source files to products/. When we improve the Web-to-C
# conversion process, we can update these files and have nice diffs to look
# at. These can then be applied to the main Tectonic tree as appropriate.

set -e

for f in xetex-pool.c xetexini.c xetex0.c xetexcoerce.h xetexd.h ; do
    cp BUILD/$f products/
    indent -linux -nut -i4 -l120 products/$f
    rm -f products/${f}~
done
