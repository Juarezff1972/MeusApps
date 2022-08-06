/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>



/*_______________________ CheckNetWareVersion _______________________________
|
| Output:   0  --  Server Version is O.K.
|           1  --  Version is too low
|           2  --  SFT level is too low
|           3  --  TTS level is too low
|           Server Error Code  --  If an error occurred.
|
| Comments:
|   This routine checks the actual Version, SubVersion, Revision, SFT and TTS
|   levels.
|___________________________________________________________________________*/

int CheckNetWareVersion(minimumVersion, minimumSubVersion, minimumRevision,
                        minimumSFT, minimumTTS)

WORD  minimumVersion;        /* Minimum major netware version number */
WORD  minimumSubVersion;     /* Minimum minor netware version number */
WORD  minimumRevision;       /* Minimum revision number              */
WORD  minimumSFT;            /* Minimum SFT Level                    */
WORD  minimumTTS;            /* Minimum TTS Level                    */
{
    BYTE requestPacket[3], replyPacket[130];
    int ccode;

    *((int *)requestPacket) = 1;
    requestPacket[2] = (BYTE)17;
    *((int *)replyPacket) = 128;
    memset(replyPacket + 2, 0, 128);

    ccode = _ShellRequest((BYTE)227, requestPacket, replyPacket);
    if (ccode != 0)
        goto Error0;

    if ((WORD)replyPacket[50] > minimumVersion)
        goto CheckSFT;

    if ((WORD)replyPacket[50] < minimumVersion)
    {
        ccode = 1;
        goto Error0;
    }

    if ((WORD)replyPacket[51] > minimumSubVersion)
        goto CheckSFT;

    if ((WORD)replyPacket[51] < minimumSubVersion)
    {
        ccode = 1;
        goto Error0;
    }

    if ((WORD)replyPacket[58] < minimumRevision)
    {
        ccode = 1;
        goto Error0;
    }

CheckSFT:
    if ((WORD)replyPacket[59] < minimumSFT)
    {
        ccode = 2;
        goto Error0;
    }

    if ((WORD)replyPacket[60] < minimumTTS)
        ccode = 3;

/* Error Recovery */
Error0:
    return (ccode);
}
