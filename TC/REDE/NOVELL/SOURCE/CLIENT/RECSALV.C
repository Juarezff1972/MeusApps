/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <string.h>
#include <ntt.h>
#include <nwdir.h>

#define MAX_REQ_BUFF_LENGTH (sizeof(WORD) + 8 + 32 + 32)

WORD RecoverSalvagableFile
(
WORD serverConnID,
BYTE dirHandle,
LONG entryID,
char *fileName,
char *newName
)
{
   BYTE requestBuff[MAX_REQ_BUFF_LENGTH];
   BYTE tmpHandle;
   char tmpPath[255];
   WORD ccode;

   
   memset( tmpPath, 0, 255 );
   /*-- Setup directory handle and path for call to OS --*/
   ccode = ConvertDirectoryHandle( serverConnID, dirHandle, fileName,
                                   &tmpHandle, tmpPath );
   if( ccode ) 
      {
      DeallocateDirectoryHandle( tmpHandle );
      return( ccode );
      }

   requestBuff[2]           = 28;
   requestBuff[3]           = tmpHandle;
   *(LONG *)&requestBuff[4] = entryID;
   requestBuff[8]           = (BYTE)strlen( tmpPath );
   strcpy((char *)&requestBuff[9], tmpPath );
   requestBuff[9 + requestBuff[8]] = (BYTE)strlen( newName );
   strcpy((char *)&requestBuff[10 + requestBuff[8]], newName );
   *(WORD *)&requestBuff[0] = 8 + requestBuff[8] + 
                              requestBuff[9 + requestBuff[8]];

   ccode = _NCPConnRequest( serverConnID, 22, requestBuff[0] + sizeof(WORD),
                            requestBuff, 0, NULL );
   DeallocateDirectoryHandle( tmpHandle );
   return( ccode );
}
