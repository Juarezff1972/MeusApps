/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <string.h>
#include <npt.h>


WORD PSGetPrinterStatus(SPXConnection, printer, status, problem, hasJob,
                        serviceMode, formNumber, formName, printerName)

    WORD     SPXConnection;     /* SPX connection to print server           */
    BYTE     printer,           /* Printer number                           */
            *status,            /* Status of printer                        */
            *problem,           /* On line/Off line/Out of paper            */
            *hasJob,            /* Does printer have an active job?         */
            *serviceMode;       /* Queue service mode                       */
    WORD    *formNumber;        /* Mounted form number                      */
    char    *formName,          /* Mounted form name                        */
            *printerName;       /* Printer name                             */

{

    BYTE     request[4],        /* Request buffer                           */
             reply[72];         /* Reply buffer                             */
    WORD     ccode;             /* Completion code                          */

    /*
        Request format
            0   Command code
            1   Printer number
    */
    request[0] = CMD_GET_PRINTER_STATUS;
    request[1] = printer;

    /*
        Make the request
    */
    ccode = PrintServerRequest(SPXConnection, request, sizeof(request),
                               reply, sizeof(reply));

    /*
        If the SPX send/receive worked, get return code from the reply packet
    */
    if (ccode == 0)
    {
        /*
            Reply format
                0 -  1  Return code
                     2  Printer status
                     3  Problem with printer
                     4  Printer has an active job
                     5  Queue service mode
                6 -  7  Mounted form number
                8 - 23  Mounted form name
               24 - 71  Printer name
        */
        if (status != NULL)
            *status = reply[2];
        if (problem != NULL)
            *problem = reply[3];
        if (hasJob != NULL)
            *hasJob = reply[4];
        if (serviceMode != NULL)
            *serviceMode = reply[5];
        if (formNumber != NULL)
            *formNumber = *(WORD *)(reply + 6);
        if (formName != NULL)
            memmove(formName, reply + 8, 16);
        if (printerName != NULL)
            memmove(printerName, reply + 24, 48);
        return (*(WORD *)reply);
    }

    /*
        If the SPX send/recieve failed, return the communications error code
    */
    else
        return (ccode);

}
