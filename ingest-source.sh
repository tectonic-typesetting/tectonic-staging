#! /bin/bash
# Copyright 2016 the Tectonic Project.
# Licensed under the MIT License.

src=state/repo/Build/source
build=state/rbuild

cd $(dirname $0)

# XeTeX base sources

mkdir -p xetexdir/image

cp $src/texk/web2c/xetexdir/*.cpp xetexdir/
cp $src/texk/web2c/xetexdir/*.c xetexdir/
cp $src/texk/web2c/xetexdir/*.h xetexdir/
cp $src/texk/web2c/xetexdir/*.mm xetexdir/
cp $src/texk/web2c/xetexdir/image/*.c xetexdir/image/
cp $src/texk/web2c/xetexdir/image/*.h xetexdir/image/
cp $src/texk/web2c/*.h xetexdir/

cp $src/texk/web2c/tex.ch xetexdir/
cp $src/texk/web2c/tex-binpool.ch xetexdir/
cp $src/texk/web2c/xetexdir/*.web xetexdir/
cp $src/texk/web2c/xetexdir/*.ch* xetexdir/
cp $src/texk/web2c/xetexdir/*.defines xetexdir/

# web2c support

mkdir -p w2c
cp $src/texk/web2c/w2c/*.h w2c/

# synctex

mkdir -p synctexdir
for f in synctex.c synctex.h synctex-common.h synctex-xetex.h ; do
    cp $src/texk/web2c/synctexdir/$f synctexdir/
done

cp $src/texk/web2c/synctexdir/synctex-xe-*.ch* synctexdir/
cp $src/texk/web2c/synctexdir/synctex-e-*.ch* synctexdir/
cp $src/texk/web2c/synctexdir/synctex-mem.ch0 synctexdir/
cp $src/texk/web2c/synctexdir/synctex-rec.ch0 synctexdir/
cp $src/texk/web2c/synctexdir/*.defines synctexdir/

# base lib

mkdir -p lib
cp $src/texk/web2c/lib/*.c lib/
cp $src/texk/web2c/lib/*.h lib/
rm lib/alloca.c lib/mfmpw32.c

# haaack

mkdir -p otangle
mv lib/main.c otangle/

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
for f in getopt.c getopt1.c kpsestat.c kpsewhich.c knj.c knj.h putenv.c readlink.c win32lib.c ; do
    rm kpathsea/$f
done

# TECkit

mkdir -p teckit
cp $src/libs/teckit/TECkit-src/source/Engine.cpp teckit/
cp $src/libs/teckit/TECkit-src/source/*.h teckit/
cp $src/libs/teckit/TECkit-src/source/NormalizationData.c teckit/
cp $src/libs/teckit/TECkit-src/source/Public-headers/*.h teckit/

# tie -- which is itself generated from another program, so its source comes
# out of the build directory. We shouldn't need to go any farther down this
# particular rabbit hole though.

mkdir -p tiedir
cp $build/texk/web2c/tie.c tiedir/

# otangle -- more rabbit-holery. Also, note hack for lib/main.c up above.

mkdir -p otangle
cp $build/texk/web2c/otangle.c otangle/
cp $build/texk/web2c/otangle.h otangle/

# Pascal-to-C conversion script and support files

mkdir -p web2c
cp $src/texk/web2c/web2c/*.sed web2c/
cp $src/texk/web2c/web2c/*.defines web2c/
cp $src/texk/web2c/web2c/*.c web2c/
cp $src/texk/web2c/web2c/*.h web2c/
cp $src/texk/web2c/web2c/*.l web2c/
cp $src/texk/web2c/web2c/*.y web2c/
rm web2c/web2c-lexer.c web2c/web2c-parser.c web2c/web2c-parser.h

# xdvipdfmx

mkdir -p dvipdfm-x
cp $src/texk/dvipdfm-x/*.c dvipdfm-x/
cp $src/texk/dvipdfm-x/*.h dvipdfm-x/
rm dvipdfm-x/xbb.c

# bibtex

mkdir -p bibtex
cp $src/texk/web2c/bibtex.web bibtex/
cp $src/texk/web2c/bibtex.ch bibtex/

# weave (for generating XeTeX The Program)

mkdir -p weave
cp $src/texk/web2c/weave.web weave/
cp $src/texk/web2c/weave.ch weave/
