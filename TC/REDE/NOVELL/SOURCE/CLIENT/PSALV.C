/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <string.h>
#include <ntt.h>
#include <nwdir.h>

#define MAX_REQ_BUFF_LENGTH (sizeof(WORD) + 7 + 255)

WORD PurgeSalvagableFile
(
WORD serverConnID,
BYTE dirHandle,
LONG entryID,
char *fileName
)
{
   BYTE  requestBuff[MAX_REQ_BUFF_LENGTH];
   BYTE  tmpHandle;
   char  tmpPath[255];
   WORD  ccode;


   memset( tmpPath, 0, 255 );
   /*-- Setup directory handle and path for call to OS --*/
   ccode = ConvertDirectoryHandle( serverConnID, dirHandle, fileName,
                                   &tmpHandle, tmpPath );
   if( ccode ) 
    {
      DeallocateDirectoryHandle( tmpHandle );
      return( ccode );
    }

   requestBuff[2]           = 29;
   requestBuff[3]           = tmpHandle;
   *(LONG *)&requestBuff[4] = entryID;
   requestBuff[8]           = (BYTE)strlen( tmpPath );
   strcpy( (char *)&requestBuff[9], tmpPath );
   *(WORD *)&requestBuff[0] = 7 + requestBuff[8];

   ccode = _NCPConnRequest( serverConnID, 22, requestBuff[0] + sizeof(WORD),
                            requestBuff, 0, NULL );

   DeallocateDirectoryHandle( tmpHandle );
   return( ccode );
}
