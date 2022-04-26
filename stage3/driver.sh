#! /bin/bash
# Copyright 2020 the Tectonic Project.
# Licensed under the MIT License.

set -e
stage3dir="$(dirname $0)"
topdir="$(cd "$stage3dir/.." && pwd)"

if [ -z "$1" -o "$1" = help ] ; then
    echo "You must supply a subcommand. Subcommands are:
copy-source    -- Copy outputs from previous stages into working directory
setup-build    -- Set up the compilation
run-build      -- Run the compilation
build-book     -- Build the book PDF
update-exports -- Update export files for this stage
"
    exit 1
fi

command="$1"
shift

function die () {
    echo >&2 "error:" "$@"
    exit 1
}

# trailing slash-dot makes the check work when `state` is a symlink:
if [ ! -d "$topdir/state/." ] ; then
    die "you must create or symlink a \`state\` directory in \`$topdir\`"
fi

state_dir="$topdir/state"


# Commands

function copy_source () {
    set -x

    rm -rf $state_dir/outputs/stage3
    mkdir -p $state_dir/outputs/stage3
    cp -r $state_dir/outputs/stage2/* $state_dir/outputs/stage3
    rm -rf $state_dir/outputs/stage3/build

    cd $topdir/stage2/exports
    find -type f |while read fn ; do
        cp $fn ../../state/outputs/stage3/$fn
    done
}

function setup_build() {
    build=$state_dir/outputs/stage3/build
    rm -rf $build
    mkdir -p $build

    $topdir/stage0/driver.sh builder-bash -c \
        "cd /state/outputs/stage3/build && python3 /source/stage1/gen-ninja.py --use-custom-otangle --build-book-tex .."
}

function run_build() {
    $topdir/stage0/driver.sh builder-bash -c \
        "cd /state/outputs/stage3/build && ninja $@"
}

function build_book() {
    cp $topdir/state/outputs/stage3/xetexdir/xewebmac.tex $topdir/state/outputs/stage3/build/
    tectonic -f plain $topdir/state/outputs/stage3/build/tectonic-book.tex
}

function update_exports() {
    for f in bibtex/bibtex.ch web2c/cvtbib.sed web2c/fixwrites.c web2c/makecpool.c \
        web2c/web2c-parser.y weave/weave.ch xetexdir/tex.ch ; do
        cp $topdir/state/outputs/stage3/$f $stage3dir/exports/$f

        case $f in
            *.c|*.h)
                $topdir/stage0/driver.sh indent-inplace $stage3dir/exports/$f
                ;;
        esac
    done
}


# Dispatch subcommands.

case "$command" in
    copy-source)
        copy_source "$@" ;;
    setup-build)
        setup_build "$@" ;;
    run-build)
        run_build "$@" ;;
    build-book)
        build_book "$@" ;;
    update-exports)
        update_exports "$@" ;;
    *)
        die "unrecognized command \"$command\"" ;;
esac
