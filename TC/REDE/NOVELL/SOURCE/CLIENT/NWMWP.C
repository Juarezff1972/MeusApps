/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <ntt.h>
#include <ctype.h>

int  _NWMapWildPath
(
char      *path,
char      *mappedPath
)
{
   int     sourceIndex;
   int     destIndex;
   /*-------------------------------------------------- function body */
    for (sourceIndex = destIndex = 0;
         path[sourceIndex];
         ++sourceIndex, ++destIndex)
    {
        switch (path[sourceIndex])
        {
        case '*':
            mappedPath[destIndex++] = 0xFF;
            mappedPath[destIndex]   = '*';
            break;
        case '?':
            mappedPath[destIndex++] = 0xFF;
            mappedPath[destIndex]   = '?' | 0x80;
            break;
        default:
            mappedPath[destIndex]   = toupper( path[sourceIndex] );
            break;
        }
    }
   mappedPath[destIndex]   = '\0';
   return(destIndex);
}      /* end of _NWMapWildPath */
