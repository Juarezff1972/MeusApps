/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <string.h>
#include <ntt.h>
#include <prolog.h>
#include <nwmisc.h>
#include <nwdir.h>
#include <niterror.h>

#define  MIN( a, b )       ( ( a ) < ( b ) ) ? ( a ) : ( b )

WORD ScanEntryForTrustees( WORD     serverConnID,
                           BYTE     directoryHandle,
                           char     *directoryPath,
                           BYTE     *trusteeBuffer,
                           BYTE     trusteeSetNumber,
                           BYTE     trusteeGroupsToReturn )
{
   WORD  isV3;
   BYTE  requestBuff[261], replyBuff[123];
   WORD  ccode;
   BYTE  tmpHandle;
   char  tmpPath[255];
   int   pathLength, tBOffset, rBOffset, i;
   BYTE  bytesToCopy, setsToCopy;


   memset( tmpPath, 0, 255 );
   /*-- Setup directory handle and path for call to OS --*/
   ccode = ConvertDirectoryHandle( serverConnID, directoryHandle, 
                                   directoryPath, &tmpHandle, tmpPath );
   if( ccode ) 
    {
      DeallocateDirectoryHandle( tmpHandle );
      return( ccode );
    }

   if( ( isV3 = IsV3Supported(serverConnID) ) == FALSE )
      {
      requestBuff[2] = 12;
      requestBuff[3] = tmpHandle;
      trusteeSetNumber++;
      requestBuff[4] = trusteeSetNumber;
      pathLength = strlen( tmpPath );
      requestBuff[5] = (BYTE)pathLength;
      memmove( &requestBuff[6], tmpPath, pathLength );
      *(WORD *)&requestBuff[0] = 4 + pathLength;
      *(WORD *)&replyBuff[0] = 49;

      ccode = _NCPConnRequest( serverConnID, 22, 261, &requestBuff[0],
                               49, &replyBuff[2] );
      if( ccode != 0 )
         {
         DeallocateDirectoryHandle( tmpHandle );
         return( ccode );
         }
      setsToCopy = MIN( trusteeGroupsToReturn, 5 );

      /*-- Move the directory path into buffer --*/
      memmove( &trusteeBuffer[0], &replyBuff[2], 16 );

      /*-- Move the creation date and time into buffer --*/
      memmove( &trusteeBuffer[16], &replyBuff[18], 4 );

      /*-- Move and swap the ownerID into buffer --*/
      memmove( &trusteeBuffer[20], &replyBuff[22], 4 );
      *(long *)&trusteeBuffer[20] = LongSwap( *(long *)&trusteeBuffer[20] );

      rBOffset = 26;
      for( i=0; i<5; i++ )
         {
         *(long *)&replyBuff[rBOffset] =
                                 LongSwap( *(long *)&replyBuff[rBOffset] );
         rBOffset += 4;
         }

      /*-- Move the number of trustee sets requested into buffer --*/
      tBOffset = 24;
      rBOffset = 26;

      bytesToCopy = setsToCopy * 4;
      memmove( trusteeBuffer + tBOffset, &replyBuff[rBOffset], bytesToCopy );
      tBOffset += bytesToCopy;
      bytesToCopy = setsToCopy;
      memmove( trusteeBuffer + tBOffset, &replyBuff[rBOffset+20],
                                                               bytesToCopy );
      }
   else
      {
      requestBuff[2] = 38;
      requestBuff[3] = tmpHandle;
      requestBuff[4] = trusteeSetNumber;
      pathLength = strlen( tmpPath );
      requestBuff[5] = (BYTE)pathLength;
      memmove( &requestBuff[6], tmpPath, pathLength );
      *(WORD *)&requestBuff[0] = 4 + pathLength;
      *(WORD *)&replyBuff[0] = 121;

      ccode   = _NCPConnRequest( serverConnID, 22, 261, &requestBuff[0],
                                 121, &replyBuff[2] );
      if( ccode != 0 )
         return( ccode );

      setsToCopy = MIN( trusteeGroupsToReturn, replyBuff[2] );
      *trusteeBuffer = replyBuff[2];
      tBOffset = 1;
      rBOffset = 3;
      for( i=0; i<setsToCopy; i++ )
         {
         *(long *)&replyBuff[rBOffset] =
                                 LongSwap( *(long *)&replyBuff[rBOffset] );
         rBOffset += 4;
         }
      rBOffset = 3;
      bytesToCopy = setsToCopy * 4;
      memmove( trusteeBuffer + tBOffset, &replyBuff[rBOffset], bytesToCopy );
      tBOffset = bytesToCopy + 1;
      bytesToCopy = setsToCopy * 2;
      memmove( trusteeBuffer + tBOffset, &replyBuff[rBOffset+80],
                                                               bytesToCopy );
      } 

   DeallocateDirectoryHandle( tmpHandle );

   return( ccode );
}
