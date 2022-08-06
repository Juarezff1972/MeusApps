/***********************************************************************
*                        Example to interface the
*                  PKWARE Data Compression Library (TM)
*                                 with
*            Microsoft QuickBasic (R) 4.5 and PDS Basic 7.1
*          Copyright 1991, By PKWARE Inc.  All Rights Reserved.
************************************************************************/
#include <stdio.h>
#include <dos.h>
#include <memory.h>

#include "implode.h"

extern pascal WriteBasic(unsigned int short *);
extern pascal ReadBasic(unsigned int short *);

unsigned far pascal Read_C(char far *buff, unsigned short int far *size);
void far pascal Write_C(char far *buff, unsigned short far *size);

static char far *BufPtr;

void Bimplode(char far *Gbuffer, int CompSize, int CompType)
{
   implode(Read_C, Write_C, Gbuffer, &CompType, &CompSize);
}

int Bexplode(char far *Gbuffer)
{
   return(explode(Read_C, Write_C, Gbuffer));
}

/* Called from implode() or explode(), which then calls back to BASIC */
unsigned far pascal Read_C(char far *buff, unsigned short int far *size)
{
   BufPtr = buff;                          /* Save buffer ptr */
   return(ReadBasic(size));
}

/* Called from implode() or explode(), which then calls back to BASIC */
void far pascal Write_C(char far *buff, unsigned short far *size)
{
   BufPtr = buff;                          /* Save buffer ptr */
   WriteBasic(size);
}

void far pascal pkcopy(char far *dest, char far *src, int count)
{
   if (count)
   {
      do
      {
         *dest++ = *src++;
      }
      while (--count);
   }
}

/* Copy the data from the BASIC Buffer into the PKWARE Buffer */
void SaveBuf(char far *buf, int Size)
{
   pk_copy(BufPtr, buf, Size);
}

/* Get the data from the PKWARE Buffer, and put it into the BASIC Buffer */
void GetBuf(char far *buf, int Size, int Offset)
{
   pk_copy(buf, BufPtr+Offset, Size);
}
