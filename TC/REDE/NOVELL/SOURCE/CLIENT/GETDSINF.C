/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <stdio.h>
#include <string.h>
#include <nit.h>
#include <niterror.h>
#include <ntt.h>

WORD  GetDataStreamInfo
(
NWBUFFER     NWBuffer,
BYTE         dataStreamType,
char         *dataStreamName,
int          *volSupports
)

{
   int      i;
   BYTE     *currPtr;
   int      nameSpaceLen, dataStreamLen;
   int      numNameSpaces, numDataStreams;
   BYTE     dataStream;
   BYTE     variableLoadedNameSpaces;
   BYTE     variableVolumesNameSpaces;
   BYTE     variableVolumesDataStreams;


   /*-- parse name space info --*/
   currPtr       = (BYTE *)NWBuffer;
   numNameSpaces = *currPtr++;
   for( i=0; i<numNameSpaces; i++ )
   {
      nameSpaceLen  = *currPtr++;
      currPtr      += nameSpaceLen;
   }

   /*-- parse data stream info --*/
   *dataStreamName = 0;    /*...null string in case data stream not found...*/
   numDataStreams  = *currPtr++;
   for( i=0; i<numDataStreams; i++ )
   {
      dataStream     = *currPtr++;
      dataStreamLen  = *currPtr++;
      if( dataStream == dataStreamType )
      {
         memcpy( dataStreamName, currPtr, dataStreamLen );
         memset( dataStreamName+dataStreamLen, 0, 1 );
      }
      currPtr += dataStreamLen;
   }

  /*-- loaded module support list --*/
   variableLoadedNameSpaces = *currPtr++;
   for( i=0; i < variableLoadedNameSpaces; i++ )
      currPtr++;

   /*-- volume support for name spaces --*/
   variableVolumesNameSpaces = *currPtr++;
   for( i=0; i < variableVolumesNameSpaces; i++ )
      *currPtr++;

   /*-- check to see if volume supports this data stream type --*/
   variableVolumesDataStreams = *currPtr++;
   *volSupports = FALSE;
   for( i=0; i < variableVolumesDataStreams; i++ )
   {
      if (*currPtr == dataStreamType)
          *volSupports = TRUE;
      currPtr++;
   }

   return( SUCCESSFUL );
}
