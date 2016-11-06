#! /usr/bin/env python
# -*- mode: python; coding: utf-8 -*-
# Copyright 2016 Peter Williams and collaborators.
# Licensed under the MIT License.

from __future__ import absolute_import, division, print_function, unicode_literals

import os, sys
from pwkit import io, ninja_syntax


config = {
    'build_name': 'BUILD',
    'base_cflags': '-g -O0',
    # pkg-config --cflags fontconfig harfbuzz harfbuzz-icu freetype2 graphite2 libpng zlib icu-uc poppler
    'pkgconfig_cflags': '-I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/poppler',
    'pkgconfig_libs': '-lfontconfig -lharfbuzz-icu -lharfbuzz -lfreetype -lgraphite2 -lpng16 -lz -licuuc -licudata -lpoppler',
}


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
            command='mkdir -p $out/web2c', # hack for web2c script stuff
            description='MKDIR $out')

    w.rule ('cc',
            command='gcc -c -o $out -MT $out -MD -MP -MF $out.d $cflags $in',
            deps='gcc',
            depfile='$out.d',
            description='CC $out')

    w.rule ('cxx',
            command='g++ -c -o $out -MT $out -MD -MP -MF $out.d $cflags $in',
            deps='gcc',
            depfile='$out.d',
            description='CXX $out')

    w.rule ('staticlib',
            command='ar cru $out $in',
            description='AR $out')

    w.rule ('executable',
            command='g++ -o $out $in $libs',
            description='LINK $out')

    w.rule ('tie',
            command='WEBINPUTS=. BUILD/tie -c $out $in', # TODO: config
            description='TIE $out')

    w.rule ('otangle',
            command='WEBINPUTS=. BUILD/otangle $in && mv $basename.p $basename.pool $outdir', # TODO: config
            description='OTANGLE $out')

    w.rule ('convert',
            command='$convert . $outdir $basename && mv $basename*.c $basename*.h $outdir',
            description='CONVERT $out')

    w.rule ('makecpool',
            command='BUILD/web2c/makecpool $basename >$out', # TODO: config
            description='MAKECPOOL $out')

    # build dir

    builddir = top / config['build_name']
    w2cbdir = builddir / 'web2c'
    w.build (str(builddir), 'ensuredir')

    # utility.

    def compile (sources=None, bldprefix=None, rule=None, **kwargs):
        objs = []

        for src in sources:
            obj = builddir / (bldprefix + src.name.replace ('.c', '.o'))
            w.build (
                str(obj), rule,
                inputs = [str(src)],
                order_only = [str(builddir)],
                variables = kwargs,
            )
            objs.append (str (obj))

        return objs

    def staticlib (sources=None, basename=None, rule=None, order_only=[], **kwargs):
        lib = builddir / ('lib' + basename + '.a')
        objs = compile (
            sources = sources,
            bldprefix = basename + '_',
            rule = rule,
            **kwargs)
        w.build (str(lib), 'staticlib',
                 inputs = objs,
                 order_only = order_only,
        )
        return lib

    def executable (output=None, sources=None, rule=None, slibs=[], libs='', **kwargs):
        """slibs are locally-built static libraries. libs is passed to the linker
        command line.

        """
        objs = compile (
            sources = sources,
            bldprefix = output.name + '_',
            rule = rule,
            **kwargs)
        objs += map (str, slibs)
        w.build (str(output), 'executable',
                 inputs = objs,
                 variables = {'libs': libs})
        return str(output) # convenience

    # "tidy_kpathutil" -- C utilities extracted from tidied-up kpathsea

    libkpu = staticlib (
        basename = 'tidy_kpathutil',
        sources = (top / 'tidy_kpathutil').glob ('*.c'),
        rule = 'cc',
        cflags = '-I. %(base_cflags)s' % config

    )

    # (tidied) kpathsea

    libkps = staticlib (
        basename = 'tidy_kpathsea',
        sources = (top / 'tidy_kpathsea').glob ('*.c'),
        rule = 'cc',
        cflags = '-DHAVE_CONFIG_H -DMAKE_KPSE_DLL -Itidy_kpathsea -I. %(base_cflags)s' % config
    )

    # teckit

    libtk = staticlib (
        basename = 'teckit',
        sources = (top / 'teckit').glob ('*.cpp'),
        rule = 'cxx',
        cflags = '-DHAVE_CONFIG_H -Iteckit -DNDEBUG %(base_cflags)s' % config,
    )

    # libmd5

    libmd5 = staticlib (
        basename = 'md5',
        sources = (top / 'libmd5').glob ('*.c'),
        rule = 'cc',
        cflags = '-DHAVE_CONFIG_H -Ilibmd5 %(base_cflags)s' % config,
    )

    # lib / libbase

    def libbase_sources ():
        for src in (top / 'lib').glob ('*.c'):
            if src.name != 'texmfmp.c': # #included in xetexdir/xetexextra.c
                yield src

    libbase = staticlib (
        basename = 'base',
        sources = libbase_sources (),
        rule = 'cc',
        cflags = '-DHAVE_CONFIG_H -Ilib -I. %(base_cflags)s' % config
    )

    # tie

    tieprog = executable (
        output = builddir / 'tie',
        sources = (top / 'tiedir').glob ('*.c'),
        rule = 'cc',
        slibs = [libbase, libkps, libkpu],
        cflags = '-DHAVE_CONFIG_H -DNOT_WEB2C -I. -Ixetexdir %(base_cflags)s' % config,
    )

    # otangle

    otangleprog = executable (
        output = builddir / 'otangle',
        sources = (top / 'otangle').glob ('*.c'),
        rule = 'cc',
        slibs = [libbase, libkps, libkpu],
        cflags = '-I. -Ilib -Ixetexdir %(base_cflags)s' % config,
    )

    # web2c programs

    def web2c_c_sources ():
        for src in (top / 'web2c').glob ('web2c*.c'):
            yield src
        yield top / 'web2c' / 'main.c'

    web2cprog = executable (
        output = w2cbdir / 'web2c',
        sources = web2c_c_sources (),
        rule = 'cc',
        slibs = [libbase, libkps, libkpu],
        cflags = '-I. %(base_cflags)s' % config,
    )

    splitupprog = executable (
        output = w2cbdir / 'splitup',
        sources = (top / 'web2c').glob ('splitup*.c'),
        rule = 'cc',
        slibs = [libbase, libkps, libkpu],
        cflags = '-I. %(base_cflags)s' % config,
    )

    fixwritesprog = executable (
        output = w2cbdir / 'fixwrites',
        sources = (top / 'web2c').glob ('fixwrites*.c'),
        rule = 'cc',
        slibs = [libbase, libkps, libkpu],
        cflags = '-I. %(base_cflags)s' % config,
    )

    makecpoolprog = executable (
        output = w2cbdir / 'makecpool',
        sources = (top / 'web2c').glob ('makecpool*.c'),
        rule = 'cc',
        slibs = [libbase, libkps, libkpu],
        cflags = '-I. %(base_cflags)s' % config,
    )

    # "tie"d xetex.ch file. Not sure if the ordering of changefiles matters so
    # I'm being paranoid here and reproducing what the TeXLive build system
    # uses.

    xetex_ch = builddir / 'xetex.ch'

    w.build (str(xetex_ch), 'tie',
             inputs = map (str, [
                 top / 'xetexdir' / 'xetex.web',
                 top / 'xetexdir' / 'tex.ch0',
                 top / 'xetexdir' / 'tex.ch',
                 top / 'synctexdir' / 'synctex-xe-def.ch0',
                 top / 'synctexdir' / 'synctex-mem.ch0',
                 top / 'synctexdir' / 'synctex-e-mem.ch0',
                 top / 'synctexdir' / 'synctex-e-mem.ch1',
                 top / 'synctexdir' / 'synctex-rec.ch0',
                 top / 'synctexdir' / 'synctex-e-rec.ch0',
                 top / 'xetexdir' / 'xetex.ch',
                 top / 'synctexdir' / 'synctex-xe-rec.ch3',
                 top / 'xetexdir' / 'tex-binpool.ch',
             ]),
             order_only = [str(builddir), tieprog],
    )

    # "otangle"d Pascal source for XeTeX.

    xetex_p = builddir / 'xetex.p'
    xetex_pool = builddir / 'xetex.pool'

    w.build ([str(xetex_p), str(xetex_pool)], 'otangle',
             inputs = map (str, [
                 top / 'xetexdir' / 'xetex.web',
                 xetex_ch,
             ]),
             order_only = [str(builddir), otangleprog],
             variables = {
                 'basename': 'xetex',
                 'outdir': str(builddir),
             },
    )

    # "convert"ed Pascal code into C code

    xetex_c = [
        builddir / 'xetex0.c',
        builddir / 'xetexini.c',
        builddir / 'xetexcoerce.h',
        builddir / 'xetexd.h',
    ]
    convert = str(top / 'web2c' / 'local-convert.sh')

    w.build (map (str, xetex_c), 'convert',
             inputs = map (str, [xetex_p, xetex_pool]),
             order_only = [str(builddir), convert, web2cprog, splitupprog, fixwritesprog],
             variables = {
                 'outdir': str(builddir),
                 'convert': convert,
                 'basename': 'xetex',
             },
    )

    # C string cpool file

    xetex_cpool = builddir / 'xetex-pool.c'

    w.build (str (xetex_cpool), 'makecpool',
             inputs = map (str, [xetex_p, xetex_pool]),
             order_only = [str(builddir), makecpoolprog],
             variables = {
                 'outdir': str(builddir),
                 'basename': str(builddir / 'xetex'),
             },
    )

    # synctex

    libsynctex = staticlib (
        basename = 'synctex',
        sources = (top / 'synctexdir').glob ('*.c'),
        rule = 'cc',
        cflags = ('-DHAVE_CONFIG_H -Ixetexdir -I. -I%(build_name)s -DU_STATIC_IMPLEMENTATION '
                  '-D__SyncTeX__ -DSYNCTEX_ENGINE_H=\\"synctexdir/synctex-xetex.h\\" '
                  '%(pkgconfig_cflags)s %(base_cflags)s' % config),
        order_only = map (str, xetex_c),
    )

    # xetex

    cflags = '-DHAVE_CONFIG_H -D__SyncTeX__ -Ixetexdir -I%(build_name)s -I. -Ilibmd5 %(pkgconfig_cflags)s %(base_cflags)s' % config
    objs = []

    def xetex_c_sources ():
        for src in (top / 'xetexdir').glob ('*.c'):
            yield src
        for src in (top / 'xetexdir' / 'image').glob ('*.c'):
            yield src
        for src in xetex_c:
            if src.name.endswith ('.c'):
                yield src
        yield xetex_cpool

    for src in xetex_c_sources ():
        obj = builddir / ('xetex_' + src.name.replace ('.c', '.o'))
        w.build (
            str(obj), 'cc',
            inputs = [str(src)],
            order_only = [str(builddir)],
            variables = {'cflags': cflags},
        )
        objs.append (str (obj))

    for src in (top / 'xetexdir').glob ('*.cpp'):
        obj = builddir / ('xetex_' + src.name.replace ('.cpp', '.o'))
        w.build (
            str(obj), 'cxx',
            inputs = [str(src)],
            order_only = [str(builddir)],
            variables = {'cflags': cflags},
        )
        objs.append (str (obj))

    objs += map (str, [libsynctex, libbase, libmd5, libtk, libkps, libkpu])
    libs = '%(pkgconfig_libs)s -lz' % config

    w.build (str(builddir / 'xetex'), 'executable',
             inputs = objs,
             variables = {'libs': libs},
    )


def outer (args):
    top = io.Path ('')
    me = io.Path (sys.argv[0]).parent

    with (me / 'build.ninja').open ('wt') as f:
        w = ninja_syntax.Writer (f)
        inner (top, w)


if __name__ == '__main__':
    import sys
    outer (sys.argv[1:])
