#ifndef TIDY_KPATHSEA_PRIVATE_H
#define TIDY_KPATHSEA_PRIVATE_H

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

/* misc */

typedef struct
{
  const_string key;
  str_llist_type *value;
} cache_entry;

typedef struct {
  const_string var;
  boolean expanding;
} expansion_type;

/* db.h */

extern void kpathsea_init_db (kpathsea kpse);
extern str_list_type *kpathsea_db_search (kpathsea kpse, const_string name, const_string path_elt, boolean all);
extern str_list_type *kpathsea_db_search_list (kpathsea kpse, string* names, const_string  path_elt, boolean all);
extern void kpathsea_db_insert (kpathsea kpse, const_string fname);

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

/* pathsearch.h */

extern string kpathsea_path_element (kpathsea kpse, const_string path);
extern string kpathsea_filename_component (kpathsea kpse, const_string path);
extern unsigned kpathsea_normalize_path (kpathsea kpse, string elt);
extern str_llist_type *kpathsea_element_dirs (kpathsea kpse, string elt);
extern string *kpathsea_path_search_list_generic (kpathsea kpse, const_string path, string* names, boolean must_exist, boolean all);

/* kpathsea struct */

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

extern KPSEDLL kpathsea_instance kpse_def_inst;

#define kpathsea_debug kpse_def_inst.debug

#ifdef __cplusplus
}
#endif

#endif /* TIDY_KPATHSEA_PRIVATE_H */
