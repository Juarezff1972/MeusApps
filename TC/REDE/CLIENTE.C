/*
 *        Arquivo cliente.c
 *        Manoel Camillo
 */

#include "c:eco.h"

/******************** Prototypes ******************/

		 // Funcoes IPX
extern void prepare_IPXBufferDescriptor    ( ECB *ecb, IPXHeader *header, BYTE *buffer, int bsize );
extern int  prepare_IPXSendECBandHeader    ( ECB *ecb, IPXHeader *header, int socket, BYTE *buffer,
                                             int bsize, IPXAddress *address, int *transportTime );
extern void prepare_IPXReceiveECBandHeader ( ECB *ecb, IPXHeader *header, int socket, BYTE *buffer, int bsize );
int         broadcast                      (ECB *ecb, IPXHeader *header, int socket );

/********************   Codigo   ******************/

/*** broadcst
*
*   Procura Servidor atraves de Broadcast
*
***/

int broadcast ( ECB *ecb, IPXHeader *header, int socket  )
{
    int transportTime;
    int i;

    IPXGetInternetworkAddress(header->destination.network);

    for ( i=0; i<6; i++ )
    {
       ecb->immediateAddress[i] = 0xff;
       header->destination.node[i] = 0xff;
    }
    header->destination.socket[0] = (BYTE) (ECO_SOCKET >>8);
    header->destination.socket[1] = (BYTE) ECO_SOCKET;
    header->source.socket[0] = (BYTE) (ECO_SOCKET >>8);
    header->source.socket[1] = (BYTE) ECO_SOCKET;

    header->packetType = 0x04;
    ecb->ESRAddress = (void (far *) ()) NULL;
    ecb->socketNumber = socket;

    IPXSendPacket( ecb );
    while ( ecb->inUseFlag );
    return(ecb->completionCode);
}


void main( void )
{
  int status, transportTime, retry;
  long timer;
  ECB receiveECB, sendECB;
  IPXHeader receiveHeader, sendHeader;
  char buffer[BUFFER_SIZE];
  int socket = ECO_SOCKET;

  if ( IPXInitialize() )
  {
     printf("\n\rIPX nao inicializado...\n\r");
     exit( EXIT_FAILURE );
  }
  status = IPXOpenSocket( (BYTE*) &socket, SHORT_LIVED );
  switch ( status )
  {
    case SOCKET_ALREADY_OPEN: printf("\n\rSoquete ja foi aberta...\n\r");
			      exit( EXIT_FAILURE );
    case SOCKET_TABLE_FULL:   printf("\n\rTabela de soquetes cheia...\n\r");
			      exit( EXIT_FAILURE );
    case IPX_NOT_INSTALLED:   printf("\n\rIPX nao esta instalado...\n\r");
			      exit( EXIT_FAILURE );
  }
  prepare_IPXBufferDescriptor(&sendECB, &sendHeader, (BYTE *)buffer, strlen("procura")+1);
  sprintf(buffer, "%s", "procura");
  prepare_IPXReceiveECBandHeader(&receiveECB, &receiveHeader, socket, (BYTE *) buffer, BUFFER_SIZE);
  retry = 0;
  while (++retry < MAX_RETRY)
  {
    if (broadcast(&sendECB, &sendHeader, socket))
    {
      printf("Erro no broadcast\n");
      exit(EXIT_FAILURE);
    }
    IPXListenForPacket(&receiveECB);
    timer = 0;
    while(receiveECB.inUseFlag)
    {
      delay( 100 );
      if(++timer == TIME_OUT) break;   // aguarda mensagem
    }
    if ((timer >= TIME_OUT) || receiveECB.completionCode)
    {
	   printf("TIMEOUT\n");
	   while (IPXCancelEvent(&receiveECB) == ECB_CANNOT_BE_CANCELLED);
    } else break;
  }
  if (++retry >= MAX_RETRY)
  {
    printf("\n\rServidor nao localizado ...\n\r");
    exit(EXIT_FAILURE);
  }
  if (receiveECB.completionCode)
  {
    printf("Erro contactando o servidor\n");
    exit(EXIT_FAILURE);
  }
  printf("Servidor contactado\n");
  if (prepare_IPXSendECBandHeader(
	      &sendECB, &sendHeader, socket, (BYTE *) buffer,
	      BUFFER_SIZE, &receiveHeader.source, &transportTime))
  {
    printf("Erro preparando ECB e Header de transmissao\n");
    exit(EXIT_FAILURE);
  }
  while(1)
  {
    printf("\n>"); gets(buffer);

    IPXListenForPacket(&receiveECB);
    IPXSendPacket(&sendECB);

    while(sendECB.inUseFlag);
    if (sendECB.completionCode)
    {
      printf("Erro de transmissao\n");
      exit(EXIT_FAILURE);
    }

    if (!strcmp(buffer, "fim"))
    {
	while (IPXCancelEvent(&receiveECB) == ECB_CANNOT_BE_CANCELLED);
	break;
    }


    while(receiveECB.inUseFlag);   // aguarda mensagem
    printf("Eco retornado\n");

    if (receiveECB.completionCode)
    {
      printf("Erro de recepcao\n");
      exit(EXIT_FAILURE);
    }
    printf("*-----------------------*\n");
    printf("Menssagem Ecoada ----->%s\n", buffer);
  }
}


