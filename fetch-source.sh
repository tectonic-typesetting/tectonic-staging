#! /bin/bash

src=/a/texlive-src/Build/source
build=/a/texlive-src/Build/source/BUILD

cd $(dirname $0)

# To validate:
#
# - base sources
# - synctex
# - (local) libmd5
# - (local) lib.a
# - libkpathsea
# - libTECkit
#
# External:
#
# - harfbuzz
# - graphite2
# - libicuuc
# - libicudata
# - libpoppler
# - libpng
# - libfreetype2
# - libz
# - libfontconfig

# Raw sources

mkdir -p xetexdir/image

cp $src/texk/web2c/xetexdir/*.cpp xetexdir/
cp $src/texk/web2c/xetexdir/*.c xetexdir/
cp $src/texk/web2c/xetexdir/*.h xetexdir/
cp $src/texk/web2c/xetexdir/image/*.c xetexdir/image/
cp $src/texk/web2c/xetexdir/image/*.h xetexdir/image/
cp $src/texk/web2c/*.h xetexdir/
rm xetexdir/XeTeX_mac.c xetexdir/XeTeXFontInst_Mac.cpp

# web2c'd sources

cp $build/texk/web2c/xetex*.c xetexdir/
cp $build/texk/web2c/xetex*.h xetexdir/

# web2c support

mkdir -p w2c
cp $src/texk/web2c/w2c/*.h w2c/

# synctex

mkdir -p synctexdir
for f in synctex.c synctex.h synctex-common.h synctex-xetex.h ; do
    cp $src/texk/web2c/synctexdir/$f synctexdir/
done

# base lib

mkdir -p lib
cp $src/texk/web2c/lib/*.c lib/
cp $src/texk/web2c/lib/*.h lib/
rm lib/alloca.c lib/main.c lib/mfmpw32.c

# libmd5

mkdir -p libmd5
cp $src/texk/web2c/libmd5/*.c libmd5/
cp $src/texk/web2c/libmd5/*.h libmd5/
rm libmd5/md5main.c

# kpathsea

mkdir -p kpathsea
cp $src/texk/kpathsea/*.c kpathsea/
cp $src/texk/kpathsea/*.h kpathsea/
cp $build/texk/kpathsea/kpathsea.h kpathsea/
cp $build/texk/kpathsea/paths.h kpathsea/
for f in getopt.c getopt1.c kpsestat.c kpsewhich.c knj.c knj.h readlink.c win32lib.c ; do
    rm kpathsea/$f
done

# TECkit

mkdir -p teckit
cp $src/libs/teckit/TECkit-src/source/Engine.cpp teckit/
cp $src/libs/teckit/TECkit-src/source/*.h teckit/
cp $src/libs/teckit/TECkit-src/source/NormalizationData.c teckit/
cp $src/libs/teckit/TECkit-src/source/Public-headers/*.h teckit/
