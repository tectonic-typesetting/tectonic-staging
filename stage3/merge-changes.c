/* Tectonic: This is a hacked version of otangle that merely emits a WEB file
 * with changefiles merged in. It is used in the process of generating the XeTeX
 * book PDF. */

#define OTANGLE
#define PKGW

#include <w2c/config.h>
#include "lib.h"
#include "cpascal.h"

#define bufsize (3000)
#define maxbytes (65535L)
#define maxtoks (65535L)
#define maxnames (10239)
#define maxtexts (10239)
#define hashsize (353)
#define longestname (400)
#define linelength (144)
#define outbufsize (288)
#define stacksize (100)
#define maxidlength (50)
#define unambiglength (30)
#define noptions (3)

typedef unsigned char ASCIIcode;
typedef text /* of  ASCIIcode */ textfile;
typedef unsigned char eightbits;
typedef integer sixteenbits;
typedef integer namepointer;
typedef integer textpointer;
typedef struct {
    sixteenbits endfield;
    sixteenbits bytefield;
    namepointer namefield;
    textpointer replfield;
    short modfield;
} outputstate;

/* Magic constants. Unfortunately C does not let you use 'const ints' as items
 * in case statements, so we reluctantly turn them into #define's. */

#define SPOTLESS 0 /* for "history" variable */
#define HARMLESS_MESSAGE 1
#define ERROR_MESSAGE 2
#define FATAL_MESSAGE 3

#define NUMBER_CHARS 65536L

#define AND_SIGN 4 /* ASCII with extension */
#define NOT_SIGN 5
#define SET_ELEMENT_SIGN 6
#define TAB_MARK 9
#define LINE_FEED 10
#define FORM_FEED 12
#define CARRIAGE_RETURN 13
#define LEFT_ARROW 24
#define NOT_EQUAL 26
#define LESS_OR_EQUAL 28
#define GREATER_OR_EQUAL 29
#define EQUIVALENCE_SIGN 30
#define OR_SIGN 31
#define SPACE 32
#define EXCLAM 33
#define DOUBLEQUOTE 34
#define OCTOTHORPE 35
#define DOLLARSIGN 36
#define PERCENT 37
#define AMPERSAND 38
#define SINGLE_QUOTE 39
#define LEFT_PAREN 40
#define RIGHT_PAREN 41
#define ASTERISK 42
#define PLUS_SIGN 43
#define COMMA 44
#define MINUS_SIGN 45
#define PERIOD 46
#define FORWARD_SLASH 47
#define ASCII_0 48
#define ASCII_1 49
#define ASCII_2 50
#define ASCII_3 51
#define ASCII_4 52
#define ASCII_5 53
#define ASCII_6 54
#define ASCII_7 55
#define ASCII_8 56
#define ASCII_9 57
#define COLON 58
#define SEMICOLON 59
#define LESS_THAN_SIGN 60
#define EQUALS_SIGN 61
#define GREATER_THAN_SIGN 62
#define QUESTION_MARK 63
#define AT_SIGN 64
#define ASCII_A 65
#define ASCII_B 66
#define ASCII_C 67
#define ASCII_D 68
#define ASCII_E 69
#define ASCII_F 70
#define ASCII_G 71
#define ASCII_H 72
#define ASCII_I 73
#define ASCII_J 74
#define ASCII_K 75
#define ASCII_L 76
#define ASCII_M 77
#define ASCII_N 78
#define ASCII_O 79
#define ASCII_P 80
#define ASCII_Q 81
#define ASCII_R 82
#define ASCII_S 83
#define ASCII_T 84
#define ASCII_U 85
#define ASCII_V 86
#define ASCII_W 87
#define ASCII_X 88
#define ASCII_Y 89
#define ASCII_Z 90
#define LEFT_BRACKET 91
#define BACK_SLASH 92
#define RIGHT_BRACKET 93
#define CARET 94
#define UNDERSCORE 95
#define BACKTICK 96
#define ASCII_a 97
#define ASCII_b 98
#define ASCII_c 99
#define ASCII_d 100
#define ASCII_e 101
#define ASCII_f 102
#define ASCII_g 103
#define ASCII_h 104
#define ASCII_i 105
#define ASCII_j 106
#define ASCII_k 107
#define ASCII_l 108
#define ASCII_m 109
#define ASCII_n 110
#define ASCII_o 111
#define ASCII_p 112
#define ASCII_q 113
#define ASCII_r 114
#define ASCII_s 115
#define ASCII_t 116
#define ASCII_u 117
#define ASCII_v 118
#define ASCII_w 119
#define ASCII_x 120
#define ASCII_y 121
#define ASCII_z 122
#define LEFT_BRACE 123
#define PIPE 124
#define RIGHT_BRACE 125
#define TILDE 126

#define PARAM 0 /* = NUL */
#define IGNORE 0
#define VERBATIM 2 /* = alpha */
#define FORCE_LINE 3
#define BEGIN_COMMENT 9 /* = tab mark */
#define END_COMMENT 10 /* = line feed */
#define OCTAL 12 /* = form feed */
#define HEX 13 /* = carriage return */
#define DOUBLE_DOT 32 /* = space */
#define CHECK_SUM 125 /* = right brace */
#define JOIN 127 /* = DEL */
#define NUMBER 128
#define MODULE_NUMBER 129
#define IDENTIFIER 130
#define CONTROL_TEXT 131
#define FORMAT 132
#define DEFINITION 133
#define BEGIN_PASCAL 134
#define MODULE_NAME 135
#define NEW_MODULE 136

#define NORMAL 0 /* "ilk" / id_lookup types */
#define NUMERIC 1
#define SIMPLE 2
#define PARAMETRIC 3

#define LESS 0 /* mod_lookup results */
#define EQUAL 1
#define GREATER 2
#define PREFIX 3
#define EXTENSION 4

#define MISC 0 /* send_out "t" codes */
#define STR 1
#define IDENT 2
#define FRAC 3

#define NUM_OR_ID 1 /* out_state codes; MISC is also relevant */
#define SIGN 2
#define SIGN_VAL 3
#define SIGN_VAL_SIGN 4
#define SIGN_VAL_VAL 5
#define UNBREAKABLE 6

/* end of magic constants */

unsigned char history;
ASCIIcode xord[256];
ASCIIcode xchr[256];
textfile webfile;
textfile changefile;
/* Tectonic: Make sure we're not clobbering these files in this hacked merge-changes tool */
/*textfile Pascalfile;*/
/*textfile pool;*/
ASCIIcode buffer[bufsize + 1];
boolean phaseone;
ASCIIcode bytemem[3][maxbytes + 1];
eightbits tokmem[4][maxtoks + 1];
sixteenbits bytestart[maxnames + 1];
sixteenbits tokstart[maxtexts + 1];
sixteenbits link[maxnames + 1];
sixteenbits ilk[maxnames + 1];
sixteenbits equiv[maxnames + 1];
sixteenbits textlink[maxtexts + 1];
namepointer nameptr;
namepointer stringptr;
integer byteptr[3];
integer poolchecksum;
textpointer textptr;
integer tokptr[4];
unsigned char z;
integer idfirst;
integer idloc;
integer doublechars;
sixteenbits hash[hashsize + 1], chophash[hashsize + 1];
ASCIIcode choppedid[unambiglength + 1];
ASCIIcode modtext[longestname + 1];
textpointer lastunnamed;
outputstate curstate;
outputstate stack[stacksize + 1];
integer stackptr;
unsigned char zo;
eightbits bracelevel;
integer curval;
ASCIIcode outbuf[outbufsize + 1];
integer outptr;
integer breakptr;
integer semiptr;
eightbits outstate;
integer outval, outapp;
ASCIIcode outsign;
schar lastsign;
ASCIIcode outcontrib[linelength + 1];
integer ii;
integer line;
integer otherline;
integer templine;
integer limit;
integer loc;
boolean inputhasended;
boolean changing;
ASCIIcode changebuffer[bufsize + 1];
integer changelimit;
namepointer curmodule;
boolean scanninghex;
eightbits nextcontrol;
textpointer currepltext;
short modulecount;
#ifdef TECTONIC_STAGE4_UNDERSCORES
const_cstring webname, chgname, pascalname, poolname;
#else
constcstring webname, chgname, pascalname, poolname;
#endif

#ifdef PKGW
static boolean pkgw_outputting_named_numeric_constant = false;
static boolean pkgw_outputting_string_constant = false;
static sixteenbits pkgw_cur_constant_id = 0;
#define pkgw_max_constants 8192
static namepointer pkgw_constant_ids[pkgw_max_constants];
static integer pkgw_constant_values[pkgw_max_constants];
static integer pkgw_n_constants = 0;

#define pkgw_max_val_stack 32

typedef enum _pkgw_val_stack_type {
    PVST_NAMED_CONSTANT,
    PVST_STRING_CONSTANT,
    PVST_NUMBER,
} pkgw_val_stack_type;

typedef struct _pkgw_val_stack_info {
    pkgw_val_stack_type type;
    integer sign;
    union {
	namepointer named;
	integer number; /* always >= 0 */
    } v;
} pkgw_val_stack_info;

static pkgw_val_stack_info pkgw_value_stack[pkgw_max_val_stack];
static integer pkgw_value_stack_size = 0;
static pkgw_val_stack_info pkgw_app_value_info;

static char *pkgw_string_values[maxnames + 1];

#endif

/* otangle.h */

void error (void);
void parsearguments (void);
void initialize (void);
void openinput (void);
boolean input_ln (textfile f);
void print_id (namepointer p);
namepointer id_lookup (eightbits t);
namepointer mod_lookup (sixteenbits l);
namepointer prefix_lookup (sixteenbits l);
void store_two_bytes (sixteenbits x);
void push_level (namepointer p);
void pop_level (void);
sixteenbits get_output (void);
void flushbuffer (void);
void app_val (integer v);
void send_out (eightbits t,sixteenbits v);
void send_sign (integer v);
#ifdef PKGW
void send_val(integer v, pkgw_val_stack_type pkgwtype, namepointer pkgwpnt);
#else
void send_val(integer v);
#endif
void send_the_output (void);
boolean linesdontmatch (void);
void primethechangebuffer (void);
void checkchange (void);
void getline (void);
eightbits controlcode (ASCIIcode c);
eightbits skipahead (void);
void skipcomment (void);
eightbits get_next (void);
void scan_numeric (namepointer p);
void scanrepl (eightbits t);
void definemacro (eightbits t);
void scanmodule (void);

/* back to otangle.c */

void error(void)
{
    integer j;
    integer k, l;
    if (phaseone) {
        if (changing)
            Fputs(stdout, ". (change file ");
        else
            Fputs(stdout, ". (");
        fprintf(stdout, "%s%ld%c\n", "l.", (long)line, ')');
        if (loc >= limit)
            l = limit;
        else
            l = loc;
        {
            register integer for_end;
            k = 1;
            for_end = l;
            if (k <= for_end)
                do
                    if (buffer[k - 1] == TAB_MARK)
                        putc(' ', stdout);
                    else
                        putc(xchr[buffer[k - 1]], stdout);
                while (k++ < for_end);
        }
        putc('\n', stdout);
        {
            register integer for_end;
            k = 1;
            for_end = l;
            if (k <= for_end)
                do
                    putc(' ', stdout);
                while (k++ < for_end);
        }
        {
            register integer for_end;
            k = l + 1;
            for_end = limit;
            if (k <= for_end)
                do
                    putc(xchr[buffer[k - 1]], stdout);
                while (k++ < for_end);
        }
        putc(' ', stdout);
    } else {

        fprintf(stdout, "%s%ld%c\n", ". (l.", (long)line, ')');
        {
            register integer for_end;
            j = 1;
            for_end = outptr;
            if (j <= for_end)
                do
                    putc(xchr[outbuf[j - 1]], stdout);
                while (j++ < for_end);
        }
        Fputs(stdout, "... ");
    }
    fflush(stdout);
    history = ERROR_MESSAGE;
}

void parsearguments(void)
{
#ifdef TECTONIC_STAGE4_UNDERSCORES
    getopt_struct longoptions[noptions + 1];
    integer getoptreturnval;
    c_int_type optionindex;
#else
    getoptstruct longoptions[noptions + 1];
    integer getoptreturnval;
    cinttype optionindex;
#endif
    integer currentoption;

    currentoption = 0;
    longoptions[currentoption].name = "help";
#ifdef TECTONIC_STAGE4_UNDERSCORES
    longoptions[currentoption].has_arg = 0;
#else
    longoptions[currentoption].hasarg = 0;
#endif
    longoptions[currentoption].flag = 0;
    longoptions[currentoption].val = 0;
    currentoption = currentoption + 1;
    longoptions[currentoption].name = "version";
#ifdef TECTONIC_STAGE4_UNDERSCORES
    longoptions[currentoption].has_arg = 0;
#else
    longoptions[currentoption].hasarg = 0;
#endif
    longoptions[currentoption].flag = 0;
    longoptions[currentoption].val = 0;
    currentoption = currentoption + 1;
    longoptions[currentoption].name = 0;
#ifdef TECTONIC_STAGE4_UNDERSCORES
    longoptions[currentoption].has_arg = 0;
#else
    longoptions[currentoption].hasarg = 0;
#endif
    longoptions[currentoption].flag = 0;
    longoptions[currentoption].val = 0;
    do {
#ifdef TECTONIC_STAGE4_UNDERSCORES
        getoptreturnval = getopt_long_only(argc, argv, "", longoptions,
                                         addressof(optionindex));
#else
        getoptreturnval = getoptlongonly(argc, argv, "", longoptions,
                                         addressof(optionindex));
#endif
        if (getoptreturnval == -1) {
            ;
        } else if (getoptreturnval == QUESTION_MARK) {
            usage("otangle");
        } else if ((strcmp(longoptions[optionindex].name, "help") == 0)) {
#ifdef TECTONIC_STAGE4_UNDERSCORES
            usage_help(OTANGLEHELP, nil);
#else
            usagehelp(OTANGLEHELP, nil);
#endif
        } else if ((strcmp(longoptions[optionindex].name, "version") == 0)) {
#ifdef TECTONIC_STAGE4_UNDERSCORES
            print_version_and_exit("This is OTANGLE, Version 4.4", nil,
                                "J. Plaice, Y. Haralambous, D.E. Knuth", nil);
#else
            printversionandexit("This is OTANGLE, Version 4.4", nil,
                                "J. Plaice, Y. Haralambous, D.E. Knuth", nil);
#endif
        }
    }
    while (!(getoptreturnval == -1));
    if ((optind + 1 != argc) && (optind + 2 != argc)) {
        fprintf(stderr, "%s%s\n", "otangle",
                ": Need one or two file arguments.");
        usage("otangle");
    }
#ifdef TECTONIC_STAGE4_UNDERSCORES
    webname = extend_filename(cmdline(optind), "web");
    if (optind + 2 == argc) {
        chgname = extend_filename(cmdline(optind + 1), "ch");
    }
    pascalname = basename_change_suffix(webname, ".web", ".p");
#else
    webname = extendfilename(cmdline(optind), "web");
    if (optind + 2 == argc) {
        chgname = extendfilename(cmdline(optind + 1), "ch");
    }
    pascalname = basenamechangesuffix(webname, ".web", ".p");
#endif
}

void initialize(void)
{
    unsigned char i;
    unsigned char wi;
    unsigned char zi;
    integer h;
    kpse_set_program_name(argv[0], "otangle");
    parsearguments();
    history = SPOTLESS;
    xchr[SPACE] = ' ';
    xchr[EXCLAM] = '!';
    xchr[DOUBLEQUOTE] = '"';
    xchr[OCTOTHORPE] = '#';
    xchr[DOLLARSIGN] = '$';
    xchr[PERCENT] = '%';
    xchr[AMPERSAND] = '&';
    xchr[SINGLE_QUOTE] = '\'';
    xchr[LEFT_PAREN] = '(';
    xchr[RIGHT_PAREN] = ')';
    xchr[ASTERISK] = '*';
    xchr[PLUS_SIGN] = '+';
    xchr[COMMA] = ',';
    xchr[MINUS_SIGN] = '-';
    xchr[PERIOD] = '.';
    xchr[FORWARD_SLASH] = '/';
    xchr[ASCII_0] = '0';
    xchr[ASCII_1] = '1';
    xchr[ASCII_2] = '2';
    xchr[ASCII_3] = '3';
    xchr[ASCII_4] = '4';
    xchr[ASCII_5] = '5';
    xchr[ASCII_6] = '6';
    xchr[ASCII_7] = '7';
    xchr[ASCII_8] = '8';
    xchr[ASCII_9] = '9';
    xchr[COLON] = ':';
    xchr[SEMICOLON] = ';';
    xchr[LESS_THAN_SIGN] = '<';
    xchr[EQUALS_SIGN] = '=';
    xchr[GREATER_THAN_SIGN] = '>';
    xchr[QUESTION_MARK] = '?';
    xchr[AT_SIGN] = '@';
    xchr[ASCII_A] = 'A';
    xchr[ASCII_B] = 'B';
    xchr[67] = 'C';
    xchr[ASCII_D] = 'D';
    xchr[ASCII_E] = 'E';
    xchr[ASCII_F] = 'F';
    xchr[71] = 'G';
    xchr[72] = 'H';
    xchr[ASCII_I] = 'I';
    xchr[74] = 'J';
    xchr[75] = 'K';
    xchr[76] = 'L';
    xchr[ASCII_M] = 'M';
    xchr[78] = 'N';
    xchr[ASCII_O] = 'O';
    xchr[ASCII_P] = 'P';
    xchr[81] = 'Q';
    xchr[82] = 'R';
    xchr[83] = 'S';
    xchr[ASCII_T] = 'T';
    xchr[85] = 'U';
    xchr[ASCII_V] = 'V';
    xchr[87] = 'W';
    xchr[ASCII_X] = 'X';
    xchr[89] = 'Y';
    xchr[ASCII_Z] = 'Z';
    xchr[LEFT_BRACKET] = '[';
    xchr[BACK_SLASH] = '\\';
    xchr[RIGHT_BRACKET] = ']';
    xchr[CARET] = '^';
    xchr[UNDERSCORE] = '_';
    xchr[BACKTICK] = '`';
    xchr[ASCII_a] = 'a';
    xchr[98] = 'b';
    xchr[99] = 'c';
    xchr[ASCII_d] = 'd';
    xchr[ASCII_e] = 'e';
    xchr[ASCII_f] = 'f';
    xchr[103] = 'g';
    xchr[104] = 'h';
    xchr[ASCII_i] = 'i';
    xchr[106] = 'j';
    xchr[107] = 'k';
    xchr[108] = 'l';
    xchr[ASCII_m] = 'm';
    xchr[ASCII_n] = 'n';
    xchr[ASCII_o] = 'o';
    xchr[ASCII_p] = 'p';
    xchr[113] = 'q';
    xchr[ASCII_r] = 'r';
    xchr[115] = 's';
    xchr[ASCII_t] = 't';
    xchr[117] = 'u';
    xchr[ASCII_v] = 'v';
    xchr[119] = 'w';
    xchr[ASCII_x] = 'x';
    xchr[ASCII_y] = 'y';
    xchr[ASCII_z] = 'z';
    xchr[LEFT_BRACE] = '{';
    xchr[PIPE] = '|';
    xchr[RIGHT_BRACE] = '}';
    xchr[TILDE] = '~';
    xchr[0] = ' ';
    xchr[127] = ' ';
    {
        register integer for_end;
        i = 1;
        for_end = 31;
        if (i <= for_end)
            do
                xchr[i] = chr(i);
            while (i++ < for_end);
    }
    {
        register integer for_end;
        i = 128;
        for_end = 255;
        if (i <= for_end)
            do
                xchr[i] = chr(i);
            while (i++ < for_end);
    }
    {
        register integer for_end;
        i = 0;
        for_end = 255;
        if (i <= for_end)
            do
                xord[chr(i)] = SPACE;
            while (i++ < for_end);
    }
    {
        register integer for_end;
        i = 1;
        for_end = 255;
        if (i <= for_end)
            do
                xord[xchr[i]] = i;
            while (i++ < for_end);
    }
    xord[' '] = SPACE;
    /*rewrite(Pascalfile, pascalname);*/
    {
        register integer for_end;
        wi = 0;
        for_end = 2;
        if (wi <= for_end)
            do {
                bytestart[wi] = 0;
                byteptr[wi] = 0;
            }
            while (wi++ < for_end);
    }
    bytestart[3] = 0;
    nameptr = 1;
    stringptr = NUMBER_CHARS;
    poolchecksum = 271828L;
    {
        register integer for_end;
        zi = 0;
        for_end = 3;
        if (zi <= for_end)
            do {
                tokstart[zi] = 0;
                tokptr[zi] = 0;
            }
            while (zi++ < for_end);
    }
    tokstart[4] = 0;
    textptr = 1;
    z = 1 % 4;
    ilk[0] = NORMAL;
    equiv[0] = 0;
    {
        register integer for_end;
        h = 0;
        for_end = hashsize - 1;
        if (h <= for_end)
            do {
                hash[h] = 0;
                chophash[h] = 0;
            }
            while (h++ < for_end);
    }
    lastunnamed = 0;
    textlink[0] = 0;
    scanninghex = false;
    modtext[0] = SPACE;

#ifdef PKGW
    for (h = 0; h < maxnames + 1; h++)
	pkgw_string_values[h] = NULL;
#endif
}

void openinput(void)
{
    webfile = kpse_open_file(webname, kpse_web_format);
    if (chgname)
        changefile = kpse_open_file(chgname, kpse_web_format);
}

boolean input_ln(textfile f)
{
    register boolean Result;
    integer finallimit;
    limit = 0;
    finallimit = 0;
    if (eof(f))
        Result = false;
    else {

        while (!eoln(f)) {

            buffer[limit] = xord[getc(f)];
            limit = limit + 1;
            if (buffer[limit - 1] != SPACE)
                finallimit = limit;
            if (limit == bufsize) {
                while (!eoln(f))
                    vgetc(f);
                limit = limit - 1;
                if (finallimit > limit)
                    finallimit = limit;
                {
                    putc('\n', stdout);
                    Fputs(stdout, "! Input line too long");
                }
                loc = 0;
                error();
            }
        }
#ifdef TECTONIC_STAGE4_UNDERSCORES
        read_ln(f);
#else
        readln(f);
#endif
        limit = finallimit;
        Result = true;
    }
    return Result;
}

/* identifiers */

void print_id(namepointer p)
{
    integer k;
    unsigned char w;
    if (p >= nameptr)
        Fputs(stdout, "IMPOSSIBLE");
    else {

        w = p % 3;
        {
            register integer for_end;
            k = bytestart[p];
            for_end = bytestart[p + 3] - 1;
            if (k <= for_end)
                do
                    putc(xchr[bytemem[w][k]], stdout);
                while (k++ < for_end);
        }
    }
}

namepointer id_lookup(eightbits t)
{
    eightbits c;
    integer i;
    integer h;
    integer k;
    unsigned char w;
    integer l;
    namepointer p, q;
    integer s;

    l = idloc - idfirst;
    h = buffer[idfirst];
    i = idfirst + 1;

    while (i < idloc) {
        h = (h + h + buffer[i]) % hashsize;
        i = i + 1;
    }

    p = hash[h];

    while (p != 0) {
        if (bytestart[p + 3] - bytestart[p] == l) {
            i = idfirst;
            k = bytestart[p];
            w = p % 3;
            while ((i < idloc) && (buffer[i] == bytemem[w][k])) {
                i = i + 1;
                k = k + 1;
            }

            if (i == idloc)
                goto found;
        }
        p = link[p];
    }

    p = nameptr;
    link[p] = hash[h];
    hash[h] = p;

found:

    if (p == nameptr || t != NORMAL) {
        if ((p != nameptr && t != NORMAL && ilk[p] == NORMAL) ||
	    (p == nameptr && t == NORMAL && buffer[idfirst] != DOUBLEQUOTE)) {
            i = idfirst;
            s = 0;
            h = 0;

            while ((i < idloc) && (s < unambiglength)) {
		choppedid[s] = buffer[i];
		h = (h + h + choppedid[s]) % hashsize;
		s = s + 1;
                i = i + 1;
            }
            choppedid[s] = 0;
        }

        if (p != nameptr) {
            if (ilk[p] == NORMAL) {
                if (t == NUMERIC) {
                    putc('\n', stdout);
                    Fputs(stdout, "! This identifier has already appeared");
                    error();
                }

                q = chophash[h];

                if (q == p) {
                    chophash[h] = equiv[p];
                } else {
                    while (equiv[q] != p)
                        q = equiv[q];
                    equiv[q] = equiv[p];
                }
            } else {
                putc('\n', stdout);
                Fputs(stdout, "! This identifier was defined before");
                error();
            }

            ilk[p] = t;
        } else {
            if (t == NORMAL && buffer[idfirst] != DOUBLEQUOTE) {
                q = chophash[h];

                while (q != 0) {
		    k = bytestart[q];
		    s = 0;
		    w = q % 3;

		    while (k < bytestart[q + 3] && s < unambiglength) {
			c = bytemem[w][k];

			if (choppedid[s] != c)
			    goto notfound;
			s = s + 1;
			k = k + 1;
		    }

		    if ((k == bytestart[q + 3]) && (choppedid[s] != 0))
			goto notfound;

		    putc('\n', stdout);
		    Fputs(stdout, "! Identifier conflict with ");

		    {
			register integer for_end;
			k = bytestart[q];
			for_end = bytestart[q + 3] - 1;
			if (k <= for_end) {
			    do
				putc(xchr[bytemem[w][k]], stdout);
			    while (k++ < for_end);
			}
		    }

		    error();
		    q = 0;

		notfound:
                    q = equiv[q];
                }

                equiv[p] = chophash[h];
                chophash[h] = p;
            }

            w = nameptr % 3;
            k = byteptr[w];

            if (k + l > maxbytes) {
                putc('\n', stdout);
                fprintf(stderr, "! Sorry, byte memory capacity exceeded");
                error();
                history = FATAL_MESSAGE;
                uexit(1);
            }

            if (nameptr > maxnames - 3) {
                putc('\n', stdout);
                fprintf(stderr, "! Sorry, name capacity exceeded");
                error();
                history = FATAL_MESSAGE;
                uexit(1);
            }

            i = idfirst;

            while (i < idloc) {
                bytemem[w][k] = buffer[i];
                k = k + 1;
                i = i + 1;
            }

            byteptr[w] = k;
            bytestart[nameptr + 3] = k;
            nameptr = nameptr + 1;

            if (buffer[idfirst] != DOUBLEQUOTE) {
                ilk[p] = t;
            } else {
                ilk[p] = NUMERIC; /* "strings are like numeric macros" */

                if (l - doublechars == 2) {
                    equiv[p] = buffer[idfirst + 1] + 0x40000000;
#ifdef PKGW
		    pkgw_string_values[p] = malloc (2);
		    /* yay, assume malloc never fails */
		    pkgw_string_values[p][0] = buffer[idfirst + 1];
		    pkgw_string_values[p][1] = '\0';
#endif
                } else {
#ifdef PKGW
		    int pkgw_idx = 0;
#endif

                    if (stringptr == NUMBER_CHARS) {
#ifdef TECTONIC_STAGE4_UNDERSCORES
                        poolname = basename_change_suffix(webname, ".web", ".pool");
#else
                        poolname = basenamechangesuffix(webname, ".web", ".pool");
#endif
                        /*rewrite(pool, poolname);*/
                    }

                    equiv[p] = stringptr + 0x40000000;
                    l = l - doublechars - 1;

                    if (l > 99) {
                        putc('\n', stdout);
                        Fputs(stdout, "! Preprocessed string is too long");
                        error();
                    }

#ifdef PKGW
		    pkgw_string_values[p] = malloc (l + 1);
		    /* yay, assume malloc never fails */
		    memset (pkgw_string_values[p], 0, l + 1);
#endif

                    stringptr = stringptr + 1;
                    /*fprintf(pool, "%c%c", xchr[ASCII_0 + l / 10], xchr[ASCII_0 + l % 10]);*/

                    poolchecksum = poolchecksum + poolchecksum + l;
                    while (poolchecksum > 0x1FFFFFB7)
                        poolchecksum = poolchecksum - 0x1FFFFFB7;

                    i = idfirst + 1;

                    while (i < idloc) {
                        /*putc(xchr[buffer[i]], pool);*/
#ifdef PKGW
			pkgw_string_values[p][pkgw_idx++] = xchr[buffer[i]];
#endif
                        poolchecksum = poolchecksum + poolchecksum + buffer[i];

                        while (poolchecksum > 0x1FFFFFB7)
                            poolchecksum = poolchecksum - 0x1FFFFFB7;

                        if (buffer[i] == DOUBLEQUOTE || buffer[i] == AT_SIGN)
                            i = i + 2;
                        else
                            i = i + 1;
                    }

                    /*putc('\n', pool);*/
                }
            }
        }
    }

    return p;
}

/* modules */

namepointer mod_lookup(sixteenbits l)
{
    /* 31 */ register namepointer Result;
    unsigned char c;
    integer j;
    integer k;
    unsigned char w;
    namepointer p;
    namepointer q;
    c = GREATER;
    q = 0;
    p = ilk[0];
    while (p != 0) {

        {
            k = bytestart[p];
            w = p % 3;
            c = EQUAL;
            j = 1;
            while ((k < bytestart[p + 3]) && (j <= l)
                   && (modtext[j] == bytemem[w][k])) {

                k = k + 1;
                j = j + 1;
            }
            if (k == bytestart[p + 3]) {

                if (j > l)
                    c = EQUAL;
                else
                    c = EXTENSION;
            } else if (j > l)
                c = PREFIX;
            else if (modtext[j] < bytemem[w][k])
                c = LESS;
            else
                c = GREATER;
        }
        q = p;
        if (c == LESS)
            p = link[q];
        else if (c == GREATER)
            p = ilk[q];
        else
            goto found;
    }
    w = nameptr % 3;
    k = byteptr[w];
    if (k + l > maxbytes) {
        putc('\n', stdout);
        fprintf(stderr, "%s%s%s", "! Sorry, ", "byte memory",
                " capacity exceeded");
        error();
        history = FATAL_MESSAGE;
        uexit(1);
    }
    if (nameptr > maxnames - 3) {
        putc('\n', stdout);
        fprintf(stderr, "%s%s%s", "! Sorry, ", "name", " capacity exceeded");
        error();
        history = FATAL_MESSAGE;
        uexit(1);
    }
    p = nameptr;
    if (c == LESS)
        link[q] = p;
    else
        ilk[q] = p;
    link[p] = 0;
    ilk[p] = 0;
    c = EQUAL;
    equiv[p] = 0;
    {
        register integer for_end;
        j = 1;
        for_end = l;
        if (j <= for_end)
            do
                bytemem[w][k + j - 1] = modtext[j];
            while (j++ < for_end);
    }
    byteptr[w] = k + l;
    bytestart[nameptr + 3] = k + l;
    nameptr = nameptr + 1;
 found:if (c != EQUAL) {
        {
            putc('\n', stdout);
            Fputs(stdout, "! Incompatible section names");
            error();
        }
        p = 0;
    }
    Result = p;
    return Result;
}

namepointer prefix_lookup(sixteenbits l)
{
    namepointer Result;
    unsigned char c;
    integer count;
    integer j;
    integer k;
    unsigned char w;
    namepointer p;
    namepointer q;
    namepointer r;

    q = 0;
    p = ilk[0];
    count = 0;
    r = 0;

    while (p != 0) {

        {
            k = bytestart[p];
            w = p % 3;
            c = EQUAL;
            j = 1;
            while ((k < bytestart[p + 3]) && (j <= l)
                   && (modtext[j] == bytemem[w][k])) {

                k = k + 1;
                j = j + 1;
            }
            if (k == bytestart[p + 3]) {

                if (j > l)
                    c = EQUAL;
                else
                    c = EXTENSION;
            } else if (j > l)
                c = PREFIX;
            else if (modtext[j] < bytemem[w][k])
                c = LESS;
            else
                c = GREATER;
        }

        if (c == LESS)
            p = link[p];
        else if (c == GREATER)
            p = ilk[p];
        else {
            r = p;
            count = count + 1;
            q = ilk[p];
            p = link[p];
        }

        if (p == 0) {
            p = q;
            q = 0;
        }
    }

    if (count != 1) {
        if (count == 0) {
            putc('\n', stdout);
            Fputs(stdout, "! Name does not match");
            error();
        } else {
            putc('\n', stdout);
            Fputs(stdout, "! Ambiguous prefix");
            error();
        }
    }

    return r;
}

/* tokens */

void store_two_bytes(sixteenbits x)
{
    if (tokptr[z] + 2 > maxtoks) {
        putc('\n', stdout);
        fprintf(stderr, "%s%s%s", "! Sorry, ", "token", " capacity exceeded");
        error();
        history = FATAL_MESSAGE;
        uexit(1);
    }
    tokmem[z][tokptr[z]] = x / 256;
    tokmem[z][tokptr[z] + 1] = x % 256;
    tokptr[z] += 2;
}

/* stack */

void push_level(namepointer p)
{
    if (stackptr == stacksize) {
        putc('\n', stdout);
        fprintf(stderr, "%s%s%s", "! Sorry, ", "stack", " capacity exceeded");
        error();
        history = FATAL_MESSAGE;
        uexit(1);
    } else {
        stack[stackptr] = curstate;
        stackptr = stackptr + 1;
        curstate.namefield = p;
        curstate.replfield = equiv[p];
        zo = curstate.replfield % 4;
        curstate.bytefield = tokstart[curstate.replfield];
        curstate.endfield = tokstart[curstate.replfield + 4];
        curstate.modfield = 0;
    }
}


void pop_level(void)
{
    if (textlink[curstate.replfield] == 0) {
        if (ilk[curstate.namefield] == PARAMETRIC) {
            nameptr = nameptr - 1;
            textptr = textptr - 1;
            z = textptr % 4;
            tokptr[z] = tokstart[textptr];
        }
    } else if (textlink[curstate.replfield] < maxtexts) {
        curstate.replfield = textlink[curstate.replfield];
        zo = curstate.replfield % 4;
        curstate.bytefield = tokstart[curstate.replfield];
        curstate.endfield = tokstart[curstate.replfield + 4];
        goto exit;
    }

    stackptr--;

    if (stackptr > 0) {
        curstate = stack[stackptr];
        zo = curstate.replfield % 4;
    }

exit:
    ;
}

#ifdef PKGW
static int pkgw_n_modules_output = 0;
#define pkgw_magic_insertion_point 3
static int pkgw_insertion_state = 0;
#endif

sixteenbits get_output(void)
{
    /* Get the next non-macro token of output. */

    sixteenbits Result;
    sixteenbits a;
    eightbits b;
    sixteenbits bal;
    integer k;
    unsigned char w;

#if 0
    /* Here we insert the values of symbolic constants very close to the top
     * of the output file. */

    if (pkgw_n_modules_output == pkgw_magic_insertion_point && pkgw_n_constants > 0) {
	switch (pkgw_insertion_state) {
	case 0: /* VERBATIM to manually prefix a letter to avoid keyword clashes */
	    pkgw_insertion_state = 1;
	    return VERBATIM;
	case 1: /* "X" prefix */
	    pkgw_insertion_state = 2;
	    return ASCII_X;
	case 2: /* end-of-VERBATIM marker */
	    pkgw_insertion_state = 3;
	    return VERBATIM;
	case 3: /* IDENTIFIER of the constant */
	    curval = pkgw_constant_ids[pkgw_n_constants-1];
	    pkgw_insertion_state = 4;
	    return IDENTIFIER;
	case 4: /* equals sign */
	    pkgw_insertion_state = 5;
	    return EQUALS_SIGN;
	case 5: /* the actual value of the constant */
	    curval = pkgw_constant_values[pkgw_n_constants-1];
	    pkgw_insertion_state = 6;
	    return NUMBER;
	case 6: /* semicolon */
	    pkgw_n_constants--; /* this one is done */
	    pkgw_insertion_state = 0;
	    return SEMICOLON;
	}
    }
#endif

restart:
    if (stackptr == 0) {
        a = 0;
        goto found;
    }

    if (curstate.bytefield == curstate.endfield) {
        curval = -(integer) curstate.modfield;
        pop_level();
        if (curval == 0)
            goto restart;
        a = MODULE_NUMBER;
        goto found;
    }

    a = tokmem[zo][curstate.bytefield];
    curstate.bytefield = curstate.bytefield + 1;

    if (a < 128) {
        if (a == PARAM) {
            push_level(nameptr - 1);
            goto restart;
        } else
            goto found;
    }

    a = (a - 128) * 256 + tokmem[zo][curstate.bytefield];
    curstate.bytefield = curstate.bytefield + 1;

    if (a < 024000) {
        switch (ilk[a]) {
        case NORMAL:
	    curval = a;
	    a = IDENTIFIER;
            break;
        case NUMERIC:
	    curval = equiv[a] - 0x40000000;
#ifdef PKGW
	    /* If we're yielding a numeric value that's obtained from a named
	     * constant. set some magic variables so that callers can be aware
	     * of that fact and take steps to preserve the name of the
	     * originating constant. Whenever this code path is invoked, the
	     * return value will eventually be handled by the NUMBER case of
	     * the main switch in `send_the_output()`. */
	    {
		unsigned char w = a % 3;
		integer k = bytestart[a];
		char c = xchr[bytemem[w][k]];

		if (c == DOUBLEQUOTE) {
		    /* This is a string constant. */
		    pkgw_outputting_named_numeric_constant = false;
		    pkgw_outputting_string_constant = true;
		} else {
		    pkgw_outputting_named_numeric_constant = true;
		    pkgw_outputting_string_constant = false;
		}

		pkgw_cur_constant_id = a;
	    }
#endif
	    a = NUMBER;
            break;
        case SIMPLE:
	    push_level(a);
	    goto restart;
            break;
        case PARAMETRIC:
	    while ((curstate.bytefield == curstate.endfield) && (stackptr > 0))
		pop_level();

	    if (stackptr == 0 || tokmem[zo][curstate.bytefield] != LEFT_PAREN) {
		putc('\n', stdout);
		Fputs(stdout, "! No parameter given for ");
		print_id(a);
		error();
		goto restart;
	    }

	    bal = 1;
	    curstate.bytefield = curstate.bytefield + 1;

	    while (true) {
		b = tokmem[zo][curstate.bytefield];
		curstate.bytefield = curstate.bytefield + 1;

		if (b == 0)
		    store_two_bytes(nameptr + 32767);
		else {
		    if (b >= 128) {
			if (tokptr[z] == maxtoks) {
			    putc('\n', stdout);
			    fprintf (stderr, "! Sorry, token capacity exceeded");
			    error();
			    history = FATAL_MESSAGE;
			    uexit(1);
			}

			tokmem[z][tokptr[z]] = b;
			tokptr[z] = tokptr[z] + 1;
			b = tokmem[zo][curstate.bytefield];
			curstate.bytefield = curstate.bytefield + 1;
		    } else {
			switch (b) {
			case LEFT_PAREN:
			    bal++;
			    break;
			case RIGHT_PAREN:
			    bal--;
			    if (bal == 0)
				goto done;
			    break;
			case SINGLE_QUOTE:
			    do {
				if (tokptr[z] == maxtoks) {
				    putc('\n', stdout);
				    fprintf (stderr, "! Sorry, token capacity exceeded");
				    error();
				    history = FATAL_MESSAGE;
				    uexit(1);
				}
				tokmem[z][tokptr[z]] = b;
				tokptr[z]++;
				b = tokmem[zo][curstate.bytefield];
				curstate.bytefield = curstate.bytefield + 1;
			    } while (b != SINGLE_QUOTE);
			    break;
			}
		    }

		    if (tokptr[z] == maxtoks) {
			putc('\n', stdout);
			fprintf(stderr, "! Sorry, token capacity exceeded");
			error();
			history = FATAL_MESSAGE;
			uexit(1);
		    }

		    tokmem[z][tokptr[z]] = b;
		    tokptr[z] = tokptr[z] + 1;
		}
	    }

	done:
	    equiv[nameptr] = textptr;
	    ilk[nameptr] = SIMPLE;
	    w = nameptr % 3;
	    k = byteptr[w];

	    if (nameptr > maxnames - 3) {
		putc('\n', stdout);
		fprintf(stderr, "! Sorry, name capacity exceeded");
		error();
		history = FATAL_MESSAGE;
		uexit(1);
	    }

	    bytestart[nameptr + 3] = k;
	    nameptr = nameptr + 1;

	    if (textptr > maxtexts - 4) {
		putc('\n', stdout);
		fprintf(stderr, "! Sorry, text capacity exceeded");
		error();
		history = FATAL_MESSAGE;
		uexit(1);
	    }

	    textlink[textptr] = 0;
	    tokstart[textptr + 4] = tokptr[z];
	    textptr = textptr + 1;
	    z = textptr % 4;
	    push_level(a);
	    goto restart;
	    break;

        default:
	    putc('\n', stdout);
	    fprintf(stderr, "! This can't happen (output)");
	    error();
	    history = FATAL_MESSAGE;
	    uexit(1);
            break;
        }
        goto found;
    }

    if (a < 050000) {
        a = a - 024000;
        if (equiv[a] != 0)
            push_level(a);
        else if (a != 0) {
	    putc('\n', stdout);
	    Fputs(stdout, "! Not present: <");
            print_id(a);
            putc('>', stdout);
            error();
        }
        goto restart;
    }

    curval = a - 050000;
    a = MODULE_NUMBER;
    curstate.modfield = curval;

#ifdef PKGW
    pkgw_n_modules_output++;
#endif

found:
    return a;
}


void flushbuffer(void)
{
    integer k;
    integer b;
    b = breakptr;
    if ((semiptr != 0) && (outptr - semiptr <= linelength))
        breakptr = semiptr;
    {
        register integer for_end;
        k = 1;
        for_end = breakptr;
        if (k <= for_end)
            do {
                /*No Pascal in hacked merge-changes program */
                /*putc(xchr[outbuf[k - 1]], Pascalfile);*/
            } while (k++ < for_end);
    }
    /*putc('\n', Pascalfile);*/
    line = line + 1;
    if (line % 100 == 0) {
        putc('.', stdout);
        if (line % 500 == 0)
            fprintf(stdout, "%ld", (long)line);
        fflush(stdout);
    }
    if (breakptr < outptr) {
        if (outbuf[breakptr] == SPACE) {
            breakptr = breakptr + 1;
            if (breakptr > b)
                b = breakptr;
        }
        {
            register integer for_end;
            k = breakptr;
            for_end = outptr - 1;
            if (k <= for_end)
                do
                    outbuf[k - breakptr] = outbuf[k];
                while (k++ < for_end);
        }
    }
    outptr = outptr - breakptr;
    breakptr = b - breakptr;
    semiptr = 0;
    if (outptr > linelength) {
        {
            putc('\n', stdout);
            Fputs(stdout, "! Long line must be truncated");
            error();
        }
        outptr = linelength;
    }
}

void app_val(integer v)
{
    integer k;

#ifdef PKGW
    boolean any_constants = false;

    if (pkgw_value_stack_size) {
	int i;

	for (i = 0; i < pkgw_value_stack_size; i++) {
	    if (pkgw_value_stack[i].type != PVST_NUMBER) {
		any_constants = true;
		break;
	    }
	}
    }
#endif

    k = outbufsize;
    do {
        outbuf[k] = v % 10;
        v = v / 10;
        k = k - 1;
    }
    while (!(v == 0));
    do {
        k = k + 1;
        {
            outbuf[outptr] = outbuf[k] + ASCII_0;
            outptr = outptr + 1;
        }
    }
    while (!(k == outbufsize));

#ifdef PKGW
    if (any_constants) {
	int i;
	integer k;
	unsigned char w;
	integer for_end;
	integer p;
	/* embed as a Pascal comment XXX totally ignoring buffer overflows! */
	if (bracelevel == 0)
	    outbuf[outptr] = LEFT_BRACE;
	else
	    outbuf[outptr] = LEFT_BRACKET;
	outptr++;

	for (i = 0; i < pkgw_value_stack_size; i++) {
	    if (i > 0) {
		outbuf[outptr] = SPACE;
		outptr++;
	    }

	    if (pkgw_value_stack[i].sign < 0) {
		outbuf[outptr] = MINUS_SIGN;
		outptr++;
	    }

	    switch (pkgw_value_stack[i].type) {
	    case PVST_NAMED_CONSTANT:
		p = pkgw_value_stack[i].v.named;
		w = p % 3;
		k = bytestart[p];
		for_end = bytestart[p + 3] - 1;
		if (k <= for_end) {
		    do {
			outbuf[outptr] = xchr[bytemem[w][k]];
			outptr++;
		    } while (k++ < for_end);
		}
		break;

	    case PVST_STRING_CONSTANT:
	    {
		char *c;

		p = pkgw_value_stack[i].v.named;
		c = pkgw_string_values[p];

		if (c == NULL) {
		    /*XXX I think this is a single character */
		} else {
		    outbuf[outptr++] = DOUBLEQUOTE;

		    for (; *c; c++) {
			char v = *c;
			if (v == '{' || v == '}')
			    v = '_';
			outbuf[outptr] = v;
			outptr++;
		    }

		    outbuf[outptr++] = DOUBLEQUOTE;
		}

		break;
	    }
	    case PVST_NUMBER:
		k = outbufsize;
		v = pkgw_value_stack[i].v.number;
		do {
		    outbuf[k] = v % 10;
		    v = v / 10;
		    k = k - 1;
		} while (v != 0);
		do {
		    k = k + 1;
		    outbuf[outptr] = outbuf[k] + ASCII_0;
		    outptr = outptr + 1;
		} while (k != outbufsize);
		break;
	    }
	}

	if (bracelevel == 0)
	    outbuf[outptr] = RIGHT_BRACE;
	else
	    outbuf[outptr] = RIGHT_BRACKET;
	outptr++;
    }

    pkgw_value_stack_size = 0;
#endif
}

void send_out(eightbits t, sixteenbits v)
{
    integer k;

restart:
    switch (outstate) {
    case NUM_OR_ID:
        if (t != FRAC) {
            breakptr = outptr;
            if (t == IDENT) {
                outbuf[outptr] = SPACE;
                outptr = outptr + 1;
            }
        }
        break;
    case SIGN:
	outbuf[outptr] = COMMA - outapp;
	outptr = outptr + 1;
	if (outptr > linelength)
	    flushbuffer();
	breakptr = outptr;
        break;
    case SIGN_VAL:
    case SIGN_VAL_SIGN:
	if ((outval < 0) || ((outval == 0) && (lastsign < 0))) {
	    outbuf[outptr] = MINUS_SIGN;
	    outptr = outptr + 1;
	} else if (outsign > 0) {
	    outbuf[outptr] = outsign;
	    outptr = outptr + 1;
	}

	app_val(abs(outval));
	if (outptr > linelength)
	    flushbuffer();
	outstate = outstate - 2;
	goto restart;
        break;
    case SIGN_VAL_VAL:
	if (t == FRAC ||
	    ((t == IDENT && v == 3 && (
		  (outcontrib[1] == ASCII_D
		   && outcontrib[2] == ASCII_I
		   && outcontrib[3] == ASCII_V) ||
		  (outcontrib[1] == ASCII_d
		   && outcontrib[2] == ASCII_i
		   && outcontrib[3] == ASCII_v) ||
		  (outcontrib[1] == ASCII_M
		   && outcontrib[2] == ASCII_O
		   && outcontrib[3] == ASCII_D) ||
		  (outcontrib[1] == ASCII_m
		   && outcontrib[2] == ASCII_o
		   && outcontrib[3] == ASCII_d)
		  )) ||
	     (t == MISC && (v == ASTERISK || v == FORWARD_SLASH)))) {
	    if (outval < 0 || (outval == 0 && lastsign < 0)) {
		outbuf[outptr] = MINUS_SIGN;
		outptr = outptr + 1;
	    } else if (outsign > 0) {
		outbuf[outptr] = outsign;
		outptr = outptr + 1;
	    }

	    app_val(abs(outval));
	    if (outptr > linelength)
		flushbuffer();
	    outsign = PLUS_SIGN;
	    outval = outapp;
#ifdef PKGW
	    pkgw_value_stack[0] = pkgw_app_value_info;
	    pkgw_value_stack_size = 1;
#endif
	} else {
	    outval = outval + outapp;
#ifdef PKGW
	    /* Web collapses the values, but we want to record how they were
	     * derived, so we append to the stack. */
	    pkgw_value_stack[pkgw_value_stack_size] = pkgw_app_value_info;
	    pkgw_value_stack_size++;
#endif
	}

	outstate = SIGN_VAL;
	goto restart;
        break;
    case MISC:
        if (t != FRAC)
            breakptr = outptr;
        break;
    }

    if (t != MISC) {
        register integer for_end;
        k = 1;
        for_end = v;
        if (k <= for_end)
            do {
                outbuf[outptr] = outcontrib[k];
                outptr = outptr + 1;
            }
            while (k++ < for_end);
    } else {
        outbuf[outptr] = v;
        outptr = outptr + 1;
    }

    if (outptr > linelength)
        flushbuffer();

    if ((t == MISC) && ((v == SEMICOLON) || (v == RIGHT_BRACE))) {
        semiptr = outptr;
        breakptr = outptr;
    }

    if (t >= IDENT)
        outstate = NUM_OR_ID;
    else
        outstate = MISC;
}

void send_sign(integer v)
{
    switch (outstate) {
    case SIGN:
    case SIGN_VAL_SIGN:
        outapp = outapp * v; /* already trailing sign: just possibly change its meaning */
#ifdef PKGW
	pkgw_app_value_info.sign *= v;
#endif
        break;
    case SIGN_VAL:
	outapp = v;
	outstate = SIGN_VAL_SIGN; /* trailing single value: just append this sign */
#ifdef PKGW
	{
	    int sgn = (v >= 0) ? 1 : -1;
	    pkgw_app_value_info.type = PVST_NUMBER;
	    pkgw_app_value_info.sign = sgn;
	    pkgw_app_value_info.v.number = sgn * v;
	}
#endif
        break;
    case SIGN_VAL_VAL:
	/* we effectively have (sign1) (val1) (sign2) (val2) and want to append (sign3).
	 * We do this by collapsing (val1) (sign2) (val2) into (val*) to yield
	 * (sign1) (val*) (sign3) */
	outval = outval + outapp;
	outapp = v;
	outstate = SIGN_VAL_SIGN;
#ifdef PKGW
	pkgw_value_stack[pkgw_value_stack_size] = pkgw_app_value_info;
	pkgw_value_stack_size++;

	{
	    int sgn = (v >= 0) ? 1 : -1;
	    pkgw_app_value_info.type = PVST_NUMBER;
	    pkgw_app_value_info.sign = sgn;
	    pkgw_app_value_info.v.number = sgn * v;
	}
#endif
        break;
    default:
	breakptr = outptr;
	outapp = v;
	outstate = SIGN; /* nothing: go into trailing-sign mode */
#ifdef PKGW
	{
	    int sgn = (v >= 0) ? 1 : -1;
	    pkgw_app_value_info.type = PVST_NUMBER;
	    pkgw_app_value_info.sign = sgn;
	    pkgw_app_value_info.v.number = sgn * v;
	}
#endif
        break;
    }

    lastsign = outapp;
}

#ifdef PKGW
void send_val(integer v, pkgw_val_stack_type pkgwtype, namepointer pkgwpnt)
#else
void send_val(integer v)
#endif
{
    switch (outstate) {
    case NUM_OR_ID:
	if (outptr == breakptr + 3 || (outptr == breakptr + 4 && outbuf[breakptr] == SPACE)) {
	    if (((outbuf[outptr - 3] == ASCII_D)
		 && (outbuf[outptr - 2] == ASCII_I)
		 && (outbuf[outptr - 1] == ASCII_V))
		|| ((outbuf[outptr - 3] == ASCII_d)
		    && (outbuf[outptr - 2] == ASCII_i)
		    && (outbuf[outptr - 1] == ASCII_v))
		|| ((outbuf[outptr - 3] == ASCII_M)
		    && (outbuf[outptr - 2] == ASCII_O)
		    && (outbuf[outptr - 1] == ASCII_D))
		|| ((outbuf[outptr - 3] == ASCII_m)
		    && (outbuf[outptr - 2] == ASCII_o)
		    && (outbuf[outptr - 1] == ASCII_d)))
		goto bad_case;
	}
	/* The most recent thing in the buffer was a number-type thing, and we
	 * just got another number type thing. Web treats this as "math" on
	 * two values that are combined with a space rather than a plus sign.
	 * For constant-name tracking, we reset the buffer. */
	outsign = SPACE;
	outstate = SIGN_VAL;
	outval = v;
#ifdef PKGW
	pkgw_value_stack[0].type = pkgwtype;
	if (pkgwtype != PVST_NUMBER) {
	    pkgw_value_stack[0].sign = 1;
	    pkgw_value_stack[0].v.named = pkgwpnt;
	} else {
	    int sgn = (v >= 0) ? 1 : -1;
	    pkgw_value_stack[0].sign = sgn;
	    pkgw_value_stack[0].v.number = sgn * v;
	}
	pkgw_value_stack_size = 1;
#endif
	breakptr = outptr;
	lastsign = 1;
        break;

    case MISC:
	if (outptr == breakptr + 1 && (outbuf[breakptr] == ASTERISK || outbuf[breakptr] == FORWARD_SLASH))
	    goto bad_case;
	/* This is a number following nothing special. For constant-tracking,
	 * we have to initialize the buffer. */
	outsign = 0;
	outstate = SIGN_VAL;
	outval = v;
#ifdef PKGW
	pkgw_value_stack[0].type = pkgwtype;
	if (pkgwtype != PVST_NUMBER) {
	    pkgw_value_stack[0].sign = 1;
	    pkgw_value_stack[0].v.named = pkgwpnt;
	} else {
	    int sgn = (v >= 0) ? 1 : -1;
	    pkgw_value_stack[0].sign = sgn;
	    pkgw_value_stack[0].v.number = sgn * v;
	}
	pkgw_value_stack_size = 1;
#endif
	breakptr = outptr;
	lastsign = 1;
        break;

    case SIGN:
	/* There's a value and then a plus or minus sign, whose polarity is
	 * stored in out_app. We just got a new value. The output code merges
	 * them, but for tracking purposes we want to record that we just
	 * combined two different values. */
	outsign = PLUS_SIGN;
	outstate = SIGN_VAL;
	outval = outapp * v;
#ifdef PKGW
	pkgw_app_value_info.v.number *= v;
	if (pkgw_app_value_info.v.number < 0) {
	    pkgw_app_value_info.v.number *= -1;
	    pkgw_app_value_info.sign *= -1;
	}

	pkgw_value_stack[pkgw_value_stack_size] = pkgw_app_value_info;
	pkgw_value_stack_size++;
#endif
        break;

    case SIGN_VAL:
	/* Theres a number, a sign, another number, and now apparently yet
	 * another number. As noted below: there's no way this can be
	 * valid. */
	outstate = SIGN_VAL_VAL;
	outapp = v;
#ifdef PKGW
	pkgw_app_value_info.type = pkgwtype;
	if (pkgwtype != PVST_NUMBER) {
	    pkgw_app_value_info.sign = 1;
	    pkgw_app_value_info.v.named = pkgwpnt;
	} else {
	    int sgn = (v >= 0) ? 1 : -1;
	    pkgw_app_value_info.sign = sgn;
	    pkgw_app_value_info.v.number = sgn * v;
	}
#endif
	putc('\n', stdout);
	Fputs(stdout, "! Two numbers occurred without a sign between them");
	error();
        break;

    case SIGN_VAL_SIGN:
	/* There's a sign, a number, another sign, and now another number.
	 * We can collapse this into the SIGN_VAL_VAL state. */
	outstate = SIGN_VAL_VAL;
	outapp = outapp * v;
#ifdef PKGW
	pkgw_app_value_info.v.number *= v;
	if (pkgw_app_value_info.v.number < 0) {
	    pkgw_app_value_info.v.number *= -1;
	    pkgw_app_value_info.sign = -1;
	}
#endif
        break;

    case SIGN_VAL_VAL:
	/* There's a sign, a number, another sign, another number, and now yet
	 * another number. As above, this is definitely not valid. */
	outval = outval + outapp;
	outapp = v;
#ifdef PKGW
	pkgw_value_stack[pkgw_value_stack_size] = pkgw_app_value_info;
	pkgw_value_stack_size++;

	pkgw_app_value_info.type = pkgwtype;
	if (pkgwtype != PVST_NUMBER) {
	    pkgw_app_value_info.sign = 1;
	    pkgw_app_value_info.v.named = pkgwpnt;
	} else {
	    int sgn = (v >= 0) ? 1 : -1;
	    pkgw_app_value_info.sign = sgn;
	    pkgw_app_value_info.v.number = sgn * v;
	}
#endif
	putc('\n', stdout);
	Fputs(stdout, "! Two numbers occurred without a sign between them");
	error();
        break;

    default:
        goto bad_case;
        break;
    }

    return;

bad_case:
#ifdef PKGW
    /* NOTE: if v < 0 we do not need a negation operation or anything; and if
     * it's a scalar integer, don't bother with anything */
    if (pkgwtype != PVST_NUMBER) {
	pkgw_value_stack[0].type = pkgwtype;
	pkgw_value_stack[0].sign = 1;
	pkgw_value_stack[0].v.named = pkgwpnt;
	pkgw_value_stack_size = 1;
    }
#endif
    if (v >= 0) {
        if (outstate == NUM_OR_ID) {
            breakptr = outptr;
	    outbuf[outptr] = SPACE;
	    outptr = outptr + 1;
        }
        app_val(v);
        if (outptr > linelength)
            flushbuffer();
        outstate = NUM_OR_ID;
    } else {
	outbuf[outptr] = LEFT_PAREN;
	outptr = outptr + 1;
	outbuf[outptr] = MINUS_SIGN;
	outptr = outptr + 1;
        app_val(-(integer) v);
	outbuf[outptr] = RIGHT_PAREN;
	outptr = outptr + 1;
        if (outptr > linelength)
            flushbuffer();
        outstate = MISC;
    }
}

void send_the_output(void)
{
    eightbits curchar;
    integer k;
    integer j;
    unsigned char w;
    integer n;

    while (stackptr > 0) {
        curchar = get_output();

    reswitch:
	switch (curchar) {
        case 0:
            break;

        case ASCII_A:
        case ASCII_B:
        case 67:
        case ASCII_D:
        case ASCII_E:
        case ASCII_F:
        case 71:
        case 72:
        case ASCII_I:
        case 74:
        case 75:
        case 76:
        case ASCII_M:
        case 78:
        case ASCII_O:
        case ASCII_P:
        case 81:
        case 82:
        case 83:
        case ASCII_T:
        case 85:
        case ASCII_V:
        case 87:
        case ASCII_X:
        case 89:
        case ASCII_Z:
        case ASCII_a:
        case 98:
        case 99:
        case ASCII_d:
        case ASCII_e:
        case ASCII_f:
        case 103:
        case 104:
        case ASCII_i:
        case 106:
        case 107:
        case 108:
        case ASCII_m:
        case ASCII_n:
        case ASCII_o:
        case ASCII_p:
        case 113:
        case ASCII_r:
        case 115:
        case ASCII_t:
        case 117:
        case ASCII_v:
        case 119:
        case ASCII_x:
        case ASCII_y:
        case ASCII_z:
	    outcontrib[1] = curchar;
	    send_out(IDENT, 1);
            break;

        case IDENTIFIER:
	    k = 0;
	    j = bytestart[curval];
	    w = curval % 3;
	    while (k < maxidlength && j < bytestart[curval + 3]) {
		k = k + 1;
		outcontrib[k] = bytemem[w][j];
		j = j + 1;
	    }
	    send_out(IDENT, k);
            break;

        case ASCII_0:
        case ASCII_1:
        case ASCII_2:
        case ASCII_3:
        case ASCII_4:
        case ASCII_5:
        case ASCII_6:
        case ASCII_7:
        case ASCII_8:
        case ASCII_9:
	    n = 0;
	    do {
		curchar = curchar - ASCII_0;

		if (n >= 0xCCCCCCC) {
		    putc('\n', stdout);
		    Fputs(stdout, "! Constant too big");
		    error();
		} else
		    n = 10 * n + curchar;

		curchar = get_output();
	    } while (!((curchar > ASCII_9) || (curchar < ASCII_0)));

#ifdef PKGW
	    send_val(n, PVST_NUMBER, 0);
#else
	    send_val(n);
#endif
	    k = 0;

	    if (curchar == ASCII_e)
		curchar = ASCII_E;

	    if (curchar == ASCII_E)
		goto get_fraction;
	    else
		goto reswitch;
            break;

        case RIGHT_BRACE:
#ifdef PKGW
	    send_val(poolchecksum, PVST_NUMBER, 0);
#else
	    send_val(poolchecksum);
#endif
            break;

        case OCTAL:
	    n = 0;
	    curchar = ASCII_0;
	    do {
		curchar = curchar - ASCII_0;
		if (n >= 0x10000000) {
		    putc('\n', stdout);
		    Fputs(stdout, "! Constant too big");
		    error();
		} else
		    n = 8 * n + curchar;
		curchar = get_output();
	    } while (!((curchar > ASCII_7) || (curchar < ASCII_0)));

#ifdef PKGW
	    send_val(n, PVST_NUMBER, 0);
#else
	    send_val(n);
#endif
	    goto reswitch;
            break;

        case HEX:
	    n = 0;
	    curchar = ASCII_0;
	    do {
		if (curchar >= ASCII_A)
		    curchar = curchar - ASCII_7;
		else
		    curchar = curchar - ASCII_0;
		if (n >= 0x8000000) {
		    putc('\n', stdout);
		    Fputs(stdout, "! Constant too big");
		    error();
		} else
		    n = 16 * n + curchar;
		curchar = get_output();
	    } while (!((curchar > ASCII_F) || (curchar < ASCII_0)
		     || ((curchar > ASCII_9) && (curchar < ASCII_A))));
#ifdef PKGW
	    send_val(n, PVST_NUMBER, 0);
#else
	    send_val(n);
#endif
	    goto reswitch;
            break;

        case NUMBER:
#ifdef PKGW
	    if (pkgw_outputting_named_numeric_constant) {
		namepointer tmp = pkgw_cur_constant_id;
		pkgw_outputting_named_numeric_constant = false;
		pkgw_cur_constant_id = 0;
		send_val(curval, PVST_NAMED_CONSTANT, tmp);
	    } else if (pkgw_outputting_string_constant) {
		namepointer tmp = pkgw_cur_constant_id;
		pkgw_outputting_string_constant = false;
		pkgw_cur_constant_id = 0;
		send_val(curval, PVST_STRING_CONSTANT, tmp);
	    } else {
		send_val(curval, PVST_NUMBER, 0);
	    }
#else
	    send_val(curval);
#endif
            break;

        case PERIOD:
	    k = 1;
	    outcontrib[1] = PERIOD;
	    curchar = get_output();

	    if (curchar == PERIOD) {
		outcontrib[2] = PERIOD;
		send_out(STR, 2);
	    } else if ((curchar >= ASCII_0) && (curchar <= ASCII_9)) {
		goto get_fraction;
	    } else {
		send_out(MISC, PERIOD);
		goto reswitch;
	    }
            break;

        case PLUS_SIGN:
        case MINUS_SIGN:
            send_sign(COMMA - curchar);
            break;

        case AND_SIGN:
	    outcontrib[1] = ASCII_a;
	    outcontrib[2] = ASCII_n;
	    outcontrib[3] = ASCII_d;
	    send_out(IDENT, 3);
            break;

        case NOT_SIGN:
	    outcontrib[1] = ASCII_n;
	    outcontrib[2] = ASCII_o;
	    outcontrib[3] = ASCII_t;
	    send_out(IDENT, 3);
            break;

        case SET_ELEMENT_SIGN:
	    outcontrib[1] = ASCII_i;
	    outcontrib[2] = ASCII_n;
	    send_out(IDENT, 2);
            break;

        case OR_SIGN:
	    outcontrib[1] = ASCII_o;
	    outcontrib[2] = ASCII_r;
	    send_out(IDENT, 2);
            break;

        case LEFT_ARROW:
	    outcontrib[1] = COLON;
	    outcontrib[2] = EQUALS_SIGN;
	    send_out(STR, 2);
            break;

        case NOT_EQUAL:
	    outcontrib[1] = LESS_THAN_SIGN;
	    outcontrib[2] = GREATER_THAN_SIGN;
	    send_out(STR, 2);
            break;

        case LESS_OR_EQUAL:
	    outcontrib[1] = LESS_THAN_SIGN;
	    outcontrib[2] = EQUALS_SIGN;
	    send_out(STR, 2);
            break;

        case GREATER_OR_EQUAL:
	    outcontrib[1] = GREATER_THAN_SIGN;
	    outcontrib[2] = EQUALS_SIGN;
	    send_out(STR, 2);
            break;

        case EQUIVALENCE_SIGN:
	    outcontrib[1] = EQUALS_SIGN;
	    outcontrib[2] = EQUALS_SIGN;
	    send_out(STR, 2);
            break;

        case DOUBLE_DOT:
	    outcontrib[1] = PERIOD;
	    outcontrib[2] = PERIOD;
	    send_out(STR, 2);
            break;

        case SINGLE_QUOTE:
	    k = 1;
	    outcontrib[1] = SINGLE_QUOTE;
	    do {
		if (k < linelength)
		    k = k + 1;
		outcontrib[k] = get_output();
	    } while (!((outcontrib[k] == SINGLE_QUOTE) || (stackptr == 0)));

	    if (k == linelength) {
		putc('\n', stdout);
		Fputs(stdout, "! String too long");
		error();
	    }

	    send_out(STR, k);
	    curchar = get_output();
	    if (curchar == SINGLE_QUOTE)
		outstate = UNBREAKABLE;
	    goto reswitch;
            break;

        case EXCLAM:
        case DOUBLEQUOTE:
        case OCTOTHORPE:
        case DOLLARSIGN:
        case PERCENT:
        case AMPERSAND:
        case LEFT_PAREN:
        case RIGHT_PAREN:
        case ASTERISK:
        case COMMA:
        case FORWARD_SLASH:
        case COLON:
        case SEMICOLON:
        case LESS_THAN_SIGN:
        case EQUALS_SIGN:
        case GREATER_THAN_SIGN:
        case QUESTION_MARK:
        case AT_SIGN:
        case LEFT_BRACKET:
        case BACK_SLASH:
        case RIGHT_BRACKET:
        case CARET:
        case UNDERSCORE:
        case BACKTICK:
        case LEFT_BRACE:
        case PIPE:
            send_out(MISC, curchar);
            break;

        case BEGIN_COMMENT:
	    if (bracelevel == 0)
		send_out(MISC, LEFT_BRACE);
	    else
		send_out(MISC, LEFT_BRACKET);
	    bracelevel = bracelevel + 1;
            break;

        case END_COMMENT:
            if (bracelevel > 0) {
                bracelevel = bracelevel - 1;
                if (bracelevel == 0)
                    send_out(MISC, RIGHT_BRACE);
                else
                    send_out(MISC, RIGHT_BRACKET);
            } else {
                putc('\n', stdout);
                Fputs(stdout, "! Extra @}");
                error();
            }
            break;

        case MODULE_NUMBER:
	    if (bracelevel == 0)
		send_out(MISC, LEFT_BRACE);
	    else
		send_out(MISC, LEFT_BRACKET);

	    if (curval < 0) {
		send_out(MISC, COLON);
#ifdef PKGW
		send_val(-(integer) curval, PVST_NUMBER, 0);
#else
		send_val(-(integer) curval);
#endif
	    } else {
#ifdef PKGW
		send_val(curval, PVST_NUMBER, 0);
#else
		send_val(curval);
#endif
		send_out(MISC, COLON);
	    }

	    if (bracelevel == 0)
		send_out(MISC, RIGHT_BRACE);
	    else
		send_out(MISC, RIGHT_BRACKET);
            break;

        case JOIN:
	    send_out(FRAC, 0);
	    outstate = UNBREAKABLE;
            break;

        case VERBATIM:
	    k = 0;
	    do {
		if (k < linelength)
		    k = k + 1;
		outcontrib[k] = get_output();
	    } while (!((outcontrib[k] == VERBATIM) || (stackptr == 0)));

	    if (k == linelength) {
		putc('\n', stdout);
		Fputs(stdout, "! Verbatim string too long");
		error();
	    }
	    send_out(STR, k - 1);
            break;

        case FORCE_LINE:
	    send_out(STR, 0);

	    while (outptr > 0) {
		if (outptr <= linelength)
		    breakptr = outptr;
		flushbuffer();
	    }
	    outstate = MISC;
            break;

        default:
	    putc('\n', stdout);
	    fprintf(stdout, "! Can't output ASCII code %ld", (long)curchar);
	    error();
            break;
        }

        goto continue_;

    get_fraction:
	do {
            if (k < linelength)
                k = k + 1;
            outcontrib[k] = curchar;
            curchar = get_output();

            if ((outcontrib[k] == ASCII_E) && ((curchar == PLUS_SIGN) || (curchar == MINUS_SIGN))) {
                if (k < linelength)
                    k = k + 1;
                outcontrib[k] = curchar;
                curchar = get_output();
            } else if (curchar == ASCII_e) {
                curchar = ASCII_E;
	    }
        } while (!((curchar != ASCII_E) && ((curchar < ASCII_0) || (curchar > ASCII_9))));

        if (k == linelength) {
            putc('\n', stdout);
            Fputs(stdout, "! Fraction too long");
            error();
        }

        send_out(FRAC, k);
        goto reswitch;

    continue_:
	;
    }
}

boolean linesdontmatch(void)
{
    /* 10 */ register boolean Result;
    integer k;
    Result = true;
    if (changelimit != limit)
        goto exit;
    if (limit > 0) {
        register integer for_end;
        k = 0;
        for_end = limit - 1;
        if (k <= for_end)
            do
                if (changebuffer[k] != buffer[k])
                    goto exit;
            while (k++ < for_end) ;
    }
    Result = false;
 exit:;
    return Result;
}

void primethechangebuffer(void)
{
    /* 22 30 10 */ integer k;
    changelimit = 0;
    while (true) {

        line = line + 1;
        if (!input_ln(changefile))
            goto exit;
        if (limit < 2)
            goto continue_;
        if (buffer[0] != AT_SIGN)
            goto continue_;
        if ((buffer[1] >= ASCII_X) && (buffer[1] <= ASCII_Z))
            buffer[1] = buffer[1] + 32; /*lowercasify*/
        if (buffer[1] == ASCII_x)
            goto done;
        if ((buffer[1] == ASCII_y) || (buffer[1] == ASCII_z)) {
            loc = 2;
            {
                putc('\n', stdout);
                Fputs(stdout, "! Where is the matching @x?");
                error();
            }
        }
 continue_:;
    }
 done:;
    do {
        line = line + 1;
        if (!input_ln(changefile)) {
            {
                putc('\n', stdout);
                Fputs(stdout, "! Change file ended after @x");
                error();
            }
            goto exit;
        }
    }
    while (!(limit > 0));
    {
        changelimit = limit;
        if (limit > 0) {
            register integer for_end;
            k = 0;
            for_end = limit - 1;
            if (k <= for_end)
                do
                    changebuffer[k] = buffer[k];
                while (k++ < for_end);
        }
    }
 exit:;
}

void checkchange(void)
{
    /* 10 */ integer n;
    integer k;
    if (linesdontmatch())
        goto exit;
    n = 0;
    while (true) {

        changing = !changing;
        templine = otherline;
        otherline = line;
        line = templine;
        line = line + 1;
        if (!input_ln(changefile)) {
            {
                putc('\n', stdout);
                Fputs(stdout, "! Change file ended before @y");
                error();
            }
            changelimit = 0;
            changing = !changing;
            templine = otherline;
            otherline = line;
            line = templine;
            goto exit;
        }
        if (limit > 1) {

            if (buffer[0] == AT_SIGN) {
                if ((buffer[1] >= ASCII_X) && (buffer[1] <= ASCII_Z))
                    buffer[1] = buffer[1] + 32;
                if ((buffer[1] == ASCII_x) || (buffer[1] == ASCII_z)) {
                    loc = 2;
                    {
                        putc('\n', stdout);
                        Fputs(stdout, "! Where is the matching @y?");
                        error();
                    }
                } else if (buffer[1] == ASCII_y) {
                    if (n > 0) {
                        loc = 2;
                        {
                            putc('\n', stdout);
                            fprintf(stdout,
                                    "%s%ld%s",
                                    "! Hmm... ",
                                    (long)n,
                                    " of the preceding lines failed to match");
                            error();
                        }
                    }
                    goto exit;
                }
            }
        }
        {
            changelimit = limit;
            if (limit > 0) {
                register integer for_end;
                k = 0;
                for_end = limit - 1;
                if (k <= for_end)
                    do
                        changebuffer[k] = buffer[k];
                    while (k++ < for_end);
            }
        }
        changing = !changing;
        templine = otherline;
        otherline = line;
        line = templine;
        line = line + 1;
        if (!input_ln(webfile)) {
            {
                putc('\n', stdout);
                Fputs(stdout, "! WEB file ended during a change");
                error();
            }
            inputhasended = true;
            goto exit;
        }
        if (linesdontmatch())
            n = n + 1;
    }
 exit:;
}

void getline(void)
{
 /* 20 */ restart:if (changing) {
        line = line + 1;
        if (!input_ln(changefile)) {
            {
                putc('\n', stdout);
                Fputs(stdout, "! Change file ended without @z");
                error();
            }
            buffer[0] = AT_SIGN;
            buffer[1] = ASCII_z;
            limit = 2;
        }
        if (limit > 1) {

            if (buffer[0] == AT_SIGN) {
                if ((buffer[1] >= ASCII_X) && (buffer[1] <= ASCII_Z))
                    buffer[1] = buffer[1] + 32;
                if ((buffer[1] == ASCII_x) || (buffer[1] == ASCII_y)) {
                    loc = 2;
                    {
                        putc('\n', stdout);
                        Fputs(stdout, "! Where is the matching @z?");
                        error();
                    }
                } else if (buffer[1] == ASCII_z) {
                    primethechangebuffer();
                    changing = !changing;
                    templine = otherline;
                    otherline = line;
                    line = templine;
                }
            }
        }
    }
    if (!changing) {
        {
            line = line + 1;
            if (!input_ln(webfile))
                inputhasended = true;
            else if (limit == changelimit) {

                if (buffer[0] == changebuffer[0]) {

                    if (changelimit > 0)
                        checkchange();
                }
            }
        }
        if (changing)
            goto restart;
    }
    loc = 0;
    buffer[limit] = SPACE;
}

eightbits controlcode(ASCIIcode c)
{
    register eightbits Result;
    switch (c) {
    case AT_SIGN:
        Result = AT_SIGN;
        break;
    case SINGLE_QUOTE:
        Result = OCTAL;
        break;
    case DOUBLEQUOTE:
        Result = HEX;
        break;
    case DOLLARSIGN:
        Result = CHECK_SUM;
        break;
    case SPACE:
    case TAB_MARK:
        Result = NEW_MODULE;
        break;
    case ASTERISK:
	fprintf(stdout, "%c%ld", '*', (long)modulecount + 1);
	fflush(stdout);
	Result = NEW_MODULE;
        break;
    case ASCII_D:
    case ASCII_d:
        Result = DEFINITION;
        break;
    case ASCII_F:
    case ASCII_f:
        Result = FORMAT;
        break;
    case LEFT_BRACE:
        Result = BEGIN_COMMENT;
        break;
    case RIGHT_BRACE:
        Result = END_COMMENT;
        break;
    case ASCII_P:
    case ASCII_p:
        Result = BEGIN_PASCAL;
        break;
    case ASCII_T:
    case ASCII_t:
    case CARET:
    case PERIOD:
    case COLON:
        Result = CONTROL_TEXT;
        break;
    case AMPERSAND:
        Result = JOIN;
        break;
    case LESS_THAN_SIGN:
        Result = MODULE_NAME;
        break;
    case EQUALS_SIGN:
        Result = VERBATIM;
        break;
    case BACK_SLASH:
        Result = FORCE_LINE;
        break;
    default:
        Result = IGNORE;
        break;
    }

    return Result;
}

eightbits skipahead(void)
{
    eightbits Result;
    eightbits c;

    while (true) {
        if (loc > limit) {
            getline();
            if (inputhasended) {
                c = NEW_MODULE;
                goto done;
            }
        }

        buffer[limit + 1] = AT_SIGN;

        while (buffer[loc] != AT_SIGN)
            loc++;

        if (loc <= limit) {
            loc += 2;
            c = controlcode(buffer[loc - 1]);
            if ((c != PARAM) || (buffer[loc - 1] == GREATER_THAN_SIGN))
                goto done;
        }
    }

 done:
    Result = c;
    return Result;
}

void skipcomment(void)
{
    /* 10 */ eightbits bal;
    ASCIIcode c;
    bal = 0;
    while (true) {

        if (loc > limit) {
            getline();
            if (inputhasended) {
                {
                    putc('\n', stdout);
                    Fputs(stdout, "! Input ended in mid-comment");
                    error();
                }
                goto exit;
            }
        }
        c = buffer[loc];
        loc = loc + 1;
        if (c == AT_SIGN) {
            c = buffer[loc];
            if ((c != SPACE) && (c != TAB_MARK) && (c != ASTERISK) && (c != ASCII_z)
                && (c != ASCII_Z))
                loc = loc + 1;
            else {

                {
                    putc('\n', stdout);
                    Fputs(stdout, "! Section ended in mid-comment");
                    error();
                }
                loc = loc - 1;
                goto exit;
            }
        } else if ((c == BACK_SLASH) && (buffer[loc] != AT_SIGN))
            loc = loc + 1;
        else if (c == LEFT_BRACE)
            bal = bal + 1;
        else if (c == RIGHT_BRACE) {
            if (bal == 0)
                goto exit;
            bal = bal - 1;
        }
    }
 exit:;
}

eightbits
get_next(void)
{
    eightbits Result;
    eightbits c;
    eightbits d;
    integer j, k;

 restart:

    if (loc > limit) {
        getline ();
        if (inputhasended) {
            c = NEW_MODULE;
            goto found;
        }
    }

    c = buffer[loc];
    loc = loc + 1;
    if (scanninghex) {
        if (((c >= ASCII_0) && (c <= ASCII_9)) || ((c >= ASCII_A) && (c <= ASCII_F)))
            goto found;
        else
            scanninghex = false;
    }

    switch (c) {
    case ASCII_A:
    case ASCII_B:
    case 67:
    case ASCII_D:
    case ASCII_E:
    case ASCII_F:
    case 71:
    case 72:
    case ASCII_I:
    case 74:
    case 75:
    case 76:
    case ASCII_M:
    case 78:
    case ASCII_O:
    case ASCII_P:
    case 81:
    case 82:
    case 83:
    case ASCII_T:
    case 85:
    case ASCII_V:
    case 87:
    case ASCII_X:
    case 89:
    case ASCII_Z:
    case ASCII_a:
    case 98:
    case 99:
    case ASCII_d:
    case ASCII_e:
    case ASCII_f:
    case 103:
    case 104:
    case ASCII_i:
    case 106:
    case 107:
    case 108:
    case ASCII_m:
    case ASCII_n:
    case ASCII_o:
    case ASCII_p:
    case 113:
    case ASCII_r:
    case 115:
    case ASCII_t:
    case 117:
    case ASCII_v:
    case 119:
    case ASCII_x:
    case ASCII_y:
    case ASCII_z:
	if (((c == ASCII_e) || (c == ASCII_E)) && (loc > 1)) {
	    if ((buffer[loc - 2] <= ASCII_9) && (buffer[loc - 2] >= ASCII_0))
		c = IGNORE;
	}

	if (c != IGNORE) {
	    loc = loc - 1;
	    idfirst = loc;

	    do {
		loc = loc + 1;
		d = buffer[loc];
	    } while (!(((d < ASCII_0) ||
			((d > ASCII_9) && (d < ASCII_A)) ||
			((d > ASCII_Z) && (d < ASCII_a)) ||
			(d > ASCII_z)) && (d != UNDERSCORE)));

	    if (loc > idfirst + 1) {
		c = IDENTIFIER;
		idloc = loc;
	    }
	} else
	    c = ASCII_E;
        break;

    case DOUBLEQUOTE:
	doublechars = 0;
	idfirst = loc - 1;

	do {
	    d = buffer[loc];
	    loc = loc + 1;
	    if ((d == DOUBLEQUOTE) || (d == AT_SIGN)) {
		if (buffer[loc] == d) {
		    loc = loc + 1;
		    d = 0;
		    doublechars = doublechars + 1;
		} else {
		    if (d == AT_SIGN) {
			putc('\n', stdout);
			Fputs(stdout, "! Double @ sign missing");
			error();
		    }
		}
	    } else if (loc > limit) {
		putc('\n', stdout);
		Fputs(stdout, "! String constant didn't end");
		error();
		d = DOUBLEQUOTE;
	    }
	} while (d != DOUBLEQUOTE);

	idloc = loc - 1;
	c = IDENTIFIER;
        break;

    case AT_SIGN:
	c = controlcode(buffer[loc]);
	loc = loc + 1;
	if (c == PARAM)
	    goto restart;
	else if (c == HEX)
	    scanninghex = true;
	else if (c == MODULE_NAME) {
	    k = 0;

	    while (true) {
		if (loc > limit) {
		    getline();
		    if (inputhasended) {
			putc('\n', stdout);
			Fputs(stdout, "! Input ended in section name");
			error();
			goto done;
		    }
		}

		d = buffer[loc];

		if (d == AT_SIGN) {
		    d = buffer[loc + 1];
		    if (d == GREATER_THAN_SIGN) {
			loc = loc + 2;
			goto done;
		    }

		    if ((d == SPACE) || (d == TAB_MARK) || (d == ASTERISK)) {
			putc('\n', stdout);
			Fputs(stdout, "! Section name didn't end");
			error();
			goto done;
		    }

		    k = k + 1;
		    modtext[k] = AT_SIGN;
		    loc = loc + 1;
		}

		loc = loc + 1;

		if (k < longestname - 1)
		    k = k + 1;

		if ((d == SPACE) || (d == TAB_MARK)) {
		    d = SPACE;
		    if (modtext[k - 1] == SPACE)
			k = k - 1;
		}
		modtext[k] = d;
	    }

	done:
	    if (k >= longestname - 2) {
		putc('\n', stdout);
		Fputs(stdout, "! Section name too long: ");

		{
		    register integer for_end;
		    j = 1;
		    for_end = 25;
		    if (j <= for_end)
			do
			    putc(xchr[modtext[j]], stdout);
			while (j++ < for_end);
		}

		Fputs(stdout, "...");
		if (history == SPOTLESS)
		    history = HARMLESS_MESSAGE;
	    }

	    if ((modtext[k] == SPACE) && (k > 0))
		k = k - 1;

	    if (k > 3) {
		if ((modtext[k] == PERIOD)
		    && (modtext[k - 1] == PERIOD)
		    && (modtext[k - 2] == PERIOD))
		    curmodule = prefix_lookup(k - 3);
		else
		    curmodule = mod_lookup(k);
	    } else
		curmodule = mod_lookup(k);
	} else if (c == CONTROL_TEXT) {
	    do {
		c = skipahead();
	    } while (c == AT_SIGN);

	    if (buffer[loc - 1] != GREATER_THAN_SIGN) {
		putc('\n', stdout);
		Fputs(stdout, "! Improper @ within control text");
		error();
	    }

	    goto restart;
	}
        break;

    case PERIOD:
        if (buffer[loc] == PERIOD) {
            if (loc <= limit) {
                c = DOUBLE_DOT;
                loc = loc + 1;
            }
        } else if (buffer[loc] == RIGHT_PAREN) {
            if (loc <= limit) {
                c = RIGHT_BRACKET;
                loc = loc + 1;
            }
        }
        break;

    case COLON:
        if (buffer[loc] == EQUALS_SIGN) {
            if (loc <= limit) {
                c = LEFT_ARROW;
                loc = loc + 1;
            }
        }
        break;

    case EQUALS_SIGN:
        if (buffer[loc] == EQUALS_SIGN) {
            if (loc <= limit) {
                c = EQUIVALENCE_SIGN;
                loc = loc + 1;
            }
        }
        break;

    case GREATER_THAN_SIGN:
        if (buffer[loc] == EQUALS_SIGN) {
            if (loc <= limit) {
                c = GREATER_OR_EQUAL;
                loc = loc + 1;
            }
        }
        break;

    case LESS_THAN_SIGN:
        if (buffer[loc] == EQUALS_SIGN) {
            if (loc <= limit) {
                c = LESS_OR_EQUAL;
                loc = loc + 1;
            }
        } else if (buffer[loc] == GREATER_THAN_SIGN) {
            if (loc <= limit) {
                c = NOT_EQUAL;
                loc = loc + 1;
            }
        }
        break;

    case LEFT_PAREN:
        if (buffer[loc] == ASTERISK) {
            if (loc <= limit) {
                c = BEGIN_COMMENT;
                loc = loc + 1;
            }
        } else if (buffer[loc] == PERIOD) {
            if (loc <= limit) {
                c = LEFT_BRACKET;
                loc = loc + 1;
            }
        }
        break;

    case ASTERISK:
        if (buffer[loc] == RIGHT_PAREN) {
            if (loc <= limit) {
                c = END_COMMENT;
                loc = loc + 1;
            }
        }
        break;

    case SPACE:
    case TAB_MARK:
        goto restart;
        break;

    case LEFT_BRACE:
	skipcomment();
	goto restart;
        break;

    case RIGHT_BRACE:
	putc('\n', stdout);
	Fputs(stdout, "! Extra }");
	error();
	goto restart;
        break;

    default:
        if (c >= 128)
            goto restart;
    }

found:
    return c;
}


void scan_numeric(namepointer p)
{
    integer accumulator = 0;
    schar nextsign = 1;
    namepointer q;
    integer val;

    while (true) {
        nextcontrol = get_next();

    reswitch:
	switch (nextcontrol) {
        case ASCII_0:
        case ASCII_1:
        case ASCII_2:
        case ASCII_3:
        case ASCII_4:
        case ASCII_5:
        case ASCII_6:
        case ASCII_7:
        case ASCII_8:
        case ASCII_9:
	    val = 0;

	    do {
		val = 10 * val + nextcontrol - ASCII_0;
		nextcontrol = get_next();
	    } while (!((nextcontrol > ASCII_9) || (nextcontrol < ASCII_0)));

	    accumulator = accumulator + nextsign * (val);
	    nextsign = 1;
	    goto reswitch;
            break;

        case OCTAL:
	    val = 0;
	    nextcontrol = ASCII_0;

	    do {
		val = 8 * val + nextcontrol - ASCII_0;
		nextcontrol = get_next();
	    } while (!((nextcontrol > ASCII_7) || (nextcontrol < ASCII_0)));

	    accumulator = accumulator + nextsign * (val);
	    nextsign = 1;
	    goto reswitch;
            break;

        case HEX:
	    val = 0;
	    nextcontrol = ASCII_0;

	    do {
		if (nextcontrol >= ASCII_A)
		    nextcontrol = nextcontrol - 7;
		val = 16 * val + nextcontrol - ASCII_0;
		nextcontrol = get_next();
	    } while (!((nextcontrol > ASCII_F) || (nextcontrol < ASCII_0)
		       || ((nextcontrol > ASCII_9) && (nextcontrol < ASCII_A))));

	    accumulator = accumulator + nextsign * (val);
	    nextsign = 1;
	    goto reswitch;
            break;

        case IDENTIFIER:
	    q = id_lookup(NORMAL);
	    if (ilk[q] != NUMERIC) {
		nextcontrol = ASTERISK;
		goto reswitch;
	    }

	    accumulator += nextsign * (equiv[q] - 0x40000000);
	    nextsign = 1;
            break;

        case PLUS_SIGN:
            break;

        case MINUS_SIGN:
            nextsign = -nextsign;
            break;

        case FORMAT:
        case DEFINITION:
        case MODULE_NAME:
        case BEGIN_PASCAL:
        case NEW_MODULE:
            goto done;
            break;

        case SEMICOLON:
	    putc('\n', stdout);
	    Fputs(stdout, "! Omit semicolon in numeric definition");
	    error();
            break;

        default:
	    putc('\n', stdout);
	    Fputs(stdout, "! Improper numeric definition will be flushed");
	    error();
	    do {
		nextcontrol = skipahead();
	    } while (!((nextcontrol >= FORMAT)));

	    if (nextcontrol == MODULE_NAME) {
		loc = loc - 2;
		nextcontrol = get_next();
	    }

	    accumulator = 0;
	    goto done;
            break;
        }
    }

done:
    if (abs(accumulator) >= 0x40000000) {
	putc('\n', stdout);
	fprintf(stdout, "%s%ld", "! Value too big: ", (long)accumulator);
	error();
        accumulator = 0;
    }

    equiv[p] = accumulator + 0x40000000;
}

void scanrepl(eightbits t)
{
    /* 22 30 31 21 */ sixteenbits a;
    ASCIIcode b;
    eightbits bal;
    bal = 0;
    while (true) {

 continue_:a = get_next();
        switch (a) {
        case LEFT_PAREN:
            bal = bal + 1;
            break;
        case RIGHT_PAREN:
            if (bal == 0) {
                putc('\n', stdout);
                Fputs(stdout, "! Extra )");
                error();
            } else
                bal = bal - 1;
            break;
        case SINGLE_QUOTE:
            {
                b = SINGLE_QUOTE;
                while (true) {

                    {
                        if (tokptr[z] == maxtoks) {
                            putc('\n', stdout);
                            fprintf(stderr,
                                    "%s%s%s",
                                    "! Sorry, ", "token", " capacity exceeded");
                            error();
                            history = FATAL_MESSAGE;
                            uexit(1);
                        }
                        tokmem[z][tokptr[z]] = b;
                        tokptr[z] = tokptr[z] + 1;
                    }
                    if (b == AT_SIGN) {

                        if (buffer[loc] == AT_SIGN)
                            loc = loc + 1;
                        else {

                            putc('\n', stdout);
                            Fputs(stdout,
                                  "! You should double @ signs in strings");
                            error();
                        }
                    }
                    if (loc == limit) {
                        {
                            putc('\n', stdout);
                            Fputs(stdout, "! String didn't end");
                            error();
                        }
                        buffer[loc] = SINGLE_QUOTE;
                        buffer[loc + 1] = 0;
                    }
                    b = buffer[loc];
                    loc = loc + 1;
                    if (b == SINGLE_QUOTE) {
                        if (buffer[loc] != SINGLE_QUOTE)
                            goto found;
                        else {

                            loc = loc + 1;
                            {
                                if (tokptr[z] == maxtoks) {
                                    putc('\n', stdout);
                                    fprintf
                                        (stderr,
                                         "%s%s%s",
                                         "! Sorry, ",
                                         "token", " capacity exceeded");
                                    error();
                                    history = FATAL_MESSAGE;
                                    uexit(1);
                                }
                                tokmem[z][tokptr[z]] = SINGLE_QUOTE;
                                tokptr[z] = tokptr[z] + 1;
                            }
                        }
                    }
                }
 found:        ;
            }
            break;
        case OCTOTHORPE:
            if (t == 3)
                a = 0;
            break;
        case IDENTIFIER:
            {
                a = id_lookup(NORMAL);
                {
                    if (tokptr[z] == maxtoks) {
                        putc('\n', stdout);
                        fprintf(stderr, "%s%s%s",
                                "! Sorry, ", "token", " capacity exceeded");
                        error();
                        history = FATAL_MESSAGE;
                        uexit(1);
                    }
                    tokmem[z][tokptr[z]] = (a / 256) + 128;
                    tokptr[z] = tokptr[z] + 1;
                }
                a = a % 256;
            }
            break;
        case MODULE_NAME:
            if (t != MODULE_NAME)
                goto done;
            else {

                {
                    if (tokptr[z] == maxtoks) {
                        putc('\n', stdout);
                        fprintf(stderr, "%s%s%s",
                                "! Sorry, ", "token", " capacity exceeded");
                        error();
                        history = FATAL_MESSAGE;
                        uexit(1);
                    }
                    tokmem[z][tokptr[z]] = (curmodule / 256) + 168;
                    tokptr[z] = tokptr[z] + 1;
                }
                a = curmodule % 256;
            }
            break;
        case VERBATIM:
            {
                {
                    if (tokptr[z] == maxtoks) {
                        putc('\n', stdout);
                        fprintf(stderr, "%s%s%s",
                                "! Sorry, ", "token", " capacity exceeded");
                        error();
                        history = FATAL_MESSAGE;
                        uexit(1);
                    }
                    tokmem[z][tokptr[z]] = 2;
                    tokptr[z] = tokptr[z] + 1;
                }
                buffer[limit + 1] = AT_SIGN;
 reswitch:        if (buffer[loc] == AT_SIGN) {
                    if (loc < limit) {

                        if (buffer[loc + 1] == AT_SIGN) {
                            {
                                if (tokptr[z] == maxtoks) {
                                    putc('\n', stdout);
                                    fprintf
                                        (stderr,
                                         "%s%s%s",
                                         "! Sorry, ",
                                         "token", " capacity exceeded");
                                    error();
                                    history = FATAL_MESSAGE;
                                    uexit(1);
                                }
                                tokmem[z][tokptr[z]] = AT_SIGN;
                                tokptr[z] = tokptr[z] + 1;
                            }
                            loc = loc + 2;
                            goto reswitch;
                        }
                    }
                } else {

                    {
                        if (tokptr[z] == maxtoks) {
                            putc('\n', stdout);
                            fprintf(stderr,
                                    "%s%s%s",
                                    "! Sorry, ", "token", " capacity exceeded");
                            error();
                            history = FATAL_MESSAGE;
                            uexit(1);
                        }
                        tokmem[z][tokptr[z]] = buffer[loc];
                        tokptr[z] = tokptr[z] + 1;
                    }
                    loc = loc + 1;
                    goto reswitch;
                }
                if (loc >= limit) {
                    putc('\n', stdout);
                    Fputs(stdout, "! Verbatim string didn't end");
                    error();
                } else if (buffer[loc + 1] != GREATER_THAN_SIGN) {
                    putc('\n', stdout);
                    Fputs(stdout,
                          "! You should double @ signs in verbatim strings");
                    error();
                }
                loc = loc + 2;
            }
            break;
        case DEFINITION:
        case FORMAT:
        case BEGIN_PASCAL:
            if (t != MODULE_NAME)
                goto done;
            else {

                {
                    putc('\n', stdout);
                    fprintf(stdout, "%s%c%s", "! @",
                            xchr[buffer[loc - 1]],
                            " is ignored in Pascal text");
                    error();
                }
                goto continue_;
            }
            break;
        case NEW_MODULE:
            goto done;
            break;
        default:
            ;
            break;
        }
        {
            if (tokptr[z] == maxtoks) {
                putc('\n', stdout);
                fprintf(stderr, "%s%s%s", "! Sorry, ", "token",
                        " capacity exceeded");
                error();
                history = FATAL_MESSAGE;
                uexit(1);
            }
            tokmem[z][tokptr[z]] = a;
            tokptr[z] = tokptr[z] + 1;
        }
    }
 done:nextcontrol = a;
    if (bal > 0) {
        if (bal == 1) {
            putc('\n', stdout);
            Fputs(stdout, "! Missing )");
            error();
        } else {

            putc('\n', stdout);
            fprintf(stdout, "%s%ld%s", "! Missing ", (long)bal, " )'s");
            error();
        }
        while (bal > 0) {

            {
                if (tokptr[z] == maxtoks) {
                    putc('\n', stdout);
                    fprintf(stderr, "%s%s%s", "! Sorry, ",
                            "token", " capacity exceeded");
                    error();
                    history = FATAL_MESSAGE;
                    uexit(1);
                }
                tokmem[z][tokptr[z]] = RIGHT_PAREN;
                tokptr[z] = tokptr[z] + 1;
            }
            bal = bal - 1;
        }
    }
    if (textptr > maxtexts - 4) {
        putc('\n', stdout);
        fprintf(stderr, "%s%s%s", "! Sorry, ", "text", " capacity exceeded");
        error();
        history = FATAL_MESSAGE;
        uexit(1);
    }
    currepltext = textptr;
    tokstart[textptr + 4] = tokptr[z];
    textptr = textptr + 1;
    if (z == 3)
        z = 0;
    else
        z = z + 1;
}

void definemacro(eightbits t)
{
    namepointer p;
    p = id_lookup(t);
    scanrepl(t);
    equiv[p] = currepltext;
    textlink[currepltext] = 0;
}

void scanmodule(void)
{
    namepointer p;

    modulecount = modulecount + 1;
    nextcontrol = 0;

    while (true) {
    continue_:
	while (nextcontrol <= FORMAT) {
            nextcontrol = skipahead();

            if (nextcontrol == MODULE_NAME) {
                loc = loc - 2;
                nextcontrol = get_next();
            }
        }

        if (nextcontrol != DEFINITION)
            goto done;

        nextcontrol = get_next();
        if (nextcontrol != IDENTIFIER) {
	    putc('\n', stdout);
	    fprintf(stdout, "%s%s",
		    "! Definition flushed, must start with ",
		    "identifier of length > 1");
	    error();
            goto continue_;
        }

        nextcontrol = get_next();

        if (nextcontrol == EQUALS_SIGN) {
	    namepointer tempid = id_lookup(NUMERIC);
            scan_numeric(tempid);
#ifdef PKGW
	    if (pkgw_n_constants == pkgw_max_constants) {
		fprintf (stderr, "seriously, too many constants?\n");
		error ();
	    }
	    pkgw_constant_ids[pkgw_n_constants] = tempid;
	    pkgw_constant_values[pkgw_n_constants] = equiv[tempid] - 0x40000000;
	    pkgw_n_constants++;
#endif
            goto continue_;
        } else if (nextcontrol == EQUIVALENCE_SIGN) {
            definemacro(2);
            goto continue_;
        } else if (nextcontrol == LEFT_PAREN) {
            nextcontrol = get_next();
            if (nextcontrol == OCTOTHORPE) {
                nextcontrol = get_next();
                if (nextcontrol == RIGHT_PAREN) {
                    nextcontrol = get_next();
                    if (nextcontrol == EQUALS_SIGN) {
                        {
                            putc('\n', stdout);
                            Fputs(stdout, "! Use == for macros");
                            error();
                        }
                        nextcontrol = EQUIVALENCE_SIGN;
                    }
                    if (nextcontrol == EQUIVALENCE_SIGN) {
                        definemacro(3);
                        goto continue_;
                    }
                }
            }
        }
        {
            putc('\n', stdout);
            Fputs(stdout, "! Definition flushed since it starts badly");
            error();
        }
    }
 done:;
    switch (nextcontrol) {
    case BEGIN_PASCAL:
        p = 0;
        break;
    case MODULE_NAME:
        {
            p = curmodule;
            do {
                nextcontrol = get_next();
            }
            while (!(nextcontrol != PLUS_SIGN));
            if ((nextcontrol != EQUALS_SIGN) && (nextcontrol != EQUIVALENCE_SIGN)) {
                {
                    putc('\n', stdout);
                    Fputs(stdout, "! Pascal text flushed, = sign is missing");
                    error();
                }
                do {
                    nextcontrol = skipahead();
                }
                while (!(nextcontrol == NEW_MODULE));
                goto exit;
            }
        }
        break;
    default:
        goto exit;
        break;
    }
    store_two_bytes(0xD000 + modulecount);
    scanrepl(MODULE_NAME);
    if (p == 0) {
        textlink[lastunnamed] = currepltext;
        lastunnamed = currepltext;
    } else if (equiv[p] == 0)
        equiv[p] = currepltext;
    else {

        p = equiv[p];
        while (textlink[p] < maxtexts)
            p = textlink[p];
        textlink[p] = currepltext;
    }
    textlink[currepltext] = maxtexts;
 exit:;
}

void mainbody(void)
{
    initialize();
    openinput();

    line = 0;
    otherline = 0;
    changing = true;
    primethechangebuffer();
    changing = !changing;
    templine = otherline;
    otherline = line;
    line = templine;
    limit = 0;
    loc = 1;
    buffer[0] = SPACE;
    inputhasended = false;

    phaseone = true;
    modulecount = 0;

    while (!inputhasended) {
	getline();
	buffer[limit + 1] = '\0';
	printf("%s\n", buffer);
    }

    uexit(0);
}

/* main.c */

/* These variables are referenced from the change files.  */
char **argv;
int argc;

/* The entry point for all the programs except TeX and Metafont, which
   have more to do.  We just have to set up the command line.  web2c
   transforms Pascal's main block into a procedure `main_body'.  */
int
main (int  ac,  string* av)
{
#ifdef __EMX__
  _wildcard (&ac, &av);
  _response (&ac, &av);
#endif

#ifdef WIN32
  _setmaxstdio(2048);
#endif

  argc = ac;
  argv = av;
  mainbody ();
  return EXIT_SUCCESS;
}

/* Return the Nth (counted as in C) argument from the command line.  */

string
cmdline (int n)
{
  if (n >= argc)
    { /* This error message should never happen, because the callers
         should always check they've got the arguments they expect.  */
      fprintf (stderr, "%s: Oops; not enough arguments.\n", argv[0]);
      uexit (1);
    }
  return argv[n];
}
