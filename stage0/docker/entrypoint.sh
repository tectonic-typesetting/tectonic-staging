#! /bin/bash
# Copyright 2016-2020 the Tectonic Project.
# Licensed under the MIT License.
#
# This is the "entrypoint" script for the TeXLive build image. When a user runs
# "docker run texlive-builder foo bar", we are invoked with $1=foo and $2=bar.

set -e

if [ -z "$1" -o "$1" = help ] ; then
    echo "You must supply a subcommand to run in the container. Commands are:

bash              -- Run a bash shell
init-build        -- Initialize a compilation of the TeXLive sources.

E.g., use 'docker run -it buildercontainer bash' to launch an interactive
shell within a long-running container.
"
    exit 1
fi

command="$1"
shift


# Commands

function init_build() {
    mkdir /state/rbuild
    cd /state/rbuild
    ../repo/Build/source/configure \
        --prefix=/state/rinstall \
        --disable-static \
        --disable-native-texlive-build \
        --disable-all-pkgs \
        --enable-web2c \
        --enable-web-progs \
        --enable-dvipdfm-x \
        \
        --disable-tex \
        --disable-etex \
        --disable-ptex \
        --disable-eptex \
        --disable-uptex \
        --disable-euptex \
        --disable-aleph \
        --disable-pdftex \
        --disable-luatex \
        --disable-luajittex \
        --disable-mp \
        --disable-pmp \
        --disable-upmp \
        --enable-xetex \
        --disable-mf \
        --disable-mf-nowin \
        --disable-mflua \
        --disable-mfluajit \
        \
        --with-system-cairo=yes \
        --with-system-freetype2=yes \
        --with-system-gd=yes \
        --with-system-gmp=yes \
        --with-system-graphite2=yes \
        --with-system-harfbuzz=yes \
        --with-system-icu=yes \
        --with-system-kpathsea=no \
        --with-system-libpaper=yes \
        --with-system-libpng=yes \
        --with-system-mpfr=yes \
        --with-system-pixman=yes \
        --with-system-poppler=yes \
        --with-system-potrace=yes \
        --with-system-ptexenc=no \
        --with-system-teckit=no \
        --with-system-xpdf=no \
        --with-system-zlib=yes \
        --with-system-zziplib=yes
}


# Dispatch subcommands.

if [ "$command" = bash ] ; then
    exec bash "$@"
elif [ "$command" = init-build ] ; then
    init_build "$@"
else
    echo "Unrecognized command \"$command\"."
    exit 1
fi
