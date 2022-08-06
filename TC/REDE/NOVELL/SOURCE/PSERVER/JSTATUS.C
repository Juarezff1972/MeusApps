/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <string.h>
#include <npt.h>


WORD PSGetPrintJobStatus(SPXConnection, printer, fileServer, queueName,
                         jobNumber, jobDescription, copiesToPrint, jobSize,
                         copiesDone, bytesIntoCopy, neededForm, text)

    WORD     SPXConnection,     /* SPX Connection number                    */
            *jobNumber,         /* Job number                               */
            *copiesToPrint,     /* Number of copies to be printed           */
            *copiesDone,        /* Copy currently being printed             */
            *neededForm;        /* Form number needed for job               */
    BYTE     printer,           /* Printer number                           */
            *text;              /* Is the job text?                         */
    char    *fileServer,        /* File server job came from                */
            *queueName,         /* Queue job came from                      */
            *jobDescription;    /* Job description                          */
    long    *jobSize,           /* Size of print job                        */
            *bytesIntoCopy;     /* Bytes into current copy                  */

{

    BYTE     request[2],        /* Request buffer                           */
             reply[165];        /* Reply buffer                             */
    WORD     ccode;             /* Completion code                          */

    /*
        Request format
            0   Command code
            1   Printer number
    */
    request[0] = CMD_GET_JOB_STATUS;
    request[1] = printer;

    /*
        Make the request
    */
    ccode = PrintServerRequest(SPXConnection, request, sizeof(request),
                               reply, sizeof(reply));

    /*
        If the SPX send/receive worked, get return code from the reply packet
    */
    if (ccode == 0)
    {
        /*
            Reply format
               0 -   1  Return code
               2 -  49  File server job came from
              50 -  97  Queue job came from
              98 -  99  Job number
             100 - 149  Job description
             150 - 151  Number of copies to be printed
             152 - 155  Number of bytes per copy
             156 - 157  Number of copies finished
             158 - 161  Number of bytes finished in current copy
             162 - 163  Reqired form for job
                   164  Is the job text?
        */
        if (fileServer != NULL)
            memmove(fileServer, reply + 2, 48);
        if (queueName != NULL)
            memmove(queueName, reply + 50, 48);
        if (jobNumber != NULL)
            *jobNumber = *(WORD *)(reply + 98);
        if (jobDescription != NULL)
            memmove(jobDescription, reply + 100, 50);
        if (copiesToPrint != NULL)
            *copiesToPrint = *(WORD *)(reply + 150);
        if (jobSize != NULL)
            *jobSize = *(long *)(reply + 152);
        if (copiesDone != NULL)
            *copiesDone = *(WORD *)(reply + 156);
        if (bytesIntoCopy != NULL)
            *bytesIntoCopy = *(long *)(reply + 158);
        if (neededForm != NULL)
            *neededForm = *(WORD *)(reply + 162);
        if (text != NULL)
            *text = *(reply + 164);

        return (*(WORD *)reply);
    }

    /*
        If the SPX send/recieve failed, return the communications error code
    */
    else
        return (ccode);

}
