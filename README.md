# Tectonic Reference-Sources Staging

This repository contains scripts for generating the “reference sources”, the
C/C++ code files that formed the basis of
[Tectonic](https://tectonic-typesetting.github.io). These files are derived from
[Norbert Preining’s Git mirror](http://git.texlive.info/texlive/) of [the
TeXLive Subversion repository](http://tug.org/svn/texlive/).

*You do not need this repository to build Tectonic.* You only need these scripts
if you want to keep tabs on the changes going into the XeTeX engine.


## Current status

On the `master` branch, the reference sources currently correspond to

#### TeXLive 2020.0 = SVN r54631


## Background

Tectonic is forked from the [XeTeX] TeX engine. However, the build process for
XeTeX (and all other mainstream TeX engines) is *extremely* baroque:

[XeTeX]: https://www.tug.org/xetex/

1. The original Knuth sources are written in a language called WEB (which is
   basically used by nobody else)
1. Those sources are then patched with a series of “change files” due to
   restrictions on distributing modified sources
1. These patched files are then converted to the Pascal language using some
   custom processing scripts
1. That Pascal code is then converted to C code using additional custom tools
1. Those C files are then compiled against a set of extension files and
   libraries written natively in C and C++

One of the big motivators for the launch of the Tectonic project was to break
out of this ridiculously unwelcoming development process. Tectonic is based on
the C/C++ files that emerge from the XeTeX build process, and is gradually
translating that code to modern Rust.

While the core TeX architecture and XeTeX are largely stable, they do, however,
evolve with time. This repository recreates the final XeTeX C/C++ files from
their source, so that modifications can be ported into the main Tectonic
codebase.


## Repository structure

There is a fundamental challenge to this translation process. We want to
automate it as much as possible, starting from pristine sources from the TeXLive
Subversion repository. But in order to do so, we must reproduce TeXLive’s
baroque build process, and in fact we want to further patch its build tools to
include extra diagnostic information in the final build outputs. This involves a
*lot* of patching of a bunch of input files.

So, the issue is that we want to maintain a fairly complex suite of patches to a
large set of input files that are themselves changing. That demands a fairly
powerful source-control tool.

Well, we've got one: git! Git's tools for merging branches provide exactly the
sort of patch-management functionality needed to maintain our derived sources
over time.

The particular structure adopted here is:

- There is a branch named `stage0` that contains scripts in a subdirectory also
  named `stage0`. The script `stage0/run.sh` performs an initial source
  extraction and copies those files into a “state” directory (see below).
- There is another branch named `stage1` that adds follow-on scripts into an
  additional subdirectory named, yes, `stage1`, with a `stage1/run.sh` script.
  This script performs the next level of processing.
- The same pattern continues on up until `stage4`.
- This `master` branch contains a `stage99` pseudo-stage that copies the final
  derived files out of the final state directory into the repo.

If you're not trying to update the sources, you don’t need to worry about these
other branches.


## Updating the sources

If you want to update the reference sources, you need the following prerequisites:

- An installation of [Docker](https://www.docker.com/).
- A checkout of the Preining TeXLive Git repository
  (`git://git.texlive.info/texlive.git`), placed or symlinked in a subdirectory
  named `state/repo` below this file. Be aware that this repository currently
  weighs in at **40 gigabytes**!

Data files associated with the staging process will land in other subdirectories
of `state/`. Create this directory, or symlink it to another hard drive with a
lot of space.

Once these are set up, the update process goes as follows:

1. Check out this repository.
1. Create and/or enter the `state/repo` directory and check out your desired
   version of the TeXLive sources. This should probably be a newer version than
   is currently extracted into `master`.
1. Switch your checkout of this repo to the `stage0` branch.
1. Run `stage0/run.sh`.
1. Commit any changes that were necessary to fix the build, or updates to
   “extracted” files reflecting the new code published in TeXLive.
1. Check out the `stage1` branch.
1. Merge in `stage0`, making sure to preserve:
   - All of the Tectonic enhancements added in the previous stage, and
   - Any modifications stemming from TeXLive updates
1. Run `stage1/run.sh`.
1. Commit changes, check out the next stage, merge, and rerun, up until the
   last separate stage (currently `stage4`)
1. Check out `master`.
1. Merge in the final stage branch.
1. Run `stage99/run.sh` to extract the final reference sources to the repository.
1. Update this file with the new SVN revision and TeXLive version.
1. Commit.

The diffs to the reference sources can then be used to inform updates to [the
main Tectonic repository][tt-main].

[tt-main]: https://github.com/tectonic-typesetting/tectonic


### Copyright and Licensing

The source files extracted from TeXLive have copyrights and licenses specified
in their contents. They are available under a variety of open-source licenses.

The infrastructure scripts in this repository are licensed under the MIT
License. Their copyright is assigned to the Tectonic Project.
