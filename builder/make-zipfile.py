#! /usr/bin/env python
# -*- mode: python; coding: utf-8 -*-
# Copyright 2016 the Tectonic Project.
# Licensed under the MIT License.

"""Create a Zip file containing all of the resources from a TeXLive
installation. Usefulness not guaranteed unless the installation was created
with the "texlive-builder/builder.sh" script.

"""
from __future__ import absolute_import, division, print_function, unicode_literals

import argparse, hashlib, io, os.path, struct, sys, zipfile
from six import PY2, reraise


ignored_base_names = set([
    'README',
    'ls-R',
])

ignored_tex_paths = set([
    'tex/generic/tex-ini-files/pdftexconfig.tex',
    'tex/luatex/hyph-utf8/etex.src',
])


def warn(fmt, *args):
    if len(args):
        msg = fmt % args
    else:
        msg = str(fmt)
    print('warning:', msg, file=sys.stderr)


def die(fmt, *args):
    if len(args):
        msg = fmt % args
    else:
        msg = str(fmt)
    raise SystemExit('error: ' + msg)


def make_arg_parser():
    p = argparse.ArgumentParser(
        description = __doc__,
    )
    p.add_argument(
        'install_dir',
        metavar = 'DIR',
        help = 'The path of the TeXLive installation directory.'
    )
    p.add_argument(
        'dest_path',
        metavar = 'PATH',
        help = 'The name of the Zip file to create.'
    )
    return p


class Bundler(object):
    def __init__(self, args, zip):
        self.args = args
        self.zip = zip
        self.item_shas = {}
        self.final_hexdigest = None


    def add_file(self, full_path):
        base = os.path.basename(full_path)

        if base in self.item_shas:
            warn('"%s" duplicates a file basename; skipping', full_path)
            return

        with io.open(full_path, 'rb') as f:
            # zipfile has no way to stream data into the archive (lame), so we
            # read the whole file into memory.
            contents = f.read()

        self.zip.writestr(base, contents)
        s = hashlib.sha256()
        s.update(contents)
        self.item_shas[base] = s.digest()


    def consider_file(self, full_path, tex_path, base_name):
        """Consider adding the specified TeXLive file to the installation tree. This
        is where all the nasty hairy logic will accumulate that enables us to
        come out with a nice pretty tarball in the end.

        """
        if base_name in ignored_base_names:
            return

        if tex_path in ignored_tex_paths:
            return

        if base_name.endswith('.log'):
            return

        if base_name.endswith('.fmt'):
            return

        self.add_file(full_path)


    def _walk_onerr(self, oserror):
        warn('error navigating installation tree: %s', oserror)


    def go(self):
        # Add a couple of version files from the builder.

        p = os.path.join(self.args.install_dir, 'SVNREV')
        if os.path.exists(p):
            self.add_file(p)
        else:
            warn('expected but did not see the file %s', p)

        p = os.path.join(self.args.install_dir, 'GITHASH')
        if os.path.exists(p):
            self.add_file(p)
        else:
            warn('expected but did not see the file %s', p)

        # Add the main tree.

        p = os.path.join(self.args.install_dir, 'texmf-dist')
        n = len(p) + 1

        for dirpath, dirnames, filenames in os.walk(p, onerror=self._walk_onerr):
            for fn in filenames:
                full = os.path.join(dirpath, fn)
                tex = full[n:]
                self.consider_file(full, tex, fn)

        # Compute a hash of it all.

        s = hashlib.sha256()
        s.update(struct.pack('>I', len(self.item_shas)))
        s.update(b'\0')

        for name in sorted(self.item_shas.keys()):
            if PY2:
                s.update(name)
            else:
                s.update(name.encode('utf8'))
            s.update(b'\0')
            s.update(self.item_shas[name])

        self.final_hexdigest = s.hexdigest()
        self.zip.writestr('SHA256SUM', self.final_hexdigest)


def main(argv):
    args = make_arg_parser().parse_args(argv[1:])

    try:
        with zipfile.ZipFile(args.dest_path, 'w', zipfile.ZIP_DEFLATED, True) as zip:
            b = Bundler(args, zip)
            b.go()
            print(b.final_hexdigest)
    except Exception as e:
        e1, e2, e3 = sys.exc_info()
        try:
            os.unlink(args.dest_path)
        except:
            pass
        reraise(e1, e2, e3)


if __name__ == '__main__':
    sys.exit(main(sys.argv))
