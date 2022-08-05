/***********************************************************************
                       Example to interface the
                  PKWARE Data Compression Library (TM)
                                 with
                  Microsoft Optimizing C Compiler (R)
         Copyright 1990-91, By PKWARE Inc.  All Rights Reserved.
***********************************************************************/

#include <stdio.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <malloc.h>
#include <memory.h>

#include "implode.h"

int InFile, OutFile;

/*
   Routine to supply data to the implode() or explode() routines.
   When this routine returns 0 bytes read, the implode() or explode()
   routines will terminate.
*/

unsigned far pascal ReadBuff(char far *buff, unsigned short int far *size)
{
   unsigned read = 0;

   _dos_read(InFile,buff,*size,&read);
   return(read);
}

/*
   Routine to write compressed data output from implode() or
   uncompressed data from explode()
*/

unsigned far pascal WriteBuff(char far *buff, unsigned short int far *size)
{
   unsigned wrote = 0;

   _dos_write(OutFile,buff,*size,&wrote);
   return(wrote);
}

void main()
{
   char far *WorkBuff;                  /* Buffer for compression tables */
   unsigned error;
   unsigned type;
   unsigned dsize;

   if (_dos_open("test.in",O_RDONLY,&InFile) != 0)
   {
      puts("Unable to open input file");
      return;
   }

   _dos_creat("test.cmp",0,&OutFile);   /* File to hold compressed data */

   WorkBuff=(char far *)_fmalloc(35256U);

   if (WorkBuff == NULL)
   {
      puts("Unable to allocate work buffer");
      return;
   }

   puts("Calling Implode");

   type=CMP_BINARY;
   dsize=4096;
   implode(ReadBuff,WriteBuff,WorkBuff,&type,&dsize);

   _ffree(WorkBuff);

   _dos_close(InFile);
   _dos_close(OutFile);

   /* compression done, now try extracting the compressed file */

   WorkBuff=(char far *)_fmalloc(12574);

   if (WorkBuff == NULL)
   {
      puts("Unable to allocate work buffer");
      return;
   }

   _dos_open("test.cmp",O_RDONLY,&InFile);      /* Compressed file    */
   _dos_creat("test.ext",0,&OutFile);           /* File to extract to */

   puts("Calling Explode");
   error=explode(ReadBuff,WriteBuff,WorkBuff);  /* Extract the file   */

   if (error)
      puts("Error in compressed file!");

   _ffree(WorkBuff);

   _dos_close(InFile);
   _dos_close(OutFile);
}
