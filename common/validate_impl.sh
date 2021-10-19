#! /bin/bash
# Copyright 2021 the Tectonic Project.
# Licensed under the MIT License.

# This script runs through the reference source generation process from the
# absolute beginning, to validate that this repo is self-consistent and
# reproducible.
#
# This script originates in the `stage0` branch, so if you modify it, you should
# do so there, and then merge your way all the way up to `master.`

# See `validate.sh` in the root -- we copy this script out of the repo to make
# sure that Git manipulations don't mess with it, so `dirname $0` isn't what we
# want here:
topdir="$1"

function die () {
    echo >&2 "fatal error:" "$@"
    exit 1
}

# trailing slash-dot makes the check work when `state` is a symlink:
if [ ! -d "$topdir/state/." ] ; then
    die "you must create or symlink a \`state\` directory in \`$topdir\`"
fi

state_dir="$topdir/state"

function ensure() {
    "$@" || die "command failed: $@"
}

function check_dirty () {
    git update-index -q --refresh || die "could not refresh git index??"
    git diff-index --quiet --cached HEAD -- || die "repository should not be dirty"
    git diff-files --quiet || die "repository should not be dirty"
}

function main() {
    echo "Checking staging repo cleanliness ..."
    check_dirty

    orig_branch="$(git symbolic-ref --short HEAD)" || die "don't run this script in detached-HEAD state"

    echo "Checking source repo cleanliness ..."
    (cd $state_dir/repo && check_dirty)

    # If possible, check that the repo is checked out to the right commit. This
    # information isn't tracked in early stages, though, so it might be missing
    # if we're only aiming for partial validation.
    revtext="$(grep "^####" "$topdir/README.md" 2>/dev/null)"
    if [ $? -ne 0 ] ; then
        echo "No info available on intended source repo commit, so not checking it"
    else
        revtext="$(echo "$revtext" |sed -e 's/.* r//')"
        echo "Checking that source repo is on SVN revision $revtext ..."

        pushd $state_dir/repo >/dev/null
        git show |grep -q "git-svn-id.*@$revtext" || die "it is not"
        popd >/dev/null
    fi

    echo "Cleaning staging output trees and validation logs ..."
    rm -rf $state_dir/outputs $state_dir/validation.*.log

    echo "Cleaning stage0 build tree ..."
    rm -rf $state_dir/rbuild $state_dir/rbuild.log

    stages="$(echo stage*)"  # if we ever have more than 10 stages, we'll need to sort smarter ...

    for stage in $stages ; do
        echo
        echo "Stage ${stage}:"

        if [[ $stage == stage99 ]] ; then
            # If dev work is being done, we might want to end up on a dev branch, not "master"
            branch="$orig_branch"
        else
            branch="$stage"
        fi

        echo "  Switching to branch $branch"
        ensure git switch -q "$branch"

        logfile="$state_dir/validate.$stage.log"
        echo "  Running stage with logs to $logfile ..."
        ensure ./$stage/run.sh >"$logfile" 2>&1

        echo "  Checking end-of-stage repo cleanliness"
        check_dirty
    done

    echo
    echo "Success! Everything is self-consistent."
    exit 0
}

main
