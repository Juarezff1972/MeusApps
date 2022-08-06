/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <nit.h>
#include <ntt.h>

typedef struct
{
    WORD      bufferLength;
    BYTE      subFunction;
    BYTE      volNumber;
    LONG      sequence;
} REQUEST_BODY;

WORD ScanVolForRestrictions
(
WORD     serverConnID,
BYTE     volumeNumber,
LONG     sequence,
NWBUFFER NWBuffer
)
{
   REQUEST_BODY    requestBody;
   WORD            ccode;
   int             i,*first, *second, temp;

   requestBody.bufferLength = IntSwap(sizeof(requestBody)-2);
   requestBody.subFunction  = 32;
   requestBody.volNumber    = volumeNumber;
   if ((sequence == 0) || (sequence == -1))
      requestBody.sequence   = 0;
   else
      requestBody.sequence     = sequence;

   *(WORD *)&NWBuffer[0] = 512;

                                             /*...0x16 file services NCPs...*/
      ccode = _NCPConnRequest(serverConnID, 0x16,sizeof(requestBody),
                                 &requestBody,NCP_MAX_BUFFER,NWBuffer);
      if (!ccode )
      {
          first = (int *)&NWBuffer[1];
          for (i = 0;i < NWBuffer[0];i++)
          {
                second  = first + 1;
                *first  = IntSwap(*first);
                *second = IntSwap(*second);
                temp    = *second;
                *second = *first;
                *first  = temp;
                first   += 4;
          }
      }
   return(ccode);
}
