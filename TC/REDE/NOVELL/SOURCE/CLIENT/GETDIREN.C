/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <string.h>
#include <ntt.h>
#include <nwmisc.h>


WORD GetDirEntry
(
  WORD            serverConnID,
  BYTE            dirHandle,
  NWDIR_ENTRY    *dirEntry
)
{
  WORD    ccode;                 /*  Completion code from OS                */
  int     isV3;                  /*  Determines if using 386 or 286         */
  BYTE    requestBuff[265];      /*  Request packet to be passed to OS      */
  BYTE    replyBuff[134];        /*  Reply packet to be received from OS    */
  int     requestLength;         /*  length of the request buffer - 2       */
  int     replyLength;           /*  length of the reply buffer -2          */


  isV3 = IsV3Supported(serverConnID);
  requestLength = 265 - 2;
  replyLength = 134 - 2;


  isV3 = IsV3Supported(serverConnID);
  if (!isV3)
  {                                       /*...send the 286 send format...*/

       *(WORD *)&requestBuff[0]           = IntSwap(requestLength);
       requestBuff[2]                     = 2;
       requestBuff[3]                     = dirHandle;

       *(WORD *)&requestBuff[4] = (WORD)( IntSwap(1) );

       requestBuff[6]              = 0;
       strcpy( &requestBuff[7], "" );

       *(WORD *)&replyBuff[0]      = IntSwap( replyLength );
       /*...send the 286 NCP...*/
       ccode = _NCPConnRequest( serverConnID, 0x16, requestLength,
                                &requestBuff[0], replyLength, &replyBuff[2] );

  }
  else
  {    /*...send the 386 send format...*/

       *(WORD *)&requestBuff[0]  = IntSwap( requestLength );
       requestBuff[2]            = 31;
       requestBuff[3]            = dirHandle;

       *(WORD *)&replyBuff[0]    = IntSwap( replyLength );
       /*...send the 386 NCP...*/
       ccode = _NCPConnRequest( serverConnID, 0x16, requestLength,
                                &requestBuff[0], replyLength, &replyBuff[2] );
  }

  if (!ccode)
       if (!isV3)
       {         /*...fill in file entry according to 286 return values...*/

         dirEntry->sequence         = (LONG)0;
         dirEntry->reserved1        = (LONG)0;
         dirEntry->attributes       = (LONG)0;
         dirEntry->reserved2[0]     = 0;
         dirEntry->reserved2[1]     = 0;
         dirEntry->NSType           = 0;
         dirEntry->nameLength       =  (BYTE)strlen( &replyBuff[2] );
         memmove( dirEntry->name, &replyBuff[2], dirEntry->nameLength );
         dirEntry->creationDate     = IntSwap( *( (WORD *)&replyBuff[18] ) );
         dirEntry->creationTime     = IntSwap( *( (WORD *)&replyBuff[20] ) );
         dirEntry->ownerID          = LongSwap( *( (LONG *)&replyBuff[22] ) );
         dirEntry->inheritedRightsMask = replyBuff[26];
         dirEntry->lastArchivedDate = (WORD)0;
         dirEntry->lastArchivedTime = (WORD)0;
         dirEntry->lastArchiverID   = (WORD)0;
         dirEntry->modifyDate       = (WORD)0;
         dirEntry->modifyTime       = (WORD)0;
         *(LONG *)&dirEntry->reserved3[0]  = (LONG)0;
         *(LONG *)&dirEntry->reserved4[0]  = (LONG)0;
       }
       else
       {                /*...the 386 return format mapped to file entry...*/

         dirEntry->sequence         = (LONG)0;
         dirEntry->reserved1        = *( (LONG *)&replyBuff[2] ) ;
         dirEntry->attributes       = *( (LONG *)&replyBuff[6] ) ;
         dirEntry->reserved2[0]     = replyBuff[10];
         dirEntry->reserved2[1]     = replyBuff[11];
         dirEntry->NSType           = replyBuff[12];
         dirEntry->nameLength       = replyBuff[13];
         memmove( dirEntry->name, &replyBuff[14], dirEntry->nameLength );
         dirEntry->creationDate     = *( (WORD *)&replyBuff[28] );
         dirEntry->creationTime     = *( (WORD *)&replyBuff[26] );
         dirEntry->ownerID          = LongSwap( (* (LONG *)&replyBuff[30] ) );
         dirEntry->lastArchivedDate = *( (WORD *)&replyBuff[36] );
         dirEntry->lastArchivedTime = *( (WORD *)&replyBuff[34] );
         dirEntry->lastArchiverID   = LongSwap( (* (LONG *)&replyBuff[38] ) );
         dirEntry->modifyDate       = *( (WORD *)&replyBuff[44] );
         dirEntry->modifyTime       = *( (WORD *)&replyBuff[42] );
         *(LONG *)&dirEntry->reserved3[0] = *( (LONG *)&replyBuff[46] );
         dirEntry->inheritedRightsMask = *( (WORD *)&replyBuff[98] );
         *(LONG *)&dirEntry->reserved4[0] = *( (LONG *)&replyBuff[100] );
       }
  return (ccode);
}
