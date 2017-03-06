#! /bin/bash
# Copyright 2016-2017 the Tectonic Project.
# Licensed under the MIT License.
#
# Set up an image that's ready to generate TeXLive packages reproducibly and
# compile the binaries under TeXLive's build system.

set -e -x

apt-get update

texlive_run_pkgs="
libdigest-perl-md5-perl
libfontconfig1
git-core
wget
"

texlive_build_pkgs="
bison
build-essential
flex
libcairo2-dev
libfreetype6-dev
libgd-dev
libgmp-dev
libgraphite2-dev
libharfbuzz-dev
libicu-dev
libmpfr-dev
libpaper-dev
libpixman-1-dev
libpng-dev
libpoppler-dev
libpoppler-private-dev
libpotrace-dev
libzzip-dev
texinfo
"

DEBIAN_FRONTEND=noninteractive apt-get install -yq --no-install-recommends \
               $texlive_run_pkgs $texlive_build_pkgs
rm -rf /var/lib/apt/lists/*

rm /setup.sh # self-destruct!
