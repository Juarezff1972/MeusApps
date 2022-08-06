
#include <string.h>
#include <stdlib.h>
#include "pstring.h"
#include "superclass.h"

void superclass::preprocess(void *data) {}

int superclass::parse(FILE *infile, char *token) {

   if (!strcmp(token, "frame")) {
      get_token(infile, token);
      frame = atoi(token) - 1;

      return 1;
   }

   return 0;
}

