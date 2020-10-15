#! /bin/bash
# Copyright 2020 the Tectonic Project.
# Licensed under the MIT License.

set -e
stage2dir="$(dirname $0)"
topdir="$(cd "$stage2dir/.." && pwd)"

if [ -z "$1" -o "$1" = help ] ; then
    echo "You must supply a subcommand. Subcommands are:
copy-source -- Copy outputs from stage0 into stage1 working directory
setup-build -- Set up the stage1 compilation
run-build   -- Run the stage1 compilation
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

    rm -rf $state_dir/outputs/stage2
    mkdir -p $state_dir/outputs/stage2
    cp -r $state_dir/outputs/stage1/* $state_dir/outputs/stage2
    rm -rf $state_dir/outputs/stage2/build

    cd $topdir/stage1/exports
    find -type f |while read fn ; do
        cp $fn ../../state/outputs/stage2/$fn
    done
}

function setup_build() {
    build=$state_dir/outputs/stage2/build
    rm -rf $build
    mkdir -p $build

    $topdir/stage0/driver.sh builder-bash -c \
        "cd /state/outputs/stage2/build && python3 /source/stage1/gen-ninja.py --use-custom-otangle .."
}

function run_build() {
    $topdir/stage0/driver.sh builder-bash -c \
        "cd /state/outputs/stage2/build && ninja $@"
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
