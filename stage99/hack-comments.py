# Copyright 2020 the Tectonic Project.
# Licensed under the MIT License.

"""
It turns out that `indent -linux` (specifically, `indent -br`) has a bug
handling statements of the following form:

```
if (condition) /* comment */
{
    {
        inner();
    }
}
```

or

```
} else /*comment*/ {
    {
        inner();
    }
}
```

(This is as of version 2.2.12.) This happens in some of our web2c-generated
files. It seems that if we change the comment to a C99-style `// comment`,
the problem is fixed. So, this script implements this modification to ensure
that we can `indent` our exported generated files, since that minimizes the
diffs and makes them at least a bit more readable.

This script transforms:

[stuff] /*comment*/     => [stuff] // comment
[stuff] /*comment*/ {   => [stuff] { // comment

"""

import os.path
import sys


def entrypoint(args):
    for path in args:
        new = path + '.new'

        with open(path, 'rb') as f_in:
            with open(new, 'wb') as f_out:
                for line in f_in:
                    working_line = line

                    final_open_brace = line.rstrip().endswith(b'{')
                    if final_open_brace:
                        working_line = line[:line.rindex(b'{')]

                    if working_line.rstrip().endswith(b'*/'):
                        e_idx = working_line.rindex(b'*/')

                        try:
                            s_idx = working_line.rindex(b'/*')
                        except ValueError:
                            # true multi-line
                            pass
                        else:
                            # transform the line-ending comment to C99
                            maybe_brace = b'{ ' if final_open_brace else b''
                            line = line[:s_idx] + maybe_brace + b'//' + line[s_idx+2:e_idx].rstrip() + b'\n'

                    f_out.write(line)

        os.rename(new, path)


if __name__ == '__main__':
    entrypoint(sys.argv[1:])
