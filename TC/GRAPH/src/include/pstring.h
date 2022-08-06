
#if !defined __PSTRING_H

#define __PSTRING_H

#include <stdio.h>

#define MAXSTRLEN	256

void lower_case(char *x);
void upper_case(char *x);
int get_token(FILE *infile, char *x);
void replace(char *x, char s, char r);

#endif

