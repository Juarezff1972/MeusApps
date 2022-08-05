/* ******************************************************************
   Example to interface the PKWARE Data Compression Library (TM)
   for C.  This is a generic 'ANSI c' example and might require 
   modifications specific to your compiler.
   Copyright 1990-91, By PKWARE Inc.  All Rights Reserved.
   ****************************************************************** */

/* Compile LARGE or COMPACT model */

#include <stdio.h>
#include <alloc.h>

#include "implode.h"

FILE *InFile, *OutFile;

/*
   Routine to supply data to the implode() or explode() routines.
   When this routine returns 0 bytes read, the implode() or explode()
   routines will terminate.
*/

unsigned far pascal ReadFile(char far *buff, unsigned short int far *size)
{
   size_t read;
   read = fread((char *)buff,*size,1,InFile);
   return((unsigned)read);
}

/*
   Routine to write compressed data output from implode() or
   uncompressed data from explode()
*/

void far pascal WriteFile(char far *buff, unsigned short far *size)
{
   fwrite((char *)buff,*size,1,OutFile);
}

void main()
{
   char *WorkBuff;               /* buffer for compression tables */
   unsigned short int error;
   unsigned short int  type;     /* ASCII or Binary compression   */
   unsigned short int  dsize;    /* Dictionary Size. 1,2 or 4K    */

   InFile = fopen("test.in","rb");

   if (InFile == NULL)
   {
      puts("Unable to open input file");
      return;
   }

   OutFile = fopen("test.cmp","wb");    /* File to hold compressed data */

   WorkBuff = (char far *)malloc(35256U);

   if (WorkBuff == NULL)
   {
      puts("Unable to allocate work buffer");
      return;
   }

   puts("Calling Implode");

   type  = CMP_ASCII;                  /* Use ASCII compression */
   dsize = 4096;                       /* Use 4K dictionary     */

   implode(ReadFile,WriteFile,WorkBuff,&type,&dsize);

   puts("Done Compressing");
   free(WorkBuff);

   fclose(InFile);
   fclose(OutFile);

   /* Compression done, now try extracting the compressed file */

   WorkBuff=(char far *)malloc(12574);
   if (WorkBuff == NULL)
   {
      puts("Unable to allocate work buffer");
      return;
   }

   InFile  = fopen("test.cmp","rb");             /* Compressed file    */
   OutFile = fopen("test.ext","wb");            /* File to extract to */

   puts("Calling Explode");
   error = explode(ReadFile,WriteFile,WorkBuff); /* Extract the file   */
   puts("Done Exploding");

   if (error)
      puts("Error in compressed file!");

   free(WorkBuff);
   fclose(InFile);
   fclose(OutFile);
}
