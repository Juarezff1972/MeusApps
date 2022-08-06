/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <npt.h>


WORD PSMarkTopOfForm(SPXConnection, printer, character)
WORD    SPXConnection;      /* SPX connection to print server           */
BYTE    printer;            /* Printer number                           */
BYTE    character;          /* Character used to mark top of form       */
{
    BYTE    request[3];         /* Request buffer                           */
    WORD    reply,              /* Reply buffer                             */
            ccode;              /* Completion code                          */

    /*
        Request format:
            0   Command code
            1   Printer number
            2   Character to mark top of page with
    */
    request[0] = CMD_MARK_PAGE;
    request[1] = printer;
    request[2] = character;

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
