/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <string.h>
#include <nit.h>
#include <ntt.h>

#define MAX_REQ_BUFF_LENGTH (3 + 255 + sizeof(WORD))
#define MAX_RET_BUFF_LENGTH (2 * sizeof(WORD))

WORD GetEffectiveRights
(
WORD serverConnID,
BYTE dirHandle,
char *path,
WORD *rights
)
{
   BYTE  requestBuff[MAX_REQ_BUFF_LENGTH]; 
   BYTE  returnBuff[MAX_RET_BUFF_LENGTH];
   WORD  ccode, requestLength;
   BYTE  tmpHandle;
   char  tmpPath[255];


   memset( tmpPath, 0, 255 );
   /*-- Call ConvertDirectoryHandle to give a proper path specification --*/
   ccode = ConvertDirectoryHandle( serverConnID, dirHandle, path, 
                                   &tmpHandle, tmpPath );
   if( ccode )
      {
      DeallocateDirectoryHandle( tmpHandle );
      return( ccode );
      }

   /* set up request buffer format */
   requestBuff[3]           = tmpHandle;
   if( tmpPath )                             /* check for null length path */
    {
      requestBuff[4]        = (BYTE)strlen( tmpPath );
      strcpy((char *)&requestBuff[5], tmpPath);
    }
   else
      requestBuff[4]        = 0;         /* if path is Null, length is zero */
   *(WORD *)&requestBuff[0] = requestBuff[4] + 3;
   requestLength            = requestBuff[4] + 3 + sizeof(WORD);

   if( IsV3Supported( serverConnID ) == FALSE )
    {
      requestBuff[2]          = 3;
      *(BYTE *)&returnBuff[0] = sizeof(BYTE);
    }
   else
    {
      requestBuff[2]          = 42;
      *(WORD *)&returnBuff[0] = sizeof(WORD);
    }
   ccode = _NCPConnRequest( serverConnID, 22, requestLength, requestBuff,
                            MAX_RET_BUFF_LENGTH, returnBuff );

   if( IsV3Supported( serverConnID ) == FALSE )
    {
      *rights = *(BYTE *)&returnBuff[0];
      *rights = (WORD)MapV3TrusteeRightsToV2((LONG)*rights);
    }
   else
      *rights = *(WORD *)&returnBuff[0];  /* for F2 NCP interface, offset 0 */

   ccode = DeallocateDirectoryHandle( tmpHandle );
   return(ccode);
}
