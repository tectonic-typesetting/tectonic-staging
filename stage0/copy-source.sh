#! /bin/bash
# Copyright 2016-2020 the Tectonic Project.
# Licensed under the MIT License.

set -xeuo pipefail

stage0dir="$(dirname $0)"
statedir="$stage0dir/../state"

# In order to get the stage1 build working nicely, we need to hack the "convert"
# program a little bit. So export it into the repo so that we can manage those
# hacks through Git's merge functionality.

cp "$statedir/repo/Build/source/texk/web2c/web2c/convert" "$stage0dir/exports/web2c-convert"

# Now, the many "outputs" that are not tracked in the repository itself.

cd "$statedir"

src=repo/Build/source
build=rbuild
out=outputs/stage0

rm -rf $out
mkdir -p $out

# XeTeX base sources

mkdir -p $out/xetexdir/image

cp $src/texk/web2c/xetexdir/*.cpp $out/xetexdir/
cp $src/texk/web2c/xetexdir/*.c $out/xetexdir/
cp $src/texk/web2c/xetexdir/*.h $out/xetexdir/
cp $src/texk/web2c/xetexdir/*.mm $out/xetexdir/
cp $src/texk/web2c/xetexdir/image/*.c $out/xetexdir/image/
cp $src/texk/web2c/xetexdir/image/*.h $out/xetexdir/image/
cp $src/texk/web2c/*.h $out/xetexdir/

cp $src/texk/web2c/tex.ch $out/xetexdir/
cp $src/texk/web2c/tex-binpool.ch $out/xetexdir/
cp $src/texk/web2c/xetexdir/*.web $out/xetexdir/
cp $src/texk/web2c/xetexdir/*.ch* $out/xetexdir/
cp $src/texk/web2c/xetexdir/*.defines $out/xetexdir/

# web2c support

mkdir -p $out/w2c
cp $src/texk/web2c/w2c/*.h $out/w2c/
cp $build/texk/web2c/w2c/*.h $out/w2c/

# synctex

mkdir -p $out/synctexdir
for f in synctex.c synctex.h synctex-common.h synctex-xetex.h ; do
    cp $src/texk/web2c/synctexdir/$f $out/synctexdir/
done

cp $src/texk/web2c/synctexdir/synctex-xe-*.ch* $out/synctexdir/
cp $src/texk/web2c/synctexdir/synctex-e-*.ch* $out/synctexdir/
cp $src/texk/web2c/synctexdir/synctex-mem.ch0 $out/synctexdir/
cp $src/texk/web2c/synctexdir/synctex-rec.ch0 $out/synctexdir/
cp $src/texk/web2c/synctexdir/*.defines $out/synctexdir/

# base lib

mkdir -p $out/lib
cp $src/texk/web2c/lib/*.c $out/lib/
cp $src/texk/web2c/lib/*.h $out/lib/
rm $out/lib/alloca.c $out/lib/mfmpw32.c

# libmd5

mkdir -p $out/libmd5
cp $src/texk/web2c/libmd5/*.c $out/libmd5/
cp $src/texk/web2c/libmd5/*.h $out/libmd5/
rm $out/libmd5/md5main.c

# kpathsea

mkdir -p $out/kpathsea
cp $src/texk/kpathsea/*.c $out/kpathsea/
cp $src/texk/kpathsea/*.h $out/kpathsea/
cp $build/texk/kpathsea/*.h $out/kpathsea/
for f in getopt.c getopt1.c kpsestat.c kpsewhich.c knj.c knj.h putenv.c readlink.c win32lib.c ; do
    rm $out/kpathsea/$f
done

# TECkit

mkdir -p $out/teckit
cp $src/libs/teckit/TECkit-src/source/Engine.cpp $out/teckit/
cp $src/libs/teckit/TECkit-src/source/*.h $out/teckit/
cp $src/libs/teckit/TECkit-src/source/NormalizationData.c $out/teckit/
cp $src/libs/teckit/TECkit-src/source/Public-headers/*.h $out/teckit/
cp $build/libs/teckit/*.h $out/teckit/

# tie -- which is itself generated from another program, so its source comes
# out of the build directory. We shouldn't need to go any farther down this
# particular rabbit hole though.

mkdir -p $out/tiedir
cp $build/texk/web2c/tie.c $out/tiedir/

# otangle -- more rabbit-holery. Also, note hack for lib/main.c up above.

mkdir -p $out/otangle
cp $build/texk/web2c/otangle.c $out/otangle/
cp $build/texk/web2c/otangle.h $out/otangle/

# Pascal-to-C conversion script and support files

mkdir -p $out/web2c
cp $src/texk/web2c/web2c/*.sed $out/web2c/
cp $src/texk/web2c/web2c/*.defines $out/web2c/
cp $src/texk/web2c/web2c/*.c $out/web2c/
cp $src/texk/web2c/web2c/*.h $out/web2c/
cp $src/texk/web2c/web2c/*.l $out/web2c/
cp $src/texk/web2c/web2c/*.y $out/web2c/
cp $src/texk/web2c/web2c/convert $out/web2c/
rm $out/web2c/web2c-lexer.c $out/web2c/web2c-parser.c $out/web2c/web2c-parser.h

# xdvipdfmx

mkdir -p $out/dvipdfm-x
cp $src/texk/dvipdfm-x/*.c $out/dvipdfm-x/
cp $src/texk/dvipdfm-x/*.h $out/dvipdfm-x/
rm $out/dvipdfm-x/xbb.c

# bibtex

mkdir -p $out/bibtex
cp $src/texk/web2c/bibtex.web $out/bibtex/
cp $src/texk/web2c/bibtex.ch $out/bibtex/

# weave (for generating XeTeX The Program)

mkdir -p $out/weave
cp $src/texk/web2c/weave.web $out/weave/
cp $src/texk/web2c/weave.ch $out/weave/