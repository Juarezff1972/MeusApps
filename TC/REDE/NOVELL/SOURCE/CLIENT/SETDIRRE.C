/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <ntt.h>

#define MAX_REQ_BUFF_LENGTH (6 + sizeof(WORD))

WORD  SetDirRestriction
(
WORD serverConnID,
BYTE dirHandle,
LONG restriction
)
{
   BYTE requestBuff[MAX_REQ_BUFF_LENGTH];
                                            /* set up request buffer format */
   *(WORD *)&requestBuff[0] = MAX_REQ_BUFF_LENGTH - sizeof(WORD);
   requestBuff[2]           = 36;
   requestBuff[3]           = dirHandle;
   *(LONG *)&requestBuff[4] = restriction;

               /* 22 is command code for "file services" directory services */
   return(_NCPConnRequest(serverConnID, 22, MAX_REQ_BUFF_LENGTH, requestBuff,
                          0, NULL));
}
