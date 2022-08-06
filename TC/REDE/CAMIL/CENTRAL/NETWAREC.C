// Protocolo de comunicacao do programa CENTRAL
// Andrey Patitucci
// Carlos E. Cruz
// Lisianne Requiao
// Luciane E. Romeiro
// Raquel Speltz
// Nenhum direito reservado - Copywrong - 1993

// A mensagem "FUNCAO: ...." especifica o que faz a funcao onde a mensagem
// estiver.

#include <nxt.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <alloc.h>
#include "netgloba.h"
#include "central.h"

ECB *trans_ECB,*conection_ECB,*request_ECB,*ack_ECB;
IPXHeader *trans_cabec,*conection_cabec,*request_cabec,*ack_cabec;
BYTE networkAddress[12];
WORD socket1=0x4545,socket2=0x4547,socket3=0x4549;
int ack_Flag=0;
int ack_error=0;

// Filas internas de conexao/desconexao e de pacotes em geral
// ----------------------------------------------------------

struct conection_queue c_queue[15];
int c_queue_in=0;
int c_queue_fi=0;
int c_fi_flag=0;

struct request_queue r_queue[25];
int r_queue_in=0;
int r_queue_fi=0;
int r_fi_flag=0;

struct timeout_queue t_queue[25];
int t_queue_in=0;
int t_queue_fi=0;
int t_fi_flag=0;

// Estruturas globais do tipo mensagem
// -----------------------------------

mensagem trans_pacote;
mensagem receive_pacote;
mensagem request_pacote;
mensagem ack_pacote;

int InicializaConexaoEscuta(void)
{

// FUNCAO: Inicializa IPX, se o mesmo nao estiver ativo retorna NIPX, abre as
// soquetes, se as mesmas nao puderem ser abertas retorna SOCK. Comeca a es-
// cutar mensagens de conexao, desconexao e pacotes de dados neste caso retor-
// na OK.
// --------------------------------------------------------------------------

  int i;

// Inicializa IPX
// --------------

  i=IPXInitialize();
  if(i==0xf0)
    return NIPX;

// Aloca memoria para cabecalhos, ECB's e mensagens
// ------------------------------------------------

  trans_ECB=(ECB *) calloc(1,sizeof(ECB));
  conection_ECB=(ECB *) calloc(1,sizeof(ECB));
  request_ECB=(ECB *) calloc(1,sizeof(ECB));
  ack_ECB=(ECB *) calloc(1,sizeof(ECB));
  trans_cabec=(IPXHeader *) calloc(1,sizeof(IPXHeader));
  conection_cabec=(IPXHeader *) calloc(1,sizeof(IPXHeader));
  request_cabec=(IPXHeader *) calloc(1,sizeof(IPXHeader));
  ack_cabec=(IPXHeader *) calloc(1,sizeof(IPXHeader));

// Abre as soquetes
// ----------------

  i=IPXOpenSocket((BYTE *) &socket1,0x00);
  if(i!=0x00)
    return SOCK;
  i=IPXOpenSocket((BYTE *) &socket2,0x00);
  if(i!=0x00)
    return SOCK;
  i=IPXOpenSocket((BYTE *) &socket3,0x00);
  if(i!=0x00)
    return SOCK;

// Inicializa ECB para aguardar mensagens de conexao/desconexao
// ------------------------------------------------------------

  PreparaReceiveConexao();
  IPXListenForPacket(conection_ECB);
  PreparaReceiveRequisicao();
  IPXListenForPacket(request_ECB);
  return OK;
}

void GerenciaFilaDeConexao(void)
{

// FUNCAO: Executa verificacao da fila de conexao/desconexao, se houverem
// elementos, informa o fato a INTERFAC.C que matem uma lista dos termi-
// nais conectados.
// ---------------------------------------------------------------------

// Verifica se a fila esta vazia
// -----------------------------

  if(c_queue_fi==c_queue_in && c_fi_flag==0)
    return;

// Retira o primeiro elemento da fila e informa a INTERFAC.C sobre a mensagem
// recebida
// --------------------------------------------------------------------------

  // Em INTERFAC.C:

  AtualizaConexoes(c_queue[c_queue_in].addr,c_queue[c_queue_in].Flag,
		   c_queue[c_queue_in].num_msg);
  c_queue_in++;
  if(c_queue_in==15) {
    c_queue_in=0;
    c_fi_flag=0;
  }
}

void GerenciaFilaDeTimeouts(void)
{

// FUNCAO: Verifica se existe algum timeout na fila de timeouts. Se hou-
// verem elementos na fila, comunica o fato a INTERFAC.C.
// ---------------------------------------------------------------------

// Verifica se a fila esta vazia
// -----------------------------

  if(t_queue_fi==t_queue_in && t_fi_flag==0)
    return;

// Retira o primeiro elemento da fila e informa a INTERFAC.C sobre a mensagem
// recebida
// --------------------------------------------------------------------------

  // Em INTERFAC.C:

  AtendeTimeouts();
  t_queue_in++;
  if(t_queue_in==25) {
    t_queue_in=0;
    t_fi_flag=0;
  }
}

void AtendeTimeouts(void)
{

// FUNCAO: Verifica se existe uma mensagem de requisicao de acesso aos dados
// correspondente a um timeout da t_queue, se nao existir, poe a requisicao
// na r_queue.
// -------------------------------------------------------------------------

  int i,j,igual;

// Se a fila de requisicoes estiver cheia retorna
// ----------------------------------------------

  if(r_queue_fi==r_queue_in && r_fi_flag) {
    return;
  }

// Verifica se a requisicao correspondente ao timeout existe
// ---------------------------------------------------------

  for(i=0;i<25;i++) {
    igual=1;
    for(j=0;j<10;j++)
      if(t_queue[t_queue_in].addr[j]!=r_queue[i].addr[j]);
	igual=0;
    if(t_queue[t_queue_in].numero!=r_queue[i].numero)
      igual=0;
    if(t_queue[t_queue_in].tipo!=r_queue[i].tipo)
      igual=0;
    if(t_queue[t_queue_in].operacao!=r_queue[i].operacao)
      igual=0;
    if(t_queue[t_queue_in].ref_prod!=r_queue[i].ref_prod)
      igual=0;
    if(strcmp(t_queue[t_queue_in].item_prod,r_queue[i].item_prod)!=0)
      igual=0;
    if(strcmp(t_queue[t_queue_in].nome_prod,r_queue[i].nome_prod)!=0)
      igual=0;
    if(t_queue[t_queue_in].qtde_prod!=r_queue[i].qtde_prod)
      igual=0;
    if(strcmp(t_queue[t_queue_in].info_prod,r_queue[i].info_prod)!=0)
      igual=0;
    if(igual==1)
      return;
  }

// Se a requisicao referente ao timeout nao existir, joga-a na fila de re-
// quisicoes.
// -----------------------------------------------------------------------

  i=r_queue_fi;
  for(j=0;j<10;j++)
    r_queue[i].addr[j]=t_queue[t_queue_in].addr[j];
  r_queue[i].numero=t_queue[t_queue_in].numero;
  r_queue[i].tipo=2;
  r_queue[i].operacao=t_queue[t_queue_in].operacao;
  r_queue[i].ref_prod=t_queue[t_queue_in].ref_prod;
  strcpy(r_queue[i].item_prod,t_queue[t_queue_in].item_prod);
  strcpy(r_queue[i].nome_prod,t_queue[t_queue_in].nome_prod);
  r_queue[i].qtde_prod=t_queue[t_queue_in].qtde_prod;
  strcpy(r_queue[i].info_prod,t_queue[t_queue_in].info_prod);
  r_queue_fi++;
  if(r_queue_fi==25) {
    r_queue_fi=0;
    r_fi_flag=1;
  }
}

void GerenciaFilaDeRequisicao(void)
{

// FUNCAO: Verifica se existe alguma requisicao de acesso aos dados na fila
// de requisicoes. Se houverem elementos na fila, comunica o fato a
// INTERFAC.C.
// ------------------------------------------------------------------------

  int i;

// Verifica se as filas estao vazias
// ---------------------------------

  if(r_queue_fi==r_queue_in && r_fi_flag==0)
    return;

// Verifica se o pacote esta coerente
// ----------------------------------

  if((r_queue[r_queue_in].operacao!=AT && r_queue[r_queue_in].operacao!=REQ
      && r_queue[r_queue_in].operacao!=DEL) ||
     (r_queue[r_queue_in].tipo!=2))
     return;

// Verifica se o terminal esta conectado e realiza operacao desejada pelo
// terminal.
// ----------------------------------------------------------------------

  i=VerificaConexao(&(r_queue[r_queue_in]));
  if(i==3)
    EnviaNack(&(r_queue[r_queue_in]));
  else if(r_queue[r_queue_in].numero==1-i)
    EnviaAck(&(r_queue[r_queue_in]));

// Executa operacao requisitada
// ----------------------------

  if(r_queue[r_queue_in].operacao==AT)
    AtualizaProdutoRequisitado(&(r_queue[r_queue_in]));
  if(r_queue[r_queue_in].operacao==REQ) {
    ConsultaProdutoRequisitado(&(r_queue[r_queue_in]));

// Envia pacote com produto consultado no banco de dados.
// -------------------------------------------------------

    i=EnviaPacote(&(r_queue[r_queue_in]));
    if(i!=OK) {
      printf("Problema na transmissao de pacotes - %d",i);
      _exit(0);
    }
  }
  if(r_queue[r_queue_in].operacao==DEL)
    DeletaProdutoRequisitado(&(r_queue[r_queue_in]));

// Ajusta indices da fila de requisicao
// ------------------------------------

  r_queue_in++;
  if(r_queue_in==25) {
    r_queue_in=0;
    r_fi_flag=0;
  }
}

void PreparaTransAckConexao(void)
{

// FUNCAO: Prepara ECB e cabecalho para transmitir uma mensagem para um ter-
// minal especifico.
// -------------------------------------------------------------------------

  int i;

  for(i=0;i<4;i++) {
    trans_cabec->destination.network[i]=conection_cabec->source.network[i];
    networkAddress[i]=conection_cabec->source.network[i];
  }
  for(i=0;i<6;i++) {
    trans_cabec->destination.node[i]=conection_cabec->source.node[i];
    networkAddress[i+4]=conection_cabec->source.node[i];
  }
  *(WORD *) trans_cabec->destination.socket=socket1;
  for(i=10;i<12;i++)
    networkAddress[i]=0x45;
  IPXGetDataAddress( (BYTE *) trans_cabec,
		     (WORD *) &trans_ECB->fragmentDescriptor[0].address);
  trans_ECB->fragmentDescriptor[0].size=sizeof(IPXHeader);
  IPXGetDataAddress( (BYTE *) &trans_pacote,
		     (WORD *) &trans_ECB->fragmentDescriptor[1].address);
  trans_ECB->fragmentDescriptor[1].size=sizeof(mensagem);
  trans_ECB->fragmentCount=2;
  trans_ECB->ESRAddress=(void(far *) ())0;
  trans_ECB->socketNumber=socket1;
  IPXGetLocalTarget((BYTE *) &networkAddress,
		    (BYTE *) &trans_ECB->immediateAddress, &i);
}


void EnviaNack(struct request_queue *r_q)
{

// FUNCAO: Envia uma mensagem do tipo Nack para um determinado terminal.
// ---------------------------------------------------------------------

  int i;

  trans_pacote.tipo=4;
  for(i=0;i<10;i++)
    networkAddress[i]=r_q->addr[i];
  i=Transmite();
  if(i!=OK) {
    printf("Falha de transmissao");
    _exit(0);
  }
}

void EnviaAck(struct request_queue *r_q)
{

// FUNCAO: Envia uma mensagem do tipo Ack para um determinado terminal.
// ---------------------------------------------------------------------

  int i;

  trans_pacote.numero=r_q->numero;
  trans_pacote.tipo=3;
  trans_pacote.operacao=r_q->operacao;
  trans_pacote.ref_prod=r_q->ref_prod;
  for(i=0;i<10;i++)
    networkAddress[i]=r_q->addr[i];
  i=Transmite();
  if(i!=OK) {
    printf("Falha de transmissao");
    _exit(0);
  }
}

int EnviaPacote(struct request_queue *r_q)
{

// FUNCAO: Envia pacotes e aguarda resposta do terminal, nesta funcao esta
// implementado o protocolo de comunicacao. Retorna OK se a transmissao do
// pacote for um sucesso. TIME se a mensagem for transmitida mais de cinco
// vezes e nenhuma resposta for enviada. NACK se mais de cinco Nack's forem
// recebidos. TRANS se houver falha de transmissao. RECEP se houver falha de
// recepcao. NCON se a conexao ainda nao foi estabeleciada. Usa protocolo do
// bit alternante.
// -----------------------------------------------------------------------

  int i,nack=0,retransmit=0;
  long j=0;

// Atribui o pacote vindo da interface ao pacote trans_pacote.
// -----------------------------------------------------------

  for(i=0;i<10;i++)
    networkAddress[i]=r_q->addr[i];
  trans_pacote.numero=r_q->numero;
  trans_pacote.tipo=r_q->tipo;
  trans_pacote.operacao=r_q->operacao;
  trans_pacote.ref_prod=r_q->ref_prod;
  for(i=0;i<20;i++) {
    trans_pacote.item_prod[i]=r_q->item_prod[i];
    trans_pacote.nome_prod[i]=r_q->nome_prod[i];
  }
  trans_pacote.qtde_prod=r_q->qtde_prod;
  for(i=0;i<255;i++)
    trans_pacote.info_prod[255]=r_q->info_prod[255];

// Comeca a escutar e envia o pacote
// ---------------------------------

  PreparaReceiveAck();
  IPXListenForPacket(ack_ECB);
  i=Transmite();
  if(i==TRANS) {
    if(ack_ECB->inUseFlag==0xfe)
      IPXCancelEvent(ack_ECB);
    return TRANS;
  }
  while(1) {

// Se nenhuma mensagem chegar, espera 10000000 loops e retransmite a mensa-
// gem (timeout). Executa no maximo 5 retransmissoes
// --------------------------------------------------------------------

    if(ack_Flag==0) {
      j++;
      if(j==10000000) {
	j=0;
	if(ack_Flag==1)
	  continue;
	if(retransmit!=-1)
	  trans_pacote.tipo=5; // Timeout
	i=Transmite();
	if(i==TRANS) {
	  if(ack_ECB->inUseFlag==0xfe)
	    IPXCancelEvent(ack_ECB);
	  return TRANS;
	}
	retransmit++;
	if(retransmit>4) {
	  if(ack_ECB->inUseFlag==0xfe)
	    IPXCancelEvent(ack_ECB);
	  return TIME;
	}
      }
    }

// Se receber uma mensagem, verifica se a mesma e um Ack d terminal
// ----------------------------------------------------------------

    if(ack_Flag==1 && ack_error==0) {
      ack_Flag=0;
      i=ReconheceAck(r_q);
      if(i==OK) {
	if(ack_ECB->inUseFlag==0xfe)
	  IPXCancelEvent(ack_ECB);
	break;
      }
     if(i==NACK) {
       retransmit=-1;
       trans_pacote.tipo=2;
       j=9999999;
       nack++;
       if(nack==5) {
	if(ack_ECB->inUseFlag==0xfe)
	  IPXCancelEvent(ack_ECB);
	return NACK;
      }
       continue;
     }
     if(i==MSGD)
       continue;
    }

// Falha na recepcao
// -----------------

    if(ack_error==1) {
      if(ack_ECB->inUseFlag==0xfe)
	IPXCancelEvent(ack_ECB);
      return RECEP;
    }
  }
  return OK;
}


int ReconheceAck(struct request_queue *r_q)
{

// FUNCAO: Verifica se a mensagem provinda do terminal corresponde a esperada.
// Retorna OK, se a mensagem for a esperada. NACK se a mensagem recebida for
// um Nack. MSGD se a mensagem for desconhecida.
// --------------------------------------------------------------------------

  int i;

// Verifica se a mensagem provem do terminal certo
// -----------------------------------------------

  for(i=0;i<4;i++)
    if(r_q->addr[i]!=ack_cabec->source.network[i])
      return MSGD;
  for(i=0;i<6;i++)
    if(r_q->addr[i+4]!=ack_cabec->source.node[i])
      return MSGD;

// Se a mensagem recebida for um Ack retorne OK
// --------------------------------------------

  if(trans_pacote.numero==ack_pacote.numero &&
     ack_pacote.tipo==3 &&
     trans_pacote.operacao==ack_pacote.operacao &&
     trans_pacote.ref_prod==ack_pacote.ref_prod)
    return OK;

// Se a mensagem recebida for um Nack retorne NACK
// -----------------------------------------------

  if(ack_pacote.tipo==4)
    return NACK;

// Se a mensagem recebida nao for a esperada retorna MSGD
// ------------------------------------------------------

  else
   return MSGD;
}

void PreparaTransmissao(void)
{

// FUNCAO: Prepara ECB e cabecalho para transmitir uma mensagem para um ter-
// minal especifico.
// -------------------------------------------------------------------------

  int i;

  for(i=0;i<4;i++)
    trans_cabec->destination.network[i]=networkAddress[i];
  for(i=0;i<6;i++)
    trans_cabec->destination.node[i]=networkAddress[i+4];
  *(WORD *) trans_cabec->destination.socket=socket1;
  for(i=10;i<12;i++)
    networkAddress[i]=0x45;
  IPXGetDataAddress( (BYTE *) trans_cabec,
		     (WORD *) &trans_ECB->fragmentDescriptor[0].address);
  trans_ECB->fragmentDescriptor[0].size=sizeof(IPXHeader);
  IPXGetDataAddress( (BYTE *) &trans_pacote,
		     (WORD *) &trans_ECB->fragmentDescriptor[1].address);
  trans_ECB->fragmentDescriptor[1].size=sizeof(mensagem);
  trans_ECB->fragmentCount=2;
  trans_ECB->ESRAddress=(void(far *) ())0;
  trans_ECB->socketNumber=socket1;
  IPXGetLocalTarget((BYTE *) &networkAddress,
		    (BYTE *) &trans_ECB->immediateAddress, &i);
}

void PreparaReceiveConexao(void)
{

// Prepara ECB de recepcao para receber mensagens de conexao dos terminais
// -----------------------------------------------------------------------

  int i;

  IPXGetDataAddress( (BYTE *) conection_cabec,
		     (WORD *) &conection_ECB->fragmentDescriptor[0].address);
  conection_ECB->fragmentDescriptor[0].size=sizeof(IPXHeader);
  IPXGetDataAddress( (BYTE *) &receive_pacote,
		     (WORD *) &conection_ECB->fragmentDescriptor[1].address);
  conection_ECB->fragmentDescriptor[1].size=sizeof(mensagem);
  conection_ECB->fragmentCount=2;
  conection_ECB->ESRAddress=(void(far *) ())ConectionESRHandler;
  conection_ECB->socketNumber=socket1;
  for(i=0;i<6;i++) {
    conection_ECB->immediateAddress[i]=0xff;
  }
}

void PreparaReceiveRequisicao(void)
{

// FUNCAO: Prepara ECB de recepcao de requisicoes de acesso aos dados.
// -------------------------------------------------------------------

  int i;

  IPXGetDataAddress( (BYTE *) request_cabec,
		     (WORD *) &request_ECB->fragmentDescriptor[0].address);
  request_ECB->fragmentDescriptor[0].size=sizeof(IPXHeader);
  IPXGetDataAddress( (BYTE *) &request_pacote,
		     (WORD *) &request_ECB->fragmentDescriptor[1].address);
  request_ECB->fragmentDescriptor[1].size=sizeof(mensagem);
  request_ECB->fragmentCount=2;
  request_ECB->ESRAddress=(void(far *) ())RequestESRHandler;
  request_ECB->socketNumber=socket2;
  for(i=0;i<6;i++) {
    request_ECB->immediateAddress[i]=0xff;
  }
}

void PreparaReceiveAck(void)
{

// FUNCAO: Prepara ack_ECB para receber resposta de um terminal.
// -------------------------------------------------------------

  int i;

  IPXGetDataAddress( (BYTE *) ack_cabec,
		     (WORD *) &ack_ECB->fragmentDescriptor[0].address);
  ack_ECB->fragmentDescriptor[0].size=sizeof(IPXHeader);
  IPXGetDataAddress( (BYTE *) &ack_pacote,
		     (WORD *) &ack_ECB->fragmentDescriptor[1].address);
  ack_ECB->fragmentDescriptor[1].size=sizeof(mensagem);
  ack_ECB->fragmentCount=2;
  ack_ECB->ESRAddress=(void(far *) ())AckESRHandler;
  ack_ECB->socketNumber=socket3;
  IPXGetLocalTarget((BYTE *) &networkAddress,
		    (BYTE *) &ack_ECB->immediateAddress, &i);
}

int Transmite(void)
{

// FUNCAO: Transimitir o pacote e verificar o sucesso da transmissao. Se hou-
// ver sucesso na transmissao, a funcao retorna OK, senao a funcao retorna
// TRANS.
// --------------------------------------------------------------------------

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

void cdecl TransmiteAckConexao(void)
{

// FUNCAO: Esta funcao e chamada pela funcao ConectionESRHandler, que esta con-
// tida no arquivo ESRCCON.ASM, quando um pacote de conexao/desconexao e re-
// cebido.
// --------------------------------------------------------------------------

  int i;

// Se a fila de conexao estiver cheia nao transmite Ack e comeca a escutar
// novamente.
// -----------------------------------------------------------------------

  if(c_queue_fi==c_queue_in && c_fi_flag) {
    IPXListenForPacket(conection_ECB);
    return;
  }

// Coloca requisicao de conexao/desconexao em ultimo na fila
// ---------------------------------------------------------

  if(receive_pacote.tipo==0)
    c_queue[c_queue_fi].Flag=1;
  if(receive_pacote.tipo==1)
    c_queue[c_queue_fi].Flag=2;

// Se nao for pacote de conexao/desconexao nao transmite Ack
// ---------------------------------------------------------

  if(receive_pacote.tipo!=0 && receive_pacote.tipo!=1) {
    IPXListenForPacket(conection_ECB);
    return;
  }
  for(i=0;i<10;i++) {
    if(i<4)
      c_queue[c_queue_fi].addr[i]=conection_cabec->source.network[i];
    if(i>=4)
      c_queue[c_queue_fi].addr[i]=conection_cabec->source.node[i-4];
  }
  c_queue[c_queue_fi].num_msg=receive_pacote.numero;
  c_queue_fi++;
  if(c_queue_fi==15) {
    c_queue_fi=0;
    c_fi_flag=1;
  }

// Prepara e transmite Ack, e comeca a escutar novamente
// -----------------------------------------------------

  trans_pacote.numero=receive_pacote.numero;
  trans_pacote.tipo=3;
  PreparaTransAckConexao();
  IPXListenForPacket(conection_ECB);
  IPXSendPacket(trans_ECB);
}

void cdecl RecebePacoteAcessoDados(void)
{

// FUNCAO: Esta funcao e chamada pela funcao RequestESRHandler, que esta con-
// tida no arquivo ESRCREQ.ASM, quando um pacote de requisicao de acesso aos
// dados e recebido.
// --------------------------------------------------------------------------

  int i;

// Se o tipo for 5 e um timeout
// ----------------------------

  if(request_pacote.tipo==5) {

// Se a fila de timeouts estiver cheia comeca a escutar novamente.
// ---------------------------------------------------------------

    if(t_queue_fi==t_queue_in && t_fi_flag) {
      IPXListenForPacket(request_ECB);
      return;
    }
    for(i=0;i<10;i++) {
      if(i<4)
	t_queue[t_queue_fi].addr[i]=request_cabec->source.network[i];
      if(i>=4)
	t_queue[t_queue_fi].addr[i]=request_cabec->source.node[i-4];
    }
    t_queue[t_queue_fi].numero=request_pacote.numero;
    t_queue[t_queue_fi].tipo=request_pacote.tipo;
    t_queue[t_queue_fi].operacao=request_pacote.operacao;
    t_queue[t_queue_fi].ref_prod=request_pacote.ref_prod;
    for(i=0;i<20;i++) {
      t_queue[t_queue_fi].item_prod[i]=request_pacote.item_prod[i];
      t_queue[t_queue_fi].nome_prod[i]=request_pacote.nome_prod[i];
    }
    t_queue[t_queue_fi].qtde_prod=request_pacote.qtde_prod;
    for(i=0;i<255;i++)
      t_queue[t_queue_fi].info_prod[i]=request_pacote.info_prod[i];
    t_queue_fi++;
    if(t_queue_fi==25) {
      t_queue_fi=0;
      t_fi_flag=1;
    }
    IPXListenForPacket(request_ECB);
    return;
  }

// Se a fila de requisicoes estiver cheia comeca a escutar novamente.
// ------------------------------------------------------------------

  if(r_queue_fi==r_queue_in && r_fi_flag) {
    IPXListenForPacket(request_ECB);
    return;
  }

  for(i=0;i<10;i++) {
    if(i<4)
      r_queue[r_queue_fi].addr[i]=request_cabec->source.network[i];
    if(i>=4)
      r_queue[r_queue_fi].addr[i]=request_cabec->source.node[i-4];
  }
  r_queue[r_queue_fi].numero=request_pacote.numero;
  r_queue[r_queue_fi].tipo=request_pacote.tipo;
  r_queue[r_queue_fi].operacao=request_pacote.operacao;
  r_queue[r_queue_fi].ref_prod=request_pacote.ref_prod;
  for(i=0;i<20;i++) {
    r_queue[r_queue_fi].item_prod[i]=request_pacote.item_prod[i];
    r_queue[r_queue_fi].nome_prod[i]=request_pacote.nome_prod[i];
  }
  r_queue[r_queue_fi].qtde_prod=request_pacote.qtde_prod;
  for(i=0;i<255;i++)
    r_queue[r_queue_fi].info_prod[i]=request_pacote.info_prod[i];
  r_queue_fi++;
  if(r_queue_fi==25) {
    r_queue_fi=0;
    r_fi_flag=1;
  }

// Comeca a escutar novamente
// --------------------------

  IPXListenForPacket(request_ECB);
}

void cdecl AguardaPacote(void)
{

// FUNCAO: Esta funcao e chamada pela rotina ESRCACK.ASM caso algum pacote
// seja recebido por ack_ECB
// ----------------------------------------------------------------------

  ack_Flag=1;
  if(ack_ECB->completionCode!=0x00)
    ack_error=1;
  PreparaReceiveAck();
  IPXListenForPacket(ack_ECB);
}