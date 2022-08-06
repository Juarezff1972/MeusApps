/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <string.h>
#include <npt.h>


WORD PSGetNextRemotePrinter(SPXConnection, printer, printerType, printerName)

    WORD     SPXConnection;     /* SPX connection to print server           */
    BYTE    *printer;           /* Sequence number.  Zero for first call    */
    WORD    *printerType;       /* Type of returned printer                 */
    char    *printerName;       /* Name of returned printer                 */

{

    BYTE     request[4],        /* Request buffer                           */
             reply[53];         /* Reply buffer                             */
    WORD     ccode;             /* Completion code                          */

    /*
        Request format
            0   Command code
            1   Sequence number
    */
    request[0] = CMD_GET_REMOTE;
    request[1] = *printer;

    /*
        Make the request
    */
    ccode = PrintServerRequest( SPXConnection, request, sizeof(request),
                                reply, sizeof(reply) );

    /*
        If the SPX send/receive worked, get return code, printer type and
        name from the reply packet
    */
    if (ccode == 0)
    {
        /*
            Reply format
                0 -  1  Return code
                     2  Sequence number
                3 -  4  Printer type
                5 - 52  Printer name

        */
        if (printer != NULL)
            *printer = reply[2];
        if (printerType != NULL)
            *printerType = *(int *)&reply[3];
        if (printerName != NULL)
            memmove(printerName, (char *)&reply[5], 48);
        return (*(WORD *)reply);
    }

    /*
        If the SPX send/recieve failed, return the communications error code
    */
    else
        return (ccode);

}
