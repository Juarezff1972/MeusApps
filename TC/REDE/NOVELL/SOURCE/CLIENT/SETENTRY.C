 /* COPYRIGHT (c) 1988, 1989 by Novell, Inc.  All Rights Reserved. */

#include <string.h>
#include <ntt.h>
#include <nwmisc.h>

#define MAX_REQ_BUFF_LENGTH (sizeof(WORD) + 11 + 128)
#define MIN(a, b)     ( (a) < (b) ? (a) : (b) )

WORD  SetEntry(   WORD         serverConnID,
                  BYTE         dirHandle,
                  BYTE         searchAttribute,
                  LONG         entryID,
                  LONG         changeBits,
                  void         *entry )
{
   BYTE  requestBuff[512];
   BYTE  replyBuff[512];
   WORD  bytesToCopy;


   /*** SET UP REQUEST BUFFER ***/
   memset( requestBuff, 0, 512 );
   memset( replyBuff, 0, 512 );
   *(WORD *)&requestBuff[0] = 510;
   *(WORD *)&replyBuff[0] = 510;
   requestBuff[2]           = 37;
   requestBuff[3]           = dirHandle;
   requestBuff[4]           = searchAttribute;
   *(LONG *)&requestBuff[5] = entryID;
   *(LONG *)&requestBuff[9] = changeBits;

   *(LONG *)&requestBuff[13] = ((NWFILE_ENTRY *)entry)->reserved1;
   if( changeBits & 0x0002 )
      *(LONG *)&requestBuff[17] = ((NWFILE_ENTRY *)entry)->attributes;
   requestBuff[21] = ((NWFILE_ENTRY *)entry)->reserved2[0];
   requestBuff[22] = ((NWFILE_ENTRY *)entry)->reserved2[1];
   requestBuff[23] = ((NWFILE_ENTRY *)entry)->NSType;


   if ( ((NWFILE_ENTRY *)entry)->NSType == DOS_NAME_SPACE )
    {
       if( changeBits & 0x0001 )
        {
           requestBuff[24] = ((NWFILE_ENTRY *)entry)->nameLength;
           bytesToCopy = MIN( ((NWFILE_ENTRY *)entry)->nameLength, 12 );
           memmove( &requestBuff[25],
                    (BYTE *)((NWFILE_ENTRY *)entry)->name,
                    bytesToCopy );
        }

       if( changeBits & 0x0004 )
           *(WORD *)&requestBuff[39] =
                           ((NWFILE_ENTRY *)entry)->creationDate;

       if( changeBits & 0x0008 )
           *(WORD *)&requestBuff[37] =
                           ((NWFILE_ENTRY *)entry)->creationTime;

       if( changeBits & 0x0010 )
           *(LONG *)&requestBuff[41] =
                           LongSwap( ((NWFILE_ENTRY *)entry)->ownerID);

       if( changeBits & 0x0020 )
           *(WORD *)&requestBuff[47] =
                           ((NWFILE_ENTRY *)entry)->lastArchivedDate;

       if( changeBits & 0x0040 )
           *(WORD *)&requestBuff[45] =
                           ((NWFILE_ENTRY *)entry)->lastArchivedTime;

       if( changeBits & 0x0080 )
           *(LONG *)&requestBuff[49] =
                        LongSwap(((NWFILE_ENTRY *)entry)->lastArchiverID);

       if( changeBits & 0x0100 )
           *(WORD *)&requestBuff[55] = ((NWFILE_ENTRY *)entry)->modifyDate;

       if( changeBits & 0x0200 )
           *(WORD *)&requestBuff[53] = ((NWFILE_ENTRY *)entry)->modifyTime;

       if( changeBits & 0x0400 )
           if( ( searchAttribute & TF_DIRECTORY ) == 0 )
              *(LONG *)&requestBuff[57] =
                           LongSwap( ((NWFILE_ENTRY *)entry)->lastModifierID );

       if( changeBits & 0x0800 )
           if( (searchAttribute & TF_DIRECTORY) == 0 )
              *(WORD *)&requestBuff[111] =
                                 ((NWFILE_ENTRY *)entry)->lastAccessDate;

       *(WORD *)&requestBuff[109] = ((NWFILE_ENTRY *)entry)->inheritedRightsMask;

       if( (searchAttribute & TF_DIRECTORY) == 0 )
        {
           *(LONG *)&requestBuff[61]  = ((NWFILE_ENTRY *)entry)->dataForkSize;
           *(LONG *)&requestBuff[133] =
                           *(LONG *)&((NWFILE_ENTRY *)entry)->reserved4[20];
           *(LONG *)&requestBuff[137] =
                           *(LONG *)&((NWFILE_ENTRY *)entry)->reserved4[24];
        }

       if( searchAttribute & TF_DIRECTORY )
        {
           *(LONG *)&requestBuff[57]  = 
                          *(LONG *)&( ((NWDIR_ENTRY *)entry)->reserved3[0] );
           *(LONG *)&requestBuff[111] =
                          *(LONG *)&( ((NWDIR_ENTRY *)entry)->reserved4[0] );
        }

    }

   if ( ((NWFILE_ENTRY *)entry)->NSType == MAC_NAME_SPACE )
    {
       if( changeBits & M_MAC_MODIFY_NAME )
        {
          requestBuff[24] = ((NWMAC_ENTRY *)entry)->nameLength;
          bytesToCopy = MIN( ((NWMAC_ENTRY *)entry)->nameLength, 32 );
          memmove( &requestBuff[25],
                   ((NWMAC_ENTRY *)entry)->name,
                   bytesToCopy );
        }
       *(LONG *)&requestBuff[57] = ((NWMAC_ENTRY *)entry)->resourceFork;
       *(LONG *)&requestBuff[61] = ((NWMAC_ENTRY *)entry)->resourceForkSize;
       if( changeBits & M_MAC_FINDER_INFO )
        {
          bytesToCopy = MIN( *((NWMAC_ENTRY *)entry)->finderInfo, 32 );
          memmove( &requestBuff[65],
                   ((NWMAC_ENTRY *)entry)->finderInfo,
                   bytesToCopy );
        }
       if( changeBits & M_MAC_PRODOS_INFO )
        {
          bytesToCopy = MIN( *((NWMAC_ENTRY *)entry)->proDosInfo, 6 );
          memmove( &requestBuff[97],
                   ((NWMAC_ENTRY *)entry)->proDosInfo,
                   bytesToCopy );
        }
    }

   return( _NCPConnRequest( serverConnID, 22, 510,
                            requestBuff, 510, &replyBuff[2] ) );

}
