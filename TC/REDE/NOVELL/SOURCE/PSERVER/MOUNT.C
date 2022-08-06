/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <npt.h>


WORD PSSetMountedForm(SPXConnection, printer, formNumber)

    WORD    SPXConnection;      /* SPX connection to print server           */
    BYTE    printer;            /* Printer number                           */
    WORD    formNumber;         /* Form number                              */

{

    BYTE    request[3];         /* Request buffer                           */
    WORD    reply,              /* Reply buffer                             */
            ccode;              /* Completion code                          */

    /*
        Request format:
            0   Command code
            1   Printer number
            2   Form number
    */
    request[0] = CMD_MOUNT_FORM;
    request[1] = printer;
    request[2] = (BYTE)formNumber;

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
