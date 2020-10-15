#! /bin/bash
# Copyright 2020 the Tectonic Project.
# Licensed under the MIT License.

set -xeuo pipefail

stage3dir="$(dirname $0)"
$stage3dir/driver.sh copy-source
$stage3dir/driver.sh setup-build
$stage3dir/driver.sh run-build
$stage3dir/../stage0/driver.sh builder-bash -c "chown -R $(id -u):$(id -g) /state/outputs/stage3"
$stage3dir/driver.sh build-book
$stage3dir/driver.sh update-exports
