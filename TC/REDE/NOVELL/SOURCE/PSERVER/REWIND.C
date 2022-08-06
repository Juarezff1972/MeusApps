/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <npt.h>


WORD PSRewindPrintJob(SPXConnection, printer, byPage, relative, copy, offset)

    WORD    SPXConnection,      /* SPX Connection number                    */
            copy;               /* Copy to reiwnd to (if absolute)          */
    BYTE    printer,            /* Printer number                           */
            byPage,             /* Rewind by page?                          */
            relative;           /* Rewind relative to current position?     */
    long    offset;             /* Offset                                   */

{

    BYTE    request[10],        /* Request buffer                           */
            reply[2];           /* Reply buffer                             */
    WORD    ccode;              /* Completion code                          */

    /*
        Request format:
            0   Command code
            1   Printer number
            2   By page (TRUE) or by byte (FALSE)
            3   Relative (TRUE) or absolute (FALSE)
        4 - 5   Copy number if absolute
        6 - 9   Offset:
                    If relative by page, number of pages
                    If relative by byte, number of bytes
                    If absolute by page, page within copy
                    If absolute by byte, byte within copy
    */
    request[0] = CMD_REWIND_PRINT_JOB;
    request[1] = printer;
    request[2] = byPage;
    request[3] = relative;
    *(WORD *)&request[4] = copy;
    *(long *)&request[6] = offset;

    /*
        Make the request
    */
    ccode = PrintServerRequest( SPXConnection, request, sizeof(request),
                                reply, sizeof(reply) );

    /*
        If the SPX send/receive worked, get return code from the reply packet
    */
    if (ccode == 0)
        return (*(WORD *)reply);

    /*
        If the SPX send/recieve failed, return the communications error code
    */
    else
        return (ccode);

}
