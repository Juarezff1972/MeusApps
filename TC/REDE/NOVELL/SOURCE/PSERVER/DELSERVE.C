/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <string.h>
#include <npt.h>


WORD PSDetachPrtServerFromFileServer( SPXConnection, fileServer, now,
                                      jobOutcome )

    WORD     SPXConnection;     /* SPX connection to print server           */
    char    *fileServer;        /* File server to detach from               */
    BYTE     now,               /* Detach immediately?                      */
             jobOutcome;        /* Outcome of current jobs                  */

{

    BYTE    request[51];        /* Request buffer                           */
    WORD    reply,              /* Reply buffer                             */
            ccode;              /* Completion code                          */

    /*
        Request format:
             0  Command code
             1  Detach immediately?
             2  Job outcome for current jobs
        3 - 50  File server to detach from
    */
    request[0] = CMD_DETACH_FROM_FILE_SERVER;
    request[1] = now;
    request[2] = jobOutcome;
    memmove(&request[3], fileServer, 48);

    /*
        Make the request
    */
    ccode = PrintServerRequest(SPXConnection, request, sizeof(request),
                               &reply, sizeof(reply));

    /*
        If the SPX send/receive worked, get return code from the reply packet
    */
    if (ccode == 0)
        return (reply);

    /*
        If the SPX send/recieve failed, return the communications error code
    */
    else
        return (ccode);
}
