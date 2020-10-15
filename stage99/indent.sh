# Copyright 2020 the Tectonic Project.
# Licensed under the MIT License.

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
