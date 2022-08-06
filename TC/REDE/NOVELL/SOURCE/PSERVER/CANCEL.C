/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <npt.h>


WORD PSCancelDownRequest(SPXConnection)

    WORD    SPXConnection;      /* SPX connection to print server           */

{

    BYTE    request;            /* Request buffer                           */
    WORD    reply,              /* Reply buffer                             */
            ccode;              /* Completion code                          */

    /*
        Request format:
            0   Command code
    */
    request = CMD_CANCEL_DOWN;

    /*
        Make the request
    */
    ccode = PrintServerRequest(SPXConnection, &request, sizeof(request),
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
