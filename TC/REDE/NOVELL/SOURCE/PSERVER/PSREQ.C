/* (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.           */

#include <string.h>
#include <npt.h>


/*-----------------------------------------------------------------------
    PrintServerRequest assumes the following:
    1)  SPX and IPX have been initialized
    2)  A valid SPX connection with the print server was created with
        PSAttachToPrintServer
  -----------------------------------------------------------------------*/

int DummyFunction( void );

extern ECB far *ps_replyReceived;   /* Points to ECB when reply is received */


/****************************************************************************/

WORD PrintServerRequest( connection, request, requestSize, reply, replySize )
WORD      connection;           /* SPX connection where request is sent     */
void     *request;              /* Buffer containing print server request   */
void     *reply;                /* Buffer where reply is received           */
int       requestSize;          /* Size of the print server request         */
int       replySize;            /* Size of reply buffer                     */
{
    ECB       requestECB;       /* The ECB for the request's SPX send       */
    SPXHeader requestHeader;    /* SPX header for the request               */
    WORD      ccode;            /* Return code                              */

    /*-----------------------------
        Initialize the request ECB
      -----------------------------*/
    requestECB.ESRAddress = ( void ( far * )() )0;
    requestECB.fragmentCount = 2;
    requestECB.fragmentDescriptor[0].address = &requestHeader;
    requestECB.fragmentDescriptor[0].size = sizeof(SPXHeader);
    requestECB.fragmentDescriptor[1].address = request;
    requestECB.fragmentDescriptor[1].size = requestSize;

    /*-----------------------------------
        Initialize the request SPX header
      -----------------------------------*/
    requestHeader.connectionControl = 0x40;
    requestHeader.dataStreamType = 0;

    /*--------------------
        Send the request
      --------------------*/
    SPXSendSequencedPacket(connection, &requestECB);

    /*-----------------------------------
        Wait for the request to be sent
      -----------------------------------*/
    while (requestECB.inUseFlag)
         IPXRelinquishControl();

    /*------------------------------------------------------
        If the request was sent correctly, wait for a reply
      ------------------------------------------------------*/
    if ( (ccode = (WORD)requestECB.completionCode) == 0)
    {
        /*----------------------------------------------------------------
            The replyReceived flag will be set by ReplyReceiveESR() when a
            SPX packet is received.
          ----------------------------------------------------------------*/
        while (ps_replyReceived == NULL)
            DummyFunction();

        /*------------------------------------------------------------------
            If the SPX send and recieve were successful, copy the reply data
          ------------------------------------------------------------------*/
        if ( (ccode = (WORD)ps_replyReceived->completionCode) == 0)
        {
            memcpy(reply,
                   (char *)(ps_replyReceived->fragmentDescriptor[1].address),
                   replySize);

        }
    }

    /*------------------------------------------------------------
        Repost the listen so it will be there for the next time
        and reset replyReceived. Cast ps_replyReceived, in case
        SPXListenForSequencedPacket is expecting a near pointer.
      ------------------------------------------------------------*/
    SPXListenForSequencedPacket( (ECB *)ps_replyReceived );
    ps_replyReceived = NULL;

    return (ccode);

}

int  DummyFunction()
{
   return(0);
}
