/* uppercasify.c: change all lowercase letters to uppercase.

   Copyright 1993, 2008 Karl Berry.

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


string
uppercasify (const_string s)
{
  string target;
  string ret = xstrdup (s);

  for (target = ret; *target; target++)
    {
      *target = TOUPPER (*target);
    }

  return ret;
}
