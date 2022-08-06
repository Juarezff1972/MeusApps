// Protocolo de comunicacao do programa TERMINAL
// Andrey Patitucci
// Carlos E. Cruz
// Lisianne Requiao
// Luciane E. Romeiro
// Raquel Speltz
// Nenhum direito reservado - Copywrong - 1993

// A mensagem "FUNCAO: ...." especifica o que faz a funcao onde a mensagem
// estiver.

#include <nxt.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <conio.h>
#include <alloc.h>
#include "terminal.h"
#include "netgloba.h"

ECB *trans_ECB;
ECB *receive_ECB;
IPXHeader *trans_cabec;
IPXHeader *receive_cabec;
BYTE central_addr[12];
WORD socket=0x4545;
int num_msg=0;
int conexao=0;
int receive_Flag=0;
int receive_error;
extern int ref_inicial;
extern int ref_final;

// Haverao duas estruturas globais do tipo mensagem
// ------------------------------------------------

mensagem trans_pacote;
mensagem receive_pacote;

int ConectaDesconectaComCentral(void)
{

// FUNCAO: Estabelecer e terminar conexao com central. Retorna OK se a co-
// nexao for estabelecida com sucesso. Retorna NIPX se o IPX nao estiver ins-
// talado. SOCK se a soquete nao puder ser aberta ou os erros da funcao Cone-
// xao. Se ja existir conexao, a mesma e desfeita e a funcao retorna OK.
// -------------------------------------------------------------------------

  int i;

  if(conexao==0) {

// Inicializa IPX
// --------------

    i=IPXInitialize();
    if(i==0xf0) {
      return NIPX;
    }

// Aloca memoria para cabecalhos, ECB's e mensagens
// ------------------------------------------------

    trans_ECB=(ECB *) calloc(1,sizeof(ECB));
    receive_ECB=(ECB *) calloc(1,sizeof(ECB));
    trans_cabec=(IPXHeader *) calloc(1,sizeof(IPXHeader));
    receive_cabec=(IPXHeader *) calloc(1,sizeof(IPXHeader));

// Abre a soquete
// --------------

    i=IPXOpenSocket((BYTE *) &socket,0x00);
    if(i!=0x00) {
      return SOCK;
    }
  }

// Define o numero da mensagem: 0 ou 1 e continuidade
// --------------------------------------------------

  trans_pacote.numero=num_msg;

// Estabelece conexao se a variavel conexao == 0
// ---------------------------------------------

  if(conexao==0) {
    trans_pacote.tipo=0;
    i=Conexao();
    if(i==OK) conexao=1;
    else IPXCloseSocket(socket);
    return i;
  }

// Termina conexao se a variavel conexao == 1
// ------------------------------------------

  if(conexao==1) {
    trans_pacote.tipo=1;
    i=Conexao();
    return i;
  }
  return OK;
}

void PreparaTransBroadcast(void)
{

// FUNCAO: Prepara ECB e cabecalho para transmitir um broadcast.
// -------------------------------------------------------------

  int i;

  for(i=0;i<6;i++)
    trans_cabec->destination.node[i]=0xff;
  *(WORD *) trans_cabec->destination.socket=socket;
  IPXGetDataAddress( (BYTE *) trans_cabec,
		     (WORD *) &trans_ECB->fragmentDescriptor[0].address);
  trans_ECB->fragmentDescriptor[0].size=sizeof(IPXHeader);
  IPXGetDataAddress( (BYTE *) &trans_pacote,
		     (WORD *) &trans_ECB->fragmentDescriptor[1].address);
  trans_ECB->fragmentDescriptor[1].size=sizeof(mensagem);
  trans_ECB->fragmentCount=2;
  trans_ECB->ESRAddress=(void(far *) ())0;
  trans_ECB->socketNumber=socket;
  for(i=0;i<6;i++)
    trans_ECB->immediateAddress[i]=0xff;
}

void PreparaRecepcao(void)
{

// FUNCAO: Prepara ECB de recepcao para receber resposta da central
// ----------------------------------------------------------------

  int i;
  BYTE node_addr[10];
  BYTE networkAddress[12];


  IPXGetDataAddress( (BYTE *) receive_cabec,
		     (WORD *) &receive_ECB->fragmentDescriptor[0].address);
  receive_ECB->fragmentDescriptor[0].size=sizeof(IPXHeader);
  IPXGetDataAddress( (BYTE *) &receive_pacote,
		     (WORD *) &receive_ECB->fragmentDescriptor[1].address);
  receive_ECB->fragmentDescriptor[1].size=sizeof(mensagem);
  receive_ECB->fragmentCount=2;
  receive_ECB->ESRAddress=(void(far *) ())ReceiveESRHandler;
  receive_ECB->socketNumber=socket;
  IPXGetInternetworkAddress(node_addr);
  for(i=0;i<12;i++) {
    if(i<10)
      networkAddress[i]=node_addr[i];
    if(i>=10)
      networkAddress[i]=0x45;
  }
  IPXGetLocalTarget((BYTE *) &networkAddress,
		    (BYTE *) &receive_ECB->immediateAddress, &i);
}


void PreparaTransmissao(void)
{

// FUNCAO: Prepara ECB's de recepcao e transmissao e cabecalho de transmissao
// com o endereco do no central (apos ter estabelecido conexao).
// --------------------------------------------------------------------------

  int i;

  for(i=0;i<4;i++)
    trans_cabec->destination.network[i]=central_addr[i];
  for(i=0;i<6;i++)
    trans_cabec->destination.node[i]=central_addr[i+4];
  *(WORD *) trans_cabec->destination.socket=socket;
  IPXGetDataAddress( (BYTE *) trans_cabec,
		     (WORD *) &trans_ECB->fragmentDescriptor[0].address);
  trans_ECB->fragmentDescriptor[0].size=sizeof(IPXHeader);
  IPXGetDataAddress( (BYTE *) &trans_pacote,
		     (WORD *) &trans_ECB->fragmentDescriptor[1].address);
  trans_ECB->fragmentDescriptor[1].size=sizeof(mensagem);
  trans_ECB->fragmentCount=2;
  trans_ECB->ESRAddress=(void(far *) ())0;
  trans_ECB->socketNumber=socket;
  IPXGetLocalTarget((BYTE *) &central_addr,
		    (BYTE *) &trans_ECB->immediateAddress, &i);
}

int Conexao(void)
{

// FUNCAO: Envia mensagens de conexao ou desconexao para a central. Retorna
// OK se a operacao for um sucesso. TIME se a mensagem de conexao/desconexao
// for transmitida mais de cinco vezes e nenhuma resposta for enviada. TRANS
// se houver falha de transmissao. RECEP se houver falha de recepcao. Usa
// protocolo do bit alternante.
// -------------------------------------------------------------------------

  int i,retransmit=0;
  long j=0;
  BYTE networkaddr[10];


// Carrega networkaddr com o endereco de rede e de no do terminal
// --------------------------------------------------------------

  IPXGetInternetworkAddress(networkaddr);

// Comeca operacao de conexao/desconexao com central
// -------------------------------------------------

  PreparaRecepcao();
  IPXListenForPacket(receive_ECB);
  i=Transmite();
  if(i==TRANS) {
    if(receive_ECB->inUseFlag==0xfe)
      IPXCancelEvent(receive_ECB);
    return TRANS;
  }
  while(1) {

// Se nenhuma mensagem chegar, espera 5000000 loops e retransmite a mensa-
// gem (timeout). Executa no maximo 5 retransmissoes
// --------------------------------------------------------------------

    if(receive_Flag==0) {
      j++;
      if(j==5000000) {
	delay(100);
	j=0;
	if(receive_Flag==1)
	  continue;
	i=Transmite();
	if(i==TRANS) {
	  if(receive_ECB->inUseFlag==0xfe)
	    IPXCancelEvent(receive_ECB);
	  return TRANS;
	}
	retransmit++;
	if(retransmit>4) {
	  if(receive_ECB->inUseFlag==0xfe)
	    IPXCancelEvent(receive_ECB);
	  return TIME;
	}
      }
    }

// Se receber uma mensagem, verifica se a mesma e um Ack da central
// ----------------------------------------------------------------

    if(receive_Flag==1 && receive_error==0) {
      receive_Flag=0;
      if(ComparaEnderecos(networkaddr)) {
	i=ReconheceAckDeConexao();
	if(i==OK) {
	  if(receive_ECB->inUseFlag==0xfe)
	    IPXCancelEvent(receive_ECB);
	  CarregaEnderecoDaCentral();
	  break;
	}
	if(i==MSGD)
	  continue;
      }
    }

// Falha na recepcao
// -----------------

    if(receive_error==1) {
      receive_error=0;
      if(receive_ECB->inUseFlag==0xfe)
	IPXCancelEvent(receive_ECB);
      return RECEP;
    }
  }
  num_msg=1-num_msg;
  return OK;
}

int ComparaEnderecos(BYTE *networkaddr)
{

// FUNCAO: Verifica se uma mensagem recebida possui o endereco de origem
// igual ao deste terminal. Retorna 1 se os endercos sao diferentes e 0 se
// os enderecos sao iguais.
// -----------------------------------------------------------------------

  int i,j;

  for(i=0;i<6;i++) {
    j=i+4;
    if(receive_cabec->source.node[i]!=networkaddr[j]) return 1;
  }
  return 0;
}

int ReconheceAckDeConexao(void)
{

// FUNCAO: Verifica se a mensagem recebida e um Ack de conexao da central.
// Se a mensagem for a esperada a funcao retorna OK ou MSGD se a mensagem
// for desconhecida.
// -------------------------------------------------------------------------

  int i;

// Se a mensagem recebida for um Ack retorne OK
// --------------------------------------------

  if(trans_pacote.numero==receive_pacote.numero &&
     receive_pacote.tipo==3)
    return OK;

// Se a mensagem recebida nao for a esperada retorna MSGD
// ------------------------------------------------------

  else
   return MSGD;
}

void CarregaEnderecoDaCentral(void)
{

// FUNCAO: Carrega a variavel central_addr com o endereco da central.
// ------------------------------------------------------------------

  int i;

  for(i=0;i<4;i++)
    central_addr[i]=receive_cabec->source.network[i];
  for(i=0;i<6;i++)
    central_addr[i+4]=receive_cabec->source.node[i];
  for(i=0;i<2;i++)
    central_addr[i+10]=receive_cabec->source.socket[i];
}

int EnviaPacoteAT_DEL_REQ(mensagem pacote,WORD soquete)
{

// FUNCAO: Envia pacote e aguarda resposta da central, nesta funcao esta
// implementado o protocolo de comunicacao. Retorna OK se a transmissao do
// pacote for um sucesso. TIME se a mensagem for transmitida mais de cinco
// vezes e nenhuma resposta for enviada. NACK se mais de cinco Nack's forem
// recebidos. TRANS se houver falha de transmissao. RECEP se houver falha de
// recepcao. NCON se a conexao ainda nao foi estabeleciada. Usa protocolo do
// bit alternante.
// -----------------------------------------------------------------------

  int i,nack=0,retransmit=0;
  long j=0;

// Verifica se a conexao ja foi estabeleciada
// ------------------------------------------

  if(conexao==0)
    return NCON;

// Atribui o pacote vindo da interface ao pacote trans_pacote.
// -----------------------------------------------------------

  CarregaPacote(pacote);

// Comeca a escutar e envia o pacote
// ---------------------------------
  PreparaRecepcao();
  IPXListenForPacket(receive_ECB);
  socket=soquete;
  i=Transmite();
  if(i==TRANS) {
    if(receive_ECB->inUseFlag==0xfe)
      IPXCancelEvent(receive_ECB);
    return TRANS;
  }
  socket=0x4545;
  while(1) {

// Se nenhuma mensagem chegar, espera 10000000 loops e retransmite a mensa-
// gem (timeout). Executa no maximo 5 retransmissoes
// --------------------------------------------------------------------

    if(receive_Flag==0) {
      j++;
      if(j==10000000) {
	delay(100);
	j=0;
	if(receive_Flag==1)
	  continue;
	if(retransmit!=-1)
	  trans_pacote.tipo=5; // Timeout
	socket=soquete;
	i=Transmite();
	if(i==TRANS) {
	  if(receive_ECB->inUseFlag==0xfe)
	    IPXCancelEvent(receive_ECB);
	  return TRANS;
	}
	socket=0x4545;
	retransmit++;
	if(retransmit>4) {
	  if(receive_ECB->inUseFlag==0xfe)
	    IPXCancelEvent(receive_ECB);
	  return TIME;
	}
      }
    }

// Se receber uma mensagem, verifica se a mesma e um Ack da central
// ----------------------------------------------------------------

    if(receive_Flag==1 && receive_error==0) {
      receive_Flag=0;
      i=ReconheceAck();
      if(i==OK) {
	if(receive_ECB->inUseFlag==0xfe)
	  IPXCancelEvent(receive_ECB);
	break;
      }
     if(i==NACK) {
       retransmit=-1;
       trans_pacote.tipo=2;
       j=9999999;
       nack++;
       if(nack==5) {
	if(receive_ECB->inUseFlag==0xfe)
	  IPXCancelEvent(receive_ECB);
	return NACK;
      }
       continue;
     }
     if(i==MSGD)
       continue;
    }

// Falha na recepcao
// -----------------

    if(receive_error==1) {
      receive_error=0;
      if(receive_ECB->inUseFlag==0xfe)
	IPXCancelEvent(receive_ECB);
      return RECEP;
    }
  }
  num_msg=1-num_msg;
  return OK;
}

void CarregaPacote(mensagem pacote)
{

// FUNCAO: Atribui o pacote vindo de INTERFAT.C ao pacote de NETWARET.C
// --------------------------------------------------------------------

  int i;

  trans_pacote.numero=num_msg;
  trans_pacote.tipo=pacote.tipo;
  trans_pacote.operacao=pacote.operacao;
  trans_pacote.ref_prod=pacote.ref_prod;
  for(i=0;i<20;i++) {
    trans_pacote.item_prod[i]=pacote.item_prod[i];
    trans_pacote.nome_prod[i]=pacote.nome_prod[i];
  }
  trans_pacote.qtde_prod=pacote.qtde_prod;
  for(i=0;i<255;i++)
    trans_pacote.info_prod[i]=pacote.info_prod[i];
}

int ReconheceAck(void)
{

// FUNCAO: Verifica se a mensagem provinda da central corresponde a esperada.
// Retorna OK, se a mensagem for a esperada. NACK se a mensagem recebida for
// um Nack. MSGD se a mensagem for desconhecida.
// --------------------------------------------------------------------------

  int i;

// Verifica se a mensagem provem da central
// ----------------------------------------

  for(i=0;i<4;i++)
    if(central_addr[i]!=receive_cabec->source.network[i])
      return MSGD;
  for(i=0;i<6;i++)
    if(central_addr[i+4]!=receive_cabec->source.node[i])
      return MSGD;
  for(i=0;i<2;i++)
    if(central_addr[i+10]!=receive_cabec->source.socket[i])
      return MSGD;

// Se a mensagem recebida for um Ack retorne OK
// --------------------------------------------

  if(trans_pacote.numero==receive_pacote.numero &&
     receive_pacote.tipo==3 &&
     trans_pacote.operacao==receive_pacote.operacao &&
     trans_pacote.ref_prod==receive_pacote.ref_prod)
    return OK;

// Se a mensagem recebida for um Nack retorne NACK
// -----------------------------------------------

  if(receive_pacote.tipo==4)
    return NACK;

// Se a mensagem recebida nao for a esperada retorna MSGD
// ------------------------------------------------------

  else
   return MSGD;
}

int RecebePacote_REQ(void)
{

// FUNCAO: Se a mensagem de requisicao foi enviada, e recebida com sucesso
// pela central, comeca aguardar pacote com o produto. Pode retornar os
// valores OK, TRANS, RECEP, TIME.
// -----------------------------------------------------------------------

  unsigned long i=0;

// Comeca a escutar
// ----------------

  PreparaRecepcao();
  IPXListenForPacket(receive_ECB);
  while(1) {

// Verifica se chegou um pacote
// ----------------------------

    if(receive_Flag==1) {
      receive_Flag=0;
      if(receive_error==1) {
	receive_error=0;
	IPXCancelEvent(receive_ECB);
	return RECEP;
      }

// Verifica se o pacote que chegou esta coerente
// ---------------------------------------------

      i=VerificaPacote();

// Se estiver coerente carrega o produto na lista de itens e produtos
// ------------------------------------------------------------------

      if(i==OK) {
	AtualizaListaDeProdutos(&receive_pacote);
	i=TransmiteAck();
	if(i==OK)
	  break;
	else {
	  IPXCancelEvent(receive_ECB);
	  return i;
	}
      }

// Senao transmite um Nack e continua esperando
// --------------------------------------------

      else {
	i=TransmiteNack();
	if(i==OK)
	  continue;
	else {
	  IPXCancelEvent(receive_ECB);
	  return i;
	}
      }
    }
    if(i==10000000) {
      IPXCancelEvent(receive_ECB);
      return TIME;
    }
  }
  IPXCancelEvent(receive_ECB);
  return OK;
}

int VerificaPacote(void)
{

// FUNCAO: Verifica se o pacote de dados recebido e o esperado. Retorna OK
// se o pacote esta coerente. MSGD se o pacote nao esta coerente.
// -----------------------------------------------------------------------

  int i;

// Verifica se a mensagem provem da central
// ----------------------------------------

  for(i=0;i<4;i++)
    if(central_addr[i]!=receive_cabec->source.network[i])
      return MSGD;
  for(i=0;i<6;i++)
    if(central_addr[i+4]!=receive_cabec->source.node[i])
      return MSGD;
  for(i=0;i<2;i++)
    if(central_addr[i+10]!=receive_cabec->source.socket[i])
      return MSGD;

// Se a mensagem recebida for a esperada retorne OK
// --------------------------------------------

  if(receive_pacote.numero==1-num_msg &&
     receive_pacote.tipo==2 &&
     receive_pacote.operacao==REQ)
    return OK;

// Se a mensagem recebida nao for a esperada retorna MSGD
// ------------------------------------------------------

  else
   return MSGD;
}

int TransmiteAck(void)
{

// FUNCAO: Transmite um Ack para o central apos receber um pacote coerente.
// ------------------------------------------------------------------------

  int i;

  trans_pacote.numero=receive_pacote.numero;
  trans_pacote.tipo=3;
  trans_pacote.operacao=receive_pacote.operacao;
  trans_pacote.ref_prod=receive_pacote.ref_prod;
  socket=0x4549;
  i=Transmite();
  socket=0x4545;
  return i;
}

int TransmiteNack(void)
{

// FUNCAO: Transmite um Nack para o central.
// -----------------------------------------

  int i;

  socket=0x4549;
  trans_pacote.tipo=4;
  i=Transmite();
  socket=0x4545;
  return i;
}

int Transmite(void)
{

// FUNCAO: Transimitir o pacote e verificar o sucesso da transmissao. Se hou-
// ver sucesso na transmissao, a funcao retorna OK, senao a funcao retorna
// TRANS.
// --------------------------------------------------------------------------

  if(conexao==0)
    PreparaTransBroadcast();
  if(conexao==1)
    PreparaTransmissao();
  IPXSendPacket(trans_ECB);
  while(trans_ECB->inUseFlag==0xff);

// Verifica sucesso da transmissao
// -------------------------------

  if(trans_ECB->completionCode==0x00)
    return OK;
  else
    return TRANS;
}

void cdecl AguardaPacote(void)
{

// FUNCAO: Esta funcao e chamada pela rotina ESRTER.ASM caso algum pacote
// seja recebido por receive_ECB
// ----------------------------------------------------------------------

  receive_Flag=1;
  if(receive_ECB->completionCode!=0x00)
    receive_error=1;
  PreparaRecepcao();
  IPXListenForPacket(receive_ECB);
}