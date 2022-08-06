/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <niterror.h>
#include <ntt.h>
#include <string.h>

WORD GetNumNameSpaceAndDataStreams
(
NWBUFFER   NWBuffer,
BYTE       *numNameSpaces,
BYTE       *numDataStreams
)
{
   int      i;
   BYTE     *currPtr;
   int      nameSpaceLen;
                                               /*...parse name space info...*/
   currPtr          = (BYTE *)NWBuffer;
   *numNameSpaces   = *currPtr++;
   for (i = 0;i < *numNameSpaces;i++)
   {
      nameSpaceLen  = *currPtr++;
      currPtr      += nameSpaceLen;
   }
                                               /*...parse data space info...*/
   *numDataStreams  = *currPtr;
   return(SUCCESSFUL);
}
