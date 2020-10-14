#! /bin/bash
# Copyright 2020 the Tectonic Project.
# Licensed under the MIT License.

set -xeuo pipefail

stage1dir="$(dirname $0)"
$stage1dir/driver.sh copy-source
$stage1dir/driver.sh setup-build
$stage1dir/driver.sh run-build
