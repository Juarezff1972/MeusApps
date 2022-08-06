/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <string.h>
#include <ntt.h>
#include <nwmisc.h>


WORD GetNameSpaceEntry
(
WORD            serverConnID,
BYTE            volumeNum,
LONG            sequence,
BYTE            NSType,
void            *entryInfo
)
{
   WORD             ccode;
   BYTE             requestBuff[9];
   BYTE             replyBuff[134];
   int              requestSize;
   int              replySize;


   requestSize = 7;
   replySize = 132;

   memset( requestBuff, 0, 9 );
   memset( replyBuff, 0, 134 );
   *(WORD *)&requestBuff[0] = IntSwap( requestSize );
   requestBuff[2]           = 48;
   requestBuff[3]           = volumeNum;
   *(LONG*)&requestBuff[4]  = sequence;
   requestBuff[8]           = NSType;
   *(WORD *)&replyBuff[0]   = IntSwap( replySize );

   ccode = _NCPConnRequest( serverConnID, 0x16, requestSize+2,
                           &requestBuff[0], replySize, &replyBuff[2] );

   if (!ccode)
      {
      /*--- see whether returned entry is a DOS namespace or MAC ---*/

      if ( replyBuff[16] == DOS_NAME_SPACE )
         {
         /*-- if it is a DOS name space then fill the NWDIR_ENTRY structure --*/
         if( *(LONG *)&replyBuff[8] & TF_DIRECTORY )
            {
            ((NWDIR_ENTRY *)entryInfo)->sequence =
                                    *( (LONG *)&replyBuff[2] ) ;
            ((NWDIR_ENTRY *)entryInfo)->reserved1 =
                                    *( (LONG *)&replyBuff[6] ) ;
            ((NWDIR_ENTRY *)entryInfo)->attributes =
                                    *( (LONG *)&replyBuff[10] ) ;
            ((NWDIR_ENTRY *)entryInfo)->reserved2[0] = replyBuff[14];
            ((NWDIR_ENTRY *)entryInfo)->reserved2[1] = replyBuff[15];
            ((NWDIR_ENTRY *)entryInfo)->NSType = replyBuff[16];
            ((NWDIR_ENTRY *)entryInfo)->nameLength = replyBuff[17];
            memmove( ( (NWDIR_ENTRY *)entryInfo)->name, &replyBuff[18],
                     ( (NWDIR_ENTRY *)entryInfo)->nameLength );
            ((NWDIR_ENTRY *)entryInfo)->creationDate =
                                    *( (WORD *)&replyBuff[32] );
            ((NWDIR_ENTRY *)entryInfo)->creationTime =
                                    *( (WORD *)&replyBuff[30] );
            ((NWDIR_ENTRY *)entryInfo)->ownerID =
                                    LongSwap( (* (LONG *)&replyBuff[34] ) );
            ((NWDIR_ENTRY *)entryInfo)->lastArchivedDate =
                                    *( (WORD *)&replyBuff[40] );
            ((NWDIR_ENTRY *)entryInfo)->lastArchivedTime =
                                    *( (WORD *)&replyBuff[38] );
            ((NWDIR_ENTRY *)entryInfo)->lastArchiverID =
                                    LongSwap( (* (LONG *)&replyBuff[42] ) );
            ((NWDIR_ENTRY *)entryInfo)->modifyDate =
                                    *( (WORD *)&replyBuff[48] );
            ((NWDIR_ENTRY *)entryInfo)->modifyTime =
                                    *( (WORD *)&replyBuff[46] );
            *(LONG *)&((NWDIR_ENTRY *)entryInfo)->reserved3[0] =
                                    *( (LONG *)&replyBuff[50] );
            ((NWDIR_ENTRY *)entryInfo)->inheritedRightsMask =
                                    *( (WORD *)&replyBuff[102] );
            *(LONG *)&((NWDIR_ENTRY *)entryInfo)->reserved4[0] =
                                    *( (LONG *)&replyBuff[104] );
            }
         }
      if ( replyBuff[16] == MAC_NAME_SPACE )
         {
         /*-- if MAC name space then fill the NWMAC_ENTRY structure --*/

         ((NWMAC_ENTRY *)entryInfo)->sequence = *( (LONG *)&replyBuff[2] ) ;
         ((NWMAC_ENTRY *)entryInfo)->reserved1 = *( (LONG *)&replyBuff[6] ) ;
         ((NWMAC_ENTRY *)entryInfo)->attributes = *( (LONG *)&replyBuff[10] );
         ((NWMAC_ENTRY *)entryInfo)->reserved2[0] = replyBuff[14];
         ((NWMAC_ENTRY *)entryInfo)->reserved2[1] = replyBuff[15];
         ((NWMAC_ENTRY *)entryInfo)->NSType = replyBuff[16];
         ((NWMAC_ENTRY *)entryInfo)->nameLength = replyBuff[17];
         memmove( ( (NWMAC_ENTRY *)entryInfo)->name, &replyBuff[18],
                  ( (NWMAC_ENTRY *)entryInfo)->nameLength );
         ((NWMAC_ENTRY *)entryInfo)->resourceFork =
                                 *( (LONG *)&replyBuff[50] );
         ((NWMAC_ENTRY *)entryInfo)->resourceForkSize =
                                 *( (LONG *)&replyBuff[54] );
         memmove( ((NWMAC_ENTRY *)entryInfo)->finderInfo, &replyBuff[58], 32 );
         memmove( ((NWMAC_ENTRY *)entryInfo)->proDosInfo, &replyBuff[90], 6 );
         }
      }

   return( ccode );
}
