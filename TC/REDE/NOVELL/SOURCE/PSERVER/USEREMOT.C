/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <string.h>
#include <npt.h>


WORD PSRequestRemotePrinter(SPXConnection, printer, printerInfo)

WORD      SPXConnection;        /* SPX connection to print server       */
BYTE      printer;              /* Number of printer to connect as      */
REMOTE_INFO
         *printerInfo;          /* Struct where printer info is retuned */
{
    BYTE     request[2];        /* Request buffer                       */
    BYTE     reply[22];         /* Reply buffer                         */
    WORD     ccode;             /* Completion code                      */

    /*
        Request format
            0   Command code
            1   Printer number
    */
    request[0] = CMD_CONNECT_REMOTE;
    request[1] = printer;

    /*
        Make the request
    */
    ccode = PrintServerRequest( SPXConnection, request, sizeof(request),
                                reply, sizeof(reply) );

    /*
        If the SPX send/receive worked, get return code, printer type and
        name from the reply packet
    */
    if (ccode == 0)
    {
        /*
            Reply format
                0 -  1  Return code
                2 -  3  Printer type
                4 -  5  Use interrupts?
                6 -  7  IRQ Number
                8 -  9  Recommended number of 512 byte blocks in buffer
               10 - 11  Use Xon/Xoff protocol?
               12 - 13  Baud rate
               14 - 15  Data bits
               16 - 17  Stop bits
               18 - 19  Parity type
               20 - 21  Socket to use to communicate with print server
        */
        if (printerInfo != NULL)
            memmove((char *)printerInfo, reply + 2, sizeof(REMOTE_INFO));
        return (*(WORD *)reply);
    }
    /*  If the SPX send/recieve failed, return the communications error code */
    else
        return (ccode);

}
