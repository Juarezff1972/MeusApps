/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <ntt.h>

#define    REQ_LENGTH    (255 +9)      /*...largest possible request body...*/

typedef  struct
{
    WORD     bufferLength;
    BYTE     subFunction;
    BYTE     volumeNumber;
} BUFFER_REQUEST;


WORD FillNameSpaceBuffer
(
WORD       serverConnID,
BYTE       volumeNum,
NWBUFFER   NWBuffer
)

{
    BUFFER_REQUEST    requestBody;
    WORD              ccode;
                                           /*...set up NCP request buffer...*/
    requestBody.bufferLength = sizeof(requestBody) - 2;
    requestBody.subFunction  = 47;                   /*...NCP subfunction...*/
    requestBody.volumeNumber = volumeNum;

    *(WORD *)NWBuffer = sizeof(NWBUFFER) - 2;
                                /*...0x16 NCP command code, file services...*/
    ccode = _NCPConnRequest(serverConnID, 0x16, sizeof(requestBody),
                            &requestBody,NCP_MAX_BUFFER,NWBuffer);
    return(ccode);
}
