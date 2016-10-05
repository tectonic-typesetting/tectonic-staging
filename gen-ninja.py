#! /usr/bin/env python
# -*- mode: python; coding: utf-8 -*-
# Copyright 2016 Peter Williams and collaborators.
# Licensed under the MIT License.

from __future__ import absolute_import, division, print_function, unicode_literals

import os, sys
from pwkit import io, ninja_syntax


def inner (top, w):
    # build.ninja gen rule.

    w.comment ('Automatically generated.')

    w.rule ('regen',
            command='./gen-ninja.py $in',
            description='GEN $out',
            generator=True)
    w.build ('build.ninja', 'regen', implicit='gen-ninja.py')

    # Base rules

    w.rule ('ensuredir',
            command='mkdir -p $out',
            description='MKDIR $out')

    w.rule ('cc',
            command='gcc -c -o $out $cflags $in',
            description='CC $out')

    w.rule ('cxx',
            command='g++ -c -o $out $cflags $in',
            description='CC $out')

    w.rule ('staticlib',
            command='ar cru $out $in',
            description='AR $out')

    # build dir

    builddir = top / 'BUILD'
    w.build (str(builddir), 'ensuredir')

    # kpathsea

    libkp = builddir / 'libkpathsea.a'
    cflags = '-DMAKE_KPSE_DLL -Ikpathsea -I. -g -O2'
    objs = []

    for src in (top / 'kpathsea').glob ('*.c'):
        obj = builddir / ('kpathsea_' + src.name.replace ('.c', '.o'))
        w.build (
            str(obj), 'cc',
            inputs = [str(src)],
            order_only = [str(builddir)],
            variables = {'cflags': cflags},
        )
        objs.append (str (obj))

    w.build (str(libkp), 'staticlib', inputs = objs)

    # teckit

    libtk = builddir / 'libteckit.a'
    cflags = '-Iteckit -DNDEBUG -g -O2'
    objs = []

    for src in (top / 'teckit').glob ('*.cpp'):
        obj = builddir / ('teckit_' + src.name.replace ('.cpp', '.o'))
        w.build (
            str(obj), 'cxx',
            inputs = [str(src)],
            order_only = [str(builddir)],
            variables = {'cflags': cflags},
        )
        objs.append (str (obj))

    w.build (str(libtk), 'staticlib', inputs = objs)


def outer (args):
    top = io.Path ('')
    me = io.Path (sys.argv[0]).parent

    with (me / 'build.ninja').open ('wt') as f:
        w = ninja_syntax.Writer (f)
        inner (top, w)


if __name__ == '__main__':
    import sys
    outer (sys.argv[1:])
