/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <npt.h>


WORD PSLoginToPrintServer( SPXConnection, accessLevel )
WORD     SPXConnection;         /* SPX connection to print server            */
BYTE    *accessLevel;           /* Access level granted to client            */
{
    BYTE     request[51];       /* Buffer to be sent to the print server     */
    BYTE     reply[3];          /* Buffer where answer will be returned      */
    WORD     ccode;             /* Completion code                           */


    /*------------------------------------------------------------
        Request buffer format:
              0     Command code
         1 - 48     File server name
        49 - 50     Client's connection number on the file server
      ------------------------------------------------------------*/
    request[0] = CMD_LOGIN_TO_PRINT_SERVER;
    GetFileServerName(GetDefaultConnectionID(), &request[1]);
    *(WORD *)&request[49] = GetConnectionNumber();

    /*-------------------
        Make the request
      -------------------*/
    ccode = PrintServerRequest( SPXConnection, request, sizeof(request),
                                reply, sizeof(reply) );


    /*------------------------------------------------------------------
        If the SPX send/receive worked, get access level and return code
        from the reply packet
      ------------------------------------------------------------------*/
    if (ccode == 0)
    {
        /*-------------------------------------
            Reply buffer format:
                0 - 1   Return code
                    2   Access level granted
          -------------------------------------*/
        if (accessLevel != NULL)
            *accessLevel = reply[2];
        return (*(WORD *)reply);
    }

    /*----------------------------------------------------------------------
        If the SPX send/recieve failed, return the communications error code
      ----------------------------------------------------------------------*/
    else
        return (ccode);
}
