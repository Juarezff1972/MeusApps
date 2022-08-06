/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <npt.h>


WORD PSSetRemoteMode(SPXConnection, printer, mode)

    WORD    SPXConnection;      /* SPX connection to print server           */
    BYTE    printer;            /* Printer number                           */
    BYTE    mode;               /* Shared or Private?                       */

{

    BYTE    request[3];         /* Request buffer                           */
    WORD    reply,              /* Reply buffer                             */
            ccode;              /* Completion code                          */

    /*
        Request format:
            0   Command code
            1   Printer number
            2   Shared or Private?
    */
    request[0] = CMD_SET_REMOTE_MODE;
    request[1] = printer;
    request[2] = mode;

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
