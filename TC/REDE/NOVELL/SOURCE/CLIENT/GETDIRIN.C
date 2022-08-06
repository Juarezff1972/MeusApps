/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <string.h>
#include <ntt.h>

typedef struct                     /* NCP request format */
{
   WORD   bufferLength;
   BYTE   subFunction;
   BYTE   dirHandle;
} requestFormat;

typedef struct                     /* NCP reply format */
{
   LONG   totalBlocks;
   LONG   availBlocks;
   LONG   totalDirEntries;
   LONG   availDirEntries;
   LONG   flags;
   BYTE   sectorsPerBlock;
   BYTE   volNameLength;
   BYTE   volName[NWMAX_VOL_NAME];
} replyFormat;

WORD GetDirInfo
(
WORD       serverConnID,
BYTE       dirHandle,
NWDIR_INFO *dirInfo
)
{
   WORD          ccode;
   requestFormat requestBody;
   replyFormat   replyBody;
                                                   /* set up request buffer */
   requestBody.bufferLength = sizeof(requestBody) - sizeof(WORD);
   requestBody.subFunction  = 45;
   requestBody.dirHandle    = dirHandle;
           /* offset 0 of replyBody(bufferLength) is set upon entry of call */
           /* but the field is not present after returning from the NCP     */
   *(WORD *)&replyBody      = sizeof(replyBody) - sizeof(WORD);

       /* 22 is the NCP command code for "file services" directory services */
   ccode = _NCPConnRequest(serverConnID, 22, sizeof(requestBody), &requestBody,
                           sizeof(replyBody), &replyBody);
   if (!ccode)
      {
      memcpy(dirInfo, &replyBody, sizeof(replyBody));
      memcpy(dirInfo->volumeName, replyBody.volName, replyBody.volNameLength);
      dirInfo->volumeName[replyBody.volNameLength] = '\0';
      }
   return (ccode);
}
