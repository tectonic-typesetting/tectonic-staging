# -*- mode: python; coding: utf-8 -*-
# Copyright 2016-2020 the Tectonic Project.
# Licensed under the MIT License.

import os.path
from pathlib import Path
import sys

sys.path.append(os.path.join(os.path.dirname(__file__), '../common'))
import ninja_syntax


config = {
    'base_cflags': '-g -O0',
    # pkg-config --cflags fontconfig harfbuzz harfbuzz-icu freetype2 graphite2 libpng zlib icu-uc
    'pkgconfig_cflags': '-I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/freetype2 -I/usr/include/libpng16',
    'pkgconfig_libs': '-lfontconfig -lharfbuzz-icu -lharfbuzz -lfreetype -lgraphite2 -lpng16 -lz -licuuc -licudata',
}


# stage2 patch: add use_custom_otangle option
def inner(src, build, w, use_custom_otangle, build_book_tex, underscores):
    config['src'] = str(src)

    # build.ninja gen rule.

    w.comment('Automatically generated.')

    custom_otangle_flag = ' --use-custom-otangle' if use_custom_otangle else ''
    book_tex_flag = ' --build-book-tex' if build_book_tex else ''
    underscores_flag = ' --underscores' if underscores else ''

    w.rule('regen',
            command=f'python3 $in{custom_otangle_flag}{book_tex_flag}{underscores_flag} {src}',
            description='GEN $out',
            generator=True)
    w.build('build.ninja', 'regen', inputs=[__file__])

    # Base rules

    w.rule('cc',
            command='gcc -c -o $out -MT $out -MD -MP -MF $out.d $cflags $in',
            deps='gcc',
            depfile='$out.d',
            description='CC $out')

    w.rule('cxx',
            command='g++ -c -o $out -MT $out -MD -MP -MF $out.d $cflags $in',
            deps='gcc',
            depfile='$out.d',
            description='CXX $out')

    w.rule('staticlib',
            command='ar cru $out $in',
            description='AR $out')

    w.rule('executable',
            command='g++ -o $out $in $libs',
            description='LINK $out')

    w.rule('lex',
            command='lex $in && mv lex.yy.c $out',
            description='FLEX $out')

    w.rule('yacc',
            command=('bison -y -d -v $in && mv y.tab.c $outbase.c && mv y.tab.h $outbase.h '
                     '&& rm -f y.output'),
            description='YACC $out')

    w.rule('makecpool',
            command='./web2c/makecpool $basename >$out',
            description='MAKECPOOL $out')

    w.rule('weave',
            command='WEBINPUTS=. ./weave $in',
            description='WEAVE $out')

    w2cbdir = build / 'web2c'

    # utility.

    def compile(sources=None, bldprefix=None, rule=None, implicit=[], **kwargs):
        objs = []

        for s in sources:
            obj = build / (bldprefix + s.name.replace('.c', '.o'))
            w.build(
                str(obj), rule,
                inputs = [str(s)],
                variables = kwargs,
                implicit = implicit,
            )
            objs.append(str(obj))

        return objs

    def staticlib(sources=None, basename=None, rule=None, order_only=[], implicit=[], **kwargs):
        lib = build / ('lib' + basename + '.a')

        objs = compile(
            sources = sources,
            bldprefix = basename + '_',
            rule = rule,
            implicit = implicit,
            **kwargs
        )

        w.build(str(lib), 'staticlib',
            inputs = objs,
            order_only = order_only,
            implicit = implicit,
        )

        return lib

    def executable(output=None, sources=None, rule=None, slibs=[], libs='', **kwargs):
        """slibs are locally-built static libraries. libs is passed to the linker
        command line.

        """
        objs = compile(
            sources = sources,
            bldprefix = output.name + '_',
            rule = rule,
            **kwargs)
        objs += [str(x) for x in slibs]
        w.build(str(output), 'executable',
                 inputs = objs,
                 variables = {'libs': libs})
        return str(output) # convenience

    # kpathsea

    libkp = staticlib (
        basename = 'kpathsea',
        sources = (src / 'kpathsea').glob ('*.c'),
        rule = 'cc',
        cflags = '-DHAVE_CONFIG_H -DMAKE_KPSE_DLL -I%(src)s/kpathsea -I%(src)s %(base_cflags)s' % config,
    )

    # teckit

    libtk = staticlib(
        basename = 'teckit',
        sources = (src / 'teckit').glob('*.cpp'),
        rule = 'cxx',
        cflags = '-DHAVE_CONFIG_H -I%(src)s/teckit -DNDEBUG %(base_cflags)s' % config,
    )

    # libmd5

    libmd5 = staticlib(
        basename = 'md5',
        sources = (src / 'libmd5').glob('*.c'),
        rule = 'cc',
        cflags = '-DHAVE_CONFIG_H -I%(src)s/libmd5 %(base_cflags)s' % config,
    )

    # pplib

    pplib = staticlib(
        basename = 'pplib',
        sources = (src / 'pplib').glob('**/*.c'),
        rule = 'cc',
        cflags = '-I%(src)s/pplib -I%(src)s/pplib/util %(base_cflags)s' % config,
    )

    # lib / libbase

    def libbase_sources():
        for s in (src / 'lib').glob('*.c'):
            if s.name != 'texmfmp.c': # #included in xetexdir/xetexextra.c
                yield s

    libbase = staticlib(
        basename = 'base',
        sources = libbase_sources(),
        rule = 'cc',
        cflags = '-DHAVE_CONFIG_H -I%(src)s/lib -I%(src)s %(base_cflags)s' % config
    )

    # tie

    tieprog = executable(
        output = build / 'tie',
        sources = (src / 'tiedir').glob('*.c'),
        rule = 'cc',
        slibs = [libbase, libkp],
        cflags = '-DHAVE_CONFIG_H -DNOT_WEB2C -I%(src)s -I%(src)s/xetexdir %(base_cflags)s' % config,
    )

    w.rule('tie',
            command='WEBINPUTS=. ./tie -c $out $in',
            description='TIE $out')

    def tie(output, inputs):
        w.build(str(output), 'tie',
            inputs = [str(x) for x in inputs],
            implicit = [tieprog],
        )

    # otangle

    if use_custom_otangle:
        otangle_c = Path('/source/stage2/tectonic-otangle.c')
    else:
        otangle_c = src / 'otangle' / 'otangle.c'

    if underscores:
        config['underscores_cflags'] = '-DTECTONIC_STAGE4_UNDERSCORES=1'
    else:
        config['underscores_cflags'] = ''

    otangleprog = executable(
        output = build / 'otangle',
        sources = [otangle_c],
        rule = 'cc',
        slibs = [libbase, libkp],
        cflags = '%(underscores_cflags)s -I%(src)s -I%(src)s/lib -I%(src)s/xetexdir %(base_cflags)s' % config,
    )

    w.rule('otangle',
            command='WEBINPUTS=. ./otangle $in',
            description='OTANGLE $out')

    def otangle(basename, inputs, pool=False):
        pascal = build / f'{basename}.p'

        if pool:
            outputs = [pascal, build / f'{basename}.pool']
        else:
            outputs = [pascal]

        w.build([str(x) for x in outputs], 'otangle',
                inputs = [str(x) for x in inputs],
                implicit = [otangleprog],
                variables = {
                    'basename': 'xetex',
                },
        )

        if pool:
            return outputs
        return pascal

    # web2c programs

    web2c_lexer = build / 'web2c-lexer.c'
    w.build(str(web2c_lexer), 'lex',
             inputs = [str(src / 'web2c' / 'web2c-lexer.l')])

    web2c_parser_c = build / 'web2c-parser.c'
    web2c_parser_h = build / 'web2c-parser.h'
    w.build([str(x) for x in [web2c_parser_c, web2c_parser_h]], 'yacc',
             inputs = [str(src / 'web2c' / 'web2c-parser.y')],
             variables = {
                 'outbase': str(build / 'web2c-parser'),
             }
    )

    web2cprog = executable(
        output = w2cbdir / 'web2c',
        sources = [
            src / 'web2c' / 'main.c',
            web2c_lexer,
            web2c_parser_c,
        ],
        implicit = [str(web2c_parser_h)],
        rule = 'cc',
        slibs = [libbase, libkp],
        cflags = '-I. -I%(src)s/web2c -I%(src)s %(base_cflags)s' % config,
    )

    splitupprog = executable(
        output = w2cbdir / 'splitup',
        sources = (src / 'web2c').glob('splitup*.c'),
        rule = 'cc',
        slibs = [libbase, libkp],
        cflags = '-I%(src)s %(base_cflags)s' % config,
    )

    fixwritesprog = executable(
        output = w2cbdir / 'fixwrites',
        sources = (src / 'web2c').glob('fixwrites*.c'),
        rule = 'cc',
        slibs = [libbase, libkp],
        cflags = '-I%(src)s %(base_cflags)s' % config,
    )

    makecpoolprog = executable(
        output = w2cbdir / 'makecpool',
        sources = (src / 'web2c').glob('makecpool*.c'),
        rule = 'cc',
        slibs = [libbase, libkp],
        cflags = '-I%(src)s %(base_cflags)s' % config,
    )

    # convert

    convertprog = str(src / 'web2c' / 'convert')

    w.rule('convert',
            command=f'rm -f $out && srcdir=$srcdir {convertprog} $basename',
            description='CONVERT $out')

    def convert(basename, inputs, full_outputs=False):
        if full_outputs:
            outputs = [
                build / f'{basename}0.c',
                build / f'{basename}ini.c',
                build / f'{basename}coerce.h',
                build / f'{basename}d.h',
            ]
        else:
            outputs = [
                build / f'{basename}.c',
                build / f'{basename}.h',
            ]

        w.build([str(x) for x in outputs], 'convert',
            inputs = [str(x) for x in inputs],
            implicit = [
                convertprog,
                web2cprog,
                splitupprog,
                fixwritesprog,
                str(src / 'web2c' / 'coerce.h'),
            ],
            variables = {
                'basename': basename,
                'srcdir': str(src),
            },
        )

        return outputs

    # "tie"d xetex.ch file. The ordering matters here.

    xetex_ch = build / 'xetex.ch'
    tie(xetex_ch, [
                 src / 'xetexdir' / 'xetex.web',
                 src / 'xetexdir' / 'tex.ch0',
                 src / 'xetexdir' / 'tex.ch',
                 src / 'xetexdir' / 'tracingstacklevels.ch',
                 # Next two are for TeX Live 2022.
                 # src / 'xetexdir' / 'partoken-102.ch',
                 # src / 'xetexdir' / 'partoken.ch',
                 src / 'synctexdir' / 'synctex-xe-def.ch0',
                 src / 'synctexdir' / 'synctex-mem.ch0',
                 src / 'synctexdir' / 'synctex-e-mem.ch0',
                 src / 'synctexdir' / 'synctex-e-mem.ch1',
                 src / 'synctexdir' / 'synctex-rec.ch0',
                 src / 'synctexdir' / 'synctex-e-rec.ch0',
                 src / 'xetexdir' / 'xetex.ch',
                 src / 'synctexdir' / 'synctex-xe-rec.ch3',
                 src / 'xetexdir' / 'char-warning-xetex.ch',
                 src / 'xetexdir' / 'tex-binpool.ch',
    ])

    # "otangle"d Pascal source for XeTeX, not-munged

    xetex_p, xetex_pool = otangle('xetex', [
        src / 'xetexdir' / 'xetex.web',
        xetex_ch,
    ], pool=True)

    # "convert"ed Pascal code into C code

    xetex_c = convert('xetex', [xetex_p, xetex_pool], full_outputs=True)

    # C string cpool file

    xetex_cpool = build / 'xetex-pool.c'

    w.build(str(xetex_cpool), 'makecpool',
             inputs = [str(x) for x in [xetex_p, xetex_pool]],
             implicit = [makecpoolprog],
             variables = {
                 'outdir': str(build),
                 'basename': str(build / 'xetex'),
             },
    )

    # synctex

    libsynctex = staticlib(
        basename = 'synctex',
        sources = (src / 'synctexdir').glob('*.c'),
        rule = 'cc',
        cflags = ('-DHAVE_CONFIG_H -I. -I%(src)s/xetexdir -I%(src)s -DU_STATIC_IMPLEMENTATION '
                  '-D__SyncTeX__ -DSYNCTEX_ENGINE_H=\\"synctexdir/synctex-xetex.h\\" '
                  '%(pkgconfig_cflags)s %(base_cflags)s' % config),
        implicit = [str(x) for x in xetex_c],
    )

    # xetex

    cflags = '-DHAVE_CONFIG_H -D__SyncTeX__ -I. -I%(src)s/xetexdir -I%(src)s -I%(src)s/libmd5 -I%(src)s/pplib %(pkgconfig_cflags)s %(base_cflags)s' % config
    objs = []

    def xetex_c_sources():
        for s in (src / 'xetexdir').glob('*.c'):
            if s.name != 'XeTeX_mac.c':
                yield s
        for s in (src / 'xetexdir' / 'image').glob('*.c'):
            yield s
        for s in xetex_c:
            if s.name.endswith('.c'):
                yield s
        yield xetex_cpool

    for s in xetex_c_sources():
        obj = build / ('xetex_' + s.name.replace('.c', '.o'))
        w.build(
            str(obj), 'cc',
            inputs = [str(s)],
            implicit = [str(x) for x in xetex_c],
            variables = {'cflags': cflags},
        )
        objs.append(str(obj))

    def xetex_cpp_sources():
        for s in (src / 'xetexdir').glob('*.cpp'):
            if s.name != 'XeTeXFontInst_Mac.cpp':
                yield s

    for s in xetex_cpp_sources():
        obj = build / ('xetex_' + s.name.replace('.cpp', '.o'))
        w.build(
            str(obj), 'cxx',
            inputs = [str(s)],
            variables = {'cflags': cflags},
        )
        objs.append(str(obj))

    objs += [str(x) for x in [libsynctex, libbase, libmd5, pplib, libtk, libkp]]
    libs = '%(pkgconfig_libs)s -lz' % config

    w.build(str(build / 'xetex'), 'executable',
             inputs = objs,
             variables = {'libs': libs},
    )

    # Bibtex source code and executable

    bibtex_p = otangle('bibtex', [
        src / 'bibtex' / 'bibtex.web',
        src / 'bibtex' / 'bibtex.ch',
    ])

    bibtex_c = convert('bibtex', [bibtex_p])

    executable(
        output = build / 'bibtex',
        sources = [bibtex_c[0], src / 'lib' / 'main.c'],  # intentional; shared `main`
        rule = 'cc',
        slibs = [libbase, libkp],
        cflags = '-I%(src)s -I%(src)s/lib -I%(src)s/xetexdir %(base_cflags)s' % config,
    )

    # Weave source code and executable

    weave_p = otangle('weave', [
        src / 'weave' / 'weave.web',
        src / 'weave' / 'weave.ch',
    ])

    weave_c = convert('weave', [weave_p])

    weaveprog = executable(
        output = build / 'weave',
        sources = [weave_c[0], src / 'lib' / 'main.c'],  # intentional; shared `main`
        rule = 'cc',
        slibs = [libbase, libkp],
        cflags = '-I%(src)s -I%(src)s/lib -I%(src)s/xetexdir %(base_cflags)s' % config,
    )

    # stage3 patch: building book TeX source

    if build_book_tex:
        mymergeprog = executable(
            output = build / 'merge-changes',
            sources = [Path('/source/stage3/merge-changes.c')],
            rule = 'cc',
            slibs = [libbase, libkp],
            cflags = '%(underscores_cflags)s -I%(src)s -I%(src)s/lib -I%(src)s/xetexdir %(base_cflags)s' % config,
        )

        w.rule('merge-changes',
            command='WEBINPUTS=. ./merge-changes $in >$out.new && mv -f $out.new $out' % config,
            description='MERGE $out')

        merged_web = build / 'tectonic-book.web'

        w.build(str(merged_web), 'merge-changes',
            inputs = [str(x) for x in [
                src / 'xetexdir' / 'xetex.web',
                xetex_ch,
            ]],
            implicit = [mymergeprog],
        )

        book_tex = build / 'tectonic-book.tex'

        w.build(str(book_tex), 'weave',
            inputs = [str(merged_web)],
            implicit = [
                weaveprog,
            ],
        )


def outer(argv0, args):
    use_custom_otangle = '--use-custom-otangle' in args
    if use_custom_otangle:
        args.remove('--use-custom-otangle')

    build_book_tex = '--build-book-tex' in args
    if build_book_tex:
        args.remove('--build-book-tex')

    underscores = '--underscores' in args
    if underscores:
        args.remove('--underscores')

    build = Path('')
    me = Path(argv0).parent
    src = Path(args[0])

    with (build / 'build.ninja').open('wt') as f:
        w = ninja_syntax.Writer(f)
        inner(src, build, w, use_custom_otangle, build_book_tex, underscores)


if __name__ == '__main__':
    import sys
    outer(sys.argv[0], sys.argv[1:])
