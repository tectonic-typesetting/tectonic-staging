# Copyright 2016-2017 the Tectonic Project.
# Licensed under the MIT License.
#
# See ../staging.sh for a simple build recipe.

FROM debian:9
MAINTAINER Peter Williams <peter@newton.cx>

VOLUME /state/
COPY setup.sh /
RUN ["/bin/bash", "/setup.sh"]
# copy entrypoint separately to speed rebuilds
COPY entrypoint.sh /
RUN ["/bin/chmod", "+x", "/entrypoint.sh"]
ENTRYPOINT ["/entrypoint.sh"]
CMD ["help"]
