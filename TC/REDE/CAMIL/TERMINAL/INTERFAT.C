// Interface do programa TERMINAL
// A mensagem "FUNCAO: ...." especifica o que faz a funcao onde a mensagem
// estiver.

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mem.h>
#include <dos.h>
#include "pulldown.h"
#include "netgloba.h"
#include "terminal.h"

#define IGNORE  0
#define RETRY   1
#define ABORT   2

// Configuracao do terminal
// ------------------------

char senha[31];
unsigned long ref_inicial;
unsigned long ref_final;
char departamento[80];

// Lista encadeada de itens
// ------------------------

struct item_list *i_list;

// Mensagens de erro para a funcao Handler
// ---------------------------------------

char *err_msg[] = {
  "protegido contra gravacao","unidade desconhecida","disco nao pronto",
  "comando desconhecido","erro de dados (CRC)","requisicao invalida",
  "erro de busca","tipo desconhecido de disco","setor nao encontrado",
  "impressora sem papel","erro de escrita","erro de leitura","falha geral",
  "reservado","reservado","mudanca de disco invalida"
};

// Ponteiro para estrutura com os dados do menu pop-up
// ---------------------------------------------------

menu_info *menu;

void main(void)
{
  int status;


// A funcao Handler vai detectar e avisar o sistema sobre erros de hardware
// ------------------------------------------------------------------------

  harderr(Handler);

  _setcursortype(_NOCURSOR);
  textmode(C80);
  textcolor(LIGHTGRAY);
  textbackground(BLACK);
  clrscr();
  cputs("Sistema de controle de estoque em rede Ethernet\r\n");
  cputs("Andrey, Carlos, Lisianne, Luciane, Raquel - 1993");
  status=BuscaConexao();
  if(status) Menu();
  textcolor(LIGHTGRAY);
  textbackground(BLACK);
  clrscr();
  _setcursortype(_NORMALCURSOR);
  cputs("That's all folks");
}

// Desabilita warnings de parametros que nao vao ser usados
// --------------------------------------------------------

#pragma warn -par

int Handler(int errval,int ax,int bp,int si)
{

// Substitui controlador de erros de hardware presente no DOS por um con-
// figurado especialmente para o programa.
// ----------------------------------------------------------------------

  char msg[80];
  unsigned di;
  int drive;
  int errorno;
  int i;
  di= _DI;

// Verifica se o erro ocorrido foi no disco, ou em outro dispositivo
// -----------------------------------------------------------------

  if(ax<0) {

// Caso nao seja erro de disco, imprime mensagem de erro e retorna ao progra-
// ma mandando abortar
// --------------------------------------------------------------------------

    Mensagem("Erro em dispositivo de E/S.");
    textcolor(LIGHTGRAY);
    textbackground(BLACK);
    clrscr();
    _setcursortype(_NORMALCURSOR);
    cputs("Aplicativo finalizado.");
    hardresume(ABORT);
  }

// Verifica qual o erro de disco ocorrido
// --------------------------------------

  drive = ax & 0x00FF;
  errorno = di & 0x00FF;

// Imprime o erro ocorrido na caixa de dialogo
// -------------------------------------------

  sprintf(msg,"Erro: %s no drive %c.\r\nTentar novamente?",
	  err_msg[errorno],'A'+drive);
  i=CaixaDeDialogo(msg);
  if(i==1)
    i=RETRY;
  else {
    textcolor(LIGHTGRAY);
    textbackground(BLACK);
    clrscr();
    _setcursortype(_NORMALCURSOR);
    cputs("Aplicativo finalizado.");
    i=ABORT;
  }

// Se escolher ABORT o programa e abortado, senao tenta novamente a ope-
// racao
// ----------------------------------------------------------------------

  hardresume(i);
  return ABORT;
}

// Reabilita warnings de parametros que nao vao ser usados
// --------------------------------------------------------

#pragma warn +par

void Exit(void)
{

// FUNCAO: Chamada por outras funcoes caso o processamento do TERMINAL nao
// possa ir adiante por motivos quaisquer; encerra aplicativo.
// ------------------------------------------------------------------------

  BuscaConexao();
  textcolor(LIGHTGRAY);
  textbackground(BLACK);
  clrscr();
  _setcursortype(_NORMALCURSOR);
  cputs("Aplicativo finalizado.");
  _exit(0);
}

void Mensagem(char *string)
{

// FUNCAO: Imprime *string em uma janela e espera alguma tecla ser pressiona-
// da.
// --------------------------------------------------------------------------

  struct text_info ti;

  gettextinfo(&ti);
  textcolor(BLACK);
  textbackground(LIGHTGRAY);
  Window(13,9,67,16);
  gotoxy(1,1);
  cputs(string);
  gotoxy(1,6);
  cputs("PRESSIONE ALGO PARA CONTINUAR");
  getch();
  CloseWindow();
  textattr(ti.attribute);
}

int CaixaDeDialogo(char *string)
{

// FUNCAO: Implementa caixa de dialogo com os botoes OK e CANCEL,
// retorna 1 se OK e 0 se CANCEL.
// --------------------------------------------------------------

  struct text_info ti;
  int i,ret_val;

  gettextinfo(&ti);
  textcolor(BLACK);
  textbackground(LIGHTGRAY);
  Window(13,9,67,19);
  gotoxy(1,1);
  cputs(string);
  gotoxy(15,8);
  cprintf("[ Ok! ]");
  gotoxy(25,8);
  cprintf("[ Cancel! ]");
  while(1) {
    i=Botao(17,8,BLACK,LIGHTGRAY,"Ok!");
    if(i==1) {
      ret_val=1;
      break;
    }
    i=Botao(27,8,BLACK,LIGHTGRAY,"Cancel!");
    if(i==1) {
      ret_val=0;
      break;
    }
  }
  CloseWindow();
  textattr(ti.attribute);
  return ret_val;
}

int BuscaConexao(void)
{

// FUNCAO: Chama a funcao ConectaDesconectaComCentral que esta em NETWARET.C
// e analisa o codigo de retorno. Se tudo estiver correto, retorna 1, senao
// retorna 0.
// -------------------------------------------------------------------------

  int i;
  struct text_info ti;

  gettextinfo(&ti);
  textcolor(BLACK);
  textbackground(CYAN);
  Window(15,9,57,11);
  cputs(" Aguarde conexao/desconexao com central");
  while(1) {
    i=ConectaDesconectaComCentral();
    switch(i) {
      case OK:
	break;
      case NIPX:
	Mensagem("IPX nao esta instalado");
	CloseWindow();
	textattr(ti.attribute);
	return 0;
      case SOCK:
	if(CaixaDeDialogo("Soquete nao pode ser aberta.\r\nTentar novamente?"))
	  continue;
	else
	  CloseWindow();
	  textattr(ti.attribute);
	  return 0;
      case TIME:
	if(CaixaDeDialogo("Central nao responde a mensagem de conexao/desconexaoTentar novamente?"))
	  continue;
	else
	  CloseWindow();
	  textattr(ti.attribute);
	  return 0;
      case TRANS:
	if(CaixaDeDialogo("Falha na transmissao de dados.\r\nTentar novamente?"))
	  continue;
	else
	  CloseWindow();
	  textattr(ti.attribute);
	  return 0;
      case RECEP:
	if(CaixaDeDialogo("Falha na recepcao de dados.\r\nTentar novamente?"))
	  continue;
	else
	  CloseWindow();
	  textattr(ti.attribute);
	  return 0;
      default:
	continue;
    }
    if(i==OK)
      break;
  }
  CloseWindow();
  textattr(ti.attribute);
  return 1;
}

void Menu(void)
{

// FUNCAO: Consulta arquivo de configuracao e implementa menu principal.
// ---------------------------------------------------------------------

  char *opc_menu[]={"Lista itens","Edita configuracao","Ajuda","Abandonar","##"};
  int sair=0,esc;

// Carrega configuracao do terminal
// --------------------------------

  CarregaConfiguracao();

// Verifica senha de operacao
// --------------------------

  textcolor(BLACK);
  textbackground(CYAN);
  Window(13,9,67,13);
  cprintf("Entre com a senha de operacao do terminal:");
  while(1) {
    window(20,12,50,12);
    textcolor(WHITE);
    textbackground(BLACK);
    clrscr();
    if(strcmp((char *)in_str(1,1,30,'h'),senha)==0)
      break;
    else {
      if(CaixaDeDialogo("Senha de operacao incorreta.\r\nTentar novamente?"))
	continue;
      else {
	CloseWindow();
	Exit();
      }
    }
  }
  CloseWindow();
  textcolor(LIGHTGRAY);
  textbackground(BLACK);
  gotoxy(1,4);
  cputs(departamento);
  gotoxy(1,5);
  cprintf("Referancia inicial: %lu  Referencia final: %lu",ref_inicial,ref_final);

// Inicializa lista encadeada de itens
// -----------------------------------

  i_list=NULL;

// Busca lista de produtos referentes ao terminal
// ----------------------------------------------

  BuscaListaDeProdutos();

// Define o aspecto do menu
// ------------------------

  menu=(menu_info *)malloc(sizeof(menu_info));
  menu->opc=0;
  menu->tipo=POPUP;
  menu->estado=CLOSE;
  menu->col_in=28;
  menu->lin_in=10;
  menu->cor_frente=BLACK;
  menu->cor_fundo=LIGHTCYAN;
  gotoxy(2,25);
  cprintf("Setas navegam nas opcoes, <Enter> seleciona opcao, <Esc> sai p/ DOS");
  textcolor(BLACK);
  textbackground(LIGHTCYAN);
  while(sair==0) {
    esc=PullDown(menu,opc_menu);
    switch(esc) {
      case 0:
	textcolor(LIGHTGRAY);
	textbackground(BLACK);
	window(1,1,80,25);
	gotoxy(1,25);
	clreol();
	cprintf(" <Esc> retorna para o menu principal");
	ListaItens();
	gotoxy(2,25);
	cprintf("Setas navegam nas opcoes, <Enter> seleciona opcao, <Esc> sai p/ DOS");
	textcolor(BLACK);
	textbackground(LIGHTCYAN);
	break;
      case 1:
	textcolor(LIGHTGRAY);
	textbackground(BLACK);
	window(1,1,80,25);
	gotoxy(1,25);
	clreol();
	cprintf(" <Esc> retorna para o menu principal");
	EditaConfiguracao();
	CarregaConfiguracao();
	gotoxy(1,4);
	clreol();
	cputs(departamento);
	gotoxy(1,5);
	clreol();
	cprintf("Referancia inicial: %lu  Referencia final: %lu",ref_inicial,ref_final);
	gotoxy(2,25);
	cprintf("Setas navegam nas opcoes, <Enter> seleciona opcao, <Esc> sai p/ DOS");
	textcolor(BLACK);
	textbackground(LIGHTCYAN);
	break;
      case 2:
	textcolor(LIGHTGRAY);
	textbackground(BLACK);
	window(1,1,80,25);
	textcolor(LIGHTGRAY);
	textbackground(BLACK);
	gotoxy(1,25);
	clreol();
	cprintf(" <Esc> retorna para o menu principal");
	IndiceAjuda();
	gotoxy(2,25);
	cprintf("Setas navegam nas opcoes, <Enter> seleciona opcao, <Esc> sai p/ DOS");
	textcolor(BLACK);
	textbackground(LIGHTCYAN);
	break;
      case ESC:
      case 3 :
	sair=1;
	break;
      default:
	break;
    }
  }
  ConectaDesconectaComCentral();
  CloseMenu(menu);
}

void BuscaListaDeProdutos(void)
{

// FUNCAO: Chama a funcao EnviaPacoteAT_DEL_REQ em NETWARET.C para buscar a
// lista de produtos que esta na faixa de referencias configurada para o ter-
// minal e carrega em uma lista encadeada.
// -------------------------------------------------------------------------

  unsigned long i;
  int j,fim=0;
  struct text_info ti;
  mensagem pacote;

  gettextinfo(&ti);
  textcolor(BLACK);
  textbackground(CYAN);
  Window(22,9,63,11);
  gotoxy(1,1);
  cprintf(" Buscando produto com ref : ");
  pacote.tipo=2;
  pacote.operacao=REQ;
  i=ref_inicial;
  while(1) {
    gotoxy(30,1);
    cprintf("%lu",i);
    pacote.ref_prod=i;
    j=EnviaPacoteAT_DEL_REQ(pacote,0x4547);
    switch(j) {
      case OK:
	RecebePacote_REQ();
	i++;
	if(i>ref_final) {
	  fim=1;
	  break;
	}
	continue;
      case NCON:
	Mensagem(" Este terminal nao foi conectado");
	CloseWindow();
	textattr(ti.attribute);
	fim=1;
	break;
      case TIME:
	if(CaixaDeDialogo("Central nao atende requisicao.\r\nTentar novamente?"))
	continue;
	else
	  CloseWindow();
	  textattr(ti.attribute);
	  fim=1;;
	  break;
      case NACK:
	if(CaixaDeDialogo("Central nao reconhece as mensagens.\r\nTentar novamente?"))
	  continue;
	else
	  CloseWindow();
	  textattr(ti.attribute);
	  fim=1;
	  break;
      case TRANS:
	if(CaixaDeDialogo("Falha na transmissao de dados.\r\nTentar novamente?"))
	  continue;
	else
	  CloseWindow();
	  textattr(ti.attribute);
	  fim=1;
	  break;
      case RECEP:
	if(CaixaDeDialogo("Falha na recepcao de dados.\r\nTentar novamente?"))
	  continue;
	else
	  CloseWindow();
	  textattr(ti.attribute);
	  fim=1;
	  break;
      default:
	continue;
    }
    if(fim==1)
      break;
  }
  CloseWindow();
  textattr(ti.attribute);
}

void AtualizaListaDeProdutos(mensagem *pacote)
{

// FUNCAO: Atualiza a lista encadeada de itens e produtos com os pacotes
// de dados que chegam da central.
// ---------------------------------------------------------------------

  int i;
  struct item_list *p,*q;
  struct product_list *r,*s;

// Se a referencia do produto for 0, nenhum produto com a referencia re-
// quisitada foi encontrada no banco de dados
// ---------------------------------------------------------------------

  if(pacote->ref_prod==0)
    return;

// Se lista vazia
// --------------

  p=(struct item_list*) i_list;
  if(i_list==NULL) {
    i_list=(struct item_list*) calloc(1,sizeof(struct item_list));
    p=(struct item_list*) i_list;
    p->anterior=NULL;
    strcpy(p->item,pacote->item_prod);
    p->mod_flag=0;
    r=(struct product_list*) calloc(1,sizeof(struct product_list));
    p->p_list=r;
    p->proximo=NULL;
    r->anterior=NULL;
    r->ref_prod=pacote->ref_prod;
    strcpy(r->nome_prod,pacote->nome_prod);
    strcpy(r->item_prod,pacote->item_prod);
    r->qtde_prod=pacote->qtde_prod;
    strcpy(r->info_prod,pacote->info_prod);
    r->mod_flag=0;
    r->proximo=NULL;
    return;
  }

// Se lista nao vazia
// ------------------

  while(1) {

// Se item ja existe
// -----------------

    if(strcmp(p->item,pacote->item_prod)==0) {

// Se lista de produtos vazia
// --------------------------

      if(p->p_list==NULL) {
	r=(struct product_list*) calloc(1,sizeof(struct product_list));
	p->p_list=r;
	r->anterior=NULL;
	r->ref_prod=pacote->ref_prod;
	strcpy(r->nome_prod,pacote->nome_prod);
	strcpy(r->item_prod,pacote->item_prod);
	r->qtde_prod=pacote->qtde_prod;
	strcpy(r->info_prod,pacote->info_prod);
	r->mod_flag=0;
	r->proximo=NULL;
	return;
      }

// Se lista de produtos nao vazia
// ------------------------------

      else {
	r=p->p_list;
	while(1) {
	  if(r->ref_prod==pacote->ref_prod)
	    return;
	  if(r->proximo==NULL)
	    break;
	  r=r->proximo;
	}
	s=(struct product_list*) calloc(1,sizeof(struct product_list));
	r->proximo=s;
	s->anterior=r;
	s->ref_prod=pacote->ref_prod;
	strcpy(s->nome_prod,pacote->nome_prod);
	strcpy(s->item_prod,pacote->item_prod);
	s->qtde_prod=pacote->qtde_prod;
	strcpy(s->info_prod,pacote->info_prod);
	s->mod_flag=0;
	s->proximo=NULL;
	return;
      }
    }
    if(p->proximo==NULL)
      break;
    p=p->proximo;
  }

// Item nao existe
// ---------------

  q=(struct item_list*) calloc(1,sizeof(struct item_list));
  p->proximo=q;
  q->anterior=p;
  strcpy(q->item,pacote->item_prod);
  q->mod_flag=0;
  r=(struct product_list*) calloc(1,sizeof(struct product_list));
  q->p_list=r;
  q->proximo=NULL;
  r->anterior=NULL;
  r->ref_prod=pacote->ref_prod;
  strcpy(r->nome_prod,pacote->nome_prod);
  strcpy(r->item_prod,pacote->item_prod);
  r->qtde_prod=pacote->qtde_prod;
  strcpy(r->info_prod,pacote->info_prod);
  r->mod_flag=0;
  r->proximo=NULL;
}

void ListaItens(void)
{

// FUNCAO: Lista os itens na tela para serem selecionados, cria novos itens.
// -------------------------------------------------------------------------

  struct text_info ti;
  struct item_list *p,*q,*top;
  unsigned char item[20];
  int i,j,pos=3,num_itens=0,item_atual,sair=0,reprint;

// Formata a tela
// --------------

  gettextinfo(&ti);
  textcolor(BLACK);
  textbackground(CYAN);
  Window(1,3,80,24);
  Quadro(1,1,74,18);
  Quadro(75,1,77,18);
  gotoxy(1,20);
  cprintf("Setas navegam, <Tab> cria novo item, <Enter> seleciona item");
  gotoxy(2,2);
  cprintf("Nome do item");
  textcolor(LIGHTGRAY);
  textbackground(BLACK);
  for(i=3;i<18;i+=2) {
    gotoxy(2,i);
    cprintf("                                                  ");
  }

// Verifica se lista de itens esta vazia, se nao estiver mostra itens na
// tela, permite inclusao de novos itens
// ---------------------------------------------------------------------

  top=p=i_list;
  if(p!=NULL) {
    item_atual=num_itens=1;
    while(p->proximo!=NULL) {
      num_itens++;
      p=p->proximo;
    }
  }
  else
    item_atual=0;
  q=top;
  while(sair==0) {

// Imprime lista de itens na tela
// ------------------------------

    i=3;
    p=top;
    if(num_itens==1) {
      gotoxy(2,i);
      cputs("                         \r");
      gotoxy(2,i);
      cputs(p->item);
    }
    if(num_itens>1) {
      while(i<18) {
	gotoxy(2,i);
	cputs("                         \r");
	gotoxy(2,i);
	cputs(p->item);
	if(p->proximo==NULL)
	  break;
	i+=2;
	p=p->proximo;
      }
    }

// Imprime setinhas no lado direito da tela
// ----------------------------------------

    textcolor(WHITE);
    textbackground(CYAN);
    if(num_itens==0) {
      p->proximo=NULL;
      top->anterior=NULL;
    }
    if(p->proximo==NULL && i>17)
      p=p->anterior;
    if(p->proximo==NULL && top->anterior==NULL) {
      gotoxy(76,2);
      putch(' ');
      gotoxy(76,17);
      putch(' ');
    }
    else if(p->proximo!=NULL && top->anterior==NULL) {
      gotoxy(76,2);
      putch(' ');
      gotoxy(76,17);
      putch(31);
    }
    else if(p->proximo==NULL && top->anterior!=NULL) {
      gotoxy(76,2);
      putch(30);
      gotoxy(76,17);
      putch(' ');
    }
    else if(p->proximo!=NULL && top->anterior!=NULL) {
      gotoxy(76,2);
      putch(30);
      gotoxy(76,17);
      putch(31);
    }
    textcolor(LIGHTGRAY);
    textbackground(BLACK);

// Edicao e criacao de itens
// -------------------------

    reprint=0;
    while(reprint==0) {
      if(num_itens>0)
	strcpy(item,q->item);
      else {
	gotoxy(2,3);
	cprintf("                         ");
	strcpy(item,"nenhum item");
      }
      j=Capc(item,2,pos,LIGHTGRAY,BLACK,19);
      switch(j) {
	case 9:
	  if(num_itens!=0) {
	    if(strcmp(item,q->item)!=0) {
	      strcpy(q->item,item);
	      q->mod_flag=1;
	    }
	  }
	  CriaItem();
	  num_itens++;
	  if(num_itens==1) {
	    q=top=i_list;
	    q=top;
	    item_atual=num_itens;
	  }
	  reprint=1;
	  break;
	case 77:
	case 80:
	  if(num_itens==0)
	    break;
	  if(strcmp(item,q->item)!=0) {
	    strcpy(q->item,item);
	    q->mod_flag=1;
	  }
	  if(item_atual==num_itens)
	    break;
	  pos+=2;
	  if(pos>17) {
	    pos=17;
	    top=top->proximo;
	    reprint=1;
	  }
	  item_atual++;
	  q=q->proximo;
	  break;
	case 72:
	case 75:
	  if(num_itens==0)
	    break;
	  if(strcmp(item,q->item)!=0) {
	    strcpy(q->item,item);
	    q->mod_flag=1;
	  }
	  if(item_atual==1 || (item_atual==0 && num_itens==0))
	    break;
	  pos-=2;
	  if(pos<3) {
	    pos=3;
	    top=top->anterior;
	    reprint=1;
	  }
	  item_atual--;
	  q=q->anterior;
	  break;
	case 13:
	  if(num_itens!=0) {
	    if(strcmp(item,q->item)!=0) {
	      strcpy(q->item,item);
	      q->mod_flag=1;
	    }
	    AjustaItensNosProdutos();
	    ListaProdutos(item_atual);
	  }
	  break;
	case 27:
	  if(num_itens!=0) {
	    if(strcmp(item,q->item)!=0) {
	      strcpy(q->item,item);
	      q->mod_flag=1;
	    }
	  }
	  reprint=sair=1;
	  break;
	default:
	  if(num_itens!=0) {
	    if(strcmp(item,q->item)!=0) {
	      strcpy(q->item,item);
	      q->mod_flag=1;
	    }
	  }
	  continue;
      }
    }
  }
  AjustaItensNosProdutos();
  CloseWindow();
  textattr(ti.attribute);
}


void CriaItem(void)
{

// FUNCAO: Cria um novo item na lista encadeada de itens.
// ------------------------------------------------------

  struct text_info ti;
  struct item_list *p,*q;
  char *item;

  gettextinfo(&ti);
  textcolor(BLACK);
  textbackground(LIGHTGRAY);
  Window(13,9,67,13);
  cprintf("Entre com o nome do novo item:");
  window(20,12,50,12);
  textcolor(WHITE);
  textbackground(BLACK);
  clrscr();
  item=(char *) in_str(1,1,19,'s');
  if(i_list==NULL) {
    i_list=(struct item_list*) calloc(1,sizeof(struct item_list));
    p=i_list;
    p->anterior=NULL;
    strcpy(p->item,item);
    p->mod_flag=0;
    p->p_list=NULL;
    p->proximo=NULL;
  }
  else {
    p=i_list;
    while(p->proximo!=NULL)
      p=p->proximo;
    q=(struct item_list*) calloc(1,sizeof(struct item_list));
    p->proximo=q;
    q->anterior=p;
    strcpy(q->item,item);
    q->mod_flag=0;
    q->p_list=NULL;
    q->proximo=NULL;
  }
  CloseWindow();
  textattr(ti.attribute);
}

void AjustaItensNosProdutos(void)
{

// Percorre listas de produtos, cujos nomes de item foram alterados, atuali-
// zando-as.
// -------------------------------------------------------------------------

  struct item_list *s;
  struct product_list *p;

  s=i_list;
  if(s!=NULL) {
    while(1) {
      if(s->mod_flag) {
	if(s->p_list!=NULL) {
	  p=s->p_list;
	  while(1) {
	    strcpy(p->item_prod,s->item);
	    if(p->proximo==NULL)
	      break;
	    p=p->proximo;
	  }
	}
      }
      if(s->proximo==NULL)
	break;
      s=s->proximo;
    }
  }
}

void ListaProdutos(int num_item)
{

// FUNCAO: Lista os produtos de um item na tela para serem editados, cria
// novos produtos.
// ----------------------------------------------------------------------

  struct text_info ti;
  struct item_list *s;
  struct product_list *p,*q,*top;
  unsigned char string[20];
  int i,j,x=14,y=3,num_produtos=0,produto_atual,sair=0,reprint;

// Procura item desejado
// ---------------------

  s=i_list;
  i=1;
  while(i<num_item) {
    s=s->proximo;
    i++;
  }

// Formata a tela
// --------------

  gettextinfo(&ti);
  textcolor(BLACK);
  textbackground(CYAN);
  Window(5,2,80,24);
  Quadro(1,1,70,18);
  Quadro(71,1,73,18);
  gotoxy(1,1);
  cprintf("ITEM: %s",s->item);
  gotoxy(1,21);
  cprintf("Setas  e <Tab> navegam, <Enter> visualiza a info do produto");
  gotoxy(20,20);
  cprintf("[ Atualiza ]");
  gotoxy(40,20);
  cprintf("[ Novo prod. ]");
  gotoxy(2,2);
  cprintf("Referencia  Nome                      Quantidade     Modificado");
  textcolor(LIGHTGRAY);
  textbackground(BLACK);
  for(i=3;i<18;i+=2) {
    gotoxy(14,i);
    cprintf("                    ");
    gotoxy(40,i);
    cprintf("      ");
  }

// Verifica se lista de produtos esta vazia, se nao estiver mostra produtos
// na tela, permite inclusao de novos produtos
// ------------------------------------------------------------------------

  top=p=s->p_list;
  if(s->p_list!=NULL) {
    produto_atual=num_produtos=1;
    while(p->proximo!=NULL) {
      num_produtos++;
      p=p->proximo;
    }
  }
  else
    produto_atual=0;
  q=top;
  while(sair==0) {

// Imprime lista de produtos na tela
// ---------------------------------

    i=3;
    p=top;
    if(num_produtos==1) {
      textcolor(BLACK);
      textbackground(CYAN);
      gotoxy(2,i);
      cputs("          \r");
      gotoxy(2,i);
      cprintf("%lu",p->ref_prod);
      if(p->mod_flag) {
	gotoxy(57,i);
	cprintf("*");
      }
      else {
	gotoxy(57,i);
	cprintf(" ");
      }
      textcolor(LIGHTGRAY);
      textbackground(BLACK);
      gotoxy(14,i);
      cputs("                    \r");
      gotoxy(14,i);
      cputs(p->nome_prod);
      gotoxy(40,i);
      cputs("      \r");
      gotoxy(40,i);
      cprintf("%u",p->qtde_prod);
    }
    if(num_produtos>1) {
      while(i<18) {
	textcolor(BLACK);
	textbackground(CYAN);
	gotoxy(2,i);
	cputs("          \r");
	gotoxy(2,i);
	cprintf("%lu",p->ref_prod);
	if(p->mod_flag) {
	  gotoxy(57,i);
	  cprintf("*");
	}
	else {
	  gotoxy(57,i);
	  cprintf(" ");
       }
	textcolor(LIGHTGRAY);
	textbackground(BLACK);
	gotoxy(14,i);
	cputs("                    \r");
	gotoxy(14,i);
	cputs(p->nome_prod);
	gotoxy(40,i);
	cputs("      \r");
	gotoxy(40,i);
	cprintf("%u",p->qtde_prod);
	if(p->proximo==NULL)
	  break;
	i+=2;
	p=p->proximo;
      }
    }

// Imprime setinhas no lado direito da tela
// ----------------------------------------

    textcolor(WHITE);
    textbackground(CYAN);
    if(num_produtos==0) {
      p->proximo=NULL;
      top->anterior=NULL;
    }
    if(p->proximo==NULL && i>17)
      p=p->anterior;
    if(p->proximo==NULL && top->anterior==NULL) {
      gotoxy(72,2);
      putch(' ');
      gotoxy(72,17);
      putch(' ');
    }
    else if(p->proximo!=NULL && top->anterior==NULL) {
      gotoxy(72,2);
      putch(' ');
      gotoxy(72,17);
      putch(31);
    }
    else if(p->proximo==NULL && top->anterior!=NULL) {
      gotoxy(72,2);
      putch(30);
      gotoxy(72,17);
      putch(' ');
    }
    else if(p->proximo!=NULL && top->anterior!=NULL) {
      gotoxy(72,2);
      putch(30);
      gotoxy(72,17);
      putch(31);
    }
    textcolor(LIGHTGRAY);
    textbackground(BLACK);

// Edicao e criacao de produtos
// -----------------------------

    reprint=0;
    while(reprint==0) {
      if(num_produtos>0)
	if(x==14)
	  strcpy(string,q->nome_prod);
	else
	  itoa(q->qtde_prod,string,10);
      else {
	x=14;
	gotoxy(14,3);
	cputs("                    \r");
	strcpy(string,"nenhum produto");
      }
      j=Capc(string,x,y,LIGHTGRAY,BLACK,19);
      switch(j) {
	case 9:
	  if(num_produtos!=0) {
	    if(strcmp(string,q->nome_prod)!=0 && x==14) {
	      strcpy(q->nome_prod,string);
	      q->mod_flag=1;
	    }
	    if(q->qtde_prod!=atoi(string) && x==40) {
	      q->qtde_prod=atoi(string);
	      q->mod_flag=1;
	    }
	    if(q->mod_flag) {
	      textcolor(BLACK);
	      textbackground(CYAN);
	      gotoxy(57,y);
	      cprintf("*");
	      textcolor(LIGHTGRAY);
	      textbackground(BLACK);
	    }
	  }
	  if(Botao(22,20,BLACK,CYAN,"Atualiza")) {
	    if(num_produtos!=0)
	      AtualizaCentral(s->p_list);
	    reprint=1;
	  }
	  else if(Botao(42,20,BLACK,CYAN,"Novo prod.")) {
	    if(CriaProduto(num_item))
	      num_produtos++;
	    if(num_produtos==1) {
	      q=top=s->p_list;
	      q=top;
	      produto_atual=num_produtos;
	    }
	    reprint=1;
	  }
	  textcolor(LIGHTGRAY);
	  textbackground(BLACK);
	  break;
	case 77:
	  if(num_produtos!=0) {
	    if(strcmp(string,q->nome_prod)!=0 && x==14) {
	      strcpy(q->nome_prod,string);
	      q->mod_flag=1;
	    }
	    if(q->qtde_prod!=atoi(string) && x==40) {
	      q->qtde_prod=atoi(string);
	      q->mod_flag=1;
	    }
	    if(q->mod_flag) {
	      textcolor(BLACK);
	      textbackground(CYAN);
	      gotoxy(57,y);
	      cprintf("*");
	      textcolor(LIGHTGRAY);
	      textbackground(BLACK);
	    }
	  }
	  if(x==14)
	    x=40;
	  break;
	case 80:
	  if(num_produtos==0)
	    break;
	  if(strcmp(string,q->nome_prod)!=0 && x==14) {
	    strcpy(q->nome_prod,string);
	    q->mod_flag=1;
	  }
	  if(q->qtde_prod!=atoi(string) && x==40) {
	    q->qtde_prod=atoi(string);
	    q->mod_flag=1;
	  }
	  if(q->mod_flag) {
	    textcolor(BLACK);
	    textbackground(CYAN);
	    gotoxy(57,y);
	    cprintf("*");
	    textcolor(LIGHTGRAY);
	    textbackground(BLACK);
	  }
	  if(produto_atual==num_produtos)
	    break;
	  y+=2;
	  if(y>17) {
	    y=17;
	    top=top->proximo;
	    reprint=1;
	  }
	  produto_atual++;
	  q=q->proximo;
	  break;
	case 75:
	  if(num_produtos!=0) {
	    if(strcmp(string,q->nome_prod)!=0 && x==14) {
	      strcpy(q->nome_prod,string);
	      q->mod_flag=1;
	    }
	    if(q->qtde_prod!=atoi(string) && x==40) {
	      q->qtde_prod=atoi(string);
	      q->mod_flag=1;
	    }
	    if(q->mod_flag) {
	      textcolor(BLACK);
	      textbackground(CYAN);
	      gotoxy(57,y);
	      cprintf("*");
	      textcolor(LIGHTGRAY);
	      textbackground(BLACK);
	    }
	  }
	  if(x==40)
	    x=14;
	  break;
	case 72:
	  if(num_produtos==0)
	    break;
	  if(strcmp(string,q->nome_prod)!=0 && x==14) {
	    strcpy(q->nome_prod,string);
	    q->mod_flag=1;
	  }
	  if(q->qtde_prod!=atoi(string) && x==40) {
	    q->qtde_prod=atoi(string);
	    q->mod_flag=1;
	  }
	  if(q->mod_flag) {
	    textcolor(BLACK);
	    textbackground(CYAN);
	    gotoxy(57,y);
	    cprintf("*");
	    textcolor(LIGHTGRAY);
	    textbackground(BLACK);
	  }
	  if(produto_atual==1 || (produto_atual==0 && num_produtos==0))
	    break;
	  y-=2;
	  if(y<3) {
	    y=3;
	    top=top->anterior;
	    reprint=1;
	  }
	  produto_atual--;
	  q=q->anterior;
	  break;
	case 13:
	  if(num_produtos!=0) {
	    if(strcmp(string,q->nome_prod)!=0 && x==14) {
	      strcpy(q->nome_prod,string);
	      q->mod_flag=1;
	    }
	    if(q->qtde_prod!=atoi(string) && x==40) {
	      q->qtde_prod=atoi(string);
	      q->mod_flag=1;
	    }
	    if(q->mod_flag) {
	      textcolor(BLACK);
	      textbackground(CYAN);
	      gotoxy(57,y);
	      cprintf("*");
	      textcolor(LIGHTGRAY);
	      textbackground(BLACK);
	    }
	    EditaInfoProd(q);
	    reprint=1;
	  }
	  break;
	case 27:
	  if(num_produtos!=0) {
	    if(strcmp(string,q->nome_prod)!=0 && x==14) {
	      strcpy(q->nome_prod,string);
	      q->mod_flag=1;
	    }
	    if(q->qtde_prod!=atoi(string) && x==40) {
	      q->qtde_prod=atoi(string);
	      q->mod_flag=1;
	    }
	    if(q->mod_flag) {
	      textcolor(BLACK);
	      textbackground(CYAN);
	      gotoxy(57,y);
	      cprintf("*");
	      textcolor(LIGHTGRAY);
	      textbackground(BLACK);
	    }
	  }
	  reprint=sair=1;
	  break;
	default:
	  if(num_produtos!=0) {
	    if(strcmp(string,q->nome_prod)!=0 && x==14) {
	      strcpy(q->nome_prod,string);
	      q->mod_flag=1;
	    }
	    if(q->qtde_prod!=atoi(string) && x==40) {
	      q->qtde_prod=atoi(string);
	      q->mod_flag=1;
	    }
	    if(q->mod_flag) {
	      textcolor(BLACK);
	      textbackground(CYAN);
	      gotoxy(57,y);
	      cprintf("*");
	      textcolor(LIGHTGRAY);
	      textbackground(BLACK);
	    }
	  }
	  continue;
      }
    }
  }
  CloseWindow();
  textattr(ti.attribute);
}

void AtualizaCentral(struct product_list *q) {

// Atualiza produtos modificados de um determinado item na central.
// ----------------------------------------------------------------

  mensagem pacote;

  while(1) {
    if(q->mod_flag==1) {
      q->mod_flag=0;
      pacote.tipo=2;
      if(strlen(q->nome_prod)>0)
	pacote.operacao=AT;
      else
	pacote.operacao=DEL;
      pacote.ref_prod=q->ref_prod;
      strcpy(pacote.item_prod,q->item_prod);
      strcpy(pacote.nome_prod,q->nome_prod);
      pacote.qtde_prod=q->qtde_prod;
      strcpy(pacote.info_prod,q->info_prod);
      EnviaPacoteAT_DEL_REQ(pacote,0x4547);
    }
    if(q->proximo==NULL)
      break;
    q=q->proximo;
  }
}

int CriaProduto(int num_item)
{

// FUNCAO: Cria um novo produto na lista encadeada de produtos.
// ------------------------------------------------------------

  struct text_info ti;
  struct item_list *p;
  struct product_list *r,*s;
  char str[2][30],*string;
  int i;

  gettextinfo(&ti);
  textcolor(BLACK);
  textbackground(BLUE);
  str[0][0]=0;
  str[1][0]=0;
  Window(10,6,70,14);
  cprintf("Criacao do produto:");
  gotoxy(1,2);
  cprintf("Referencia do produto:");
  gotoxy(1,4);
  cprintf("Nome do produto:");
  textcolor(WHITE);
  textbackground(BLACK);
  for(i=1;i<59;i++) {
    gotoxy(i,3);
    cprintf(" ");
    gotoxy(i,5);
    cprintf(" ");
  }
  while(1) {
    gotoxy(1,3);
    cprintf("               ");
    string=(char *) in_str(1,3,9,'s');
    strcpy(str[0],string);
    if(str[0][0]==0) {
      Mensagem("Nenhuma referencia foi entrada.");
      continue;
    }
    for(i=0;i<10;i++) {
      if(str[0][i]==0)
	break;
      if((str[0][i]<48 || str[0][i]>57) && str[0][i]>0) {
	Mensagem("O campo referencia nao apresenta valores numericos.");
	 i=99;
	 break;
      }
      if((unsigned long) atol(str[0]) > ref_final ||
	 (unsigned long) atol(str[0]) < ref_inicial) {
	Mensagem("Referencia fora dos limites.");
	i=99;
	break;
      }
    }
    if(i==99)
      continue;
    else
      break;
  }
  while(1) {
    gotoxy(1,5);
    cprintf("                         ");
    string=(char *) in_str(1,5,19,'s');
    strcpy(str[1],string);
    if(str[1][0]==0) {
      Mensagem("Nenhuma nome de produto foi entrado.");
      continue;
    }
    else
      break;
  }

// Verifica se a referencia entrada ja existe na lista
// ---------------------------------------------------

  p=i_list;
  while(1) {
    if(p->p_list!=NULL) {
      r=p->p_list;
      while(1) {
	if(atol(str[0])==r->ref_prod) {
	  Mensagem("Produto com esta referencia ja existe");
	  CloseWindow();
	  textattr(ti.attribute);
	  return 0;
	}
	if(r->proximo==NULL)
	  break;
	r=r->proximo;
      }
    }
    if(p->proximo==NULL)
      break;
    p=p->proximo;
  }

// Atualiza produto na lista encadeada
// -----------------------------------

  i=1;
  p=i_list;
  if(p!=NULL)
    while(i<num_item) {
      p=p->proximo;
      i++;
    }
  if(p->p_list==NULL) {
    r=(struct product_list*) calloc(1,sizeof(struct product_list));
    p->p_list=r;
    r->anterior=NULL;
    r->ref_prod=atol(str[0]);
    strcpy(r->nome_prod,str[1]);
    strcpy(r->item_prod,p->item);
    r->qtde_prod=0;
    strcpy(r->info_prod,"");
    r->mod_flag=1;
    r->proximo=NULL;
  }
  else {
    r=p->p_list;
    while(r->proximo!=NULL)
      r=r->proximo;
    s=(struct product_list*) calloc(1,sizeof(struct product_list));
    r->proximo=s;
    s->anterior=r;
    s->ref_prod=atol(str[0]);
    strcpy(s->nome_prod,str[1]);
    strcpy(s->item_prod,p->item);
    s->qtde_prod=0;
    strcpy(s->info_prod,"");
    s->mod_flag=1;
    s->proximo=NULL;
  }
  CloseWindow();
  textattr(ti.attribute);
  return 1;
}

void EditaInfoProd(struct product_list *q)
{

// Edita informacoes referentes a um produto (info_prod).
// ------------------------------------------------------

  struct text_info ti;
  char info[255];
  int sair=0,i;

  gettextinfo(&ti);
  textcolor(BLACK);
  textbackground(BLUE);
  Window(10,6,70,14);
  strcpy(info,q->info_prod);
  cprintf("Informacoes sobre o produto %s :",q->nome_prod);
  while(sair==0) {
    i=Capc(info,1,2,BLACK,BLUE,254);
    switch(i) {
      case 13:
	if(strcmp(q->info_prod,info)!=0) {
	  strcpy(q->info_prod,info);
	  q->mod_flag=1;
	}
	sair=1;
	break;
      case 27:
	sair=1;
	break;
      default:
	continue;
    }
  }
  CloseWindow();
  textattr(ti.attribute);
}

void CarregaConfiguracao(void)
{

// FUNCAO: Verifica se o terminal esta configurado e carrega configuracao.
// -----------------------------------------------------------------------

  char ter[]="terminal.cfg",ref[11];
  FILE *tercfg;
  int i;

// Abre o arquivo TERMINAL.CFG
// ---------------------------

  while(1) {
    if((tercfg=fopen("terminal.cfg","rb"))==NULL) {
      if(CaixaDeDialogo("O arquivo TERMINAL.CFG nao foi encontrado ou nao\r\n\
pode ser aberto.\r\nDeseja criar novo arquivo de configuracao?\r\n\r\nOBS: Sera ne\
cessaria a senha de configuracao."))
	EditaConfiguracao();
      else
	Exit();
    }
    else
      break;
  }
// Verifica se o arquivo de configuracao e valido
// ----------------------------------------------

  rewind(tercfg);
  fread(senha,1,12,tercfg);
  for(i=0;i<12;i++)
    senha[i]=senha[i]+25;
  senha[12]=0;
  if(strcmp(senha,ter)==0) {
    fread(senha,1,30,tercfg);
    for(i=0;i<30;i++)
      senha[i]=senha[i]+25;
    senha[30]=0;
//
    fread(ref,1,10,tercfg);
    for(i=0;i<10;i++)
      ref[i]=ref[i]+25;
    ref[10]=0;
    ref_inicial=atol(ref);
//
    fread(ref,1,10,tercfg);
    for(i=0;i<10;i++)
      ref[i]=ref[i]+25;
    ref[10]=0;
    ref_final=atol(ref);
//
    fread(departamento,1,77,tercfg);
    for(i=0;i<77;i++)
      departamento[i]=departamento[i]+25;
    departamento[77]=0;
  }
  else {
      if(CaixaDeDialogo("O arquivo TERMINAL.CFG encontrado nao e valido.\r\n\
Deseja criar novo arquivo de configuracao?\r\n\r\nOBS: Sera ne\
cessaria a senha de configuracao."))
	EditaConfiguracao();
      else
	Exit();
  }
  fclose(tercfg);
}

void EditaConfiguracao(void)
{

// FUNCAO: Edita o arquivo TERMINAL.CFG, que determina a configuracao do
// TERMINAL.
// ---------------------------------------------------------------------

  FILE *tercfg;
  struct text_info ti;
  char str[4][80],ter[]="terminal.cfg";
  int i,j,y,max,sair=0;

  gettextinfo(&ti);
  textcolor(BLACK);
  textbackground(CYAN);
  Window(13,9,67,13);

// Verifica senha de configuracao
// ------------------------------

  cprintf("Entre com a senha de configuracao de terminal:");
  while(1) {
    window(20,12,30,12);
    textcolor(WHITE);
    textbackground(BLACK);
    clrscr();
    if(CmpPassword(atol((char *)in_str(1,1,10,'h'))))
      break;
    else {
      if(CaixaDeDialogo("Senha de configuracao incorreta.\r\nTentar novamente?"))
	continue;
      else {
	CloseWindow();
	textattr(ti.attribute);
	return;
      }
    }
  }
  CloseWindow();

// Abre arquivo de configuiracao, se nao existir, e criado
// -------------------------------------------------------

  if((tercfg=fopen("terminal.cfg","rb"))==NULL)
    if((tercfg=fopen("terminal.cfg","wb"))==NULL) {
      Mensagem("Arquivo de configuracao nao pode ser criado/editado.");
      Exit();
    }

// Verifica se ja havia dados no arquivo, ou se e arquivo novo
// -----------------------------------------------------------

  rewind(tercfg);
  fread(str[0],1,12,tercfg);
  for(i=0;i<12;i++)
    str[0][i]=str[0][i]+25;
  str[0][12]=0;

// Se ja existia o arquivo, seus dados sao carregados
// --------------------------------------------------

  if(strcmp(str[0],ter)==0) {
    for(i=0;i<4;i++)
      setmem(str[i],80,0);
    fread(str[0],1,30,tercfg);
    for(i=0;i<30;i++)
      str[0][i]=str[0][i]+25;
//
    fread(str[1],1,10,tercfg);
    for(i=0;i<10;i++)
      str[1][i]=str[1][i]+25;
//
    fread(str[2],1,10,tercfg);
    for(i=0;i<10;i++)
      str[2][i]=str[2][i]+25;
//
    fread(str[3],1,77,tercfg);
    for(i=0;i<77;i++)
      str[3][i]=str[3][i]+25;
  }
  else
    for(i=0;i<4;i++)
      setmem(str[i],80,0);
  fclose(tercfg);

// Edicao da configuracao
// ----------------------

  textcolor(BLACK);
  textbackground(CYAN);
  Window(1,6,80,21);
  cprintf("EDICAO DE TERMINAL.CFG:");
  gotoxy(1,2);
  cprintf("Senha de operacao:");
  gotoxy(1,4);
  cprintf("Referencia inicial:");
  gotoxy(1,6);
  cprintf("Referencia Final:");
  gotoxy(1,8);
  cprintf("Nome do departamento:");
  gotoxy(35,12);
  cprintf("[ Ok! ]");
  gotoxy(1,14);
  cprintf("Setas, <Enter> e <Tab> navegam, <Esc> cancela.");
  textcolor(WHITE);
  textbackground(BLACK);
  for(i=1;i<79;i++) {
    gotoxy(i,3);
    cprintf(" ");
    gotoxy(i,5);
    cprintf(" ");
    gotoxy(i,7);
    cprintf(" ");
    gotoxy(i,9);
    cprintf(" ");
  }
  gotoxy(1,3);
  cputs(str[0]);
  gotoxy(1,5);
  cputs(str[1]);
  gotoxy(1,7);
  cputs(str[2]);
  gotoxy(1,9);
  cputs(str[3]);
  j=0;
  y=3;
  max=30;
  while(sair==0) {
    i=Capc(str[j],1,y,WHITE,BLACK,max);
    switch(i) {
      case 13:
      case 77:
      case 80:
	y+=2;
	if(y>9) y=3;
	j++;
	if(j>3) j=0;
	if(j==0) max=30;
	if(j==1 || j==2) max=10;
	else max=77;
	break;
      case 72:
      case 75:
	y-=2;
	if(y<3) y=9;
	j--;
	if(j<0) j=3;
	if(j==0) max=30;
	if(j==1 || j==2) max=10;
	else max=77;
	break;
      case 27:
	CloseWindow();
	textattr(ti.attribute);
	fclose(tercfg);
	return;
      case 9:
	if(Botao(37,12,BLACK,CYAN,"Ok!")) {
	  if(str[0][0]==0 || str[1][0]==0 || str[2][0]==0 || str[3][0]==0) {
	    Mensagem("Faltam dados a serem entrados.");
	    break;
	  }
	  for(i=0;i<10;i++) {
	    if(str[1][i]==0)
	      break;
	    if((str[1][i]<48 || str[1][i]>57) && str[1][i]>0) {
	      Mensagem("O campo Referencia inicial nao apresenta valores\r\nnumericos.");
	      i=99;
	      break;
	    }
	    if(str[1][i]==0)
	      break;
	    if((str[2][i]<48 || str[2][i]>57) && str[2][i]>0) {
	      Mensagem("O campo Referencia final nao apresenta valores\r\nnumericos.");
	      i=99;
	      break;
	    }
	  }
	  if(i==99)
	    break;
	  if((unsigned long) atol(str[1]) > (unsigned long) atol(str[2])) {
		    Mensagem("Referencia inicial maior que a final ou Referencia\r\nfinal fora do limite.");
	    break;
	  }
	  sair=1;
	  break;
	}
      default:
	continue;
    }
  }

// Salva os dados coletados em disco
// ---------------------------------

  if((tercfg=fopen("terminal.cfg","wb"))==NULL) {
    Mensagem("Impossivel salvar configuracao.");
    Exit();
  }
  rewind(tercfg);
  for(i=0;i<12;i++)
    ter[i]=ter[i]-25;
  fwrite(ter,1,12,tercfg);
//
  for(i=0;i<30;i++)
    str[0][i]=str[0][i]-25;
  fwrite(str[0],1,30,tercfg);
//
  for(i=0;i<10;i++)
    str[1][i]=str[1][i]-25;
  fwrite(str[1],1,10,tercfg);
//
  for(i=0;i<10;i++)
    str[2][i]=str[2][i]-25;
  fwrite(str[2],1,10,tercfg);
//
  for(i=0;i<77;i++)
    str[3][i]=str[3][i]-25;
  fwrite(str[3],1,77,tercfg);

  CloseWindow();
  textattr(ti.attribute);
  fclose(tercfg);
}

void IndiceAjuda(void)
{

// FUNCAO: Menu que contem o indice da ajuda.
// ------------------------------------------

  char *indice[]={"Edicao de Itens e Produtos","Configuracao do Terminal",
		  "Sobre o Programa","##"};
  int esc;
  menu_info *menu;
  struct text_info ti;

  gettextinfo(&ti);
  textcolor(BLACK);
  textbackground(LIGHTGRAY);
  menu=(menu_info *)malloc(sizeof(menu_info));
  menu->opc=0;
  menu->tipo=POPUP;
  menu->estado=CLOSE;
  menu->col_in=29;
  menu->lin_in=16;
  menu->cor_frente=BLACK;
  menu->cor_fundo=LIGHTGRAY;
  esc=PullDown(menu,indice);
  switch(esc) {
    case ESC:
      CloseMenu(menu);
      free(menu);
      textattr(ti.attribute);
      return;
    case 0:
      CloseMenu(menu);
      Rolagem("ter1.hlp");
      free(menu);
      textattr(ti.attribute);
      return;
    case 1:
      CloseMenu(menu);
      free(menu);
      Rolagem("ter2.hlp");
      textattr(ti.attribute);
      return;
    case 2:
      CloseMenu(menu);
      Rolagem("ter3.hlp");
      free(menu);
      textattr(ti.attribute);
      return;
    default:
      CloseMenu(menu);
      free(menu);
      textattr(ti.attribute);
      return;
  }
}