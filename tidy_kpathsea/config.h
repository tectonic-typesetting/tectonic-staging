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


/* systypes.h */

#include <sys/types.h>
#define __TYPES__

/* debug.h */

#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <assert.h>
#include <tidy_kpathsea/types.h>

#define KPSE_DEBUG

/* Test if a bit is on.  */
#define KPATHSEA_DEBUG_P(bit) (kpse->debug & (1 << (bit)))

#if defined (KPSE_COMPAT_API)

/* Set a bit.  */
#define KPSE_DEBUG_SET(bit) kpathsea_debug |= 1 << (bit)

/* Test if a bit is on.  */
#define KPSE_DEBUG_P(bit) (kpathsea_debug & (1 << (bit)))

#endif /* KPSE_COMPAT_API */

#define KPSE_DEBUG_STAT 0               /* stat calls */
#define KPSE_DEBUG_HASH 1               /* hash lookups */
#define KPSE_DEBUG_FOPEN 2              /* fopen/fclose calls */
#define KPSE_DEBUG_PATHS 3              /* search path initializations */
#define KPSE_DEBUG_EXPAND 4             /* path element expansion */
#define KPSE_DEBUG_SEARCH 5             /* searches */
#define KPSE_DEBUG_VARS 6               /* variable values */
#define KPSE_LAST_DEBUG KPSE_DEBUG_VARS

/* A printf for the debugging.  */
#define DEBUGF_START() do { fputs ("kdebug:", stderr)
#define DEBUGF_END()        fflush (stderr); } while (0)

#define DEBUGF(str)                                                     \
  DEBUGF_START (); fputs (str, stderr); DEBUGF_END ()
#define DEBUGF1(str, e1)                                                \
  DEBUGF_START (); fprintf (stderr, str, e1); DEBUGF_END ()
#define DEBUGF2(str, e1, e2)                                            \
  DEBUGF_START (); fprintf (stderr, str, e1, e2); DEBUGF_END ()
#define DEBUGF3(str, e1, e2, e3)                                        \
  DEBUGF_START (); fprintf (stderr, str, e1, e2, e3); DEBUGF_END ()
#define DEBUGF4(str, e1, e2, e3, e4)                                    \
  DEBUGF_START (); fprintf (stderr, str, e1, e2, e3, e4); DEBUGF_END ()

#ifdef __cplusplus
extern "C" {
#endif

#undef fopen
#define fopen kpse_fopen_trace
extern KPSEDLL FILE *fopen (const char *filename, const char *mode);
#undef fclose
#define fclose kpse_fclose_trace
extern KPSEDLL int fclose (FILE *);

/* lib.h */

#define FATAL_PERROR(str) do {			       \
  fprintf (stderr, "%s: ", kpse_def->invocation_name); \
  perror (str); exit (EXIT_FAILURE); } while (0)
#define START_FATAL() do { \
  fprintf (stderr, "%s: fatal: ", kpse_def->invocation_name);
#define END_FATAL() fputs (".\n", stderr); exit (1); } while (0)

#define FATAL(str)                                                      \
  START_FATAL (); fputs (str, stderr); END_FATAL ()
#define FATAL1(str, e1)                                                 \
  START_FATAL (); fprintf (stderr, str, e1); END_FATAL ()
#define FATAL2(str, e1, e2)                             \
   START_FATAL (); fprintf (stderr, str, e1, e2); END_FATAL ()
#define FATAL3(str, e1, e2, e3)                             \
   START_FATAL (); fprintf (stderr, str, e1, e2, e3); END_FATAL ()
#define FATAL4(str, e1, e2, e3, e4)                             \
   START_FATAL (); fprintf (stderr, str, e1, e2, e3, e4); END_FATAL ()
#define FATAL5(str, e1, e2, e3, e4, e5)                             \
   START_FATAL (); fprintf (stderr, str, e1, e2, e3, e4, e5); END_FATAL ()
#define FATAL6(str, e1, e2, e3, e4, e5, e6)                       \
   START_FATAL (); fprintf (stderr, str, e1, e2, e3, e4, e5, e6); END_FATAL ()

#define START_WARNING() do { fputs ("warning: ", stderr)
#define END_WARNING() fputs (".\n", stderr); fflush (stderr); } while (0)

#define WARNING(str)                                                    \
  START_WARNING (); fputs (str, stderr); END_WARNING ()
#define WARNING1(str, e1)                                               \
  START_WARNING (); fprintf (stderr, str, e1); END_WARNING ()
#define WARNING2(str, e1, e2)                                           \
  START_WARNING (); fprintf (stderr, str, e1, e2); END_WARNING ()
#define WARNING3(str, e1, e2, e3)                                       \
  START_WARNING (); fprintf (stderr, str, e1, e2, e3); END_WARNING ()
#define WARNING4(str, e1, e2, e3, e4)                                   \
  START_WARNING (); fprintf (stderr, str, e1, e2, e3, e4); END_WARNING ()

#define LIB_START_FATAL() do { \
  fprintf (stderr, "%s: fatal: ", kpse->invocation_name);

#define LIB_FATAL(str)                                                  \
  LIB_START_FATAL (); fputs (str, stderr); END_FATAL ()
#define LIB_FATAL1(str, e1)                                             \
  LIB_START_FATAL (); fprintf (stderr, str, e1); END_FATAL ()
#define LIB_FATAL2(str, e1, e2)                                         \
  LIB_START_FATAL (); fprintf (stderr, str, e1, e2); END_FATAL ()

#define STREQ(s1, s2) (((s1) != NULL) && ((s2) != NULL) && (strcmp (s1, s2) == 0))
#define STRNEQ(s1, s2, n) ((s1) && (s2) && (strncmp (s1, s2, n) == 0))

#define FILESTRCASEEQ STREQ
#define FILESTRNCASEEQ STRNEQ
#define FILECHARCASEEQ(c1,c2) ((c1) == (c2))

#define MAX_INT_LENGTH 21

#define ENVVAR(test, default) (getenv (test) ? (test) : (default))

extern KPSEDLL string concat (const_string s1, const_string s2);
extern KPSEDLL string concat3 (const_string, const_string, const_string);
extern KPSEDLL string xstrdup (const_string s);
extern KPSEDLL string uppercasify (const_string s);
extern KPSEDLL unsigned atou (const_string);
extern KPSEDLL boolean same_file_p (const_string filename1, const_string filename2);
extern KPSEDLL const_string xbasename (const_string name);
extern KPSEDLL string xdirname (const_string name);
extern KPSEDLL const_string find_suffix (const_string name);
extern KPSEDLL string remove_suffix (const_string name);
extern KPSEDLL string make_suffix (const_string s,  const_string suffix);
extern KPSEDLL string make_prefix (string stem_prefix, string name);
extern KPSEDLL const_string extend_filename (const_string name, const_string suffix);
extern KPSEDLL void kpathsea_xputenv (kpathsea kpse, const_string var, const_string value);
extern KPSEDLL void kpathsea_xputenv_int (kpathsea kpse, const_string var, int value);
extern KPSEDLL void xputenv (const_string var, const_string value);
extern KPSEDLL void xputenv_int (const_string var, int value);
extern KPSEDLL string xgetcwd (void);
extern KPSEDLL boolean kpathsea_dir_p (kpathsea kpse, string fn);
extern KPSEDLL boolean dir_p (string fn);
extern KPSEDLL int dir_links (const_string fn, long nlinks);
extern KPSEDLL int kpathsea_dir_links (kpathsea kpse, const_string fn, long nlinks);
extern KPSEDLL FILE *xfopen (const_string filename, const_string mode);
extern KPSEDLL void xfclose (FILE *fp, const_string filename);
extern KPSEDLL void xfseek (FILE *fp, long offset, int wherefrom, const_string filename);
extern KPSEDLL void xfseeko (FILE *fp, off_t offset, int wherefrom, const_string filename);
extern KPSEDLL long xftell (FILE *fp, const_string filename);
extern KPSEDLL off_t xftello (FILE *fp, const_string filename);
extern KPSEDLL address xmalloc (size_t size);
extern KPSEDLL address xrealloc (address old_address, size_t new_size);
extern KPSEDLL address xcalloc (size_t nelem, size_t elsize);

/* (Re)Allocate N items of type T using xmalloc/xrealloc.  */
#define XTALLOC(n, t) ((t *) xmalloc ((n) * sizeof (t)))
#define XTALLOC1(t) XTALLOC (1, t)
#define XRETALLOC(addr, n, t) ((addr) = (t *) xrealloc (addr, (n) * sizeof(t)))

#ifdef __cplusplus
}
#endif

#define ST_NLINK_TRICK

#endif /* not KPATHSEA_CONFIG_H */
