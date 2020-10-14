# Stage 1: Minimally patched local build

This stage tries to locally build XeTeX and related tools, independently of the
TeXLive build system. The build still happens inside the Docker container, but
we use our own Ninja-based build system.
