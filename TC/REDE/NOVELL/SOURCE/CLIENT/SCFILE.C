/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <ctype.h>
#include <string.h>
#include <ntt.h>
#include <nwmisc.h>
#include <nwdir.h>

#define MIN(a, b)     ( (a) < (b) ? (a) : (b) )

WORD ScanFileEntry
(
   WORD            serverConnID,
   BYTE            dirHandle,
   char            *searchPath,
   BYTE            searchAttributes,
   LONG            *sequence,
   NWFILE_ENTRY    *fileEntry
)
{
   WORD   ccode;                 /*  completion code from call to OS     */
   int    isV3;                  /*  true if using 386 netware           */
   BYTE   searchLength;          /*  length of the searchPath            */
   BYTE   requestBuff[265];      /*  request package sent to OS          */
   BYTE   replyBuff[134];        /*  reply package receive from call     */
   int    requestLength;         /*  length of the request buffer - 2    */
   int    replyLength;           /*  length of the reply buffer -2       */
   BYTE   tmpHandle;             
   char   tmpPath[255];

    memset( requestBuff, 0, 265 );
    memset( replyBuff, 0, 134 );
    isV3 = IsV3Supported(serverConnID);
    requestLength = 265 - 2;
    replyLength = 134 - 2;
    if (!isV3)
    {
        /*---------  Send out the package for a 286 request -------*/

         *(WORD *)&requestBuff[0]      = IntSwap(requestLength);
         requestBuff[2]                = 15;
         *(WORD *)&requestBuff[3]      = (WORD)*sequence;
         requestBuff[5]                = dirHandle;
         requestBuff[6]                = searchAttributes;
         searchLength = (BYTE)MIN( strlen(searchPath), 255 );
         requestBuff[7]                = searchLength;
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
            {
            DeallocateDirectoryHandle( tmpHandle );
            return( ccode );
            }

         *(WORD *)&requestBuff[0] = IntSwap(requestLength);
         requestBuff[2]           = 30;
         requestBuff[3]           = tmpHandle;
         requestBuff[4]           = searchAttributes;
         *(LONG *)&requestBuff[5] = *sequence;

         /*----- Parse the string for wild cards and mark them -----*/

         requestBuff[9] = (BYTE)(_NWMapWildPath( tmpPath, &requestBuff[10]) );

         *(LONG *)&replyBuff[0]   = IntSwap( replyLength );

         /*...send the 386 NCP...*/
         ccode = _NCPConnRequest( serverConnID, 0x16, requestLength,
                                  &requestBuff[0], replyLength, &replyBuff[2] );
    }
    if( !ccode )
      if( !isV3 )
       {
        /*----- Fill in the fileEntry according to 286 return values -----*/

         *sequence = *( (WORD *)&replyBuff[2] );
         fileEntry->sequence = *( (WORD *)&replyBuff[2] );
         fileEntry->reserved1 = (LONG)0;
         fileEntry->attributes = *( (LONG *)&replyBuff[10] );
         fileEntry->reserved2[0] = 0;
         fileEntry->reserved2[1] = 0;
         fileEntry->NSType = 0;
         fileEntry->nameLength = (BYTE)strlen( &replyBuff[4] );
         memmove( fileEntry->name, &replyBuff[4], fileEntry->nameLength );
         fileEntry->creationTime = (WORD) 0;
         fileEntry->creationDate = IntSwap( *( (WORD *)&replyBuff[24] ) );
         fileEntry->ownerID = LongSwap( *( (LONG *)&replyBuff[32] ) );
         fileEntry->lastArchivedTime = IntSwap( *( (WORD *)&replyBuff[38] ) );
         fileEntry->lastArchivedDate = IntSwap( *( (WORD *)&replyBuff[36] ) );
         fileEntry->lastArchiverID = (LONG)0;
         fileEntry->modifyTime = IntSwap( *((WORD *)&replyBuff[30]) );
         fileEntry->modifyDate = IntSwap( *( (WORD *)&replyBuff[28] ) );
         fileEntry->lastModifierID = (LONG)0;
         fileEntry->dataForkSize = LongSwap( *( (LONG *)&replyBuff[20] ) );
         fileEntry->inheritedRightsMask = (WORD)0;
         fileEntry->lastAccessDate = IntSwap( *( (WORD *)&replyBuff[26] ) );
         *(LONG *)&fileEntry->reserved4[20] = (LONG)0;
         *(LONG *)&fileEntry->reserved4[24] = (LONG)0;
       }
      else
       {
        /*----- Fill in the fileEntry according to 386 return values -----*/

         *sequence = *( (LONG *)&replyBuff[2] );
         fileEntry->sequence = *( (LONG *)&replyBuff[2] );
         fileEntry->reserved1 = *( (LONG *)&replyBuff[6] );
         fileEntry->attributes = *( (LONG *)&replyBuff[10] );
         fileEntry->reserved2[0] = replyBuff[14];
         fileEntry->reserved2[1] = replyBuff[15];
         fileEntry->NSType = replyBuff[16];
         fileEntry->nameLength = replyBuff[17];
         memmove( fileEntry->name, &replyBuff[18], fileEntry->nameLength );
         fileEntry->creationTime = *( (WORD *)&replyBuff[30] );
         fileEntry->creationDate = *( (WORD *)&replyBuff[32] );
         fileEntry->ownerID = LongSwap( *( (LONG *)&replyBuff[34] ) );
         fileEntry->lastArchivedTime = *( (WORD *)&replyBuff[38] );
         fileEntry->lastArchivedDate = *( (WORD *)&replyBuff[40] );
         fileEntry->lastArchiverID = LongSwap( *( (LONG *)&replyBuff[42] ) );
         fileEntry->modifyTime = *( (WORD *)&replyBuff[46] );
         fileEntry->modifyDate = *( (WORD *)&replyBuff[48] );
         fileEntry->lastModifierID = LongSwap( *( (LONG *)&replyBuff[50] ) );
         fileEntry->dataForkSize = *( (LONG *)&replyBuff[54] );
         fileEntry->inheritedRightsMask = *( (WORD *)&replyBuff[102] );
         fileEntry->lastAccessDate = *( (WORD *)&replyBuff[104] );
         *(LONG *)&fileEntry->reserved4[20] = *( (LONG *)&replyBuff[126] );
         *(LONG *)&fileEntry->reserved4[24] = *( (LONG *)&replyBuff[130] );
         DeallocateDirectoryHandle( tmpHandle );
       }
    else
      if( isV3 )
         DeallocateDirectoryHandle( tmpHandle );
         
    return( ccode );
}
