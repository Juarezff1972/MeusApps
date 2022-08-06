/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */
#include <string.h>
#include <ntt.h>
#include <nwdir.h>

WORD MoveEntry
(
WORD     serverConnID,
BYTE     sourceAttributes,
BYTE     sourceDirHandle,
char     *sourcePath,
BYTE     destDirHandle,
char     *destPath
)
{
   NWBUFFER  requestBody;
   WORD      requestLength;
   char      transformPath[NWMAX_NCP_PATH + 12];
   int       destIndex;           
   int       totalPathLength;
   BYTE      tmpHandle1, tmpHandle2;
   char      tmpPath1[255], tmpPath2[255];
   WORD      ccode;


   memset( tmpPath1, 0, 255 );
   /*-- Setup directory handle and path for call to OS --*/
   ccode = ConvertDirectoryHandle( serverConnID, sourceDirHandle, sourcePath,
                                   &tmpHandle1, tmpPath1 );
   if( ccode ) 
    {
      DeallocateDirectoryHandle( tmpHandle1 );
      return( ccode );
    }

   memset( tmpPath2, 0, 255 );
   ccode = ConvertDirectoryHandle( serverConnID, destDirHandle, destPath,
                                   &tmpHandle2, tmpPath2 );
   if( ccode ) 
    {
      DeallocateDirectoryHandle( tmpHandle1 );
      DeallocateDirectoryHandle( tmpHandle2 );
      return( ccode );
    }

   requestBody[2]    = 46;
   requestBody[3]    = tmpHandle1;
   requestBody[4]    = sourceAttributes;
   if( tmpPath1 )
   {
      (void) _NWMapWildPath( tmpPath1, transformPath );
      /* put count of sub-entries in request and map path */
      requestBody[5] = (BYTE)( _NWMapLenPrecPath( transformPath,
                                                  &requestBody[6],
                                                  &totalPathLength ) );
   }
   else
      requestBody[5] = 0;
                           /* set index of start of destination section */
   destIndex = 6 + totalPathLength;
   requestBody[destIndex] = tmpHandle2;
   if( tmpPath2 )
    {
      (void) _NWMapWildPath( tmpPath2, transformPath );
      requestBody[ destIndex + 1] = (BYTE)( _NWMapLenPrecPath(
                                                  transformPath,
                                                  &requestBody[destIndex + 2],
                                                  &totalPathLength ) );
      requestLength = totalPathLength + destIndex + 2;
    }
   else
    {
      requestBody[destIndex + 1] = 0;
      requestLength = destIndex + 2;
    }
   requestBody[0]  = (BYTE)( requestLength - sizeof(WORD) ) / 256;
   requestBody[1]  = (BYTE)( requestLength - sizeof(WORD) ) % 256;

   ccode = _NCPConnRequest( serverConnID, 22, requestLength, 
                            requestBody, 0, NULL );

   DeallocateDirectoryHandle( tmpHandle1 );
   DeallocateDirectoryHandle( tmpHandle2 );
   return( ccode );
}
