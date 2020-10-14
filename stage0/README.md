# Stage 0: extracting the source

This stage builds TeXLive using TeXLive's own build system inside a Docker
container, and then copies source files into the local state tree so that we can
start working towards our own augmented build.

The output source files are totally pristine and unmodified from what we get
from the TeXLive build.