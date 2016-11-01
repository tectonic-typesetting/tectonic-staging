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

/* System dependencies that are figured out by `configure'.  */
#include <kpathsea/c-auto.h>

/* Some drivers have partially integrated kpathsea changes.  */
#ifndef KPATHSEA
#define KPATHSEA 34
#endif

#include <kpathsea/c-proto.h>  /* KPSEDLL.  */

#include <kpathsea/debug.h>    /* Runtime tracing.  */
#include <kpathsea/lib.h>      /* STREQ, etc. */
#include <kpathsea/types.h>    /* <sys/types.h>, boolean, string, etc. */
#include <kpathsea/progname.h> /* for kpse_invocation_*name */

#define ST_NLINK_TRICK

#endif /* not KPATHSEA_CONFIG_H */
