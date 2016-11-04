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
#include <getopt.h>
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

/* paths.h */

#define DEFAULT_TEXMFDBS "/nonesuch"
#define DEFAULT_WEB2C "/nonesuch"
#define DEFAULT_TEXINPUTS "/nonesuch"
#define DEFAULT_MFINPUTS "/nonesuch"
#define DEFAULT_MPINPUTS "/nonesuch"
#define DEFAULT_TEXFORMATS "/nonesuch"
#define DEFAULT_MFBASES "/nonesuch"
#define DEFAULT_MPMEMS "/nonesuch"
#define DEFAULT_TEXPOOL "/nonesuch"
#define DEFAULT_MFPOOL "/nonesuch"
#define DEFAULT_MPPOOL "/nonesuch"
#define DEFAULT_VFFONTS "/nonesuch"
#define DEFAULT_TFMFONTS "/nonesuch"
#define DEFAULT_PKFONTS "/nonesuch"
#define DEFAULT_GFFONTS "/nonesuch"
#define DEFAULT_GLYPHFONTS "/nonesuch"
#define DEFAULT_MISCFONTS "/nonesuch"
#define DEFAULT_TEXFONTMAPS "/nonesuch"
#define DEFAULT_BIBINPUTS "/nonesuch"
#define DEFAULT_BSTINPUTS "/nonesuch"
#define DEFAULT_MLBIBINPUTS "/nonesuch"
#define DEFAULT_MLBSTINPUTS "/nonesuch"
#define DEFAULT_RISINPUTS "/nonesuch"
#define DEFAULT_BLTXMLINPUTS "/nonesuch"
#define DEFAULT_MFTINPUTS "/nonesuch"
#define DEFAULT_TEXPSHEADERS "/nonesuch"
#define DEFAULT_T1FONTS "/nonesuch"
#define DEFAULT_AFMFONTS "/nonesuch"
#define DEFAULT_TTFONTS "/nonesuch"
#define DEFAULT_OPENTYPEFONTS "/nonesuch"
#define DEFAULT_T42FONTS "/nonesuch"
#define DEFAULT_LIGFONTS "/nonesuch"
#define DEFAULT_TEXCONFIG "/nonesuch"
#define DEFAULT_INDEXSTYLE "/nonesuch"
#define DEFAULT_ENCFONTS "/nonesuch"
#define DEFAULT_CMAPFONTS "/nonesuch"
#define DEFAULT_SFDFONTS "/nonesuch"
#define DEFAULT_FONTFEATURES "/nonesuch"
#define DEFAULT_FONTCIDMAPS "/nonesuch"
#define DEFAULT_PDFTEXCONFIG "/nonesuch"
#define DEFAULT_TRFONTS "/nonesuch"
#define DEFAULT_MPSUPPORT "/nonesuch"
#define DEFAULT_TEXDOCS "/nonesuch"
#define DEFAULT_TEXSOURCES "/nonesuch"
#define DEFAULT_WEBINPUTS "/nonesuch"
#define DEFAULT_CWEBINPUTS "/nonesuch"
#define DEFAULT_OFMFONTS "/nonesuch"
#define DEFAULT_OPLFONTS "/nonesuch"
#define DEFAULT_OVFFONTS "/nonesuch"
#define DEFAULT_OVPFONTS "/nonesuch"
#define DEFAULT_OTPINPUTS "/nonesuch"
#define DEFAULT_OCPINPUTS "/nonesuch"
#define DEFAULT_LUAINPUTS "/nonesuch"
#define DEFAULT_CLUAINPUTS "/nonesuch"
#define DEFAULT_TEXMFSCRIPTS "/nonesuch"
#define DEFAULT_TEXMFCNF "{$SELFAUTOLOC}"

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

/* line.h */

extern KPSEDLL string read_line (FILE *f);

/* pathsearch.h */

extern string kpathsea_path_element (kpathsea kpse, const_string path);
extern string kpathsea_filename_component (kpathsea kpse, const_string path);
extern unsigned kpathsea_normalize_path (kpathsea kpse, string elt);
extern str_llist_type *kpathsea_element_dirs (kpathsea kpse, string elt);
extern KPSEDLL string kpathsea_path_search (kpathsea kpse, const_string path, const_string name, boolean must_exist);
extern KPSEDLL string *kpathsea_all_path_search (kpathsea kpse, const_string path, const_string name);
extern string *kpathsea_path_search_list_generic (kpathsea kpse, const_string path, string* names, boolean must_exist, boolean all);
extern KPSEDLL string kpse_path_search (const_string path, const_string name, boolean must_exist);
extern KPSEDLL string *kpse_all_path_search (const_string path, const_string name);

/* proginit.h */

extern KPSEDLL void kpathsea_init_prog (kpathsea kpse, const_string prefix, unsigned dpi,
					const_string mode, const_string fallback);

extern KPSEDLL void kpse_init_prog (const_string prefix,  unsigned dpi,  const_string mode,
				    const_string fallback);

/* progname.h */

extern KPSEDLL string kpathsea_selfdir (kpathsea kpse, const_string argv0);
extern KPSEDLL void kpathsea_set_program_name (kpathsea kpse, const_string argv0, const_string progname);
extern KPSEDLL string kpse_selfdir (const_string argv0);
extern KPSEDLL void kpse_set_program_name (const_string argv0, const_string progname);
extern KPSEDLL string kpse_program_basename (const_string argv0);

/* readable.h */

extern KPSEDLL string kpathsea_readable_file (kpathsea kpse, string name);
extern KPSEDLL string kpse_readable_file (string name);

/* tex-file.h */

extern void kpathsea_init_fallback_resolutions (kpathsea kpse, string envvar);
extern KPSEDLL void kpathsea_set_program_enabled (kpathsea kpse,
    kpse_file_format_type fmt, boolean value, kpse_src_type level);
extern KPSEDLL void kpathsea_maketex_option (kpathsea kpse,
    const_string fmtname, boolean value);
extern KPSEDLL void kpathsea_set_suffixes (kpathsea kpse,
    kpse_file_format_type format, boolean alternate, ...);
extern KPSEDLL const_string kpathsea_init_format (kpathsea kpse,
    kpse_file_format_type format);
extern KPSEDLL const_string kpathsea_init_format_return_varlist (kpathsea kpse,
  kpse_file_format_type format);
extern KPSEDLL string kpathsea_find_file (kpathsea kpse, const_string name,
    kpse_file_format_type format,  boolean must_exist);
extern KPSEDLL string *kpathsea_find_file_generic (kpathsea kpse,
     const_string name, kpse_file_format_type format, boolean must_exist,
     boolean all);
extern KPSEDLL boolean kpathsea_in_name_ok (kpathsea kpse, const_string fname);
extern KPSEDLL boolean kpathsea_out_name_ok (kpathsea kpse, const_string fname);
extern KPSEDLL boolean kpathsea_in_name_ok_silent (kpathsea kpse, const_string fname);
extern KPSEDLL boolean kpathsea_out_name_ok_silent (kpathsea kpse, const_string fname);
extern KPSEDLL FILE *kpathsea_open_file (kpathsea kpse, const_string name,
                                         kpse_file_format_type format);
extern KPSEDLL void kpathsea_reset_program_name (kpathsea kpse, const_string progname);
extern void kpse_init_fallback_resolutions (string envvar);
extern KPSEDLL void kpse_set_program_enabled (kpse_file_format_type fmt,
                                         boolean value, kpse_src_type level);
extern KPSEDLL void kpse_maketex_option (const_string fmtname,  boolean value);
extern KPSEDLL void kpse_set_suffixes (kpse_file_format_type format, boolean alternate, ...);
extern KPSEDLL const_string kpse_init_format (kpse_file_format_type);
extern KPSEDLL string kpse_find_file (const_string name, kpse_file_format_type format,  boolean must_exist);
extern KPSEDLL string *kpse_find_file_generic (const_string name, kpse_file_format_type format,
      boolean must_exist, boolean all);
extern KPSEDLL boolean kpse_in_name_ok (const_string fname);
extern KPSEDLL boolean kpse_out_name_ok (const_string fname);

#define kpse_find_mf(name)   kpse_find_file (name, kpse_mf_format, true)
#define kpse_find_mft(name)  kpse_find_file (name, kpse_mft_format, true)
#define kpse_find_pict(name) kpse_find_file (name, kpse_pict_format, true)
#define kpse_find_tex(name)  kpse_find_file (name, kpse_tex_format, true)
#define kpse_find_tfm(name)  kpse_find_file (name, kpse_tfm_format, true)
#define kpse_find_ofm(name)  kpse_find_file (name, kpse_ofm_format, true)
#define kpse_find_vf(name) kpse_find_file (name, kpse_vf_format, false)
#define kpse_find_ovf(name) kpse_find_file (name, kpse_ovf_format, false)

extern KPSEDLL FILE *kpse_open_file (const_string, kpse_file_format_type);
extern KPSEDLL void kpse_reset_program_name (const_string progname);

/* tex-glyph.h */

typedef enum
{
  kpse_glyph_source_normal,  /* the searched-for font: already existed */
  kpse_glyph_source_alias,   /* : was an alias for an existing file */
  kpse_glyph_source_maketex, /* : was created on the fly */
  kpse_glyph_source_fallback /* : wasn't found, but the fallback font was */
} kpse_glyph_source_type;

typedef struct
{
  const_string name;            /* font name found */
  unsigned dpi;                 /* size found, for glyphs */
  kpse_file_format_type format; /* glyph format found */
  kpse_glyph_source_type source;        /* where we found it */
} kpse_glyph_file_type;

#define KPSE_GLYPH_FILE_NAME(f) ((f).name)
#define KPSE_GLYPH_FILE_DPI(f) ((f).dpi)
#define KPSE_GLYPH_FILE_FORMAT(f) ((f).format)
#define KPSE_GLYPH_FILE_SOURCE(f) ((f).source)


extern KPSEDLL string kpathsea_find_glyph (kpathsea kpse,
                                  const_string font_name, unsigned dpi,
                                  kpse_file_format_type format,
                                  kpse_glyph_file_type *glyph_file);

#define KPSE_BITMAP_TOLERANCE(r) ((r) / 500.0 + 1)

extern KPSEDLL boolean kpathsea_bitmap_tolerance (kpathsea kpse,
                                  double dpi1, double dpi2);

extern KPSEDLL string kpse_find_glyph (const_string font_name, unsigned dpi,
                                  kpse_file_format_type format,
                                  kpse_glyph_file_type *glyph_file);

#define kpse_find_pk(font_name, dpi, glyph_file) \
  kpse_find_glyph (font_name, dpi, kpse_pk_format, glyph_file)
#define kpse_find_gf(font_name, dpi, glyph_file) \
  kpse_find_glyph (font_name, dpi, kpse_gf_format, glyph_file)

extern KPSEDLL boolean kpse_bitmap_tolerance (double dpi1, double dpi2);

/* tex-hush.h */

extern KPSEDLL boolean kpathsea_tex_hush (kpathsea kpse, const_string what);
extern KPSEDLL boolean kpse_tex_hush (const_string what);

/* tex-make.h */

extern KPSEDLL string kpathsea_make_tex (kpathsea kpse,
                                 kpse_file_format_type format,
                                 const_string base_file);
extern KPSEDLL string kpse_make_tex (kpse_file_format_type format,
                             const_string base_file);

/* tilde.h */

extern string kpathsea_tilde_expand (kpathsea kpse, string filename);

/* variable.h */

extern KPSEDLL string kpathsea_var_value (kpathsea kpse, const_string var);
extern KPSEDLL string kpathsea_var_expand (kpathsea kpse, const_string src);
extern KPSEDLL string kpse_var_value (const_string var);
extern KPSEDLL string kpse_var_expand (const_string src);

/* version.h */

extern KPSEDLL const char *kpathsea_version_string;
extern KPSEDLL const char *kpathsea_bug_address;

/* xopendir.h */

extern DIR *xopendir (const_string dirname);
extern void xclosedir (DIR *);

/* xstat.h */

#define SAME_FILE_P(s1, s2) ((s1).st_ino == (s2).st_ino && (s1).st_dev == (s2).st_dev)

extern struct stat xstat (const_string path);
extern struct stat xlstat (const_string path);

#ifdef __cplusplus
}
#endif

#endif /* not TIDY_KPATHSEA_COLLECTED_H */
