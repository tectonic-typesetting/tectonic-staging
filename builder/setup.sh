#! /bin/bash
# Copyright 2016 Peter Williams <peter@newton.cx>
# Licensed under the MIT License.
#
# Set up an image that's ready to generate TeXLive packages reproducibly.

set -e -x

apt-get update
DEBIAN_FRONTEND=noninteractive apt-get install -yq --no-install-recommends \
	       libfontconfig1 \
	       git-core \
	       libdigest-perl-md5-perl \
	       wget
rm -rf /var/lib/apt/lists/*

rm /setup.sh # self-destruct!
