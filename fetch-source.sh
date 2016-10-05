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


mkdir -p src

# Raw sources.

cp $src/texk/web2c/xetexdir/*.cpp src/
cp $src/texk/web2c/xetexdir/*.c src/
cp $src/texk/web2c/xetexdir/*.h src/
cp $src/texk/web2c/xetexdir/image/*.c src/
cp $src/texk/web2c/xetexdir/image/*.h src/

# web2c'd sources.

cp $build/texk/web2c/xetex*.c src/
cp $build/texk/web2c/xetex*.h src/

# synctex

mkdir -p synctex
for f in synctex.c synctex-xetex.h ; do
    cp $src/texk/web2c/synctexdir/$f synctex/
done

# base lib

mkdir -p baselib
cp $src/texk/web2c/lib/*.c baselib/
cp $src/texk/web2c/lib/*.h baselib/

# libmd5

mkdir -p libmd5
cp $src/texk/web2c/libmd5/*.c libmd5/
cp $src/texk/web2c/libmd5/*.h libmd5/

# kpathsea

mkdir -p kpathsea
cp $src/texk/kpathsea/*.c kpathsea/
cp $src/texk/kpathsea/*.h kpathsea/
cp $build/texk/kpathsea/paths.h kpathsea/
for f in knj.c knj.h win32lib.c ; do
    rm kpathsea/$f
done

# TECkit

mkdir -p teckit
cp $src/libs/teckit/TECkit-src/source/Engine.cpp teckit/
cp $src/libs/teckit/TECkit-src/source/*.h teckit/
cp $src/libs/teckit/TECkit-src/source/NormalizationData.c teckit/
cp $src/libs/teckit/TECkit-src/source/Public-headers/*.h teckit/
