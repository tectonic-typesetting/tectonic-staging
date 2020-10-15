# Copyright 2020 the Tectonic Project
# Licensed under the MIT License

import os.path
import re
import sys

BDY = b'\\b'

class Rewriter(object):
    def __init__(self):
        seen_short = set()
        rewriters = []

        with open(os.path.join(os.path.dirname(__file__), 'symbols.txt')) as f:
            for line in f:
                pieces = line.strip().split()
                underscored = pieces[0]
                short = underscored.replace('_', '')

                if short in seen_short:
                    print(f'BAD: clashing short symbols {short}', file=sys.stderr)
                    continue

                if len(pieces) > 1 and 'IMPL' in pieces[1]:
                    # A symbol that is defined natively in the C/C++ code,
                    # so that if we naively rename the short form, we'll get clashes.
                    # We work around by renaming the underscore-y name first.
                    regex = BDY + underscored.encode('ascii') + BDY
                    rewriters.append((re.compile(regex), (underscored + '_impl').encode('ascii')))

                regex = BDY + short.encode('ascii') + BDY
                rewriters.append((re.compile(regex), underscored.encode('ascii')))
                seen_short.add(short)

        self.rewriters = rewriters

    def rewrite_file(self, path):
        with open(path, 'rb') as f_in:
            with open(path + '.new', 'wb') as f_out:
                for line in f_in:
                    for regex, repl in self.rewriters:
                        line = regex.sub(repl, line)

                    # Filter out '#define foo_bar foo_bar'
                    pieces = line.strip().split()
                    if len(pieces) == 3 and pieces[0] == b'#define' and pieces[1] == pieces[2]:
                        continue

                    f_out.write(line)

        os.rename(path + '.new', path)

EXTENSIONS = [
    '.c',
    '.cpp',
    '.defines',
    '.h',
    '.mm',
]

def entrypoint(args):
    rewriter = Rewriter()

    for dirpath, _, filenames in os.walk(args[0]):
        if 'dvipdfm-x' in dirpath:
            continue

        for fn in filenames:
            for ext in EXTENSIONS:
                if fn.endswith(ext):
                    full = os.path.join(dirpath, fn)
                    print(full, '...')
                    rewriter.rewrite_file(full)
                    break


if __name__ == '__main__':
    entrypoint(sys.argv[1:])
