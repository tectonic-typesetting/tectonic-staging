#! /bin/bash
# Copyright 2020 the Tectonic Project.
# Licensed under the MIT License.

set -xeuo pipefail

stage4dir="$(dirname $0)"
$stage4dir/driver.sh copy-source
$stage4dir/driver.sh setup-build
$stage4dir/driver.sh run-build
$stage4dir/../stage0/driver.sh builder-bash -c "chown -R $(id -u):$(id -g) /state/outputs/stage4"
