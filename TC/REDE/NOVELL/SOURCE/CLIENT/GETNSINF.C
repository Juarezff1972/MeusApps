/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <ntt.h>
#include <nit.h>
#include <niterror.h>
#include <string.h>


WORD GetNameSpaceInfo
(
NWBUFFER    NWBuffer,
BYTE        NSType,
char        *nameSpaceName,
int         *OSReady,
int         *volSupports
)
{
   int     i;
   BYTE    *currPtr;
   int     nameSpaceLen, dataStreamLen;
   int     numNameSpaces, numDataStreams;
   BYTE    variableLoadedNameSpaces;
   BYTE    variableVolumesNameSpaces;
                                               /*...parse name space info...*/
   currPtr        = (BYTE *)NWBuffer;
   *nameSpaceName = 0;      /*...null string in case name space not found...*/
   numNameSpaces  = *currPtr++;
   for (i=0;i<numNameSpaces;i++)
   {
      nameSpaceLen  = *currPtr++;
      if (NSType == i)
      {
           memcpy(nameSpaceName,currPtr,nameSpaceLen);
           memset(nameSpaceName+nameSpaceLen,NULL,1);
      }
      currPtr      += nameSpaceLen;
   }
                                               /*...parse data space info...*/
   numDataStreams = *currPtr++;
   for (i=0;i<numDataStreams;i++)
   {
      currPtr++;
      dataStreamLen  = *currPtr++;
      currPtr        += dataStreamLen;
   }
                  /*...check operating system support for this name space...*/
   *OSReady = FALSE;
   variableLoadedNameSpaces = *currPtr++;
   for (i=0;i<variableLoadedNameSpaces;i++)
   {
      if (*currPtr == NSType)
           *OSReady = TRUE;
      currPtr++;
   }
                            /*...check volume support for this name space...*/
   *volSupports = FALSE;
   variableVolumesNameSpaces = *currPtr++;
   for (i=0;i<variableVolumesNameSpaces;i++)
   {
      if (*currPtr == NSType)
           *volSupports = TRUE;
      currPtr++;
   }

   return(SUCCESSFUL);
}
