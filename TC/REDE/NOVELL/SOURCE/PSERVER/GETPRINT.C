/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <string.h>
#include <npt.h>


WORD PSGetPrintersServicingQueue(SPXConnection, fileServer, queue,
                                 maxPrinters, numPrinters, printerList)

    WORD     SPXConnection;         /* SPX connection to print server       */
    char    *fileServer,            /* File server where queue resides      */
            *queue;                 /* Name of queue                        */
    BYTE     maxPrinters,           /* Maximum number of printers to return */
            *numPrinters,           /* Number of printers servicing queue   */
            *printerList;           /* List of printers servicing queue     */

{

    BYTE     request[98],       /* Request buffer                           */
             reply[50];         /* Reply buffer.  (Lots of room for growth) */
    WORD     ccode;             /* Completion code                          */

    /*
        Request format:
                 0  Command code
            1 - 48  File server name
           49 - 96  Queue name
                97  Maximum number of printers to return
    */
    request[0] = CMD_GET_PRINTERS_FOR_QUEUE;
    memmove(&request[1], fileServer, 48);
    memmove(&request[49], queue, 48);
    request[97] = maxPrinters;

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
                0 -  1  Return code
                     2  Number of printers servicing queue
                3 - 49  Printer numbers of printers servicing queue
        */
        if (numPrinters != NULL)
            *numPrinters = reply[2];
        if (printerList != NULL)
            memmove(printerList, &reply[3], maxPrinters);
        return (*(WORD *)reply);
    }

    /*
        If the SPX send/recieve failed, return the communications error code
    */
    else
        return (ccode);
}
