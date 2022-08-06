/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <npt.h>


WORD PSDownPrintServer(SPXConnection, goDownNow, jobOutcome)

    WORD    SPXConnection;      /* SPX connection to print server           */
    BYTE    goDownNow,          /* Go down immediately?                     */
            jobOutcome;         /* What to do with the current jobs         */

{

    BYTE    request[3];         /* Request buffer                           */
    WORD    reply,              /* Reply buffer                             */
            ccode;              /* Completion code                          */

    /*
        Request format:
            0   Command code
            1   Go down now?
            2   If go down immediately, job outcome for current jobs
    */
    request[0] = CMD_DOWN;
    request[1] = goDownNow;
    request[2] = jobOutcome;

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
