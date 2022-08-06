/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>


int GetFileServerDescriptionStrings(companyName, revision, revisionDate,
                                    copyrightNotice)
char *companyName, *revision, *revisionDate, *copyrightNotice;
{
    BYTE requestPacket[3], replyPacket[514];
    int ccode, length;

    *((WORD *)requestPacket) = 1;
    requestPacket[2] = (BYTE)201;
    *((WORD *)replyPacket) = 512;

    ccode = _ShellRequest((BYTE)227, requestPacket, replyPacket);
    if (!ccode)
     {
        strcpy(companyName, (char *)(replyPacket + 2));
        length = 3 + strlen((char *)(replyPacket + 2));
        strcpy(revision, (char *)(replyPacket + length));
        length += strlen((char *)(replyPacket + length)) + 1;
        strcpy(revisionDate, (char *)(replyPacket + length));
        length += strlen((char *)(replyPacket + length)) + 1;
        strcpy(copyrightNotice, (char *)(replyPacket + length));
     }
    return (ccode);
}
