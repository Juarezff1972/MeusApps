/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <ntt.h>
#include <nwmisc.h>

#define NO_RESTRICTION_VALUE 0x40000000
#define MAX_REQ_BUFF_LENGTH  (6 + sizeof(WORD))
#define MAX_RET_BUFF_LENGTH  (8 + sizeof(WORD))

WORD  GetObjectDiskRestrictions
(
WORD serverConnID,
BYTE volNumber,
LONG objectID,
LONG *volRestriction,
LONG *blocksInUse
)
{

   BYTE requestBuff[MAX_REQ_BUFF_LENGTH], returnBuff[MAX_RET_BUFF_LENGTH];
   WORD ccode;
                                            /* set up request buffer format */
   *(WORD *)&requestBuff[0] = MAX_REQ_BUFF_LENGTH - sizeof(WORD);
   requestBuff[2]           = 41;
   requestBuff[3]           = volNumber;
   *(LONG *)&requestBuff[4] = LongSwap(objectID);
                                             /* set up return buffer format */
   *(WORD *)&returnBuff[0] = MAX_RET_BUFF_LENGTH - sizeof(WORD);
       /* 22 is the NCP command code for "file services" directory services */
   ccode = _NCPConnRequest(serverConnID, 22, MAX_REQ_BUFF_LENGTH, requestBuff,
                           MAX_RET_BUFF_LENGTH, returnBuff);
                                   /* for F2 NCP interface offset 0 doesn't */
                                      /* contain the buffer len upon return */
   *volRestriction = *(LONG *)&returnBuff[0];
   if (*volRestriction == NO_RESTRICTION_VALUE)/* 40,000,000 means no restr */
      *volRestriction = 0;
   *blocksInUse    = *(LONG *)&returnBuff[4];

   return (ccode);
}
