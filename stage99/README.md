# Stage "99": copy out reference sources

Now that all of the source munging has been done, this final stage copies the
source files into the repository tree.

Once you have gotten to this stage and successfully updated the reference
sources, you should commit the updates in blocks using `git add -p` generously,
to make it easier to review the diffs for application to the actual Tectonic
source tree. See, for example, the series of commits in `git log
baf27b6..af66811`. The different major program components are separated, and the
"translation-related" changes (associated with changing section numbers and
whatnot) are separated out as much as possible.

These updates should log the original and new Subversion revision numbers and
TeXLive version, and eventually update the "Current Status" section of the
toplevel README.
