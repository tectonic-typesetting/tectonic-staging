Tectonic Staging
================

This repository contains scripts for extracting files needed for Tectonic from
a Git mirror of
[the TeXLive Subversion repository](http://tug.org/svn/texlive/). These files
are the source code used as the basis for the Tectonic engine, and the
compressed “container” files with different TeXLive packages that can be used
to generate a Tectonic bundle file.

*You do not need this repository to build Tectonic.*

Requirements are:

- Docker.
- [ninja](https://ninja-build.org/).
- C and C++ compilers.
- A Git mirror of the TeXLive Subversion repository.

There is no official Git mirror of the TeXLive Subversion repository. My
version is generated with `git-svn` and is 21 gigabytes. Creating your own Git
mirror is probably pretty hard on the TeXLive servers, so please avoid doing
so. If you want to use the scripts in this repository, contact me (Peter
Williams <peter@newton.cx>) and I’ll provide you with my repository to use as
a basis.

The Git mirror should live in a subdirectory named `state/repo` below this
file. Data files associated with the staging process will land in other
subdirectories of `state`.

*This* repository has two main branches. The `vendor` branch has the raw
source code extracted from TeXLive. The `master` branch has modifications to
that code to get a minimal running `xetex` engine generated from somewhat more
user-friendly C code than is generated in the TeXLive build system.


Updating the mirror
-------------------

Use `./staging.sh svn-pull` to synchronize with the official repository.


Updating the container files
----------------------------

The first time you do anything, run `./staging.sh build-image` to make a
standardized Docker image for running the TeXLive scripts and build process.
Then run `./staging.sh update-containers` to generate versioned container
files that will land in `state/containers`.


Making a bundle file
--------------------

Run `./staging.sh make-zipfile $DESTPATH $PACKAGENAMES` where `$DESTPATH` is
the name of the Zip file you want to produce and `$PACKAGENAMES` is a list of
the TeXLive packages that should be installed to produce the bundle file.


Updating the reference source code
----------------------------------

1. **Make sure you’re on the vendor branch!** This is essential.
2. Build TeXLive in a directory named `state/rbuild`. **TODO**: this should
   totally be done in the Docker container.
3. Run `./staging.sh ingest-source`.
4. If no files were modified (`git status`), there’s nothing more to do.
5. If files were modified, commit the resulting changes. On the current
   `vendor` branch.
6. Check out the `master` branch.
7. Merge in the changes from `vendor`.
8. Run `ninja` to build the sample `xetex` binary.


Copyright and Licensing
=======================

The source files extracted from TeXLive have copyrights and licenses specified
in their contents. They are available under a variety of open-source licenses.

The infrastructure scripts in this repository are licensed under the MIT
License. Their copyright is assigned to the Tectonic Project.
