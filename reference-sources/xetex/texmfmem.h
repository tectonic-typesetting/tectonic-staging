/* texmfmem.h: the memory_word type, which is too hard to translate
   automatically from Pascal.  We have to make sure the byte-swapping
   that the (un)dumping routines do suffices to put things in the right
   place in memory.

   A memory_word can be broken up into a `two_halves' or a
   `four_quarters', and a `two_halves' can be further broken up.  Here is
   a picture.  ..._M = most significant byte, ..._L = least significant
   byte.
   
   The halfword fields are four bytes if we are building a big TeX or MF;
   this leads to further complications:
   
   BigEndian:
   two_halves.v:  RH_MM RH_ML RH_LM RH_LL LH_MM LH_ML LH_LM LH_LL
   two_halves.u:  ---------JUNK----------  B0         B1
   four_quarters:   B0    B1    B2    B3

   LittleEndian:
   two_halves.v:  LH_LL LH_LM LH_ML LH_MM RH_LL RH_LM RH_ML RH_MM
   two_halves.u:  B1          B0
   four_quarters: ---------JUNK----------  B3    B2    B1    B0

   I guess TeX and Metafont never refer to the B1 and B0 in the
   four_quarters structure as the B1 and B0 in the two_halves.u structure.
   
   The B0 and B1 fields are declared short instead of quarterword,
   because they are used in character nodes to store a font number and a
   character.  If left as a quarterword (which is a single byte), we
   couldn't support more than 256 fonts. (If shorts aren't two bytes,
   this will lose.)
   
   In the old four-byte memory structure (something more needs to be
   done to handle >256 fonts):
   
   If BigEndian:
   two_halves.v:  RH_M  RH_L  LH_M  LH_L
   two_halves.u:  JNK1  JNK2    B0    B1
   four_quarters:   B0    B1    B2    B3
   
   If LittleEndian:
   two_halves.v:  LH_L  LH_M  RH_L  RH_M
   two_halves.u:    B1    B0  JNK1  JNK2
   four_quarters:   B3    B2    B1    B0
   
   In Aleph, quarterwords are two octets, so the picture becomes simpler:
   
   BigEndian:
   two_halves.v:  RH_MM RH_ML RH_LM RH_LL LH_MM LH_ML LH_LM LH_LL
   two_halves.u:  ---------JUNK---------- ----B0----- ----B1-----
   four_quarters: ----B0----- ----B1----- ----B2----- ----B3-----
   twoints:      ---------CINT0--------- ---------CINT1---------
   
   LittleEndian:
   two_halves.v:  LH_LL LH_LM LH_ML LH_MM RH_LL RH_LM RH_ML RH_MM
   two_halves.u:  ----B1----- ----B0-----
   four_quarters: ----B3----- ----B2----- ----B1----- ----B0-----
   twoints:      ---------CINT1--------- ---------CINT0---------
   
   This file can't be part of texmf.h, because texmf.h gets included by
   {tex,mf,mp}d.h before the `halfword' etc. types are defined.  So we
   include it from the change file instead.
*/

/* Aleph is sufficiently different to separate the definition. */
#if !defined(Aleph) && !defined(epTeX) && !defined(eupTeX) && !defined(upTeX)

typedef union
{
  struct
  {
#ifdef WORDS_BIGENDIAN
    halfword RH, LH;
#else
    halfword LH, RH;
#endif
  } v;

  struct
  { /* Make B0,B1 overlap the most significant bytes of LH.  */
#ifdef WORDS_BIGENDIAN
    halfword junk;
    short B0, B1;
#else /* not WORDS_BIGENDIAN */
  /* If 32-bit memory words, have to do something.  */
#if defined (SMALLTeX) || defined (SMALLMF) || defined (SMALLMP)
    fixme
#else
    short B1, B0;
#endif /* big memory words */
#endif /* LittleEndian */
  } u;
} two_halves;

typedef struct
{
  struct
  {
#ifdef WORDS_BIGENDIAN
    quarterword B0, B1, B2, B3;
#else
    quarterword B3, B2, B1, B0;
#endif
  } u;
} four_quarters;

typedef union
{
#ifdef TeX
  glueratio gr;
  two_halves hh;
#else
  two_halves hhfield;
#endif
#ifdef XeTeX
  void_pointer ptr;
#endif
#ifdef WORDS_BIGENDIAN
  integer cint;
  four_quarters qqqq;
#else /* not WORDS_BIGENDIAN */
  struct
  {
#if defined (TeX) && !defined (SMALLTeX) || defined (MF) && !defined (SMALLMF) || defined (MP) && !defined (SMALLMP)
    halfword junk;
#endif /* big {TeX,MF,MP} */
    integer CINT;
  } u;

  struct
  {
#ifndef XeTeX
#if defined (TeX) && !defined (SMALLTeX) || defined (MF) && !defined (SMALLMF) || defined (MP) && !defined (SMALLMP)
    halfword junk;
#endif /* big {TeX,MF,MP} */
#endif
    four_quarters QQQQ;
  } v;
#endif /* not WORDS_BIGENDIAN */
} memory_word;


/* fmemory_word for font_list; needs to be only four bytes.  This saves
   significant space in the .fmt files. (Not true in XeTeX, actually!) */
typedef union
{
#ifdef WORDS_BIGENDIAN
  integer cint;
  four_quarters qqqq;
#else /* not WORDS_BIGENDIAN */
  struct
  {
#ifdef XeTeX
    halfword junk; /* quarterword is really 16 bits in XeTeX, so integer does not fill the union */
#endif
    integer CINT;
  } u;

  struct
  {
    four_quarters QQQQ;
  } v;
#endif /* not WORDS_BIGENDIAN */
} fmemory_word;

/* To keep the original structure accesses working, we must go through
   the extra names C forced us to introduce.  */
#define	b0 u.B0
#define	b1 u.B1
#define	b2 u.B2
#define	b3 u.B3

#define rh v.RH
#define lhfield	v.LH

#ifndef WORDS_BIGENDIAN
#define cint u.CINT
#endif

#ifndef WORDS_BIGENDIAN
#define qqqq v.QQQQ
#endif

#else /* Aleph || epTeX || eupTeX || upTeX */

typedef union
{
  struct
  {
#ifdef WORDS_BIGENDIAN
    halfword RH, LH;
#else
    halfword LH, RH;
#endif
  } v;

  struct
  { /* Make B0,B1 overlap the most significant bytes of LH.  */
#ifdef WORDS_BIGENDIAN
    halfword junk;
    quarterword B0, B1;
#else /* not WORDS_BIGENDIAN */
  /* If 32-bit memory words, have to do something.  */
#if defined (SMALLTeX) || defined (SMALLMF) || defined (SMALLMP)
    fixme
#else
    quarterword B1, B0;
#endif /* big memory words */
#endif /* LittleEndian */
  } u;
} two_halves;

typedef struct
{
  struct
  {
#ifdef WORDS_BIGENDIAN
    quarterword B0, B1, B2, B3;
#else
    quarterword B3, B2, B1, B0;
#endif
  } u;
} four_quarters;

typedef struct
{
#ifdef WORDS_BIGENDIAN
  integer CINT0, CINT1;
#else
  integer CINT1, CINT0;
#endif
} twoints;
  
typedef struct
{
  glueratio GLUE;
} glues;

typedef union
{
  two_halves hh;
  four_quarters qqqq;
  twoints ii;
  glues gg;
} memory_word;

#define b0 u.B0
#define b1 u.B1
#define b2 u.B2
#define b3 u.B3

#define rh v.RH
#define lhfield v.LH

#define cint ii.CINT0
#define cint1 ii.CINT1

#define gr gg.GLUE

#endif /* Aleph || epTeX || eupTeX || upTeX */
