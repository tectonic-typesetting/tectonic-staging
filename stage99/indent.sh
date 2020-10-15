# Copyright 2020 the Tectonic Project.
# Licensed under the MIT License.

set -euo pipefail

cd /state/outputs/stage99

while [ $# -gt 0 ] ; do
  fn="$1"
  shift

  indent -linux -nut -i4 -l120 $fn
  rm -f $fn~
done
