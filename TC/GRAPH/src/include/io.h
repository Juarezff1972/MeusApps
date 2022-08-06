



#include <stdio.h>
#include "pstring.h"

#if !defined __IO_H
#define __IO_H


class sfile {

   public:
      char filename[MAXSTRLEN];
      char *data;
      unsigned int size;
      char *ptr;

      sfile() {
         size = 0;
         data = (char *)NULL;
      }

      ~sfile() {
         if (data != (char *)NULL)
            delete [] data;
      }

      int scan_data(char *fname);
      unsigned short scan_ushort();
      unsigned char scan_uchar();
      unsigned int scan_uint();
      void skip(int x);
      void sread(char *buffer, unsigned int len);
      void sseek(unsigned int x);
};


float          getfloat(FILE *infile);
unsigned short getuchar(FILE *infile);
unsigned short getushort(FILE *infile);
unsigned short getrushort(FILE *infile);
unsigned int   getuint(FILE *infile);
double         getdouble(FILE *infile);
int            getstring(FILE *infile, char *buffer, int maxlen);
void           putuchar(FILE *outfile, unsigned char val);
void           putushort(FILE *outfile, unsigned short val);
int            putuint(FILE *outfile, unsigned int val);

#endif