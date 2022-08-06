/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <string.h>
#include <npt.h>


WORD PSGetAttachedServers( SPXConnection, sequence, fileServer )

    WORD     SPXConnection;     /* SPX Connection number                    */
    WORD    *sequence;          /* Sequence number.  Zero for first call    */
    char    *fileServer;        /* File server name                         */

{

    BYTE     request[2],        /* Request buffer                           */
             reply[51];         /* Reply buffer                             */
    WORD     ccode;             /* Completion code                          */

    /*
        Request format
            0   Command code
            1   Sequence number
    */
    request[0] = CMD_GET_ATTACHED_SERVERS;
    request[1] = (BYTE)*sequence;

    /*
        Make the request
    */
    ccode = PrintServerRequest( SPXConnection, request, sizeof(request),
                                reply, sizeof(reply) );

    /*
        If the SPX send/receive worked, get return code from the reply packet
    */
    if (ccode == 0)
    {
        /*
            Reply format
                  0 -  1    Return code
                       2    Sequence number
                  3 - 50    File server name
        */
        *sequence = (WORD)reply[2];
        if (fileServer != NULL)
            memmove(fileServer, &reply[3], 48);
        return (*(WORD *)reply);
    }

    /*
        If the SPX send/recieve failed, return the communications error code
    */
    else
        return (ccode);
}
