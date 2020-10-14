# Copyright 2016-2020 the Tectonic Project.
# Licensed under the MIT License.
#
# See ../staging.sh for a simple build recipe.

FROM ubuntu:20.04

VOLUME /source/
VOLUME /state/

COPY setup.sh /
RUN ["/bin/bash", "/setup.sh"]
# copy entrypoint separately to speed rebuilds
COPY entrypoint.sh /
RUN ["/bin/chmod", "+x", "/entrypoint.sh"]
ENTRYPOINT ["/entrypoint.sh"]
CMD ["help"]
