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

#ifndef TIDY_KPATHSEA_PUBLIC_H
#define TIDY_KPATHSEA_PUBLIC_H

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <float.h>
#include <getopt.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* high-level defines */

#ifndef __cplusplus
#ifndef true
#define true 1
#define false 0
#endif /* not true */
#endif /* not __cplusplus */

#ifndef FALSE
#define FALSE false
#define TRUE true
#endif /* FALSE */

#define KPSEDLL
#define KPSE_COMPAT_API 1
#define DEV_NULL "/dev/null"
#define KPATHSEA 34
#define TRANSFORM(x) (x)
#define __TYPES__
#define KPSE_DEBUG
#define KPSEVERSION "kpathsea version 6.2.3/dev"
#define MAKE_TEX_FMT_BY_DEFAULT 0
#define MAKE_TEX_MF_BY_DEFAULT 0
#define MAKE_TEX_PK_BY_DEFAULT 0
#define MAKE_TEX_TEX_BY_DEFAULT 0
#define MAKE_TEX_TFM_BY_DEFAULT 0
#define SIZEOF_LONG 8
#define STDC_HEADERS 1
#define _DARWIN_USE_64_BIT_INODE 1
#define ST_NLINK_TRICK

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

/* lib.h */

#define FATAL_PERROR(str) do {			       \
  fprintf (stderr, "%s: ", kpse_invocation_name); \
  perror (str); exit (EXIT_FAILURE); } while (0)
#define START_FATAL() do { \
  fprintf (stderr, "%s: fatal: ", kpse_invocation_name);
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

/* config.h */

#define STREQ(s1, s2) (((s1) != NULL) && ((s2) != NULL) && (strcmp (s1, s2) == 0))
#define STRNEQ(s1, s2, n) ((s1) && (s2) && (strncmp (s1, s2, n) == 0))
#define FILESTRCASEEQ STREQ
#define FILESTRNCASEEQ STRNEQ
#define FILECHARCASEEQ(c1,c2) ((c1) == (c2))
#define MAX_INT_LENGTH 21

/* basic types */

typedef int boolean;

typedef char *string;
typedef const char *const_string;
typedef void *address;

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*p_record_input) (const_string);
typedef void (*p_record_output) (const_string);

typedef enum
{
  kpse_gf_format,
  kpse_pk_format,
  kpse_any_glyph_format,
  kpse_tfm_format,
  kpse_afm_format,
  kpse_base_format,
  kpse_bib_format,
  kpse_bst_format,
  kpse_cnf_format,
  kpse_db_format,
  kpse_fmt_format,
  kpse_fontmap_format,
  kpse_mem_format,
  kpse_mf_format,
  kpse_mfpool_format,
  kpse_mft_format,
  kpse_mp_format,
  kpse_mppool_format,
  kpse_mpsupport_format,
  kpse_ocp_format,
  kpse_ofm_format,
  kpse_opl_format,
  kpse_otp_format,
  kpse_ovf_format,
  kpse_ovp_format,
  kpse_pict_format,
  kpse_tex_format,
  kpse_texdoc_format,
  kpse_texpool_format,
  kpse_texsource_format,
  kpse_tex_ps_header_format,
  kpse_troff_font_format,
  kpse_type1_format,
  kpse_vf_format,
  kpse_dvips_config_format,
  kpse_ist_format,
  kpse_truetype_format,
  kpse_type42_format,
  kpse_web2c_format,
  kpse_program_text_format,
  kpse_program_binary_format,
  kpse_miscfonts_format,
  kpse_web_format,
  kpse_cweb_format,
  kpse_enc_format,
  kpse_cmap_format,
  kpse_sfd_format,
  kpse_opentype_format,
  kpse_pdftex_config_format,
  kpse_lig_format,
  kpse_texmfscripts_format,
  kpse_lua_format,
  kpse_fea_format,
  kpse_cid_format,
  kpse_mlbib_format,
  kpse_mlbst_format,
  kpse_clua_format,
  kpse_ris_format,
  kpse_bltxml_format,
  kpse_last_format /* one past last index */
} kpse_file_format_type;

typedef enum
{
  kpse_src_implicit,   /* C initialization to zero */
  kpse_src_compile,    /* configure/compile-time default */
  kpse_src_texmf_cnf,  /* texmf.cnf, the kpathsea config file */
  kpse_src_client_cnf, /* application config file, e.g., config.ps */
  kpse_src_env,        /* environment variable */
  kpse_src_x,          /* X Window System resource */
  kpse_src_cmdline     /* command-line option */
} kpse_src_type;

/* global instance opaque struct def */

typedef struct kpathsea_instance kpathsea_instance;
typedef kpathsea_instance *kpathsea;
extern KPSEDLL kpathsea kpse_def;

extern string kpse_pkgw_get_definst_program_name (void);
#define kpse_program_name (kpse_pkgw_get_definst_program_name ())

extern string kpse_pkgw_get_definst_invocation_name (void);
#define kpse_invocation_name (kpse_pkgw_get_definst_invocation_name ())

extern void kpse_pkgw_set_definst_record_input (p_record_input val);
extern void kpse_pkgw_set_definst_record_output (p_record_output val);
extern void kpse_pkgw_set_definst_make_tex_discard_errors (boolean val);

/* these utility routines are (ab)used outside of kpse */

extern KPSEDLL string concat (const_string s1, const_string s2);
extern KPSEDLL string concat3 (const_string, const_string, const_string);
extern KPSEDLL string concatn(const_string str1, ...);
extern KPSEDLL string xstrdup (const_string s);
extern KPSEDLL const_string xbasename (const_string name);
extern KPSEDLL const_string find_suffix (const_string name);
extern KPSEDLL void xputenv (const_string var, const_string value);
extern KPSEDLL string xgetcwd (void);
extern KPSEDLL FILE *xfopen (const_string filename, const_string mode);
extern KPSEDLL void xfclose (FILE *fp, const_string filename);
extern KPSEDLL address xmalloc (size_t size);
extern KPSEDLL address xrealloc (address old_address, size_t new_size);
extern KPSEDLL address xcalloc (size_t nelem, size_t elsize);

#define XTALLOC(n, t) ((t *) xmalloc ((n) * sizeof (t)))
#define XTALLOC1(t) XTALLOC (1, t)
#define XRETALLOC(addr, n, t) ((addr) = (t *) xrealloc (addr, (n) * sizeof(t)))

#ifndef isblank
#define isblank(c) ((c) == ' ' || (c) == '\t')
#endif
#define ISBLANK(c) (isascii (c) && isblank ((unsigned char)c))

/* c-fopen.h */

#define FOPEN_A_MODE "ab"
#define FOPEN_R_MODE "r"
#define FOPEN_W_MODE "wb"
#define FOPEN_RBIN_MODE "rb"
#define FOPEN_WBIN_MODE "wb"
#define FOPEN_ABIN_MODE "ab"

/* c-pathch.h */

/* What separates filename components?  */
#ifndef DIR_SEP
# define DIR_SEP '/'
# define DIR_SEP_STRING "/"
#endif

#ifndef IS_DIR_SEP
#define IS_DIR_SEP(ch) ((ch) == DIR_SEP)
#endif

/* absolute.h */

extern KPSEDLL boolean kpse_absolute_p (const_string filename, boolean relative_ok);

/* line.h */

extern KPSEDLL string read_line (FILE *f);

/* progname.h */

extern KPSEDLL void kpse_set_program_name (const_string argv0, const_string progname);
extern KPSEDLL string kpse_program_basename (const_string argv0);

/* readable.h */

extern KPSEDLL string kpse_readable_file (string name);

/* tex-file.h */

extern KPSEDLL void kpse_set_program_enabled (kpse_file_format_type fmt, boolean value, kpse_src_type level);
extern KPSEDLL void kpse_maketex_option (const_string fmtname,  boolean value);
extern KPSEDLL string kpse_find_file (const_string name, kpse_file_format_type format,  boolean must_exist);
extern KPSEDLL boolean kpse_in_name_ok (const_string fname);
extern KPSEDLL boolean kpse_out_name_ok (const_string fname);
extern KPSEDLL FILE *kpse_open_file (const_string, kpse_file_format_type);
extern KPSEDLL void kpse_reset_program_name (const_string progname);

#define kpse_find_mf(name)   kpse_find_file (name, kpse_mf_format, true)
#define kpse_find_mft(name)  kpse_find_file (name, kpse_mft_format, true)
#define kpse_find_pict(name) kpse_find_file (name, kpse_pict_format, true)
#define kpse_find_tex(name)  kpse_find_file (name, kpse_tex_format, true)
#define kpse_find_tfm(name)  kpse_find_file (name, kpse_tfm_format, true)
#define kpse_find_ofm(name)  kpse_find_file (name, kpse_ofm_format, true)
#define kpse_find_vf(name) kpse_find_file (name, kpse_vf_format, false)
#define kpse_find_ovf(name) kpse_find_file (name, kpse_ovf_format, false)

/* variable.h */

extern KPSEDLL string kpse_var_value (const_string var);

/* version.h */

extern KPSEDLL const char *kpathsea_version_string;

#ifdef __cplusplus
}
#endif

#endif /* not TIDY_KPATHSEA_PUBLIC_H */
