/*
 *        Arquivo servidor.c
 *        Manoel Camillo
 */

#include "c:eco.h"
/******************** Prototypes ******************/

extern void prepare_IPXBufferDescriptor    ( ECB *ecb, IPXHeader *header, BYTE *buffer, int bsize );
extern int  prepare_IPXSendECBandHeader    ( ECB *ecb, IPXHeader *header, int socket, BYTE *buffer,
					     int bsize, IPXAddress *address, int *transportTime );
extern void prepare_IPXReceiveECBandHeader ( ECB *ecb, IPXHeader *header, int socket, BYTE *buffer, int bsize );


void main( void )
{
  int status,transportTime,i;
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
    case SOCKET_ALREADY_OPEN:
			      printf("\n\rSoquete ja foi aberta...\n\r");
			      exit( EXIT_FAILURE );
    case SOCKET_TABLE_FULL:
			      printf("\n\rTabela de soquetes cheia...\n\r");
			      exit( EXIT_FAILURE );
    case IPX_NOT_INSTALLED:
			      printf("\n\rIPX nao esta instalado...\n\r");
			      exit( EXIT_FAILURE );
  }
  prepare_IPXReceiveECBandHeader(
	      &receiveECB, &receiveHeader, socket,
	      (BYTE *) buffer, BUFFER_SIZE);
  while(1)
  {
    IPXListenForPacket(&receiveECB);
    printf("Esperando ...\n");

    while(receiveECB.inUseFlag);   // aguarda mensagem
    if (receiveECB.cn r, BUFFER_SIZE);s gnCode) continue;ensaSTAerro,chesceizar(receiveando ...Mif (recr, sebi   ---->%swhi,ER_SIZEf("Esperando ...Orirecr--------------->t( EXIT_Ffecb(i=0; i<4;i++)rando ...%xi,E, socket,
	 .source.network( EXIT_Ffecb(i=0; i<6;i++)rando ...%xi,E, socket,
	 .source.node) EXIT_Ffecb(i=0; i<2;i++)rando ...%xwhi,E, socket,
	 .source.   (BYle(receiveB.cn!strcmp(R_SIZE);"fim")FERreaka mensaparminFERprograma(receiveanECBandHeader    ( ECB *ecceiveECB, &PXHeade, &PXHet,
	      (BYTfer, BUFFER_SIZE)eiveECB, strlen(R_SIZE)+1der, socket,
	 .sourceder,i;
  ECB reeiveECB, le(receiveHeader veECB);PXHeade) EXIT_FECB.inPXHeade  // aguardaare_IPX