/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <string.h>
#include <npt.h>


WORD PSDeleteNotifyObject(SPXConnection, printer, fileServer, objectName,
                          objectType)

    WORD     SPXConnection;     /* SPX connection to print server           */
    BYTE     printer;           /* Printer number                           */
    char    *fileServer,        /* Name of file server where object resides */
            *objectName;        /* Name of object to be notified            */
    WORD     objectType;        /* Type of the object to be notified        */

{

    BYTE     request[100];      /* Request buffer                           */
    WORD     reply,             /* Reply buffer                             */
             ccode;             /* Completion code                          */

    /*
        Request format:
                 0  Command code
                 1  Printer number
           2 -  49  File server name
          50 -  97  Object name
          98 -  99  Object type
    */
    request[0] = CMD_DELETE_NOTIFY;
    request[1] = printer;
    memmove(&request[2], fileServer, 48);
    memmove(&request[50], objectName, 48);
    *(WORD *)&request[98] = objectType;

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
