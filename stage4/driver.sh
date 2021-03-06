#! /bin/bash
# Copyright 2020 the Tectonic Project.
# Licensed under the MIT License.

set -e
stage4dir="$(dirname $0)"
topdir="$(cd "$stage4dir/.." && pwd)"

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

function copy_source() {
    set -x

    rm -rf $state_dir/outputs/stage4
    mkdir -p $state_dir/outputs/stage4
    cp -r $state_dir/outputs/stage3/* $state_dir/outputs/stage4
    rm -rf $state_dir/outputs/stage4/build

    cd $topdir/stage3/exports
    find -type f |while read fn ; do
        cp $fn ../../state/outputs/stage4/$fn
    done

    $topdir/stage0/driver.sh builder-bash -c \
        "python3 /source/stage4/rewrite-symbols.py /state/outputs/stage4"
}

function setup_build() {
    build=$state_dir/outputs/stage4/build
    rm -rf $build
    mkdir -p $build

    $topdir/stage0/driver.sh builder-bash -c \
        "cd /state/outputs/stage4/build && python3 /source/stage1/gen-ninja.py --use-custom-otangle --build-book-tex --underscores .."
}

function run_build() {
    $topdir/stage0/driver.sh builder-bash -c \
        "cd /state/outputs/stage4/build && ninja $@"
}


# Dispatch subcommands.

case "$command" in
    copy-source)
        copy_source "$@" ;;
    setup-build)
        setup_build "$@" ;;
    run-build)
        run_build "$@" ;;
    *)
        die "unrecognized command \"$command\"" ;;
esac
