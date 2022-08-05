#include "myutilities.h"
#include <stdarg.h>
#include <stddef.h>

void bye(char *, ...);
char *chk_malloc(int);
FILE *chk_fopen(char *, char *);

void bye(char *fmt, ...)
{
   va_list args;

   va_start(args, fmt);
   vfprintf(stderr, fmt, args);
   va_end(args);
   exit(1);
}

char *chk_malloc(int size)
{
   char *space;
   char *malloc(size_t);

   if ((space = malloc((size_t) size)) == NULL)
      bye("Panic, cannot allocate %d bytes\n", size);
   return space;
}

FILE *chk_fopen(char *name, char *mode)
{
   FILE *file;

   if ((file = fopen(name, mode)) == NULL)
      bye("Panic: error opening %s with mode %s\n", name, mode);
   return file;
}
