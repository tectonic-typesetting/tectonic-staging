#! /bin/bash
# Copyright 2016-2020 the Tectonic Project.
# Licensed under the MIT License.
#
# Set up an image that's ready to build the binaries under TeXLive's build
# system.

deps="
bison
build-essential
file
flex
git-core
indent
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
python3
texinfo
wget
"

set -ex
apt-get update
DEBIAN_FRONTEND=noninteractive apt-get install -yq --no-install-recommends $deps
rm -rf /var/lib/apt/lists/*
rm /setup.sh  # self-destruct!
