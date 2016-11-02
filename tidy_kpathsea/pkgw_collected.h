/* Collected kpathsea files in the tidied workalike version.

   Copyright 1993, 1994, 2008, 2009 Karl Berry.
   Copyright 1999, 2005 Olaf Weber.

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

#ifndef TIDY_KPATHSEA_COLLECTED_H
#define TIDY_KPATHSEA_COLLECTED_H

#include <ctype.h>
#include <errno.h>

#include <tidy_kpathsea/types.h>
#include <tidy_kpathsea/c-proto.h>

#include <dirent.h>
#define NAMLEN(dirent) strlen ((dirent)->d_name)

#ifdef __cplusplus
extern "C" {
#endif

/* c-ctype.h */

/* Be sure we have `isascii'.  */
#ifndef WIN32
#if !(defined(HAVE_DECL_ISASCII) && HAVE_DECL_ISASCII)
#define isascii(c) (((c) & ~0x7f) == 0)
#endif
#endif

#define ISALNUM(c) (isascii (c) && isalnum((unsigned char)c))
#define ISALPHA(c) (isascii (c) && isalpha((unsigned char)c))
#define ISASCII isascii
#define ISCNTRL(c) (isascii (c) && iscntrl((unsigned char)c))
#define ISDIGIT(c) (isascii (c) && isdigit ((unsigned char)c))
#define ISGRAPH(c) (isascii (c) && isgraph((unsigned char)c))
#define ISLOWER(c) (isascii (c) && islower((unsigned char)c))
#define ISPRINT(c) (isascii (c) && isprint((unsigned char)c))
#define ISPUNCT(c) (isascii (c) && ispunct((unsigned char)c))
#define ISSPACE(c) (isascii (c) && isspace((unsigned char)c))
#define ISUPPER(c) (isascii (c) && isupper((unsigned char)c))
#define ISXDIGIT(c) (isascii (c) && isxdigit((unsigned char)c))
#define TOASCII toascii
#define TOLOWER(c) (ISUPPER (c) ? tolower ((unsigned char)c) : (c))
#define TOUPPER(c) (ISLOWER (c) ? toupper ((unsigned char)c) : (c))

/* This isn't part of the usual <ctype.h>, but it's useful sometimes.  */
#ifndef isblank
#define isblank(c) ((c) == ' ' || (c) == '\t')
#endif

#define ISBLANK(c) (isascii (c) && isblank ((unsigned char)c))

/* absolute.h */

extern KPSEDLL boolean kpathsea_absolute_p (kpathsea kpse, const_string filename, boolean relative_ok);
extern KPSEDLL boolean kpse_absolute_p (const_string filename, boolean relative_ok);

#ifdef __cplusplus
}
#endif

#endif /* not TIDY_KPATHSEA_COLLECTED_H */