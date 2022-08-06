/*
 *      COMUM.C
 *      Manoel Camillo
 */
#include "c:eco.h"
/******************** Prototypes ******************/

		 // Funcoes IPX
void    prepare_IPXBufferDescriptor    ( ECB *ecb, IPXHeader *header, BYTE *buffer, int bsize );
int     prepare_IPXSendECBandHeader    ( ECB *ecb, IPXHeader *header, int socket, BYTE *buffer,
                                         int bsize, IPXAddress *address, int *transportTime );
void    prepare_IPXReceiveECBandHeader ( ECB *ecb, IPXHeader *header, int socket, BYTE *buffer, int bsize );

/********************   Codigo   ******************/

/*** prepare_IPXBufferDescriptor
*
*   Prepara uma descricao de buffer entre uma ecp e um header, contendo
*   apenas um fragmento de dados
*
***/
void prepare_IPXBufferDescriptor( ECB *ecb, IPXHeader *header, BYTE *buffer, int bsize )
{
    IPXGetDataAddress((BYTE *) header,
		      (WORD *) &ecb->fragmentDescriptor[0].address);
    ecb->fragmentDescriptor[0].size = sizeof(IPXHeader) ;
    IPXGetDataAddress((BYTE *) buffer,
		      (WORD *) &ecb->fragmentDescriptor[1].address);
    ecb->fragmentDescriptor[1].size = bsize;
    ecb->fragmentCount = 2 ;
}

/*** prepare_IPXSendECBandHeader
*
*   Prepara uma ecb e header para transmissao.
*   Usa a funcao prepare_IPXBufferDescritor
*
***/
int prepare_IPXSendECBandHeader( ECB *ecb, IPXHeader *header, int socket, BYTE *buffer,
                                 int bsize , IPXAddress *address, int *transportTime )
{
    memcpy(&header->destination, address, sizeof(IPXAddress));
    header->packetType = 0x04;
    prepare_IPXBufferDescriptor(ecb, header, buffer, bsize);
    ecb->ESRAddress = (void (far *) ()) NULL;
    ecb->socketNumber = socket;
    return (IPXGetLocalTarget(header->destination.network,
                              ecb->immediateAddress,
                              transportTime));
}

/*** prepare_IPXReceiveECBandHeader ( ECB *ecb, IPXHeader *header, int socket, BYTE *buffer, int bsize, (void (far *)()) esr );
*
*   Prepara uma ecb e header para recepcao.
*   Usa a funcao prepare_IPXBufferDescritor
*
***/
void    prepare_IPXReceiveECBandHeader ( ECB *ecb, IPXHeader *header, int socket, BYTE *buffer, int bsize)
{
    header->packetType = 0x04;
    prepare_IPXBufferDescriptor(ecb, header, buffer, bsize);
    ecb->ESRAddress = (void (far *) ()) NULL;
    ecb->socketNumber = socket;
}

