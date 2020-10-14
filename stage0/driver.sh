#! /bin/bash
# Copyright 2016-2020 the Tectonic Project.
# Licensed under the MIT License.

image_name=tectonic-texlive-builder

set -e
thisdir="$(dirname $0)"
topdir="$(cd "$thisdir/.." && pwd)"

if [ -z "$1" -o "$1" = help ] ; then
    echo "You must supply a subcommand. Subcommands are:

build-image  -- Create/update to builder Docker image.
builder-bash -- Run a shell in a temporary builder container.
init-build   -- Initialize a Docker-based compilation of the TeXLive binaries.
run-build    -- Launch a Docker-based compilation of the TeXLive binaries.
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

docker_args=(
    -v "$topdir":/source:ro,z
    -v "$state_dir":/state:rw,z
)


# Commands

function build_image () {
    docker build -t $image_name:latest $thisdir/docker/
}


function builder_bash () {
    [ -d $state_dir/repo ] || die "no such directory $state_dir/repo"
    exec docker run -it --rm "${docker_args[@]}" $image_name bash "$@"
}


function init_build() {
    [ -d $state_dir/repo ] || die "no such directory $state_dir/repo"
    [ ! -d $state_dir/rbuild ] || die "directory $state_dir/rbuild may not exist before starting build"
    exec docker run -it --rm "${docker_args[@]}" $image_name init-build
}


function run_build() {
    [ -d $state_dir/repo ] || die "no such directory $state_dir/repo"
    [ -d $state_dir/rbuild ] || die "no such directory $state_dir/rbuild"
    echo "Building with logs to $state_dir/rbuild.log ..."
    docker run -it --rm "${docker_args[@]}" $image_name bash -c 'cd /state/rbuild && make' \
        &> $state_dir/rbuild.log \
        || die "build exited with an error code! consult the log file"
}


# Dispatch subcommands.

case "$command" in
    build-image)
        build_image "$@" ;;
    builder-bash)
        builder_bash "$@" ;;
    init-build)
        init_build "$@" ;;
    run-build)
        run_build "$@" ;;
    *)
        die "unrecognized command \"$command\"" ;;
esac
