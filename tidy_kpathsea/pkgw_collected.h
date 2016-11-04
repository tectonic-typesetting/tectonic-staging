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
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>

#include <tidy_kpathsea/types.h>
#include <tidy_kpathsea/c-proto.h>

/* c-dir.h */

#include <dirent.h>
#define NAMLEN(dirent) strlen ((dirent)->d_name)

/* c-limits.h */

#if defined (HAVE_FLOAT_H) && !defined (FLT_MAX)
#include <float.h>
#endif

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

/* c-fopen.h */

#ifndef FOPEN_A_MODE
#define FOPEN_A_MODE "ab"
#endif

#ifndef FOPEN_R_MODE
#define FOPEN_R_MODE "r"
#endif

#ifndef FOPEN_W_MODE
#define FOPEN_W_MODE "wb"
#endif

#ifndef FOPEN_RBIN_MODE
#define FOPEN_RBIN_MODE "rb"
#endif

#ifndef FOPEN_WBIN_MODE
#define FOPEN_WBIN_MODE "wb"
#endif

#ifndef FOPEN_ABIN_MODE
#define FOPEN_ABIN_MODE "ab"
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif
#define SET_BINARY(f) (void)0

/* c-pathch.h */

/* What separates filename components?  */
#ifndef DIR_SEP
# define DIR_SEP '/'
# define DIR_SEP_STRING "/"
#endif

#ifndef IS_DIR_SEP
#define IS_DIR_SEP(ch) ((ch) == DIR_SEP)
#endif
#ifndef IS_DIR_SEP_CH
#define IS_DIR_SEP_CH(ch) IS_DIR_SEP(ch)
#endif
#ifndef IS_DEVICE_SEP /* No `devices' on, e.g., Unix.  */
#define IS_DEVICE_SEP(ch) 0
#endif
#ifndef NAME_BEGINS_WITH_DEVICE
#define NAME_BEGINS_WITH_DEVICE(name) 0
#endif
#ifndef IS_UNC_NAME /* Unc names are in practice found on Win32 only. */
#define IS_UNC_NAME(name) 0
#endif

/* What separates elements in environment variable path lists?  */
#ifndef ENV_SEP
# define ENV_SEP ':'
# define ENV_SEP_STRING ":"
#endif /* not ENV_SEP */

#ifndef IS_ENV_SEP
#define IS_ENV_SEP(ch) ((ch) == ENV_SEP)
#endif

/* absolute.h */

extern KPSEDLL boolean kpathsea_absolute_p (kpathsea kpse, const_string filename, boolean relative_ok);
extern KPSEDLL boolean kpse_absolute_p (const_string filename, boolean relative_ok);

/* cnf.h */

extern KPSEDLL const_string kpathsea_cnf_get (kpathsea kpse, const_string name);
extern KPSEDLL const_string kpse_cnf_get (const_string var);

/* concatn.h */

extern KPSEDLL string concatn(const_string str1, ...);

/* db.h */

extern void kpathsea_init_db (kpathsea kpse);
extern str_list_type *kpathsea_db_search (kpathsea kpse, const_string name, const_string path_elt, boolean all);
extern str_list_type *kpathsea_db_search_list (kpathsea kpse, string* names, const_string  path_elt, boolean all);
extern void kpathsea_db_insert (kpathsea kpse, const_string fname);

/* default.h */

extern string kpathsea_expand_default (kpathsea kpse, const_string path, const_string dflt);

/* expand.h */

extern string kpathsea_expand (kpathsea kpse, const_string s);

extern KPSEDLL string kpathsea_brace_expand (kpathsea kpse, const_string path);
extern KPSEDLL string kpathsea_path_expand (kpathsea kpse, const_string path);
extern KPSEDLL string kpse_brace_expand (const_string path);
extern KPSEDLL string kpse_path_expand (const_string path);

/* fn.h */

typedef struct
{
  string str;
  unsigned allocated;
  unsigned length; /* includes the terminating null byte, if any */
} fn_type;

#define FN_STRING(fn) ((fn).str)
#define FN_ALLOCATED(fn) ((fn).allocated)
#define FN_LENGTH(fn) ((fn).length)

extern fn_type fn_init (void);
extern fn_type fn_copy0 (const_string s,  unsigned len);
extern void fn_free (fn_type *f);
extern void fn_1grow (fn_type *f, char c);
extern void fn_grow (fn_type *f, const_string source, unsigned length);
extern void fn_str_grow (fn_type *f, const_string s);
extern void fn_shrink_to (fn_type *f, unsigned loc);

/* fontmap.h */

extern const_string *kpathsea_fontmap_lookup (kpathsea kpse, const_string key);

#ifdef __cplusplus
}
#endif

#endif /* not TIDY_KPATHSEA_COLLECTED_H */
