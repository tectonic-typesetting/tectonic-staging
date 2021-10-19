# Copyright 2021 the Tectonic Project.
# Licensed under the MIT License.

# This script is run inside the Docker container. Keep the `indent` settings
# consistent with those used in `stage0/driver.sh`.

set -euo pipefail

cd /state/outputs/stage99

# See the comment in this script for a description of why it's (currently) needed
python3 /source/stage99/hack-comments.py "$@"

while [ $# -gt 0 ] ; do
  fn="$1"
  shift

  indent -linux -nut -i4 -l120 $fn
  rm -f $fn~
done
