/* Some definitions that get appended to the `coerce.h' file that web2c
   outputs.  */
/* $Id$ */

/* The C compiler ignores most unnecessary casts (i.e., casts of
   something to its own type).  However, for structures, it doesn't.
   Therefore, we have to redefine these macros so they don't cast
   their argument (of type memoryword or fourquarters, respectively).  */

#ifdef	printword
#undef	printword
#define	printword(x)	zprintword (x)
#endif

#ifdef	tfmqqqq
#undef	tfmqqqq
#define tfmqqqq(x)	ztfmqqqq (x)
#endif

#ifdef	eqdestroy
#undef	eqdestroy
#define	eqdestroy(x)	zeqdestroy(x)
#endif

/* And we use the opportunity to declare a few functions that could not be
   declared in texmfmp.h, because they need typedefs not yet known at that
   point.  */
extern str_number get_job_name (str_number);

#ifdef XeTeX
/* XeTeX redefines "ASCII" types.... */
typedef packed_UTF16_code packedASCIIcode;
#endif
extern void call_edit (packedASCIIcode *, pool_pointer, integer, integer);

#ifdef MF
extern void blankrectangle (screencol, screencol, screenrow, screenrow);
extern void paintrow (screenrow, pixelcolor, transspec, screencol);
#if defined(MFLua) || defined(MFLuaJIT)
#include <mfluadir/mfluac.h>
#endif
#endif

extern str_number make_full_name_string(void);

#ifdef TeX
extern string gettexstring (str_number);
/* Prototypes for source-specials functions... */
extern boolean is_new_source (str_number, int);
extern pool_pointer make_src_special (str_number, int);
extern void remember_source_info (str_number, int);

#ifdef pdfTeX
#include <pdftexdir/pdftex.h>
#endif /* pdfTeX */

#ifdef XeTeX
#include <xetexdir/xetex.h>
#endif /* XeTeX */

#ifdef __SyncTeX__
#include <synctexdir/synctex.h>
#endif
#endif /* TeX */
