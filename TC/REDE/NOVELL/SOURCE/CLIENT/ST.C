/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <nit.h>
#include <ntt.h>
#include <string.h>

#define MAX_REQ_BUFF_LENGTH (255+9+sizeof(WORD))    /* largest request body */

WORD SetTrustee
(
WORD   connectionID,
BYTE   dirHandle,
char   *path,
LONG   trusteeObjectID,
LONG   trusteeRightsMask
)
{
   WORD   requestLength;
   BYTE   requestBody[MAX_REQ_BUFF_LENGTH];
   BYTE   tmpHandle;
   char   tmpPath[255];
   WORD   ccode;


      memset( tmpPath, 0, 255 );
      /*-- Setup directory handle and path for call to OS --*/
      ccode = ConvertDirectoryHandle( connectionID, dirHandle, 
                                      path, &tmpHandle, tmpPath );

      if( ccode ) 
      {
         DeallocateDirectoryHandle( tmpHandle );
         return( ccode );
      }

      if( IsV3Supported( connectionID ) == FALSE )
       {
         requestBody[2]          = 13;          /* subFunction for 2.x */
         requestBody[3]          = tmpHandle;
         *((LONG *)&requestBody[4]) = LongSwap(trusteeObjectID);
         requestBody[8]          = MapV3TrusteeRightsToV2(trusteeRightsMask);
         if (path)          
          {
            requestBody[9]          = (BYTE)strlen( tmpPath );
            strcpy((char *)requestBody + 10, tmpPath );
          }
         else
            requestBody[9]          = 0;

         requestLength              = requestBody[9] + 8 + sizeof(WORD);
         *(WORD *)requestBody       = requestBody[9] + 8;

       }
      else
       {
         requestBody[2]               = 39;          /* subFunction for 3.x */
         requestBody[3]               = tmpHandle;
         *(LONG *)&requestBody[4]     = LongSwap( trusteeObjectID );
         *(WORD *)&requestBody[8]     = (WORD)trusteeRightsMask;
         if( tmpPath )       /* check for null path, set length accordingly */
          {
            requestBody[10]           = (BYTE)strlen( tmpPath );
            strcpy( (char *)requestBody + 11, tmpPath );
          }
         else
            requestBody[10]           = 0;

         requestLength                = requestBody[10] + 9 + sizeof(WORD);
         *(WORD *)requestBody         = requestBody[10] + 9;
       }

   ccode = _NCPConnRequest( connectionID, 22, requestLength, 
                            requestBody, 0, NULL );

   DeallocateDirectoryHandle( tmpHandle );
   return( ccode );
}

BYTE MapV3TrusteeRightsToV2
(
LONG trusteeRightsV3
)
{
   BYTE   trusteeRightsV2;

   if (trusteeRightsV3 & TR_SUPERVISOR)
      trusteeRightsV2   = TA_ALL;
   else
   {
      trusteeRightsV2   = (BYTE) trusteeRightsV3;
      if (trusteeRightsV3 & (TR_READ|TR_WRITE|TR_CREATE))
         trusteeRightsV2   |= TA_OPEN;
   }
   return(trusteeRightsV2);
}



LONG MapV2TrusteeRightsToV3
(
BYTE trusteeRightsV2
)
{
   LONG   trusteeRightsV3;

   trusteeRightsV3   = (LONG) trusteeRightsV2 & ~TA_OPEN;

   return(trusteeRightsV3);
}
