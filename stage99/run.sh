#! /bin/bash
# Copyright 2020 the Tectonic Project.
# Licensed under the MIT License.

set -euo pipefail

stage99dir="$(dirname $0)"
cd $stage99dir/..

prev_src=state/outputs/stage4
prev_bld=$prev_src/build

# Queue up the tree into `state` so that we can run `indent`
# inside the Docker container

stage=state/outputs/stage99
rm -rf $stage
mkdir -p $stage/{bibtex,dvipdfm-x,lib,synctex,teckit,xetex}

cp $prev_bld/bibtex.[ch] $stage/bibtex/

cp $prev_src/dvipdfm-x/*.c $stage/dvipdfm-x/
cp $prev_src/dvipdfm-x/*.h $stage/dvipdfm-x/

cp $prev_src/lib/texmfmp.c $stage/lib/

cp $prev_src/synctexdir/*.c $stage/synctex/
cp $prev_src/synctexdir/*.h $stage/synctex/

cp $prev_src/teckit/*.c $stage/teckit/
cp $prev_src/teckit/*.cpp $stage/teckit/
cp $prev_src/teckit/*.h $stage/teckit/

cp $prev_src/xetexdir/*.c $stage/xetex/
cp $prev_src/xetexdir/*.cpp $stage/xetex/
cp $prev_src/xetexdir/*.h $stage/xetex/
cp $prev_src/xetexdir/*.mm $stage/xetex/
cp $prev_src/xetexdir/image/*.c $stage/xetex/
cp $prev_src/xetexdir/image/*.h $stage/xetex/
cp $prev_bld/xetex*.c $stage/xetex/
cp $prev_bld/xetex*.h $stage/xetex/

# Indent

stage0/driver.sh builder-bash /source/stage99/indent.sh \
  bibtex/bibtex.c \
  bibtex/bibtex.h \
  xetex/xetex0.c \
  xetex/xetexcoerce.h \
  xetex/xetexd.h \
  xetex/xetexini.c \
  xetex/xetex-pool.c

stage0/driver.sh builder-bash -c "chown -R $(id -u):$(id -g) /state/outputs/stage99"

# Copy back into the repo

cp -r state/outputs/stage99/* reference-sources/
