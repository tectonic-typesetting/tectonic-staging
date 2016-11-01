/* Collected kpathsea files in the tidied workalike version.

   Copyright 1993, 1994, 1995, 2008, 2009, 2010, 2011 Karl Berry.
   Copyright 1997, 2002, 2005 Olaf Weber.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library; if not, see <http://www.gnu.org/licenses/>.  */

#include <tidy_kpathsea/config.h>
#include <tidy_kpathsea/pkgw_collected.h>
#include <tidy_kpathsea/c-pathch.h>

/* Sorry this is such a system-dependent mess, but I can't see any way
   to usefully generalize.  */

boolean
kpathsea_absolute_p (kpathsea kpse, const_string filename, boolean relative_ok)
{
  boolean absolute;
  boolean explicit_relative;

  absolute = IS_DIR_SEP (*filename);
  explicit_relative
    = relative_ok
      && (*filename == '.' && (IS_DIR_SEP (filename[1])
                         || (filename[1] == '.' && IS_DIR_SEP (filename[2]))));

  (void)kpse; /* currenty not used */
  /* FIXME: On UNIX an IS_DIR_SEP of any but the last character in the name
     implies relative.  */
  return absolute || explicit_relative;
}

boolean
kpse_absolute_p (const_string filename, boolean relative_ok)
{
    return kpathsea_absolute_p (kpse_def, filename, relative_ok);
}

unsigned
atou (const_string s)
{
  int i = atoi (s);

  if (i < 0)
    FATAL1 ("I expected a positive number, not %d", i);
  return i;
}
