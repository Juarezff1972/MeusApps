/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <ntt.h>

int _NWMapLenPrecPath
(
char      *path,
char      *mappedPath,
int       *totalPathLength
)
{
   int      lengthIndex;                      /* index for nex length to go */
   int      lengthCount;                             /* length of sub-entry */
   int      subEntryCount;                    /* number of sub-path entries */
   int      sourceIndex;                                 /* index into path */
   int      destIndex;                             /* index into mappedPath */
   char      ch;

   lengthIndex   = 0;
   lengthCount   = 0;
   subEntryCount = 0;
   sourceIndex   = 0;
   destIndex     = 1;               /* leave room for length of first piece */

   if (path[sourceIndex] == '\\' || path[sourceIndex] == '/')
      /* skipp leading slash */
      ++sourceIndex;
   while (ch  = path[sourceIndex])
   {
      if (ch == '\\' || ch == '/' || ch == ':')
      {
         mappedPath[lengthIndex]   = lengthCount;
         lengthIndex               = destIndex++;
         lengthCount               = 0;
         ++subEntryCount;
      }
      else
      {
         mappedPath[destIndex++]   = ch;
         ++lengthCount;
      }
      ++sourceIndex;
   }                                                   /* end of while path */
   mappedPath[lengthIndex]   = lengthCount;
   mappedPath[destIndex]   = '\0';
   *totalPathLength         = destIndex;
   return(subEntryCount + 1);
}
