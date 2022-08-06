/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <string.h>
#include <npt.h>


WORD PSAddQueueToPrinter(SPXConnection, printer, fileServer, queue, priority)

    WORD     SPXConnection;     /* SPX connection to print server           */
    BYTE     printer,           /* Printer number                           */
             priority;          /* Priority of this queue                   */
    char    *fileServer,        /* Name of file server where queue resides  */
            *queue;             /* Name of queue to be add to service list  */

{

    BYTE     request[99];       /* Request buffer                           */
    WORD     reply,             /* Reply buffer                             */
             ccode;             /* Completion code                          */

    /*
        Request format:
                 0  Command code
                 1  Printer number
            2 - 49  File server name
           50 - 97  Queue name
                98  Priority
    */
    request[0] = CMD_ADD_QUEUE;
    request[1] = printer;
    memmove(&request[2], fileServer, 48);
    memmove(&request[50], queue, 48);
    request[98] = priority;

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
