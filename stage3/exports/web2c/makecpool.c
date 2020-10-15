/* Copyright 2007, 2008 Taco Hoekwater.
   You may freely use, modify and/or distribute this file.  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#endif

static const char __svn_version[] =
    "$Id: makecpool.c 1230 2008-05-03 11:11:32Z oneiros $ $URL: svn://scm.foundry.supelec.fr/svn/luatex/trunk/src/texk/web2c/luatexdir/makecpool.c $";

int main(int argc, char *argv[])
{
    char filename[20];
    FILE *fh;
    char data[1024];
    int is_metafont = 0;
    int is_luatex = 0;

#ifdef WIN32
    setmode(fileno(stdout), _O_BINARY);
#endif
    if (argc != 2) {
        fprintf(stderr, "%s: need exactly one argument (base name).\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    strcpy(filename, argv[1]);
    strcat(filename, ".pool");
    fh = fopen(filename, "r");
    if (!fh) {
        fprintf(stderr, "%s: can't open %s for reading.\n", argv[0], filename);
        exit(EXIT_FAILURE);
    }
    if (strstr(filename, "luatex.pool") != NULL)
        is_luatex = 1;
    else if (strstr(filename, "mf.pool") != NULL)
        is_metafont = 1;
    printf("/*\n"
           " * This file is auto-generated by makecpool.\n"
           " *   %s %s\n"
           " */\n"
           "\n"
           "#define EXTERN extern\n"
           "#include \"%sd.h\"\n"
           "#include <stdio.h>\n"
           "#include <string.h>\n" "\n" "static const char *poolfilearr[] = {\n", argv[0], argv[1], argv[1]);
    while (fgets(data, 1024, fh)) {
        int i;
        int len = strlen(data);
        int o = 0;              /* skip first bytes */
        if (data[len - 1] == '\n') {
            data[len - 1] = 0;
            len--;
        }
        if (data[0] == '*')
            break;
        if (data[0] >= '0' && data[0] <= '9' && data[1] >= '0' && data[1] <= '9') {
            o = 2;
        }
        printf("  \"");
        for (i = o; i < len; i++) {
            if (data[i] == '"' || data[i] == '\\')
                putchar('\\');
            if (data[i] == '?')
                printf("\" \"");        /* suppress trigraphs */
            putchar(data[i]);
        }
        printf("\",\n");
    }
    fclose(fh);
    printf("  NULL };\n"
           "int loadpoolstrings (integer spare_size) {\n"
           "  const char *s;\n"
           "  str_number g=0;\n"
           "  int i=0,j=0;\n"
           "  while ((s = poolfilearr[j++])) {\n"
           "    int l = strlen (s);\n" "    i += l;\n" "    if (i>=spare_size) return 0;\n");
    if (is_luatex)
        printf("    while (l-- > 0) str_pool[pool_ptr++] = *s++;\n" "    g = make_string();\n");
    else
        printf("    while (l-- > 0) str_pool[pool_ptr++] = *s++;\n" "    g = make_string();\n");
    if (is_metafont)
        printf("    strref[g]= 127;\n");
    printf("  }\n" "  return g;\n" "}\n");
    return EXIT_SUCCESS;
}
