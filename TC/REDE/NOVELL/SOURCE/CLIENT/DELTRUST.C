/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <string.h>
#include <ntt.h>
#include <nwmisc.h>

#ifndef FALSE
   #define FALSE            0
#endif
   /* largest possible request buffer */
#define MAX_REQ_BUFF_LENGTH (255 + 8 + sizeof(WORD))

WORD DeleteTrustee
(
WORD   serverConnID,
BYTE   dirHandle,
char   *path,
LONG   objectID
)
{
   BYTE   requestBuff[MAX_REQ_BUFF_LENGTH];
   WORD   requestLength;
                                            /* set up request buffer format */
   requestBuff[3]           = dirHandle;
   requestBuff[8]           = 0;                                 /* ignored */

   *(LONG *)&requestBuff[4] = LongSwap(objectID);
   if (path)                                       /* check for NULL string */
      {
      requestBuff[9] = (BYTE)strlen(path);
      strcpy((char *)&requestBuff[10], path);
      }
   else
      requestBuff[9] = 0;                /* if path is NULL, length is zero */
   *(WORD *)requestBuff = (WORD)(8 + requestBuff[9]);
   requestLength = requestBuff[9] + 8 + sizeof(WORD);

   if (IsV3Supported(serverConnID) == FALSE)
      requestBuff[2] = 14;
   else
      requestBuff[2] = 43;
                                        /* NCP request 22 for file services */
   return(_NCPConnRequest(serverConnID, 22, requestLength, requestBuff,
                          0, NULL));
}
