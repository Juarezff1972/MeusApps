/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <ntt.h>
#include <nwmisc.h>

#define MAX_REQ_BUFF_LENGTH (10 + sizeof(WORD))

WORD  SetVolRestriction
(
WORD serverConnID,
BYTE volNumber,
LONG objectID,
LONG restriction
)
{
   BYTE requestBuff[MAX_REQ_BUFF_LENGTH];

                                            /* set up request buffer format */
   *(WORD *)&requestBuff[0] = MAX_REQ_BUFF_LENGTH - sizeof(WORD);
   requestBuff[2]           = 33;
   requestBuff[3]           = volNumber;
   *(LONG *)&requestBuff[4] = LongSwap(objectID);
   *(LONG *)&requestBuff[8] = restriction;

                                  /* 22 for NCP command code, file services */
   return(_NCPConnRequest(serverConnID, 22, MAX_REQ_BUFF_LENGTH, requestBuff,
                          0, NULL));
}
