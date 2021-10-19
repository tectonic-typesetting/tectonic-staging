#! /bin/bash
# Copyright 2021 the Tectonic Project.
# Licensed under the MIT License.

# A wrapper for the script that runs through the reference source generation
# process from the absolute beginning, to validate that this repo is
# self-consistent and reproducible.
#
# Because we have to check out various Git branches as the script runs, and bash
# can get messed up if the file underlying a script changes underneath it, we
# use this simple wrapper to make sure that the script we're running is not
# being affected by our Git manipulations.
#
# This script originates in the `stage0` branch, so if you modify it, you should
# do so there, and then merge your way all the way up to `master.`

set -e
topdir="$(dirname $0)"
clone=$(mktemp)
cp $topdir/common/validate_impl.sh "$clone"
chmod +x "$clone"
exec "$clone" "$topdir"
