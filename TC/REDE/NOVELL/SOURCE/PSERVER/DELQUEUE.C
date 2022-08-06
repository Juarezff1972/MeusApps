/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <string.h>
#include <npt.h>


WORD PSDeleteQueueFromPrinter(SPXConnection, printer, fileServer, queue,
                              now, jobOutcome)

    WORD     SPXConnection;     /* SPX connection to print server           */
    BYTE     printer;           /* Printer number                           */
    char    *fileServer,        /* Name of file server where queue resides  */
            *queue;             /* Name of queue to be add to service list  */
    BYTE     now,               /* Quit servicing the queue immediately?    */
             jobOutcome;        /* Outcome of current job                   */

{

    BYTE     request[100];      /* Request buffer                           */
    WORD     reply,             /* Reply buffer                             */
             ccode;             /* Completion code                          */

    /*
        Request format:
                 0  Command code
                 1  Printer number
                 2  Quit servicing the queue immediately?
                 3  Outcome of current job
            4 - 51  File server name
           52 - 99  Queue name
    */
    request[0] = CMD_DELETE_QUEUE;
    request[1] = printer;
    request[2] = now;
    request[3] = jobOutcome;
    memmove(&request[4], fileServer, 48);
    memmove(&request[52], queue, 48);

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
