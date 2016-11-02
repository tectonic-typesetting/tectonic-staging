/* Collected kpathsea files in the tidied workalike version.

   Copyright 1993, 1994, 1995, 2008, 2009, 2010, 2011 Karl Berry.
   Copyright 1997, 2002, 2005 Olaf Weber.

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
#include <tidy_kpathsea/c-pathch.h>
#include <tidy_kpathsea/c-stat.h>
#include <tidy_kpathsea/cnf.h>
#include <tidy_kpathsea/concatn.h>
#include <tidy_kpathsea/db.h>
#include <tidy_kpathsea/debug.h>
#include <tidy_kpathsea/hash.h>
#include <tidy_kpathsea/line.h>
#include <tidy_kpathsea/paths.h>
#include <tidy_kpathsea/pathsearch.h>
#include <tidy_kpathsea/tex-file.h>
#include <tidy_kpathsea/variable.h>
#include <tidy_kpathsea/xstat.h>

/* absolute.c  */

boolean
kpathsea_absolute_p (kpathsea kpse, const_string filename, boolean relative_ok)
{
  boolean absolute;
  boolean explicit_relative;

  absolute = IS_DIR_SEP (*filename);
  explicit_relative
    = relative_ok
      && (*filename == '.' && (IS_DIR_SEP (filename[1])
                         || (filename[1] == '.' && IS_DIR_SEP (filename[2]))));

  (void)kpse; /* currenty not used */
  /* FIXME: On UNIX an IS_DIR_SEP of any but the last character in the name
     implies relative.  */
  return absolute || explicit_relative;
}

boolean
kpse_absolute_p (const_string filename, boolean relative_ok)
{
    return kpathsea_absolute_p (kpse_def, filename, relative_ok);
}

/* atou.c */

unsigned
atou (const_string s)
{
  int i = atoi (s);

  if (i < 0)
    FATAL1 ("I expected a positive number, not %d", i);
  return i;
}

/* cnf.c */


/* By using our own hash table, instead of the environment, we
   complicate variable expansion (because we have to look in two
   places), but we don't bang so much on the system.  DOS and System V
   have very limited environment space.  Also, this way
   `kpse_init_format' can distinguish between values originating from
   the cnf file and ones from environment variables, which can be useful
   for users trying to figure out what's going on.  */

#define CNF_HASH_SIZE 751
#define CNF_NAME "texmf.cnf"


/* Do a single line in a cnf file: if it's blank or a comment or
   erroneous, skip it.  Otherwise, parse
     <variable>[.<program>] [=] <value>
   Do this even if the <variable> is already set in the environment,
   since the envvalue might contain a trailing :, in which case we'll be
   looking for the cnf value.

   We return NULL if ok, an error string otherwise.  */

static string
do_line (kpathsea kpse, string line)
{
  unsigned len;
  string start;
  string value, var;
  string prog = NULL;

  /* Skip leading whitespace.  */
  while (*line && ISSPACE (*line))
    line++;

  /* More to do only if we have non-comment material left.  */
  if (*line == 0 || *line == '%' || *line == '#')
    return NULL;

  /* Remove trailing comment: a % or # preceded by whitespace.  Also
     remove any whitespace before that.  For example, the value for
       foo = a#b  %something
     is a#b.  */
  value = line + strlen (line) - 1; /* start at end of line */
  while (value > line) {
    if (*value == '%' || *value == '#') {
      value--;                      /* move before comment char */
      while (ISSPACE (*value))
        *value-- = 0;               /* wipe out as much preceding whitespace
      continue;                        (and comment) as we find */
    }
    value--;                        /* move before the new null byte */
  }

  /* The variable name is everything up to the next space or = or `.'.  */
  start = line;
  while (*line && !ISSPACE (*line) && *line != '=' && *line != '.')
    line++;

  /* `line' is now one character past the end of the variable name.  */
  len = line - start;
  if (len == 0) {
    return ("No cnf variable name");
  }

  var = (string) xmalloc (len + 1);
  strncpy (var, start, len);
  var[len] = 0;

  /* If the variable is qualified with a program name, find out which. */
  while (*line && ISSPACE (*line))
    line++;
  if (*line == '.') {
    /* Skip spaces, then everything up to the next space or =.  */
    line++;
    while (ISSPACE (*line))
      line++;
    start = line;
    while (!ISSPACE (*line) && *line != '=')
      line++;

    /* It's annoying to repeat all this, but making a tokenizing
       subroutine would be just as long and annoying.  */
    len = line - start;
    prog = (string) xmalloc (len + 1);
    strncpy (prog, start, len);
    prog[len] = 0;
  }

  /* Skip whitespace, an optional =, more whitespace.  */
  while (*line && ISSPACE (*line))
    line++;
  if (*line == '=') {
    line++;
    while (*line && ISSPACE (*line))
      line++;
  }

  /* The value is whatever remains.  Remove trailing whitespace.  */
  start = line;
  len = strlen (start);
  while (len > 0 && ISSPACE (start[len - 1]))
    len--;
  if (len == 0) {
    return ("No cnf value");
  }

  value = (string) xmalloc (len + 1);
  strncpy (value, start, len);
  value[len] = 0;

  /* Suppose we want to write a single texmf.cnf that can be used under
     both NT and Unix.  This is feasible except for the path separators
     : on Unix, ; on NT.  We can't switch NT to allowing :'s, since :
     is the drive separator.  So we switch Unix to allowing ;'s.  On the
     other hand, we don't want to change IS_ENV_SEP and all the rest.

     So, simply translate all ;'s in the path
     values to :'s if we are a Unix binary.  (Fortunately we don't use ;
     in other kinds of texmf.cnf values.)  */

  if (IS_ENV_SEP(':')) {
      string loc;
      for (loc = value; *loc; loc++) {
          if (*loc == ';')
              *loc = ':';
      }
  }

  /* We want TEXINPUTS.prog to override plain TEXINPUTS.  The simplest
     way is to put both in the hash table (so we don't have to write
     hash_delete and hash_replace, and keep track of values' sources),
     and then look up the .prog version first in `kpse_cnf_get'.  */
  if (prog) {
    string lhs = concat3 (var, ".", prog);
    free (var);
    free (prog);
    var = lhs;
  }
  /* last-ditch debug */
  /* fprintf (stderr, "kpse/cnf.c hash_insert(%s,%s)\n", var, value); */
  hash_insert (&(kpse->cnf_hash), var, value);

  /* We should check that anything remaining is preceded by a comment
     character, but we don't.  Sorry.  */
  return NULL;
}

/* Read all the configuration files in the path.  */

static void
read_all_cnf (kpathsea kpse)
{
  kpse->cnf_hash = hash_create (CNF_HASH_SIZE);
}

/* Read the cnf files on the first call.  Return the first value in the
   returned list -- this will be from the last-read cnf file.  */

const_string
kpathsea_cnf_get (kpathsea kpse, const_string name)
{
  string ctry;
  const_string ret, *ret_list;

  /* When we expand the compile-time value for DEFAULT_TEXMFCNF,
     we end up needing the value for TETEXDIR and other variables,
     so kpse_var_expand ends up calling us again.  No good.  Except this
     code is not sufficient, somehow the ls-R path needs to be
     computed when initializing the cnf path.  Better to ensure that the
     compile-time path does not contain variable references.  */
  if (kpse->doing_cnf_init)
    return NULL;

  if (kpse->cnf_hash.size == 0) {
    /* Read configuration files and initialize databases.  */
    kpse->doing_cnf_init = true;
    read_all_cnf (kpse);
    kpse->doing_cnf_init = false;

    /* Since `kpse_init_db' recursively calls us, we must call it from
       outside a `kpse_path_element' loop (namely, the one in
       `read_all_cnf' above): `kpse_path_element' is not reentrant.  */
    kpathsea_init_db (kpse);
  }

  /* First look up NAME.`kpse->program_name', then NAME.  */
  assert (kpse->program_name);
  ctry = concat3 (name, ".", kpse->program_name);
  ret_list = hash_lookup (kpse->cnf_hash, ctry);
  free (ctry);
  if (ret_list) {
    ret = *ret_list;
    free (ret_list);
  } else {
    ret_list = hash_lookup (kpse->cnf_hash, name);
    if (ret_list) {
      ret = *ret_list;
      free (ret_list);
    } else {
      ret = NULL;
    }
  }

  return ret;
}

#if defined(KPSE_COMPAT_API)
const_string
kpse_cnf_get (const_string name)
{
    return kpathsea_cnf_get(kpse_def, name);
}
#endif

/* concat3.c */

string
concat3 (const_string s1,  const_string s2,  const_string s3)
{
  int s2l = s2 ? strlen (s2) : 0;
  int s3l = s3 ? strlen (s3) : 0;
  string answer
      = (string) xmalloc (strlen(s1) + s2l + s3l + 1);
  strcpy (answer, s1);
  if (s2) strcat (answer, s2);
  if (s3) strcat (answer, s3);

  return answer;
}

/* concat.c */

/* Return the concatenation of S1 and S2.  See `concatn.c' for a
   `concatn', which takes a variable number of arguments.  */

string
concat (const_string s1,  const_string s2)
{
  unsigned s1len = strlen(s1);
  unsigned s2len = strlen(s2);
  string answer = (string) xmalloc (s1len + s2len + 1);
  strcpy (answer, s1);
  strcat (answer + s1len, s2);

  return answer;
}

/* concatn.c */

/* OK, it would be epsilon more efficient to compute the total length
   and then do the copying ourselves, but I doubt it matters in reality.  */

string
concatn (const_string str1, ...)
{
  string arg;
  string ret;
  va_list ap;

  if (!str1)
    return NULL;

  ret = xstrdup (str1);

  va_start (ap, str1);
  while ((arg = va_arg (ap, string)) != NULL)
    {
      string temp = concat (ret, arg);
      free (ret);
      ret = temp;
    }
  va_end (ap);

  return ret;
}

/* debug.c */

#ifdef KPSE_DEBUG /* whole file */

/* If the real definitions of fopen or fclose are macros, we lose -- the
   #undef won't restore them. */

FILE *
fopen (const char *filename,  const char *mode)
{
#undef fopen
  FILE *ret = fopen (filename, mode);
#if defined (KPSE_COMPAT_API)
  kpathsea kpse = kpse_def;
  if (KPATHSEA_DEBUG_P (KPSE_DEBUG_FOPEN))
    DEBUGF3 ("fopen(%s, %s) => 0x%lx\n", filename, mode, (unsigned long) ret);
#endif
  return ret;
}

int
fclose (FILE * f)
{
#undef fclose
  int ret = fclose (f);
#if defined (KPSE_COMPAT_API)
  kpathsea kpse = kpse_def;
  if (KPATHSEA_DEBUG_P (KPSE_DEBUG_FOPEN))
    DEBUGF2 ("fclose(0x%lx) => %d\n", (unsigned long) f, ret);
#endif
  return ret;
}

#endif /* KPSE DEBUG */

/* dir.c */

/* Return true if FN is a directory or a symlink to a directory,
   false if not. */

boolean
kpathsea_dir_p (kpathsea kpse, string fn)
{
  /* FIXME : using the stat() replacement in gnuw32,
         we could avoid this win32 specific code. However,
         I wonder if it would be as fast as this one is ?
  */
  struct stat stats;
  return stat (fn, &stats) == 0 && S_ISDIR (stats.st_mode);
}

#if defined(KPSE_COMPAT_API)
boolean
dir_p (string fn)
{
    return kpathsea_dir_p (kpse_def, fn);
}
#endif


/*
  Return -1 if FN isn't a directory, else its number of links.
  Duplicate the call to stat; no need to incur overhead of a function
  call for that little bit of cleanliness.

  The process is a bit different under Win32 : the first call
  memoizes the nlinks value, the following ones retrieve it.
*/
int
kpathsea_dir_links (kpathsea kpse, const_string fn, long nlinks)
{
  const_string *hash_ret;

  if (kpse->link_table.size == 0)
    kpse->link_table = hash_create (457);

#ifdef KPSE_DEBUG
  /* This is annoying, but since we're storing integers as pointers, we
     can't print them as strings.  */
  if (KPATHSEA_DEBUG_P (KPSE_DEBUG_HASH))
    kpse->debug_hash_lookup_int = true;
#endif

  hash_ret = hash_lookup (kpse->link_table, fn);

#ifdef KPSE_DEBUG
  if (KPATHSEA_DEBUG_P (KPSE_DEBUG_HASH))
    kpse->debug_hash_lookup_int = false;
#endif

  /* Have to cast the int we need to/from the const_string that the hash
     table stores for values. Let's hope an int fits in a pointer.  */
  if (hash_ret) {
      nlinks = (long) *hash_ret;
  } else {
      struct stat stats;
      if (stat (fn, &stats) == 0 && S_ISDIR (stats.st_mode))
        nlinks = stats.st_nlink;
      else
        nlinks = -1;
      /* It's up to us to copy the value.  */
      hash_insert(&(kpse->link_table), xstrdup(fn), (const_string)nlinks);

#ifdef KPSE_DEBUG
      if (KPATHSEA_DEBUG_P (KPSE_DEBUG_STAT))
        DEBUGF2 ("dir_links(%s) => %ld\n", fn, nlinks);
#endif
  }

  /* In any case, return nlinks
     (either 0, the value inserted or the value retrieved. */
  return nlinks;
}

#if defined (KPSE_COMPAT_API)
int
dir_links (const_string fn, long nlinks)
{
    return kpathsea_dir_links (kpse_def, fn, nlinks);
}
#endif

/* extend-fname.c */

const_string
extend_filename (const_string name, const_string default_suffix)
{
  const_string new_s;
  const_string suffix = find_suffix (name);

  new_s = suffix == NULL ? concat3 (name, ".", default_suffix)
                         : name;
  return new_s;
}

/* file-p.c */

/* Test whether FILENAME1 and FILENAME2 are actually the same file.  If
   stat fails on either of the names, we return false, without error.  */

boolean
same_file_p (const_string filename1,  const_string filename2)
{
    struct stat sb1, sb2;
    /* These are put in variables only so the results can be inspected
       under gdb.  */
    int r1 = stat (filename1, &sb1);
    int r2 = stat (filename2, &sb2);

    return r1 == 0 && r2 == 0 ? SAME_FILE_P (sb1, sb2) : false;
}

/* find-suffix.c */

const_string
find_suffix (const_string name)
{
  const_string dot_pos = strrchr (name, '.');
  const_string p;

  if (dot_pos == NULL)
    return NULL;

  for (p = dot_pos + 1; *p; p++) {
    if (IS_DIR_SEP (*p))
      return NULL;
  }

  return dot_pos + 1;
}
