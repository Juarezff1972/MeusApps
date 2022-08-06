
#include <stdio.h>
#include <string.h>

#include "io.h"



float getfloat(FILE *infile) {

   unsigned char buf[4];
   float y;

   fread(buf,4,1,infile);
   *((unsigned int *)(&y)) =  (((((buf[0]<<8) | buf[1])<<8) | buf[2])<<8) | buf[3];

   return y;
}


unsigned short getuchar(FILE *infile) {

   unsigned char buf;

   fread(&buf,1,1,infile);
   return buf;
}


unsigned short getushort(FILE *infile) {

   unsigned char buf[2];

   fread(buf,2,1,infile);
   return (buf[0]<<8) | buf[1];
}


unsigned short getrushort(FILE *infile) {

   unsigned char buf[2];

   fread(buf,2,1,infile);
   return (buf[1]<<8) | buf[0];
}


unsigned int getuint(FILE *infile) {

   unsigned char buf[4];

   fread(buf,4,1,infile);
   return (((((buf[0]<<8) | buf[1])<<8) | buf[2])<<8) | buf[3];
}


double getdouble(FILE *infile) {

   double buf;

   fread(&buf, 1, 8, infile);
   return buf;
}


int getstring(FILE *infile, char *buffer, int maxlen) {

   int len = 0;
   int status;

   do {
      if ((status = fread(&buffer[len], 1, 1, infile)) != 1)
         break;
      len++;
   } while (len <= maxlen-1 && buffer[len-1]);

   if (status != 1)
      return 0;

   if (!buffer[len-1])
      return len;

   return -len;         // if not eoln and buffer full, negate len...
}


void putuchar(FILE *outfile, unsigned char val) {

   fwrite(&val,1,1,outfile);
}


void putushort(FILE *outfile, unsigned short val) {

   unsigned char buf[2];

   buf[0] = (val>>8);
   buf[1] = val & 0xff;
   fwrite(buf,2,1,outfile);
}


int putuint(FILE *outfile, unsigned int val) {

   unsigned char buf[4];

   buf[0] = (unsigned char)(val>>24);
   buf[1] = (unsigned char)(val>>16) & 0xff;
   buf[2] = (unsigned char)(val>>8)  & 0xff;
   buf[3] = (unsigned char)val       & 0xff;
   return fwrite(buf,4,1,outfile);
}


/* ***********************************************************************
*********************************************************************** */
int sfile::scan_data(char *fname) {

   FILE *infile;
   unsigned int tsize;

   if (!(infile = fopen(fname, "rb"))) {
      printf("Error: Cannot access %s...\n", fname);
      return 0;
   }

   strcpy(filename, fname);

   fseek(infile, 0, SEEK_END);
   tsize = ftell(infile);
   fseek(infile, 0, SEEK_SET);

   if (data == (char *)NULL || tsize != size) {
      if (data != (char *)NULL)
         delete [] data;
      size = tsize;
      data = new char[size];
   }

   fread(data, size, 1, infile);

   ptr = data;

   fclose(infile);
   return 1;
}


unsigned short sfile::scan_ushort() {

   unsigned char buf[2];

   buf[0] = *ptr; ptr++;
   buf[1] = *ptr; ptr++;

   return (buf[0]<<8) | buf[1];
}


unsigned char sfile::scan_uchar() {

   unsigned char buf;

   buf = *ptr; ptr++;

   return buf;
}


unsigned int sfile::scan_uint() {

   unsigned char buf[4];

   buf[0] = *ptr; ptr++;
   buf[1] = *ptr; ptr++;
   buf[2] = *ptr; ptr++;
   buf[3] = *ptr; ptr++;

   return (((((buf[0]<<8) | buf[1])<<8) | buf[2])<<8) | buf[3];
}


void sfile::skip(int x) {

   ptr += x;
}


void sfile::sread(char *buffer, unsigned int len) {

   memcpy(buffer, ptr, len);
   ptr += len;
}

void sfile::sseek(unsigned int x) {

   ptr = data + x;
}
