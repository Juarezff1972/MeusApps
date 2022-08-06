/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <string.h>
#include <ntt.h>
#include <nwmisc.h>
#include <nwdir.h>

#define MIN(a, b)     ( (a) < (b) ? (a) : (b) )

WORD ScanDirEntry
(
  WORD           serverConnID,      /* Connection ID of server to get info on */
  BYTE           dirHandle,         /* Directory handle of drive              */
  char          *searchPath,        /* Path of directory to get info on       */
  BYTE           searchAttributes,  /* Attributes of directory to look for    */
  LONG          *sequence,          /* Sequence number of directory entry     */
  NWDIR_ENTRY   *dirEntry           /* Structure to receive info on directory */
)
{
    WORD    ccode;                 /*  Completion code from OS              */
    int     isV3;                  /*  Determines if using 386 or 286       */
    BYTE    requestBuff[265];      /*  Request packet to be passed to OS    */
    BYTE    replyBuff[142];        /*  Reply packet to be received from OS  */
    BYTE    searchLength;          /*  length of the searchPath             */
    int     requestLength;         /*  length of the request buffer - 2     */
    int     replyLength;           /*  length of the reply buffer -2        */
    BYTE    tmpHandle;
    char    tmpPath[255];


    isV3 = IsV3Supported(serverConnID);
    requestLength = 265 - 2;
    replyLength = 142 - 2;
    memset( &requestBuff[0], 0, 265 );
    memset( &replyBuff[0], 0, 142 );

    if (!isV3)
     {    /*...send the 286 send format...*/
       *(WORD *)&requestBuff[0]           = IntSwap(requestLength);
       requestBuff[2]                     = 2;
       requestBuff[3]                     = dirHandle;

       if (*sequence == 0 || *sequence == -1)
         *(WORD *)&requestBuff[4] = (WORD)( IntSwap(1) );
       else
         *(WORD *)&requestBuff[4] = (WORD)( IntSwap( (int)(*sequence + 1) ) );

       requestBuff[6]              = (BYTE)strlen( searchPath );
       searchLength = MIN( requestBuff[6], 255 );
       memmove( &requestBuff[7], searchPath, searchLength );

       *(WORD *)&replyBuff[0]      = IntSwap( replyLength );
       /*...send the 286 NCP...*/
       ccode = _NCPConnRequest( serverConnID, 0x16, requestLength,
                                &requestBuff[0], replyLength, &replyBuff[2] );
     }
    else
     {   /*...send the 386 send format...*/

       memset( tmpPath, 0, 255 );
       /*-- Setup directory handle and path for call to OS --*/
       ccode = ConvertDirectoryHandle( serverConnID, dirHandle, 
                                       searchPath, &tmpHandle, tmpPath );
       if( ccode ) 
         {
         DeallocateDirectoryHandle( tmpHandle );
         return( ccode );
         }

       *(WORD *)&requestBuff[0]  = IntSwap( requestLength );
       requestBuff[2]            = 30;
       requestBuff[3]            = tmpHandle;
       requestBuff[4]            = (BYTE)(searchAttributes | TF_DIRECTORY);
       *(LONG *)&requestBuff[5]  = *sequence;
       requestBuff[9]  = (BYTE)(_NWMapWildPath( tmpPath, &requestBuff[10] ) );

       *(WORD *)&replyBuff[0]    = IntSwap( replyLength );
       /*...send the 386 NCP...*/
       ccode = _NCPConnRequest( serverConnID, 0x16, requestLength,
                                &requestBuff[0], replyLength, &replyBuff[2] );
     }

    if( !ccode )
      if( !isV3 )
       { /*...fill in file entry according to 286 return values...*/
         dirEntry->sequence        = *(WORD *)&replyBuff[28];
         dirEntry->reserved1       = (LONG)0;
         dirEntry->attributes      = (LONG)0;
         dirEntry->reserved2[0]    = 0;
         dirEntry->reserved2[1]    = 0;
         dirEntry->NSType          = 0;
         dirEntry->nameLength      =  (BYTE)strlen( &replyBuff[2] );
         memmove( dirEntry->name, &replyBuff[2], dirEntry->nameLength );
         dirEntry->creationDate    = IntSwap( *( (WORD *)&replyBuff[18] ) );
         dirEntry->creationTime    = IntSwap( *( (WORD *)&replyBuff[20] ) );
         dirEntry->ownerID         = LongSwap( *( (LONG *)&replyBuff[22] ) );
         dirEntry->inheritedRightsMask = replyBuff[26];
         *(WORD *)sequence         = *(WORD *)&replyBuff[28];
         dirEntry->lastArchivedDate = (WORD)0;
         dirEntry->lastArchivedTime = (WORD)0;
         dirEntry->lastArchiverID   = (WORD)0;
         dirEntry->modifyDate       = (WORD)0;
         dirEntry->modifyTime       = (WORD)0;
         *(LONG *)&dirEntry->reserved3[0] = (LONG)0;
         *(LONG *)&dirEntry->reserved4[0] = (LONG)0;
       }
      else
       { /*...the 386 return format mapped to file entry...*/

         *sequence                  = *( (LONG *)&replyBuff[2] );
         dirEntry->sequence         = *( (LONG *)&replyBuff[2] ) ;
         dirEntry->reserved1        = *( (LONG *)&replyBuff[6] ) ;
         dirEntry->attributes       = *( (LONG *)&replyBuff[10] ) ;
         dirEntry->reserved2[0]     = replyBuff[14];
         dirEntry->reserved2[0]     = replyBuff[15];
         dirEntry->NSType           = replyBuff[16];
         dirEntry->nameLength       = replyBuff[17];
         memmove( dirEntry->name, &replyBuff[18], dirEntry->nameLength );
         dirEntry->creationDate     = *( (WORD *)&replyBuff[32] );
         dirEntry->creationTime     = *( (WORD *)&replyBuff[30] );
         dirEntry->ownerID          = LongSwap( (* (LONG *)&replyBuff[34] ) );
         dirEntry->lastArchivedDate = *( (WORD *)&replyBuff[40] );
         dirEntry->lastArchivedTime = *( (WORD *)&replyBuff[38] );
         dirEntry->lastArchiverID   = LongSwap( (* (LONG *)&replyBuff[42] ) );
         dirEntry->modifyDate       = *( (WORD *)&replyBuff[48] );
         dirEntry->modifyTime       = *( (WORD *)&replyBuff[46] );
         memmove( &dirEntry->reserved3[0], &replyBuff[50], 56 );
         dirEntry->inheritedRightsMask = *( (WORD *)&replyBuff[106] );
         memmove( &dirEntry->reserved4[0], &replyBuff[108], 26 );
         DeallocateDirectoryHandle( tmpHandle );
       }
    else
       if( isV3 )
          DeallocateDirectoryHandle( tmpHandle );

    return (ccode);
}
