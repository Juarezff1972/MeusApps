
#include <stdio.h>
#include <string.h>

static char char_diff = 'A' - 'a';


/* **************************************************
************************************************** */
void lower_case(char *x) {

   char *c;

   for (c = x; *c; c++)
      if (*c >= 'A' && *c <= 'Z')
         *c -= char_diff;
}


/* **************************************************
************************************************** */
void upper_case(char *x) {

   char *c;

   for (c = x; *c; c++)
      if (*c >= 'a' && *c <= 'z')
         *c += char_diff;
}


/* **************************************************
************************************************** */
void replace(char *x, char s, char r) {

   char *c;

   for (c = x; *c; c++)
      if (*c == s)
         *c = r;
}


/* **************************************************
************************************************** */
int get_token(FILE *infile, char *x) {

   int errorflag;

   do {
      if (fscanf(infile, "%255s", x) == EOF)
         return 0;

      if (!strcmp("//", x))
         while ((errorflag=fgetc(infile)) != '\n')
            if (errorflag == EOF)
               return 0;
   } while (!strcmp("//", x));

   return 1;
}

