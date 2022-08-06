/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.                 */


#include <string.h>
#include <nit.h>
#include <niterror.h>
#undef FA_HIDDEN
#undef FA_SYSTEM
#include <npt.h>
#include <sap.h>

/****************************************************************************/

extern void far ReplyReceivedESR(void);

/****************************************************************************/

#define NUMBER_OF_BUFFERS   2   /* Number of listen buffers needed          */
#define LISTEN_BUFFER_SIZE  512 /* Maximum data size received from server   */

WORD     ps_commSocket;         /* Socket used to talk to print server      */
ECB far *ps_replyReceived=NULL; /* Points to ECB when a reply is received   */

int      haveConnection = FALSE;/* TRUE if connected to a print server      */

struct                          /* Listen buffers                           */
{
    ECB         ecb;            /* ECB                                      */
    SPXHeader   header;         /* SPX Header                               */
    BYTE                        /* Area for receiving data from print server*/
                data[LISTEN_BUFFER_SIZE];
}listenBuffer[NUMBER_OF_BUFFERS];

/****************************************************************************/

WORD PSAttachToPrintServer(printServerName, connectionID)

    char    *printServerName;   /* Name of print server to attach to        */
    WORD    *connectionID;      /* SPX Connection ID, if successful         */

{

    BYTE     byteJunk,
             propertyValue[128];
    int      i,
             ccode;
    WORD     wordJunk,
             connections;
    ECB      connectionECB;
    SPXHeader
             connectionHeader;

    /*
        Insure that we are only connected to one print server at a time
    */
    if (haveConnection == FALSE)
        haveConnection = TRUE;
    else
        return (PSC_ALREADY_ATTACHED_TO_PSERVER);

    /*
        Initialize IPX
    */
    if (IPXInitialize())
    {
        /*
            SPX_NOT_INSTALLED is equal to 0x00, which we are returning
            for successful completion so return something else
        */
        return(PSC_IPX_NOT_INITIALIZED);
    }

    /*
        Initialize SPX
    */
    ccode = SPXInitialize(&byteJunk, &byteJunk, &wordJunk, &connections);

    /*
        If SPX is not installed or the number of connections is less then
        the minimum needed
    */
    if (ccode == SPX_NOT_INSTALLED)
    {
        /*
            SPX_NOT_INSTALLED is equal to 0x00, which we are returning
            for successful completion so return something else
        */
        return(PSC_SPX_NOT_INITIALIZED);
    }

    /*
        If there are no available connections
    */
    if (connections < 1)
    {
        return (PSC_NO_AVAILABLE_SPX_CONNECTS);
    }

    /*
        Open an IPX socket
    */
    ps_commSocket = 0;
    ccode = IPXOpenSocket((BYTE *)&ps_commSocket, (BYTE)0);
    if (ccode)
    {
        /*
            An error here indicates that the IPX socket table full
        */
        return (PSC_NO_AVAILABLE_IPX_SOCKETS);
    }

    /*
        Initialize and post the listen buffers' ECBs
    */
    for (i=0; i < NUMBER_OF_BUFFERS; i++)
    {
        /*
            Let us know when information is received
        */
        listenBuffer[i].ecb.ESRAddress = ReplyReceivedESR;

        /*
            Set the socket number to the communication socket
        */
        listenBuffer[i].ecb.socketNumber = ps_commSocket;

        /*
            There are two fragments
        */
        listenBuffer[i].ecb.fragmentCount = 2;

        /*
            The first fragment is the SPX header
        */
        listenBuffer[i].ecb.fragmentDescriptor[0].address =
                &(listenBuffer[i].header);
        listenBuffer[i].ecb.fragmentDescriptor[0].size =
                sizeof(listenBuffer[i].header);

        /*
            The second fragment is for data
        */
        listenBuffer[i].ecb.fragmentDescriptor[1].address =
                listenBuffer[i].data;
        listenBuffer[i].ecb.fragmentDescriptor[1].size =
                LISTEN_BUFFER_SIZE;

        /*
            Listen for a incoming packet
        */
        SPXListenForSequencedPacket(&listenBuffer[i].ecb);
    }
    /*
        Get the address of the specified print server
    */
    ccode = ReadPropertyValue(printServerName, OT_ADVERTISING_PRINT_SERVER,
            "NET_ADDRESS", 1, propertyValue, NULL, NULL);
    if (ccode)
    {
        /*
            Close the IPX socket, which will cancel all the listens
        */
        IPXCloseSocket(ps_commSocket);

        /*
            Determine what the error was and exit
        */
        if (ccode == NO_SUCH_OBJECT)
            return (PSC_NO_SUCH_PRINT_SERVER);
        else
            return (PSC_UNABLE_TO_GET_SERVER_ADDR);
    }

    /*
        Initialize the connection packet
    */
        /*
            Initialize the ECB
        */
            /*
                We'll poll to find out when the connection has finished
            */
            connectionECB.ESRAddress = ( void ( far * )() )0;

            /*
                Set the socket number to the communication socket
            */
            connectionECB.socketNumber = ps_commSocket;

            /*
                We have one fragment, which is the SPX header
            */
            connectionECB.fragmentCount = 1;
            connectionECB.fragmentDescriptor[0].address = &connectionHeader;
            connectionECB.fragmentDescriptor[0].size = sizeof(SPXHeader);
        /*
            Initialize the SPX header
        */
            /*
                The destination address is the value of the bindery property
            */
            memcpy((BYTE *)&connectionHeader.destination, propertyValue, 12);
    /*
        Attempt to establish a connection with the print server
    */
    ccode = SPXEstablishConnection(0, 1, connectionID, &connectionECB);

    if (ccode)
    {
        /*
            Close the IPX socket, which will cancel all the listens
        */
        IPXCloseSocket(ps_commSocket);

        if (ccode == SPX_CONNECTION_TABLE_FULL)
            return ((WORD)ccode);
        else
            return (PSC_UNABLE_TO_CONNECT_TO_SERVER);
    }

    while (connectionECB.inUseFlag != 0)
      IPXRelinquishControl();
    if ((ccode = connectionECB.completionCode) != 0)
    {
        /*
            Close the IPX socket, which will cancel all the listens
        */
        IPXCloseSocket(ps_commSocket);

        if (    ccode == SPX_CONNECTION_TABLE_FULL ||
                ccode == SPX_NO_ANSWER_FROM_TARGET)
            return ((WORD)ccode);
        else
            return (PSC_UNABLE_TO_CONNECT_TO_SERVER);
    }
    return(0);
}

/****************************************************************************/

WORD PSDetachFromPrintServer(connectionID)
WORD        connectionID;
{
    ECB        ecb;
    SPXHeader  header;


    /*
        Show that we are not connected to a PrintServer
    */
    haveConnection = FALSE;

    /*
        Initialize the ECB inorder to terminate the connection
    */
        /*
            We'll poll to find out when it has terminated
        */
        ecb.ESRAddress = ( void ( far * )() )0;

        /*
            The SPX header is the only fragment
        */
        ecb.fragmentCount = 1;
        ecb.fragmentDescriptor[0].address = &header;
        ecb.fragmentDescriptor[0].size = sizeof(header);

    /*
        Terminate the SPX connection
    */
    SPXTerminateConnection(connectionID, &ecb);

    /*
        Wait until the terminate has finished
    */
    while (ecb.inUseFlag)
      IPXRelinquishControl();

    /*
        If the connection wasn't terminated one way or another return
        an error
    */
    if (ecb.completionCode == SPX_INVALID_CONNECTION)
        return( (WORD)ecb.completionCode );

    /*
        Close the socket, this will also terminate any listens or
        sends that are pending
    */
    IPXCloseSocket(ps_commSocket);

    return (0);
}

/****************************************************************************/

void far ReplyReceivedESR()
{
   _LoadDS();
   _ReplyReceivedESR( &ps_replyReceived );
}
