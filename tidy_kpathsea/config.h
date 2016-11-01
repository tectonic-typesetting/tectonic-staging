/* config.h: master configuration file, included first by all compilable
   source files (not headers).

   Copyright 1993, 1995, 1996, 1997, 2008, 2010, 2011, 2016 Karl Berry.
   Copyright 2000, 2003, 2004, 2005 Olaf Weber.

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

#ifndef KPATHSEA_CONFIG_H
#define KPATHSEA_CONFIG_H

#define DEV_NULL "/dev/null"

#define HAVE_ASSERT_H 1
#define HAVE_DECL_ISASCII 1
#define HAVE_DECL_PUTENV 1
#define HAVE_DIRENT_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_FLOAT_H 1
#define HAVE_GETCWD 1
#define HAVE_GETWD 1
#define HAVE_LIMITS_H 1
#define HAVE_MEMCMP 1
#define HAVE_MEMCPY 1
#define HAVE_MEMORY_H 1
#define HAVE_PWD_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRCHR 1
#define HAVE_STRING_H 1
#define HAVE_STRINGS_H 1
#define HAVE_STRRCHR 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_UNISTD_H 1

#define KPSEVERSION "kpathsea version 6.2.3/dev"
#define MAKE_TEX_FMT_BY_DEFAULT 0
#define MAKE_TEX_MF_BY_DEFAULT 0
#define MAKE_TEX_PK_BY_DEFAULT 0
#define MAKE_TEX_TEX_BY_DEFAULT 0
#define MAKE_TEX_TFM_BY_DEFAULT 0
#define SIZEOF_LONG 8
#define STDC_HEADERS 1
#define _DARWIN_USE_64_BIT_INODE 1

/* Some drivers have partially integrated kpathsea changes.  */
#ifndef KPATHSEA
#define KPATHSEA 34
#endif

#include <tidy_kpathsea/c-proto.h>  /* KPSEDLL.  */

#define TRANSFORM(x) (x)

#include <tidy_kpathsea/debug.h>    /* Runtime tracing.  */
#include <tidy_kpathsea/lib.h>      /* STREQ, etc. */
#include <tidy_kpathsea/types.h>    /* <sys/types.h>, boolean, string, etc. */
#include <tidy_kpathsea/progname.h> /* for kpse_invocation_*name */

#define ST_NLINK_TRICK

#endif /* not KPATHSEA_CONFIG_H */
