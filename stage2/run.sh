#! /bin/bash
# Copyright 2020 the Tectonic Project.
# Licensed under the MIT License.

set -xeuo pipefail

stage2dir="$(dirname $0)"
$stage2dir/driver.sh copy-source
$stage2dir/driver.sh setup-build
$stage2dir/driver.sh run-build
$stage2dir/../stage0/driver.sh builder-bash -c "chown -R $(id -u):$(id -g) /state/outputs/stage2"
