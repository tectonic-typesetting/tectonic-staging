#! /bin/bash
# Copyright 2020 the Tectonic Project.
# Licensed under the MIT License.

set -e
stage1dir="$(dirname $0)"
topdir="$(cd "$stage1dir/.." && pwd)"

if [ -z "$1" -o "$1" = help ] ; then
    echo "You must supply a subcommand. Subcommands are:
copy-source    -- Copy outputs from stage0 into stage1 working directory
setup-build    -- Set up the stage1 compilation
run-build      -- Run the stage1 compilation
update-exports -- Update files in `exports/`
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
    rm -rf $state_dir/outputs/stage1
    mkdir -p $state_dir/outputs/stage1
    cp -r $state_dir/outputs/stage0/* $state_dir/outputs/stage1
}

function setup_build() {
    build=$state_dir/outputs/stage1/build
    rm -rf $build
    mkdir -p $build

    $topdir/stage0/driver.sh builder-bash -c \
        "cd /state/outputs/stage1/build && python3 /source/stage1/gen-ninja.py .."
}

function run_build() {
    $topdir/stage0/driver.sh builder-bash -c \
        "cd /state/outputs/stage1/build && ninja $@"
}

function update_exports() {
    for f in web2c/fixwrites.c web2c/main.c web2c/splitup.c web2c/web2c-lexer.l \
        web2c/web2c-parser.y xetexdir/xetex.defines ; do
        cp $topdir/state/outputs/stage1/$f $stage1dir/exports/$f
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
    update-exports)
        update_exports "$@" ;;
    *)
        die "unrecognized command \"$command\"" ;;
esac
