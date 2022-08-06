// Interface do programa CENTRAL
// Andrey Patitucci
// Carlos E. Cruz
// Lisianne Requiao
// Luciane E. Romeiro
// Raquel Speltz
// Nenhum direito reservado - Copywrong - 1993

// A mensagem "FUNCAO: ...." especifica o que faz a funcao onde a mensagem
// estiver.

#include <nxt.h>
#include <alloc.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <mem.h>
#include <dos.h>
#include "netgloba.h"
#include "central.h"
#include "databank.h"

// Lista encadeada de terminais conectados
// ---------------------------------------

struct conection_list *c_list;

void main(void)
{
  clrscr();
  gotoxy(1,25);
  printf("Aguardando conexoes");
  InicializaCentral();
  while(!kbhit()) {
    GerenciaFilaDeConexao();
    GerenciaFilaDeTimeouts();
    GerenciaFilaDeRequisicao();
  }
}

void InicializaCentral(void)
{

// FUNCAO: Inicializa tudo o que e necessario para a central entrar em
// operacao.
// -------------------------------------------------------------------

  int i;

// Inicializa operacoes em NETWARE.C
// ---------------------------------

  i=InicializaConexaoEscuta();
  if(i!=OK) {
    printf("Problema de inicializacao: %d",i);
    exit(0);
  }

// Inicializacao da lista de terminais conectados
// ----------------------------------------------

  c_list=(struct conection_list *) calloc(1,sizeof(struct conection_list));
  c_list->proximo=NULL;
}

void AtualizaConexoes(BYTE *addr,int operacao,int num_msg)
{

// FUNCAO: Atualiza lista de terminais conectados e imprime na tela estado
// atual dos terminais.
// -----------------------------------------------------------------------

  struct conection_list *p,*q;
  int i,j,exist=0;

// Verifica se o terminal que requisitou a operacao ja esta conectado
// ------------------------------------------------------------------

  q=p=c_list;
  while(p->proximo!=NULL) {
    q=p;
    p=p->proximo;
    exist=1;
    for(i=0;i<10;i++)
      if(addr[i]!=p->addr[i])
	exist=0;
    if(exist==1) break;
  }

// Se o terminal ainda nao se encontra na lista, e a operacao e de conexao,
// inclui terminal na lista
// ------------------------------------------------------------------------

  if(exist==0 && operacao==1) {
    q=(struct conection_list *) calloc(1,sizeof(struct conection_list));
    p->proximo=q;
    for(i=0;i<10;i++)
      q->addr[i]=addr[i];
    q->ultimo_num_msg=num_msg;
    q->proximo=NULL;
  }

// Se o terminal ja existir e a operacao for de desconexao, elimina terminal
// da lista
// -------------------------------------------------------------------------

  if(exist==1 && operacao==2) {
    q->proximo=p->proximo;
    free(p);
  }

// Atualiza indices dos terminais e imprime lista de conectados na tela
// --------------------------------------------------------------------

  p=c_list;
  clrscr();
  i=1;
  while(p->proximo!=NULL) {
    p=p->proximo;
    p->numero=i;
    gotoxy(1,i);
    for(j=0;j<10;j++)
      printf("%d",p->addr[j]);
    printf("          Terminal %d\n",i);
    i++;
  }
  gotoxy(1,25);
  cprintf("Aguardando conexoes");
}

int VerificaConexao(struct request_queue *r_q)
{

// FUNCAO: Verifica se o terminal esta conectado e retorna o ultimo numero
// de mensagem enviado pelo mesmo, ou 3 se o mesmo nao estiver conectado.
// -----------------------------------------------------------------------

  struct conection_list *p=c_list;
  int i,igual;

// Verifica se o terminal esta conectado
// -------------------------------------

  if(p->proximo==NULL)
    igual=0;
  else {
    do {
      p=p->proximo;
      igual=1;
      for(i=0;i<10;i++) {
	if(p->addr[i]!=r_q->addr[i])
	  igual=0;
      }
      if(igual==1)
	break;
    } while(p->proximo!=NULL);
  }
  i=p->ultimo_num_msg;
  p->ultimo_num_msg=r_q->numero;
  if(igual==0)
    return 3;
  return i;
}

void AtualizaProdutoRequisitado(struct request_queue *r_q)
{

// FUNCAO: Atualiza produto requisitado no banco de dados.
// -------------------------------------------------------

  FILE *datb;
  int i,j;
  char *num1,*num2,ref[10],quant[6];
  char prod_buffer[310];

// Abre arquivo de banco de dados
// ------------------------------

  if((datb=fopen("databank.dat","r+b"))==NULL)
    if((datb=fopen("databank.dat","w+b"))==NULL)
      return;

// Carrega prod_buffer e atualiza produto
// --------------------------------------

  num1=ultoa(r_q->ref_prod,num1,10);
  num2=itoa(r_q->qtde_prod,num2,10);
  strcpy(ref,num1);
  strcpy(quant,num2);
  setmem(prod_buffer,310,0);
  for(j=0;j<10;j++) {
    prod_buffer[j]=ref[j];
    if(ref[j]==0)
      break;
  }
  for(i=0,j=10;i<20;i++,j++) {
    prod_buffer[j]=r_q->item_prod[i];
    if(r_q->item_prod[i]==0)
      break;
  }
  for(i=0,j=30;i<20;i++,j++) {
    prod_buffer[j]=r_q->nome_prod[i];
    if(r_q->nome_prod[i]==0)
      break;
  }
  for(i=0,j=50;i<5;i++,j++) {
    prod_buffer[j]=quant[i];
    if(quant[i]==0)
      break;
  }
  for(i=0,j=55;i<255;i++,j++) {
    prod_buffer[j]=r_q->info_prod[i];
    if(r_q->info_prod[i]==0)
      break;
  }
  DataBank(datb,prod_buffer,310,Atu);
  fclose(datb);
}

void ConsultaProdutoRequisitado(struct request_queue *r_q)
{

// FUNCAO: Consulta banco de dados para ver se existe um produto, se
// existir, o prduto e enviado ao terminal requisitante.
// -----------------------------------------------------------------

  int i,j,status;
  FILE *datb;
  char *num,ref[10];
  char prod_buffer[310];

// Abre arquivo de banco de dados
// ------------------------------

  if((datb=fopen("databank.dat","r+b"))==NULL) {
    if((datb=fopen("databank.dat","w+b"))==NULL) {
      printf("Impossivel abrir arquivo de banco de dados");
      _exit(0);
    }
  }

// Consulta banco de dados para ver se existe o produto
// ----------------------------------------------------

  num=ultoa(r_q->ref_prod,num,10);
  strcpy(ref,num);
  for(i=0;i<10;i++) {
    prod_buffer[i]=ref[i];
    if(ref[i]==0)
      break;
  }
  DataBank(datb,prod_buffer,310,Req);
  fclose(datb);

// Se nao achar o produto, o valor da referencia sera 0
// ----------------------------------------------------

  if((strcmp(prod_buffer,"000000000"))==0)
    r_q->ref_prod=0;

// Se achar o produto, carrega todos os campos
// -------------------------------------------

  if((strcmp(prod_buffer,"000000000"))!=0) {
    for(i=0;i<10;i++) {
      num[i]=prod_buffer[i];
      if(prod_buffer[i]==0)
	break;
    }
    r_q->ref_prod=atoi(num);
    for(i=0,j=10;i<20;i++,j++) {
      r_q->item_prod[i]=prod_buffer[j];
      if(prod_buffer[j]==0)
	break;
    }
    for(i=0,j=30;i<20;i++,j++) {
      r_q->nome_prod[i]=prod_buffer[j];
      if(prod_buffer[j]==0)
	break;
    }
    for(i=0,j=50;i<5;i++,j++) {
      num[i]=prod_buffer[j];
      if(prod_buffer[j]==0)
	break;
    }
    r_q->qtde_prod=atoi(num);
    for(i=0,j=55;i<255;i++,j++) {
      r_q->info_prod[i]=prod_buffer[j];
      if(prod_buffer[j]==0)
	break;
    }
  }
}

void DeletaProdutoRequisitado(struct request_queue *r_q)
{

// FUNCAO: Deleta produto requisitado no banco de dados.
// -----------------------------------------------------

  int i;
  FILE *datb;
  char *num,ref[10];
  char prod_buffer[310];

// Abre arquivo de banco de dados
// ------------------------------

  if((datb=fopen("databank.dat","r+b"))==NULL) {
    if((datb=fopen("databank.dat","w+b"))==NULL) {
      printf("Impossivel abrir arquivo de banco de dados");
      _exit(0);
    }
  }

// Deleta Produto no banco de dados
// --------------------------------

  num=ultoa(r_q->ref_prod,num,10);
  strcpy(ref,num);
  for(i=0;i<10;i++) {
    prod_buffer[i]=ref[i];
    if(ref[i]==0)
      break;
  }
  DataBank(datb,prod_buffer,310,Del);
  fclose(datb);
}