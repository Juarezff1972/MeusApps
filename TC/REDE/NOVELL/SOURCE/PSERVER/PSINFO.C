/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <string.h>
#include <npt.h>


WORD PSGetPrintServerInfo(SPXConnection, info, size)

    WORD     SPXConnection;     /* SPX connection to print server           */
    PS_INFO *info;              /* Print server information structure       */
    WORD     size;              /* Maximum number of bytes to return        */

{

    BYTE     request,           /* Request buffer                           */
             reply[100];        /* Reply buffer.  We left room for growth!  */
    WORD     ccode;             /* Completion code                          */

    /*
        Request format
            0   Command code
    */
    request = CMD_GET_PRINT_SERVER_INFO;

    /*
        Make the request
    */
    ccode = PrintServerRequest(SPXConnection, &request, sizeof(request),
                               reply, sizeof(reply));

    /*
        If the SPX send/receive worked, get print server info and return
        code from the reply packet
    */
    if (ccode == 0)
    {
        if (info != NULL)
            memmove(info, &reply[2], size);
        return (*(WORD *)reply);
    }

    /*
        If the SPX send/recieve failed, return the communications error code
    */
    else
        return (ccode);

}
