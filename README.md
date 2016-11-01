standalone xetex
================

Research progress investigating the feasibility of building a standalone TeX
engine that doesn't have the complex filesystem crawling characteristics of
the standard ones.


Current status
--------------

Can compile a `xetex` that only uses some system libraries. To run:

```
TEXFORMATS=/a/share/texlive/texmf-var// ./BUILD/xetex
```

To compile the minimal sample LaTeX document in `tests/latex-minimal`:

```
cd tests/latex-minimal
export TEXINPUTS=.:/a/share/texlive/texmf-dist//
export TEXFORMATS=/a/share/texlive/texmf-var//
../../BUILD/xetex -fmt=xelatex -no-pdf main
xdvipdfmx main
```

The more full “xenia” test:

```
cd tests/latex-minimal
export TEXINPUTS=.:/a/share/texlive/texmf-dist//
export TEXFORMATS=/a/share/texlive/texmf-var//
export TFMFONTS=/a/share/texlive/texmf-dist//
../../BUILD/xetex -fmt=xelatex -no-pdf paper
../../BUILD/xetex -fmt=xelatex -no-pdf paper # rerun for crossrefs
xdvipdfmx paper
```


Origin of source code
---------------------

The script `fetch-source.sh` copies source code out of a *partially-built*
TeXLive tree. We need to do the build to generate the C source code from the
original Web source, since there's no way I'm reimplementing all of that
stuff. (For now.)

Fetch source into the `vendor` branch. Ideally that branch will be a
buildable, totally stock version of xetex with some bad settings ... but we'll
see how long that lasts. The `master` branch hacks things to move towards the
standalone paradigm.
