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

/* basic types */

typedef int boolean;

typedef char *string;
typedef const char *const_string;
typedef void *address;

#ifdef __cplusplus
extern "C" {
#endif

/* str-list.h */

typedef struct
{
  unsigned length;
  string *list;
} str_list_type;

typedef struct
{
  unsigned length;
  const_string *list;
} cstr_list_type;

#define STR_LIST_LENGTH(l) ((l).length)
#define STR_LIST(l) ((l).list)
#define STR_LIST_ELT(l, n) STR_LIST (l)[n]
#define STR_LIST_LAST_ELT(l) STR_LIST_ELT (l, STR_LIST_LENGTH (l) - 1)

static inline str_list_type
str_list_init (void)
{
  str_list_type ret;

  STR_LIST_LENGTH (ret) = 0;
  STR_LIST (ret) = NULL;

  return ret;
}

static inline cstr_list_type
cstr_list_init (void)
{
  cstr_list_type ret;

  STR_LIST_LENGTH (ret) = 0;
  STR_LIST (ret) = NULL;

  return ret;
}

extern KPSEDLL void str_list_add (str_list_type *l, string s);
extern KPSEDLL void cstr_list_add (cstr_list_type *l, const_string s);

extern void str_list_concat (str_list_type * target, str_list_type more);
extern void str_list_free (str_list_type *l);
extern void str_list_concat_elements (str_list_type *target, str_list_type more);
extern void str_list_uniqify (str_list_type *l);

/* str-llist.h */

struct str_llist_elt
{
  string str;
  boolean moved;
  struct str_llist_elt *next;
};

typedef struct str_llist_elt str_llist_elt_type;
typedef struct str_llist_elt *str_llist_type;

#define STR_LLIST(sl) ((sl).str)
#define STR_LLIST_MOVED(sl) ((sl).moved)
#define STR_LLIST_NEXT(sl) ((sl).next)

extern void str_llist_add (str_llist_type *l, string e);
extern void str_llist_float (str_llist_type *l, str_llist_elt_type *e);

/* end str-llist.h */

typedef void (*p_record_input) (const_string);
typedef void (*p_record_output) (const_string);

typedef struct
{
  const_string key;
  str_llist_type *value;
} cache_entry;

typedef struct {
  const_string var;
  boolean expanding;
} expansion_type;

/* hash.h */

typedef struct hash_element_struct
{
  const_string key;
  const_string value;
  struct hash_element_struct *next;
} hash_element_type;

typedef struct
{
  hash_element_type **buckets;
  unsigned size;
} hash_table_type;

extern KPSEDLL hash_table_type hash_create (unsigned size);
extern KPSEDLL void hash_insert (hash_table_type *table, const_string key, const_string value);
extern void hash_insert_normalized (hash_table_type *table, const_string key, const_string value);
extern KPSEDLL void hash_remove (hash_table_type *table,  const_string key, const_string value);
extern KPSEDLL const_string *hash_lookup (hash_table_type table, const_string key);
extern void hash_print (hash_table_type table, boolean summary_only);

/* from old tex-file.h */

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

typedef struct
{
  const_string type;            /* Human-readable description.  */
  string path;                  /* The search path to use.  */
  const_string raw_path;        /* Pre-$~ (but post-default) expansion.  */
  const_string path_source;     /* Where the path started from.  */
  const_string override_path;   /* From client environment variable.  */
  const_string client_path;     /* E.g., from dvips's config.ps.  */
  const_string cnf_path;        /* From texmf.cnf.  */
  const_string default_path;    /* If all else fails.  */
  const_string *suffix;         /* For kpse_find_file to check for/append.  */
  const_string *alt_suffix;     /* More suffixes to check for.  */
  boolean suffix_search_only;   /* Only search with a suffix?  */
  const_string program;         /* ``mktexpk'', etc.  */
  int argc;                     /* Count of standard arguments.  */
  const_string *argv;           /* Standard arguments to `program'.  */
  boolean program_enabled_p;    /* Invoke `program'?  */
  kpse_src_type program_enable_level; /* Who said to invoke `program'.  */
  boolean binmode;              /* Open files in binary mode?  */
} kpse_format_info_type;

typedef struct kpathsea_instance *kpathsea;

typedef struct kpathsea_instance {
    /* from cnf.c */
    p_record_input record_input;        /* for --recorder */
    p_record_output record_output;      /* for --recorder */
    hash_table_type cnf_hash;           /* used by read_all_cnf */
    boolean doing_cnf_init;             /* for kpse_cnf_get */
    /* from db.c */
    hash_table_type db;                 /* The hash table for all ls-R's */
    hash_table_type alias_db;           /* The hash table for the aliases */
    str_list_type db_dir_list;          /* list of ls-R's */
    /* from debug.c */
    unsigned debug;                     /* for --kpathsea-debug */
    /* from dir.c */
    hash_table_type link_table;         /* a hash of links-per-dir */
    /* from elt-dir.c */
    cache_entry *the_cache;
    unsigned cache_length;
    /* from fontmap.c */
    hash_table_type map;                /* the font mapping hash */
    const_string map_path;              /* path for kpse_fontmap_format */
    /* from hash.c */
    /* Print the hash values as integers if this is nonzero.  */
    boolean debug_hash_lookup_int;
    /* from path-elt.c */
    string elt;                         /* static buffer for return value */
    unsigned elt_alloc;
    const_string path;                  /* path we're currently working on */
    /* from pathsearch.c */
    boolean followup_search;
    FILE *log_file;
    boolean log_opened;                 /* Need to open the log file? */
    /* from progname.c */
    string invocation_name;
    string invocation_short_name;
    string program_name;                /* pretended name */
    int ll_verbose;                     /* for symlinks (conditional) */
    /* from tex-file.c */
    /* If non-NULL, try looking for this if can't find the real font.  */
    const_string fallback_font;
    /* If non-NULL, default list of fallback resolutions comes from this
       instead of the compile-time value.  Set by dvipsk for the R config
       cmd.  *SIZES environment variables override/use as default.  */
    const_string fallback_resolutions_string;
    /* If non-NULL, check these if can't find (within a few percent of) the
       given resolution.  List must end with a zero element.  */
    unsigned *fallback_resolutions;
    kpse_format_info_type format_info[kpse_last_format];
    /* from tex-make.c */
    /* We never throw away stdout, since that is supposed to be the filename
       found, if all is successful.  This variable controls whether stderr
       is thrown away.  */
    boolean make_tex_discard_errors;
    FILE *missfont;
    /* from variable.c  */
    expansion_type *expansions; /* sole variable of this type */
    unsigned expansion_len ;
    /* from xputenv.c */
    /* These record the strings we've set and have to keep around.
       This function can be called many times during a run, and this
       allows us to reclaim memory we allocated.  */
    char **saved_env;           /* keep track of changed items */
    int saved_count;
} kpathsea_instance;

/* these come from kpathsea.c */
extern KPSEDLL kpathsea kpathsea_new (void) ;
extern KPSEDLL void kpathsea_finish (kpathsea kpse) ;

#define kpse_bug_address kpathsea_bug_address

extern KPSEDLL kpathsea_instance kpse_def_inst;
extern KPSEDLL kpathsea kpse_def;

#define kpathsea_debug               kpse_def_inst.debug
#define kpse_program_name            kpse_def_inst.program_name
#define kpse_record_input            kpse_def_inst.record_input
#define kpse_record_output           kpse_def_inst.record_output
#define kpse_make_tex_discard_errors kpse_def_inst.make_tex_discard_errors
#define kpse_fallback_font           kpse_def_inst.fallback_font
#define kpse_fallback_resolutions_string  kpse_def_inst.fallback_resolutions_string
#define kpse_fallback_resolutions    kpse_def_inst.fallback_resolutions
#define kpse_format_info             kpse_def_inst.format_info
#define kpse_debug_hash_lookup_int   kpse_def_inst.debug_hash_lookup_int

#undef kpse_invocation_name
#define kpse_invocation_name         kpse_def_inst.invocation_name
#undef kpse_invocation_short_name
#define kpse_invocation_short_name   kpse_def_inst.invocation_short_name

/* config.h */

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


/* c-dir.h */

#define NAMLEN(dirent) strlen ((dirent)->d_name)

/* c-limits.h */

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
