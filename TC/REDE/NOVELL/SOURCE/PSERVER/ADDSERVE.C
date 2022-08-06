/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <string.h>
#include <npt.h>


WORD PSAttachPrintServerToFileServer(SPXConnection, fileServer, password)

    WORD    SPXConnection;          /* SPX connection to print server       */
    char    *fileServer;            /* File server to attach to             */
    char    *password;              /* Password used to attach to server    */

{

    BYTE    request[177];       /* Request buffer                           */
    WORD    reply,              /* Reply buffer                             */
            ccode;              /* Completion code                          */

    /*
        Request format:
              0 Command code
        1 -  48 File server name
       49 - 176 Password print server uses to login to file server
    */
    request[0] = CMD_ATTACH_TO_FILE_SERVER;
    memmove(&request[1], fileServer, 48);
    memmove(&request[49], password, 128);

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
