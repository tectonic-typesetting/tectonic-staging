#! /bin/bash
# Copyright 2020 the Tectonic Project.
# Licensed under the MIT License.

set -e
stage3dir="$(dirname $0)"
topdir="$(cd "$stage3dir/.." && pwd)"

if [ -z "$1" -o "$1" = help ] ; then
    echo "You must supply a subcommand. Subcommands are:
copy-source -- Copy outputs from previous stages into working directory
setup-build -- Set up the compilation
run-build   -- Run the compilation
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
