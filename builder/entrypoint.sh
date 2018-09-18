#! /bin/bash
# Copyright 2016 the Tectonic Project.
# Licensed under the MIT License.
#
# This is the "entrypoint" script for the TeXLive build image. When a user
# runs "docker run texlive-builder foo bar", we are invoked with $1=foo and
# $2=bar.

set -e

if [ -z "$1" -o "$1" = help ] ; then
    echo "You must supply a subcommand to run in the container. Commands are:

bash              -- Run a bash shell
init-build        -- Initialize a compilation of the TeXLive sources.
install-profile   -- Create an installation based on a fixed TeXLive \"profile\" file.
update-containers -- Rebuild the TeXLive \"container\" files

E.g., use 'docker run -it buildercontainer bash' to launch an interactive
shell within a long-running container.
"
    exit 1
fi

command="$1"
shift


function _precise_version () {
    # Log the precise version of the TeXLive tree that we are using. This is
    # done by making files named GITHASH and SVNREV in a directory named in
    # $1. We exit with an error if the tree is not pristine. This must be run
    # from a directory inside the Git repository.

    destdir="$1"

    if ! git update-index -q --refresh ; then
	echo >&2 "error: git update-index failed"
	exit 1
    fi

    if [ -n "$(git diff-index --name-only HEAD --)" ] ; then
	echo >&2 "error: refusing to make packages from dirty working tree"
	exit 1
    fi

    git show-ref --head |head -n1 |awk '{print $1}' >"$destdir/GITHASH"
    git show -s |grep git-svn-id |sed -e 's/.*@//' -e 's/ .*//' >"$destdir/SVNREV"
}


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

function install_profile () {
    profile="$1"
    shift
    install_dest="$1"
    shift
    chown_spec="$1"
    shift

    cd /state/repo/
    _precise_version "$install_dest"
    Master/install-tl --repository /state/containers --profile "$profile"

    if [ $# -gt 0 ] ; then
	cd "$install_dest"
	# NOTE: leading "./" is essential!
	./bin/*/tlmgr --repository /state/containers install "$@"
    fi

    chown -R "$chown_spec" "$install_dest"
}


function update_containers () {
    cd /state/repo/
    _precise_version /state/containers

    args=(--all --gpgcmd=true --location /state/containers)
    if [ ! -e /state/containers/tlpkg/texlive.tlpdb ] ; then
	args+=(--recreate)
    fi

    exec Master/tlpkg/bin/tl-update-containers "${args[@]}"
}


# Dispatch subcommands.

if [ "$command" = bash ] ; then
    exec bash "$@"
elif [ "$command" = init-build ] ; then
    init_build "$@"
elif [ "$command" = install-profile ] ; then
    install_profile "$@"
elif [ "$command" = update-containers ] ; then
    update_containers "$@"
else
    echo "Unrecognized command \"$command\"."
    exit 1
fi
