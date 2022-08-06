/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <string.h>
#include <npt.h>


WORD PSGetNotifyObject(SPXConnection, printer, sequence, fileServer,
                       objectName, objectType, initial, next)

    WORD     SPXConnection,     /* SPX connection to print server           */
            *sequence,          /* Sequence number.  Zero for first call    */
            *initial,           /* Seconds before initial notice            */
            *next;              /* Seconds before next notice               */
    BYTE     printer;           /* Printer number                           */
    WORD    *objectType;        /* Type of object to be notified            */
    char    *objectName,        /* Name of object to be notified            */
            *fileServer;        /* File server where notify object resides  */

{

    BYTE     request[4],        /* Request buffer                           */
             reply[106];        /* Reply buffer                             */
    WORD     ccode;             /* Completion code                          */

    /*
        Request format
            0   Command code
            1   Printer number
        2 - 3   Sequence number
    */
    request[0] = CMD_SCAN_NOTIFY_LIST;
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
                  4 -  51   File server where notify object resides
                 52 -  99   Object name to be notified
                100 - 101   Object type to be notified
                102 - 103   Seconds till first notice
                104 - 105   Seconds till next notice
        */
        *sequence = *(WORD *)(reply + 2);
        if (fileServer != NULL)
            memmove(fileServer, reply + 4, 48);
        if (objectName != NULL)
            memmove(objectName, reply + 52, 48);
        if (objectType != NULL)
            *objectType = *(int *)&reply[100];
        if (initial != NULL)
            *initial = *(WORD *)&reply[102];
        if (next != NULL)
            *next = *(WORD *)&reply[104];
        return (*(WORD *)reply);
    }

    /*
        If the SPX send/recieve failed, return the communications error code
    */
    else
        return (ccode);
}
