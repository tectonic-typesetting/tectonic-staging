/****************************************************************************\
 Part of the XeTeX typesetting system
 Copyright (c) 1994-2008 by SIL International
 Copyright (c) 2009 by Jonathan Kew

 SIL Author(s): Jonathan Kew

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the copyright holders
shall not be used in advertising or otherwise to promote the sale,
use or other dealings in this Software without prior written
authorization from the copyright holders.
\****************************************************************************/

/* additional declarations we want to slip in for xetex */

#define native_node_size                        6
#define native_node_text(p)                     ((unsigned short*)(&(mem[(p) + native_node_size])))

#define get_native_char(p,i)                      native_node_text(p)[i]
#define set_native_char(p,i,v)                    native_node_text(p)[i] = v

#define get_native_usv(p,i) \
  ((native_node_text(p)[i] >= 0xd800 && native_node_text(p)[i] < 0xdc00) ? \
    0x10000 + (native_node_text(p)[i] - 0xd800) * 0x400 + native_node_text(p)[(i)+1] - 0xdc00 : \
    native_node_text(p)[i])

/* p is native_word node; g is XeTeX_use_glyph_metrics flag */
#define set_native_metrics(p,g)                   measure_native_node(&(mem[p]), g)

#define set_native_glyph_metrics(p,g)              measure_native_glyph(&(mem[p]), g)

#define set_justified_native_glyphs(p)             store_justified_native_glyphs(&(mem[p]))

#define get_native_italic_correction(p)            get_native_italic_correction_impl(&(mem[p]))
#define get_native_glyph_italic_correction(p)       get_native_glyph_italic_correction_impl(&(mem[p]))

#define get_native_glyph(p,i)                     get_native_glyph_impl(&(mem[p]), i)

#define make_xdv_glyph_array_data(p)                makeXDVGlyphArrayData(&(mem[p]))
#define xdv_buffer_byte(i)                        xdv_buffer[i]

#define get_native_word_cp(p,s)                    get_native_word_cp_impl(&(mem[p]), s)

#define pic_node_size                           9

#define deref(p)                                (*(p))

#define find_pic_file(a,b,c,d)                    find_pic_file(a, b, c, d)

#define pic_path_byte(p,i)                        ((unsigned char*)&(mem[p+pic_node_size]))[i]

#define dvi_open_out(f)                           open_dvi_output(&(f))

#define null_ptr                                 (NULL)
#define glyphinfobyte(p,k)                      ((unsigned char*)p)[k]
#define cast_to_ushort(x)                         (unsigned short)(x)

/* easier to do the bit-twiddling here than in Pascal */
/* read fields from a 32-bit math code */
#define math_fam_field(x)                         (((unsigned)(x) >> 24) & 0xFF)
#define math_class_field(x)                       (((unsigned)(x) >> 21) & 0x07)
#define math_char_field(x)                        ((unsigned)(x) & 0x1FFFFF)
/* calculate pieces to assign to a math code */
#define set_family_field(x)                       (((unsigned)(x) & 0xFF) << 24)
#define set_class_field(x)                        (((unsigned)(x) & 0x07) << 21)

/* aliases */
/* prototypes used in xetex.web */
#include "XeTeXOTMath.h"

/* Unicode file reading modes */
#define AUTO                                    0       /* default: will become one of 1..3 at file open time, after sniffing */
#define UTF8                                    1
#define UTF16BE                                 2
#define UTF16LE                                 3
#define RAW                                     4
#define ICUMAPPING                              5
#ifdef WIN32
#define WIN32CONSOLE                            6
#endif

/* we don't use xchr, so change the cpascal.h definition of this... */
#undef Xchr
#define Xchr(x)                                 (x)

#include "trans.h"                      /* functions for affine transform operations */
#include <teckit/TECkit_Common.h>       /* include this before XeTeX_ext.h */
#include "XeTeX_ext.h"                  /* other extension functions */

#include <math.h>
/* apparently M_PI isn't defined by <math.h> under VC++ */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
