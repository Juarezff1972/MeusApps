/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <string.h>
#include <ntt.h>
#include <nwmisc.h>
#include <nwdir.h>

#define MIN(a, b)     ( (a) < (b) ? (a) : (b) )

WORD ScanFilePhysical
(
WORD            serverConnID,
BYTE            dirHandle,
char            *searchPath,
BYTE            searchAttributes,
LONG            *sequence,
NWPHYS_ENTRY    *physDirEntry
)
{
    WORD  ccode;
    int   isV3;
    int   i;
    BYTE  requestBuff[265];
    BYTE  replyBuff[174];
    int   requestLength;
    int   replyLength;
    int   searchLength;
    BYTE  tmpHandle;
    char  tmpPath[255];


    isV3 = IsV3Supported( serverConnID );
    requestLength = 265 - 2;
    replyLength = 174 - 2;

    if (!isV3)
     {   /*...send the 286 send format...*/

         *(WORD *)&requestBuff[0]      = IntSwap(requestLength);
         requestBuff[2]                = 15;
         *(WORD *)&requestBuff[3]      = (WORD)*sequence;
         requestBuff[5]                = dirHandle;
         requestBuff[6]                = searchAttributes;
         searchLength = MIN( strlen(searchPath), 255 );
         requestBuff[7]                = (BYTE)searchLength;
         memmove( &requestBuff[8], searchPath, searchLength );

         *(WORD *)&replyBuff[0]        = IntSwap(replyLength);

         /*...send the 286 NCP...*/
         ccode = _NCPConnRequest( serverConnID, 0x17, requestLength,
                                &requestBuff[0], replyLength, &replyBuff[2] );
     }
    else
     {
        /*---------  Send out the package for a 386 request -------*/

         memset( tmpPath, 0, 255 );
         /*-- Setup directory handle and path for call to OS --*/
         ccode = ConvertDirectoryHandle( serverConnID, dirHandle, 
                                         searchPath, &tmpHandle, tmpPath );
         if( ccode ) 
            return( ccode );

         *(WORD *)&requestBuff[0] = IntSwap(requestLength);
         requestBuff[2]           = 40;
         requestBuff[3]           = tmpHandle;
         requestBuff[4]           = searchAttributes;
         *(LONG *)&requestBuff[5] = *sequence;

         /*----- Parse the string for wild cards and mark them -----*/

         requestBuff[9] = (BYTE)(_NWMapWildPath( tmpPath, &requestBuff[10] ));

         *(WORD *)&replyBuff[0]   = IntSwap( replyLength );

         ccode = _NCPConnRequest( serverConnID, 0x16, requestLength,
                                  &requestBuff[0], replyLength, &replyBuff[2] );
     }


    if( !ccode )
      if( !isV3 )
       { /*...fill in file entry according to 286 return values...*/
         *sequence = *( (WORD *)&replyBuff[2] );
         physDirEntry->sequence  = *( (WORD *)&replyBuff[2] );
         physDirEntry->reserved1 = (LONG)0;
         physDirEntry->attributes = *( (LONG *)&replyBuff[10] );
         physDirEntry->reserved2[0] = 0;
         physDirEntry->reserved2[1] = 0;
         physDirEntry->NSType = 0;
         physDirEntry->nameLength = (BYTE)strlen( &replyBuff[4] );
         memmove( physDirEntry->name, &replyBuff[4], physDirEntry->nameLength );
         physDirEntry->creationTime = (WORD) 0;
         physDirEntry->creationDate = IntSwap( *( (WORD *)&replyBuff[24] ) );
         physDirEntry->ownerID = LongSwap( *( (LONG *)&replyBuff[32] ) );
         physDirEntry->lastArchivedTime = IntSwap( *( (WORD *)&replyBuff[38] ) );
         physDirEntry->lastArchivedDate = IntSwap( *( (WORD *)&replyBuff[36] ) );
         physDirEntry->lastArchiverID = (LONG)0;
         physDirEntry->modifyTime = IntSwap( *( (WORD *)&replyBuff[30] ) );
         physDirEntry->modifyDate = IntSwap( *( (WORD *)&replyBuff[28] ) );
         physDirEntry->lastModifierID = (LONG)0;
         physDirEntry->dataForkSize = LongSwap( *( (LONG *)&replyBuff[20] ) );
         memmove( &physDirEntry->reserved3[0], 0, 44 );
         physDirEntry->inheritedRightsMask = (WORD)0;
         physDirEntry->lastAccessDate = IntSwap( *( (WORD *)&replyBuff[26] ) );
         *(LONG *)&physDirEntry->reserved4[20] = (LONG)0;
         *(LONG *)&physDirEntry->reserved4[24] = (LONG)0;
         for( i=0; i<10; i++)
            physDirEntry->forkSize[i] = 0;
       }
      else
       {
        /*----- Fill in the physDirEntry according to 386 return values -----*/

         *sequence = *( (LONG *)&replyBuff[2] );
         physDirEntry->sequence  = *( (LONG *)&replyBuff[2] );
         physDirEntry->reserved1 = *( (LONG *)&replyBuff[6] );
         physDirEntry->attributes = *( (LONG *)&replyBuff[10] );
         physDirEntry->reserved2[0] = replyBuff[14];
         physDirEntry->reserved2[1] = replyBuff[15];
         physDirEntry->NSType = replyBuff[16];
         physDirEntry->nameLength = (BYTE)strlen( &replyBuff[18] );
         memmove( physDirEntry->name, &replyBuff[18], physDirEntry->nameLength );
         physDirEntry->creationTime = *( (WORD *)&replyBuff[30] );
         physDirEntry->creationDate = *( (WORD *)&replyBuff[32] );
         physDirEntry->ownerID = LongSwap( *( (LONG *)&replyBuff[34] ) );
         physDirEntry->lastArchivedTime = *( (WORD *)&replyBuff[38] );
         physDirEntry->lastArchivedDate = *( (WORD *)&replyBuff[40] );
         physDirEntry->lastArchiverID = LongSwap( *( (LONG *)&replyBuff[42] ) );
         physDirEntry->modifyTime = *( (WORD *)&replyBuff[46] );
         physDirEntry->modifyDate = *( (WORD *)&replyBuff[48] );
         physDirEntry->lastModifierID = LongSwap( *( (LONG *)&replyBuff[50] ) );
         physDirEntry->dataForkSize = *( (LONG *)&replyBuff[54] );
         memmove( &physDirEntry->reserved3[0], &replyBuff[58], 44 );
         physDirEntry->inheritedRightsMask = *( (WORD *)&replyBuff[102] );
         physDirEntry->lastAccessDate = *( (WORD *)&replyBuff[104] );
         *(LONG *)&physDirEntry->reserved4[20] = *( (LONG *)&replyBuff[126] );
         *(LONG *)&physDirEntry->reserved4[24] = *( (LONG *)&replyBuff[130] );
         memmove( physDirEntry->forkSize, &replyBuff[134], 40 );
         DeallocateDirectoryHandle( tmpHandle );
       }
    else
       if( isV3 )
          DeallocateDirectoryHandle( tmpHandle );

    return(ccode);
}
