#! /usr/bin/env python
# -*- mode: python; coding: utf-8 -*-
# Copyright 2016 Peter Williams
# Licensed under the MIT License.

"""Rewrite "otangle" Pascal output to use more symbolic variable names that
are annoying to compute inside our hacked version of otangle.

"""
from __future__ import absolute_import, division, print_function

import sys

def die (fmt, *args):
    if len (args):
        text = fmt % args
    else:
        text = str (fmt)

    raise SystemExit ('error: ' + text)


def decode (item):
    try:
        return int (item)
    except ValueError:
        return item


def _one_to_pascal (item):
    if isinstance (item, int):
        if item < 0:
            return 'M%d' % (-item)
        if item == 0:
            return ''
        return 'P%d' % item
    return item.replace ('_', '')


def key_to_pascal (key):
    return 'X' + ''.join (_one_to_pascal (x) for x in key)


MAGIC_LINE_NUM = 2


def main (argv):
    if len (argv) != 2:
        die ('usage: %s <pascal-file-name>', argv[0])

    pascal = argv[1]

    # Pass 1. Build up table of symbolic constants.

    constants = {}

    with open (pascal, 'r') as f:
        for line in f:
            while True:
                try:
                    i = line.index ('{<}') # index of beginning of this delimiter
                except ValueError:
                    break

                j = i + 3 # index of the value of the constant
                k = j + line[j:].index ('{') # index of the beginning of the expansion marker
                l = k + line[k:].index ('}') # index of the end of the expansion marker

                key = tuple (b for b in (decode (b) for b in line[k+1:l].split ()) if b != 0)
                value = int (line[j:k])

                prev = constants.get (key)
                if prev is not None and value != prev:
                    die ('conflicting value for %r: %d, %d', key, prev, value)
                constants[key] = value

                # On this pass, can just throw away previous bits:
                line = line[l+1:]

    # Pass 2. Emit constants and munge the program text.

    with open (pascal, 'r') as f:
        for line_num, line in enumerate (f):
            if line_num == MAGIC_LINE_NUM:
                # At the top, emit values for all of our magic constants.
                for key, value in sorted (constants.iteritems ()):
                    print ('%s=%d;' % (key_to_pascal (key), value))

            while True:
                try:
                    i = line.index ('{<}') # index of beginning of this delimiter
                except ValueError:
                    break

                j = i + 3 # index of the value of the constant
                k = j + line[j:].index ('{') # index of the beginning of the expansion marker
                l = k + line[k:].index ('}') # index of the end of the expansion marker

                key = tuple (decode (b) for b in line[k+1:l].split ())

                # Munge this value out of the line:
                line = line[:i] + key_to_pascal (key) + line[l+1:]

            print (line, end='')


if __name__ == '__main__':
    main (sys.argv)
