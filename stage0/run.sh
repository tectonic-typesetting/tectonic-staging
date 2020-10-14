#! /bin/bash
# Copyright 2020 the Tectonic Project.
# Licensed under the MIT License.

stage0dir="$(dirname $0)"

set -xeuo pipefail

$stage0dir/driver.sh build-image
$stage0dir/driver.sh init-build
$stage0dir/driver.sh run-build
$stage0dir/copy-source.sh
