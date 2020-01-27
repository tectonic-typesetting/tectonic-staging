# Tectonic Staging

This repository contains scripts for extracting files needed for
[Tectonic](https://tectonic-typesetting.github.io) from
[Norbert Preining’s Git mirror](http://git.texlive.info/texlive/) of
[the TeXLive Subversion repository](http://tug.org/svn/texlive/). The
extracted files are essential to two aspects of Tectonic:

1. The many TeX files tracked in the TeXLive repository are assembled into
   Tectonic “bundle” files.
2. The TeXLive repository also contains the source code used as the basis for
   the Tectonic engine, and we track the changes to that code to absorb
   bugfixes and maintain compatibility with the upstream projects as best we
   can.

*You do not need this repository to build Tectonic.* You only need these scripts
if you want to make your own bundle of TeX files, or you want to keep tabs on
the changes going into the XeTeX engine.

<!-- NOTE: edit this file on the `vendor` branch, and merge changes into `master`! -->


## Getting started: creating the builder image

If you *do* want to do one of the above-mentioned tasks, you will need at a
minimum:

- An installation of [Docker](https://www.docker.com/).
- A checkout of the Preining TeXLive Git repository
  (`git://git.texlive.info/texlive.git`), placed or symlinked in a
  subdirectory named `state/repo` below this file. Be aware that this
  repository currently weighs in at **40 gigabytes**!

Data files associated with the staging process will land in other
subdirectories of `state/`.

Regardless of what your end goal is, the first step is to create a Docker
container that will host most of the computations — this promotes
reproducibility and portability, regardless of what kind of system you are
using. To create this container, run:

```
./staging.sh build-image
```


## Creating your own bundle

To make your own bundle, beyond Docker and the Git checkout, you also need:

- A Python 2 interpreter. (FIXME: we should make the scripts compatible with
  Python 2 and Python 3.)

The first step to creating your own bundle is to create TeXLive “containers” —
which are different than Docker containers. A Docker container is an
encapsulated Linux machine that provides a reproducible build environment.
TeXLive containers are archives containing the files associated with the
various TeXLive packages.

To create TeXLive container files for all of the packages associated with your
TeXLive checkout, run:

```
./staging.sh update-containers
```

This will use a Docker builder container to generate TeXLive container files
in `state/containers`. The script will furthermore copy those files to
`state/versioned`, altering the names to record the exact version of each
package. *Note that the results of this step will depend on what version of
the TeXLive tree you currently have checked out in `state/repo`.*

Once you have created your TeXLive containers, the script
`builder/make-zipfile.py` can compile them into a single master Zip file. The
operation `./staging.sh make-base-zipfile $DESTPATH` will do this for the
standard Tectonic base bundle, `tlextras`. It does so using the helper
`./staging.sh make-installation`.

For bundles to be hosted on the web, the operation `./staging.sh zip2iter`
will convert the resulting Zip file to the “indexed tar” format used for
Web-based bundles. **TODO**: this is not adequately documented at all.


## Updating the reference source code

To update the reference source you will also need:

- A Python 2 interpreter. (FIXME: we should make the scripts compatible with
  Python 2 and Python 3.)
- [ninja](https://ninja-build.org/).
- C and C++ compilers.
- [indent](https://www.gnu.org/software/indent/manual/indent.html).

More preliminaries: this repository has two main branches. The `vendor` branch
has the raw source code extracted from TeXLive. The `master` branch has
modifications to that code to get a minimal running `xetex` engine, which
generated from somewhat more user-friendly C code than is generated in the
TeXLive build system.

To update the Tectonic reference source code, you need to compile some parts
of TeXLive using the builder container.

1. **Make sure you’re on the `vendor` branch of this repository!** This is
   essential.
2. If you have not already done so, run `./staging.sh init-build`. This will
   initialize the TeXLive compilation to be run inside a Docker container.
3. Run `./staging.sh run-build` to build the needed files.
4. Run `./staging.sh ingest-source` to ingest the source files into this
   repository.
5. If no files were modified (`git status`), there’s nothing more to do.
6. If files were modified, commit the resulting changes. On the current
   `vendor` branch.
7. Check out the `master` branch.
8. Merge in the changes from `vendor`, using a commit message like “Update
   master to TeXLive NNNNN“, where NNNNN is the relevant TeXLive Subversion
   revision number.
9. If you have not already done so, run `./gen-ninja.py` to generate the Ninja
   build specification file.
10. Run `ninja` to verify that the sample `xetex` binary builds.
11. Run `./staging.sh update-products` to update the prettified C code in
    `products/`.

At this point, automation has run its course. You must read and understand the
changes to the files in `products/`, and decide how best to manually import
them into the main Tectonic codebase.

**TODO**: be super explicit about where in the main codebase we write down
what version of the reference source we are derived from, and tell people to
update that when importing changes.

Note: there is a separate standalone XeTeX repository
[on SourceForge](https://sourceforge.net/p/xetex/code/ci/master/tree/)
(`git://git.code.sf.net/p/xetex/code`). As of January 2018, it appears that
the version maintained in the TeXLive tree is at least as new and possibly
better-maintained. So, that is what we use as our reference. It's not fully
clear to me (PKGW) how the two forks interact.


# Copyright and Licensing

The source files extracted from TeXLive have copyrights and licenses specified
in their contents. They are available under a variety of open-source licenses.

The infrastructure scripts in this repository are licensed under the MIT
License. Their copyright is assigned to the Tectonic Project.
