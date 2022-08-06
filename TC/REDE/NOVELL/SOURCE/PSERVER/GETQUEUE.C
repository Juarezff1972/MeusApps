/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <string.h>
#include <npt.h>


WORD PSGetQueuesServiced(SPXConnection, printer, sequence, fileServer,
                         queue, priority)

    WORD     SPXConnection,     /* SPX connection to print server           */
            *sequence;          /* Sequence number.  Zero for first call    */
    BYTE     printer;           /* Printer number                           */
    char    *fileServer,        /* Name of file server where queue resides  */
            *queue;             /* Name of queue                            */
    BYTE    *priority;          /* Priority of queue                        */

{

    BYTE     request[4],        /* Request buffer                           */
             reply[101];        /* Reply buffer                             */
    WORD     ccode;             /* Completion code                          */

    /*
        Request format
            0   Command code
            1   Printer number
        2 - 3   Sequence number
    */
    request[0] = CMD_SCAN_QUEUE_LIST;
    request[1] = printer;
    *(WORD *)(request + 2) = *sequence;

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
                  0 -   1   Return code
                  2 -   3   Sequence number
                  4 -  51   File server where queue resides
                 52 -  99   Queue name
                      100   Priority
        */
        *sequence = *(WORD *)(reply + 2);
        if (fileServer != NULL)
            memmove(fileServer, reply + 4, 48);
        if (queue != NULL)
            memmove(queue, reply + 52, 48);
        if (priority != NULL)
            *priority = reply[100];
        return (*(WORD *)reply);
    }

    /*
        If the SPX send/recieve failed, return the communications error code
    */
    else
        return (ccode);

}
