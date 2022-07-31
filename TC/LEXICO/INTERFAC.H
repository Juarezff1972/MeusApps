#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <a:\09.09\parsing.h>

#define FALSE        0
#define TRUE         1
#define ERRO        -1

#define MOUSE        0x33
#define BACKSPACE    0x08
#define ENTER        13
#define ESC          27
#define TAB          9
#define SHIFT_TAB    15

#define TMOUSE       1     /* indica evento realizado pelo mouse */
#define TTECLADO     2     /* indica evento realizado pelo teclado*/

#define SIMPLES      1
#define BORDA        2

#define LIGHT        10

#define ALT_D        32       /* CODIGO DAS TECLAS */
#define ALT_F        33
#define ALT_A        30
#define ALT_M        50
#define DEL          83

#define F1           59
#define F2           60
#define F3           61
#define F4           62
#define F5           63
#define F6           64
#define F7           65
#define F8           66
#define F9           67
#define F10          68
#define PARA_CIMA    72       /* CODIGO DAS TECLAS */
#define PARA_BAIXO   80
#define DIREITA      77
#define ESQUERDA     75

#define NADA         57

#define LINHA_INICIAL_MENU 31         /* Linha inicial do menu principal*/
#define LINHA_FINAL_MENU   49         /* Linha inicial do menu principal*/
#define TAMANHO_SUB_MENU   15
#define INICIAL_SUB_MENU   48         /* Linha inicial do sub menu */
#define COLUNA_INICIAL      7         /* coluna inicial do menu principal */
#define JANELAX            200
#define JANELAY            100

#define MENU_PRINCIPAL   1            /* opcoes onde pode ocorrer eventos */
#define SUB_MENU         2
#define DEFAULT          3

#define NAO_BOTAO        1            /* opcoes de campos das janelas */
#define BOTAO				 2
#define BOTAO_DEFAULT    3


#define MAIS_NT                   111    /* codigos dos campos das janelas */
#define MAIS_PRODUCAO             112
#define OK                        113
#define CANCEL                    114
#define ENTRADA_NT                115
#define ENTRADA_PRODUCAO          116

#define  MENU_GRAMATICA           100
#define  SUB_GRAMATICA_CRIAR      110
#define  SUB_GRAMATICA_CARREGAR   120
#define  SUB_GRAMATICA_SALVAR     130

#define  MENU_TEXTO               200
#define  SUB_TEXTO_CRIAR          210
#define  SUB_TEXTO_CARREGAR       220
#define  SUB_TEXTO_SALVAR         230

#define  MENU_EXECUTAR            300
#define  SUB_EXECUTAR_RODAR       310
#define  SUB_EXECUTAR_PASSO       320

#define  MENU_OPCOES              400
#define  SUB_OPCOES_COR           410
#define  SUB_OPCOES_SAIR          420



typedef struct no {                   /* estrutura para os botoes  */
	  int x1;
	  int y1;                            /* tamanho dos botoes      */
	  int x2;
	  int y2;
	  int indicacao;                /* para indicar qual e o tipo de campo */
     char *dado;                   /* para conter o ponteiro para a string,
                                      se  o campo for NAO_BOTAO   */
     int max_tam;                  /* para conter o numero maximo de cacracteres
                                     da string,se  o campo for NAO_BOTAO   */
     int codigo;
	  char *nome;
	  struct no *prox;
	  struct no *ant;
} lista_botao;

typedef struct _sub_menu {   /* estrutura dos menus secundarios */
	int linha_inicial ,linha_final; /* linha inicial e final de cada opcao
						                    do sub menu */
	int codigo;                     /* codigo do sub menu */
	char *mensagem,                 /* mensagem a ser mostrada em baixo */
		  *nome;                     /* Nome da opcao do sub menu */
	struct _sub_menu *sub_prox;     /* ponteiro p/ a proxima opcao do
						                    sub menu */
	struct _sub_menu *sub_ant;      /* ponteiro p/ a opcao anterior do
						                    sub menu */
	struct no *botoes;              /* ponteiro p/ as opcoes da opcao */
   int janelax,janelay;            /* tamanho da janela da opcao */
   char *nome_janela;
}sub_menu;     /* sub menu Lista encadeada Circular */

typedef struct _menu {     /* estrutura do menu principal */
	int   coluna_inicial,coluna_final; /* coluna inicial e final de
							                    cada opcao do sub menu */
		  int   coluna_sub_menu, /* coluna final do sub menu */
				  linha_sub_menu;  /* linha final do sub menu */
	int   codigo;          /* codigo da opcao do menu */
	char  *mensagem,       /* mensagem a ser mostrada em baixo */
			*nome;           /* nome da opcao do menu */
	struct _menu *men_prox; /* ponteriro para o proximo opcao do menu */
	struct _menu *men_ant;  /* ponteriro para a opcao anterior do menu */
	struct _sub_menu *sub_menu_prox; /* ponteiro para o sub menu */
}menu_principal;  /* menu Lista encadeada Circular */

menu_principal *primeiro_menu,  /* Aponta para a lista do menu principal */
					*menu_primario,  /* Ponteiro que aponta p/ a opcao selecionada
					                    do menu principal */
					*menu_primario_anterior; /* ponteiro que aponta para a opcao antiga */
sub_menu       *menu_secundario,/* Ponteiro que aponta p/ a opcao selecionada
											  do sub menu  */
					*menu_secundario_anterior,
               *opcao_ativa,   /* Ponteiro que aponta p/ a opcao do sub menu
                                  escolhida */
               *mensag_erro,   /* Ponteiro que aponta p/ a mensagem de erro */
               *quarda_opcao_ativa;  /* Ponteiro que quarda a opcao_ativa ate
                                        que a mensagem de erro seja tratada */
lista_botao    *campo_escolhido, /*ponteiro que aponta qual o campo da opcao
                                   esclhida */
               *campo_default;   /*ponteiro que aponta para o campo da opcao
                                   default */

int bot_esq, bot_dir, bot_meio, posicaox, posicaoy;  /* variaveis usadas
																		  pelo MOUSE      */
int tecla1,tecla2;  /* variaveis usadas para armazenar a tecla pressionada */
int leve , normal , forte,
	 borda , contorno , fundo_menu, fundo_tela, menu_acionado;
int evento,        /* variavel que indica o evento ocorrido (MOUSE/TECLADO) */
	 onde = DEFAULT,/* indica aonde que ocorreu o evento                     */
	 fim = FALSE;   /* indica quando o programa acbou */

char *mensagem_default;



/* ***********************************************************************
 * Nome da funcao : init_grafico
 * Funcao         : Inicializa o modo grafico, se tiver erro aborta.

 * ***********************************************************************/
void init_grafico(void)

{
    int driver = DETECT, mode, errorcode;
	initgraph(&driver, &mode, "D:\\BC\\BGI");
     errorcode = graphresult();
     if( errorcode != grOk )
	 {   printf( "Erro no modo grafico: %s",grapherrormsg( errorcode ) );
		  exit( 1 );
	  }
}

/* ***********************************************************************
 * Nome da funcao : init_mouse
 * Funcao         : Inicializa o mouse, se tiver erro aborta.

 * ***********************************************************************/
void init_mouse( void )
{
    union REGS regs;

	 regs.x.ax = 0x00;                 /* inicializa o mouse */
    int86(MOUSE, &regs, &regs);
    if (regs.x.ax == 0)
	 {   outtextxy(10,20, "Erro no mouse: " );
	     exit( 1 );
     }


	 regs.x.ax = 0x01;                  /* inicializa cursor do mouse */
	 int86(MOUSE, &regs, &regs);

    regs.x.ax = 0x07;                  /* define o alcance horizontal */
	 regs.x.cx = 0;
    regs.x.dx = 640;
    int86(MOUSE, &regs, &regs);

    regs.x.ax = 0x08;                  /* define o alcance vertical  */
    regs.x.cx = 0;
    regs.x.dx = 480;
	 int86(MOUSE, &regs, &regs);


}

/*** FUNCAO que liga o MOUSE *******/
void mouse_on( void )
{
    union REGS regs;
	 regs.x.ax = 0x01;
	 int86(MOUSE, &regs, &regs);
}

/*** FUNCAO que desliga o MOUSE *******/
void mouse_off( void )
{
    union REGS regs;
    regs.x.ax = 0x02;
    int86(MOUSE, &regs, &regs);
}

/* ********************************************************************* *
 * Nome da funcao : posiciona_mouse
 * Funcao         : posiciona o mouse na posicao desejada

 * ********************************************************************* */
void posiciona_mouse (int x , int y )
{
    union REGS regs;

	 regs.x.ax = 0x04;     /* posiciona o mouse */
	 regs.x.cx = x;
	 regs.x.dx = y;
    int86(MOUSE, &regs, &regs);
}

/* ***********************************************************************
 * Nome da funcao : status_mouse
 * Funcao         : Retorna a posicao e os status dos botoes

 * ***********************************************************************/
void status_mouse( void )
{
    union REGS regs;
    bot_esq  = 0;
    bot_dir  = 0;
    bot_meio = 0;
	 regs.x.ax = 0x03;
    int86(MOUSE, &regs, &regs);
    posicaox = regs.x.cx;
	 posicaoy = regs.x.dx;
	 if ( regs.x.bx & 1)
		 bot_esq  = 1;
    if ( regs.x.bx & 2)
       bot_dir  = 1;
	 if ( regs.x.bx & 4)
       bot_meio = 1;
}

void finaliza_mouse ( void )
{
    union REGS regs;
	 regs.x.ax = 0x02;
    int86(MOUSE, &regs, &regs);
	 closegraph();
}

/****** FUNCAO que pega a tecla pressionda, e armazena em tecla1 e tecla2 **/
void pega_tecla_pressionada ( void )
{
   tecla1 = getch();
   if (tecla1 == 0)
      tecla2 = getch();
   else
      tecla2 = 0;
}

void inicializa_mensagem_de_erro( void )
{
   lista_botao *aux_botao;

   mensag_erro = (sub_menu *)malloc(sizeof(sub_menu));
   mensag_erro->codigo = ERRO;
   mensag_erro->sub_prox = NULL;
   mensag_erro->sub_ant = NULL;
   mensag_erro->nome = NULL;
   mensag_erro->mensagem = NULL;
   mensag_erro->linha_inicial = 0;
   mensag_erro->linha_final = 0;
   mensag_erro->janelax = 0;
   mensag_erro->janelay = 150;
   mensag_erro->nome_janela = NULL;

   aux_botao = (lista_botao *)malloc(sizeof(lista_botao));
   mensag_erro->botoes = aux_botao;
   aux_botao->x2 = 70;
   aux_botao->y2 = 40;
   aux_botao->nome = malloc(3);
   strcpy(aux_botao->nome,"OK");
   aux_botao->codigo = OK;
   aux_botao->indicacao = BOTAO_DEFAULT;
   aux_botao->max_tam = 0;
   aux_botao->dado = NULL;
   aux_botao->prox = aux_botao;
   aux_botao->ant = aux_botao;

   aux_botao = (lista_botao *)malloc(sizeof(lista_botao));
   aux_botao->prox = mensag_erro->botoes;
   aux_botao->ant = mensag_erro->botoes->ant;
   mensag_erro->botoes->ant->prox = aux_botao;
   mensag_erro->botoes->ant = aux_botao;
   aux_botao->x2 = 70;
   aux_botao->y2 = 40;
   aux_botao->nome = malloc(6);
   strcpy(aux_botao->nome,"CANCEL");
   aux_botao->codigo = CANCEL;
   aux_botao->indicacao = BOTAO;
   aux_botao->max_tam = 0;
   aux_botao->dado = NULL;

}

void inicializa ( void )
{
	mensagem_default = malloc(strlen("F10 - Menu")+1);
	strcpy(mensagem_default,"F10 - Menu");
	menu_primario = NULL;
	menu_secundario = NULL;
	menu_primario_anterior = NULL;
	menu_secundario_anterior = NULL;
	primeiro_menu = NULL;
   opcao_ativa = NULL;
   quarda_opcao_ativa = NULL;
   inicializa_mensagem_de_erro();
}

void define_cor_tela ( int tipo )
{
	switch (tipo)
	{
		case LIGHT :
						break;
		default    :
						 borda         = YELLOW;
						 contorno      = BLACK;
						 fundo_menu    = CYAN;
						 menu_acionado = LIGHTGRAY;
						 fundo_tela    = WHITE;
	}
}

void def_color( int color )
{
 settextstyle(DEFAULT_FONT,0,1);

/* settextstyle(SANS_SERIF_FONT,0,1); */
 switch (color)
 {
  case GREEN:    leve   = LIGHTGREEN;
		 normal = GREEN;
		 forte  = DARKGRAY;
		 break;
  case RED:
		 LIGHTRED: leve   = LIGHTRED;
		 normal = RED;
		 forte  = DARKGRAY;
		 break;

  case MAGENTA:
  case LIGHTMAGENTA: leve   = LIGHTMAGENTA;
			  normal = MAGENTA;
			  forte  = RED;
			  break;

  case BLUE:
  case LIGHTBLUE:
  case CYAN: leve   = CYAN;
		  normal = LIGHTBLUE;
		  forte  = BLUE;
		  break;

  default:
		leve   = WHITE;
		normal = LIGHTGRAY;
		forte  = DARKGRAY;
 }
}

void tela_janela ( int x , int y , int h , int v, char *texto )
{
   mouse_off();
   def_color(fundo_menu);
	setfillstyle(SOLID_FILL,normal);  /* era fundo_menu */
	bar( x , y , x+h , y+v );
	setcolor( leve );
	line( x , y , x+h , y );
	line( x , y , x , y+v );
	line( x+h-6 , y+6 , x+h-6 , y+v-6 );
	line( x+6 , y+v-6 , x+h-6 , y+v-6 );
	setcolor( forte );
	line( x+h , y , x+h , y+v );
	line( x , y+v , x+h , y+v );
	line( x+6 , y+6 , x+h-6 , y+6 );
	line( x+6 , y+6 , x+6 , y+v-6 );
   setcolor(contorno);
   outtextxy((x+(h/2))-strlen(texto)*4,y+10,texto);
   mouse_on();
}

void tela( int x , int y , int h , int v ,int tela, int cor )
{
	  int i;

	  mouse_off();

	  def_color( cor );
	  setfillstyle( SOLID_FILL , normal );
	  switch ( tela )
	  {
			case SIMPLES :
							 bar( x , y , x+h , y+v );
							 setcolor( leve );
							 line( x , y , x+h , y );
							 line( x , y , x , y+v );
							 line( x+h-6 , y+6 , x+h-6 , y+v-6 );
							 line( x+6 , y+v-6 , x+h-6 , y+v-6 );
							 setcolor( forte );
							 line( x+h , y , x+h , y+v );
							 line( x , y+v , x+h , y+v );
							 line( x+6 , y+6 , x+h-6 , y+6 );
							 line( x+6 , y+6 , x+6 , y+v-6 );

							 break;
         case BORDA   :
							 bar( x , y , x + h , y + v );
                      for( i=0 ; i<6 ; i++ )
                      {   setcolor( leve );
								  line( x+i , y+i , x+ h-i , y+i );
								  line( x+i , y+i , x+i , y + v-i );
	                       setcolor( forte );
	                       line( x + h-i , y+i , x + h-i , y+ v-i );
								  line( x +i , y+ v-i , x + h-i , y+ v-i );
							  }
                       break;
     }

     mouse_on();
}



void saida(int x, int y, char *pal )
{
    int tam, aux;

    mouse_off();

	 tam = strlen(pal)*8 + 20 ;
	 tela(x,y,tam,40,SIMPLES,20);
    aux = getcolor();
    setcolor(BLACK);
	 outtextxy(x+10, y+15, pal);
	 setcolor(aux);

    mouse_on();
}

void limpa( int x, int y, int x1, int y1, int cor)
{
	int aux;

	mouse_off();

   aux = getcolor();
	setfillstyle( SOLID_FILL , cor );
   bar(x,y,x1,y1);
   setcolor(aux);

	mouse_on();
}

void mostra_mensagem (char *mensagem)
{
	setfillstyle(SOLID_FILL,fundo_tela);
	bar(79,459,634,474);
	outtextxy(80,464,mensagem);
}
void desenha_tela_fundo ( void )
{
   mouse_off();
	setfillstyle(SOLID_FILL,fundo_tela);
	bar(7,50,634,452);
   mouse_on();
}

void nome_prog ( char *name )
{
   mouse_off();

   setfillstyle(SOLID_FILL,fundo_tela);
	bar(7,7,634,25);
   setfillstyle(SOLID_FILL,borda);
   bar(4,25,638,30);
	setcolor(contorno);
	line(7,25,635,25);
	line(7,30,635,30);

	outtextxy(320-(strlen(name))*5,12,name);

	mouse_on();
}
/***************************************************************************
 Funcao monta_menu
 Objetivo : motar o fundo do menu principal.
***************************************************************************/
void monta_menu ( void )
{
   menu_principal *menu_aux;

   menu_aux = primeiro_menu;
	setfillstyle(SOLID_FILL,fundo_menu);
   bar(7,LINHA_INICIAL_MENU,634,LINHA_FINAL_MENU);
   setcolor(contorno);
	line(7,LINHA_FINAL_MENU,634,LINHA_FINAL_MENU);
	do
	{
      outtextxy(menu_aux->coluna_inicial+5,35,menu_aux->nome);
	   menu_aux = menu_aux->men_prox;
   }while(menu_aux != primeiro_menu );
}

/***************************************************************************
 Funcao tela_fundo
 Objetvo : Construir a tela de fundo (bordas, etc..)
***************************************************************************/
void tela_fundo ( int tipo, char* texto )
{

	mouse_off();

	define_cor_tela (tipo);

	setfillstyle(SOLID_FILL,fundo_tela);
	bar(1,1,640,480);

	setfillstyle(SOLID_FILL,borda);   /* constroi as bordas da tela */
	bar(1,1,640,6);
	bar(1,1,6,480);
	bar(1,475,640,480);
	bar(635,1,640,480);

	setcolor(contorno);              /* Faz o Contorno */
	rectangle(1,1,640,480);
	rectangle(6,6,635,475);

	setfillstyle(SOLID_FILL,fundo_tela);
	bar(7,459,634,474);
	setfillstyle(SOLID_FILL,borda);
	bar(4,453,638,458);
	setcolor(contorno);
	line(7,453,635,453);
	line(7,458,635,458);

	mostra_mensagem(mensagem_default);

	monta_menu();
   nome_prog(texto);

	mouse_on();
}

void pressiona_botao( lista_botao *aux_bot )
{
     int x1 , y1 , x2 , y2 , i;
     x1 = aux_bot->x1;
     x2 = (aux_bot->x1)+(aux_bot->x2);
     y1 = aux_bot->y1;
     y2 = (aux_bot->y1)+(aux_bot->y2);

     mouse_off();

     def_color( menu_acionado );        /* MUDAR DEPOIS *******************/
     setfillstyle( SOLID_FILL , normal );
     bar( x1+2 , y1+2 , x2-2 , y2-2 );
     for( i=0 ; i<3 ; i++ ) {
	      setcolor( forte );
	      line( x1+i , y1+i , x2-i , y1+i );
	      line( x1+i , y1+i , x1+i , y2-i );
	      setcolor( leve );
	      line( x2-i , y1+i , x2-i , y2-i );
         line( x1+i , y2-i , x2-i , y2-i );
     }
     setcolor(DARKGRAY);
     outtextxy( x1+12 , y1+12 , aux_bot->nome );

     mouse_on();
}

void larga_botao( lista_botao *aux_bot )
{
	  int x1 , y1 , x2 , y2 , i;
	  x1 = aux_bot->x1;
	  x2 = (aux_bot->x1)+(aux_bot->x2);
	  y1 = aux_bot->y1;
	  y2 = (aux_bot->y1)+(aux_bot->y2);

     mouse_off();
	  def_color(  menu_acionado );			/* MUDAR DEPOIS ***********************/
	  setfillstyle( SOLID_FILL , normal );
	  bar( x1 , y1 , x2 , y2 );
	  for( i=0 ; i<3 ; i++ )
	  {
			setcolor( leve );
			line( x1+i , y1+i , x2-i , y1+i );
			line( x1+i , y1+i , x1+i , y2-i );
			setcolor( forte );
			line( x2-i , y1+i , x2-i , y2-i );
			line( x1+i , y2-i , x2-i , y2-i );
	  }
	  setcolor(BLACK);
	  outtextxy( x1+10 , y1+10 , aux_bot->nome );
     mouse_on();
}

lista_botao *find_botao(lista_botao *param_botao)
{
   union REGS regs;
   lista_botao *botao_aux;

   botao_aux = param_botao;
   do
   {
	      botao_aux = botao_aux->prox;
   }while ((posicaox < botao_aux->x1 || posicaox > (botao_aux->x1 + botao_aux->x2) ||
	         posicaoy < botao_aux->y1 || posicaoy > (botao_aux->y1 + botao_aux->y2)) &&
            botao_aux != param_botao );

   if (posicaox < botao_aux->x1 || posicaox > (botao_aux->x1 + botao_aux->x2) ||
       posicaoy < botao_aux->y1 || posicaoy > (botao_aux->y1 + botao_aux->y2))
       return (NULL);

   if (botao_aux->indicacao == NAO_BOTAO)
      return (botao_aux);
   pressiona_botao ( botao_aux );
   posiciona_mouse(posicaox,posicaoy);
   do{
       regs.x.ax = 0x03;
                                      /* mantem em baixo enquanto estiver */
       int86(MOUSE, &regs, &regs);    /* pressionado o botao do mouse     */
       posicaox = regs.x.cx;
       posicaoy = regs.x.dx;
   }while((regs.x.bx == 1)&&(posicaox >= botao_aux->x1) && (posicaox <= (botao_aux->x1 + botao_aux->x2)) &&
       (posicaoy >= botao_aux->y1) && (posicaoy <= (botao_aux->y1 + botao_aux->y2)));

   larga_botao ( botao_aux );
   posiciona_mouse(posicaox,posicaoy);
   if ((posicaox >= botao_aux->x1) && (posicaox <= (botao_aux->x1 + botao_aux->x2)) &&
       (posicaoy >= botao_aux->y1) && (posicaoy <= (botao_aux->y1 + botao_aux->y2)))
       return( botao_aux );
   else
       return (NULL);
}

lista_botao *acha_botao_default(lista_botao *param_botao)
{
   lista_botao *botao_aux;

   botao_aux = param_botao;
   do
   {
	      botao_aux = botao_aux->prox;
   }while ((botao_aux->indicacao != BOTAO_DEFAULT) && (botao_aux != param_botao));

   if (botao_aux->indicacao == BOTAO_DEFAULT)
      return (botao_aux);
   else
      return (NULL);
}

lista_botao *acha_botao_codigo(lista_botao *param_botao,int codigo)
{
   lista_botao *botao_aux;

   botao_aux = param_botao;
   do
   {
	      botao_aux = botao_aux->prox;
   }while ((botao_aux->codigo != codigo) && (botao_aux != param_botao));

   if (botao_aux->codigo == codigo)
      return (botao_aux);
   else
      return (NULL);
}

/***************************************************************************
  Funcao : find_sub_menu
  Objetivo : Retorna um ponteiro do sub menu , indicando qual opcao esta, na
             posicao que foi passada.
  Parametros : coordenadas onde foi acionada uma opcao.
***************************************************************************/
sub_menu *find_sub_menu (int posicaoy)
{
	menu_principal  *menu_aux;
	sub_menu *sub_menu_aux;

	menu_aux = menu_primario;
	sub_menu_aux = menu_aux->sub_menu_prox;

	do
	{
		sub_menu_aux = sub_menu_aux->sub_prox;
	}while(((sub_menu_aux->linha_inicial > posicaoy) ||
           (sub_menu_aux->linha_final   < posicaoy)) &&
			  (sub_menu_aux != menu_aux->sub_menu_prox));

	if ((sub_menu_aux->linha_inicial <= posicaoy) &&
       (sub_menu_aux->linha_final   >= posicaoy))
       return( sub_menu_aux );
	else
       return( NULL);
}

/***************************************************************************
  Funcao : acha_sub_menu
  Objetivo : Retorna um ponteiro do sub menu , indicando qual opcao
				 do sub menu pertence o codigo.
  Parametros : codigo da opcao.
***************************************************************************/
sub_menu *acha_sub_menu (int codigo)
{
	menu_principal  *menu_aux;
	sub_menu        *sub_menu_aux;

   menu_aux = primeiro_menu;
   do
   {
       sub_menu_aux = menu_aux->sub_menu_prox;
       do
	    {
		    sub_menu_aux = sub_menu_aux->sub_prox;
	    }while((sub_menu_aux->codigo!=codigo)&&(sub_menu_aux != menu_aux->sub_menu_prox));
		 menu_aux = menu_aux->men_prox;
   }while((sub_menu_aux->codigo != codigo) && (menu_aux != primeiro_menu ));

   if(sub_menu_aux->codigo == codigo)
      return( sub_menu_aux );
   else
      return(NULL);
}

/***************************************************************************
  Funcao : find_menu_principal
  Objetivo : Retorna um ponteiro do menu principal, indicando qual opcao
				 esta, na posicao que foi passada.
  Parametros : coordenadas onde foi acionada uma opcao.
***************************************************************************/
menu_principal *find_menu_principal (int posicaox)
{
	menu_principal  *menu_aux;

	menu_aux = primeiro_menu;
   do
	{
	      menu_aux = menu_aux->men_prox;
   }while(((menu_aux->coluna_inicial > posicaox) ||
			  (menu_aux->coluna_final   < posicaox)) &&
           (menu_aux != primeiro_menu));

   if ((menu_aux->coluna_inicial < posicaox) &&
		 (menu_aux->coluna_final   > posicaox))
       return( menu_aux );
	else
       return( NULL);

}

/***************************************************************************
  Funcao : acha_menu_principal
  Objetivo : Retorna um ponteiro do menu principal, indicando qual opcao
             do menu pertence o codigo.
  Parametros : codigo da opcao.
***************************************************************************/
menu_principal *acha_menu_principal (int codigo)
{
   menu_principal  *menu_aux;

   menu_aux = primeiro_menu;
   do
   {
			menu_aux = menu_aux->men_prox;
   }while((menu_aux->codigo != codigo) && (menu_aux != primeiro_menu ));

   return( menu_aux );
}

void insere_menu_principal (char *nome,int codigo, char *mensagem)
{
	menu_principal  *menu_aux;

   nome = strupr(nome);

   menu_aux =  (menu_principal *)malloc( sizeof( menu_principal ) );

   menu_aux->mensagem = malloc(strlen(mensagem)+1);
   strcpy(menu_aux->mensagem,mensagem);
	menu_aux->codigo = codigo;
   menu_aux->nome = malloc(strlen(nome)+1);
   strcpy(menu_aux->nome,nome);
   menu_aux->sub_menu_prox  = NULL;
	menu_aux->coluna_sub_menu= 0;
   menu_aux->linha_sub_menu =INICIAL_SUB_MENU;

   if (primeiro_menu == NULL)
		{
	       menu_aux->men_prox = menu_aux;
	       menu_aux->men_ant  = menu_aux;
	       primeiro_menu      = menu_aux;
	       menu_aux->coluna_inicial = COLUNA_INICIAL;
	       menu_aux->coluna_final   = COLUNA_INICIAL + (strlen(nome))*8 + 30;
		}
   else
		{
         menu_aux->men_ant   = primeiro_menu->men_ant;
         menu_aux->men_prox  = primeiro_menu;
			primeiro_menu->men_ant->men_prox = menu_aux;
         primeiro_menu->men_ant           = menu_aux;
         menu_aux->coluna_inicial = menu_aux->men_ant->coluna_final + 1;
         menu_aux->coluna_final   = menu_aux->coluna_inicial + (strlen(nome))*8 + 30;
		}
}

void insere_sub_menu (int menu_codigo, char *sub, int codigo, char *mensagem)
{                     /* codigo do menu principal, nome do sub menu, codigo
			 do sub menu */
	menu_principal  *menu_aux;
   sub_menu *sub_menu_aux;

	sub = strupr(sub);

	menu_aux = acha_menu_principal(menu_codigo);/* para achar o ponteiro do qual
						menu principal  */

   /* para ver se precisa aumentar as colunas do sub menu */
   if (menu_aux->coluna_sub_menu < (menu_aux->coluna_inicial + strlen(sub)*8 +30))
		menu_aux->coluna_sub_menu = (menu_aux->coluna_inicial + strlen(sub)*8 +30);
   menu_aux->linha_sub_menu = menu_aux->linha_sub_menu + TAMANHO_SUB_MENU;
   sub_menu_aux = (sub_menu *)malloc( sizeof ( sub_menu ) );

	sub_menu_aux->mensagem = malloc(strlen(mensagem)+1);
   strcpy(sub_menu_aux->mensagem,mensagem);
	sub_menu_aux->nome   = malloc(strlen(sub)+1);
   strcpy(sub_menu_aux->nome,sub);
	sub_menu_aux->codigo = codigo;
   sub_menu_aux->janelax = 0;
   sub_menu_aux->janelay = 0;
	sub_menu_aux->botoes = NULL;

	if (menu_aux->sub_menu_prox == NULL) /* se for o primeiro sub menu */
      {
		 sub_menu_aux->sub_prox  = sub_menu_aux;
		 sub_menu_aux->sub_ant   = sub_menu_aux;
       menu_aux->sub_menu_prox = sub_menu_aux;
		 sub_menu_aux->linha_inicial = INICIAL_SUB_MENU;
		 sub_menu_aux->linha_final   = INICIAL_SUB_MENU + TAMANHO_SUB_MENU;
      }
   else
      {
		 sub_menu_aux->sub_ant  = menu_aux->sub_menu_prox->sub_ant;
		 sub_menu_aux->sub_prox = menu_aux->sub_menu_prox;
       menu_aux->sub_menu_prox->sub_ant->sub_prox = sub_menu_aux;
		 menu_aux->sub_menu_prox->sub_ant           = sub_menu_aux;
		 sub_menu_aux->linha_inicial = sub_menu_aux->sub_ant->linha_final + 1;
		 sub_menu_aux->linha_final   = sub_menu_aux->linha_inicial + TAMANHO_SUB_MENU;
		}
}

void insere_opcoes ( int codigo, int indicacao, int cod, char *nome,
                     int dimensaox, int dimensaoy, int max_tam)
{
   lista_botao *aux_botao;
   sub_menu    *aux_sub_menu;

   aux_botao = (lista_botao *)malloc(sizeof(lista_botao));
   aux_sub_menu = acha_sub_menu(codigo);
   if (aux_sub_menu->botoes == NULL)
   {
      aux_botao->prox = aux_botao;
      aux_botao->ant  = aux_botao;
      aux_sub_menu->botoes = aux_botao;
   }
   else
   {
      aux_botao->ant  = aux_sub_menu->botoes->ant;
      aux_botao->prox = aux_sub_menu->botoes;
      aux_sub_menu->botoes->ant->prox = aux_botao;
      aux_sub_menu->botoes->ant       = aux_botao;
   }
   aux_botao->max_tam = max_tam;
   aux_botao->codigo = cod;
   aux_botao->x2 = dimensaox;
   aux_botao->y2 = dimensaoy;
   aux_botao->indicacao = indicacao;
   aux_botao->dado = NULL;
   aux_botao->nome = malloc(strlen(nome)+1);
   strcpy(aux_botao->nome,nome);
}

void seta_janela ( int codigo, char *nome, int dimensaox, int dimensaoy)
{
   sub_menu *aux_sub_menu;

   aux_sub_menu = acha_sub_menu(codigo);
   aux_sub_menu->nome_janela = malloc(strlen(nome)+1);
   strcpy(aux_sub_menu->nome_janela,nome);
   aux_sub_menu->janelax = dimensaox;
   aux_sub_menu->janelay = dimensaoy;
}

void desenha_menu ( void )
{
 menu_principal  *menu_aux;
 sub_menu        *sub_menu_aux;

 if ((menu_primario != menu_primario_anterior) ||
	  (menu_secundario != menu_secundario_anterior))/* se o evento ocasionou
								alguma alteracao */
 {
	if ((menu_primario != menu_primario_anterior) &&
	  (menu_secundario != menu_secundario_anterior))
	{
	  desenha_tela_fundo();
	}
	mouse_off();
	if (menu_primario != menu_primario_anterior)
	{
		if (menu_primario_anterior!= NULL)
		{
		setfillstyle(SOLID_FILL,fundo_menu);
		bar(menu_primario_anterior->coluna_inicial,LINHA_INICIAL_MENU,menu_primario_anterior->coluna_final,LINHA_FINAL_MENU);
		outtextxy(menu_primario_anterior->coluna_inicial+5,LINHA_FINAL_MENU - 14,menu_primario_anterior->nome);
		setcolor(contorno);
		line(menu_primario_anterior->coluna_inicial,LINHA_FINAL_MENU,menu_primario_anterior->coluna_final,LINHA_FINAL_MENU);
		}
		if (menu_primario != NULL)
		{
		setfillstyle(SOLID_FILL,menu_acionado);
		bar(menu_primario->coluna_inicial,LINHA_INICIAL_MENU,menu_primario->coluna_final,LINHA_FINAL_MENU);
		outtextxy(menu_primario->coluna_inicial+5,LINHA_FINAL_MENU - 14,menu_primario->nome);
		}
	}

	if (menu_primario != NULL)
	{
	  if (menu_secundario != NULL)
	  {
		if (menu_secundario != menu_secundario_anterior)
		{
			if (menu_primario == menu_primario_anterior)
			{
			  if (menu_secundario_anterior!= NULL)   /* apaga o antigo set tiver */
			  {
			    setfillstyle(SOLID_FILL,fundo_menu);
			    bar(menu_primario_anterior->coluna_inicial,menu_secundario_anterior->linha_inicial+1,
			    menu_primario_anterior->coluna_sub_menu,menu_secundario_anterior->linha_final-1);
			    outtextxy(menu_primario_anterior->coluna_inicial+5,menu_secundario_anterior->linha_final-10,
					   menu_secundario_anterior->nome);
			    setcolor(contorno);
			    rectangle(menu_primario_anterior->coluna_inicial,menu_secundario_anterior->linha_inicial+1,
					   menu_primario_anterior->coluna_sub_menu,menu_secundario_anterior->linha_final-1);

			    if (menu_secundario != NULL)    /* pinta o novo menu se tiver */
			    {
			    setfillstyle(SOLID_FILL,menu_acionado);
			    bar(menu_primario->coluna_inicial,menu_secundario->linha_inicial+1,
			    menu_primario->coluna_sub_menu,menu_secundario->linha_final-1);
			    outtextxy(menu_primario->coluna_inicial+5,menu_secundario->linha_final-10,
					   menu_secundario->nome);
			    setcolor(contorno);
			    rectangle(menu_primario->coluna_inicial,menu_secundario->linha_inicial+1,
					   menu_primario->coluna_sub_menu,menu_secundario->linha_final-1);
             }
			  }
           else
			  {
			    if (menu_secundario != NULL)   /* para desenhar o sub menu
																	  do novo respectivo
																	  menu_principal */
			   {
				    sub_menu_aux = menu_secundario;
				    setfillstyle(SOLID_FILL,fundo_menu);
				    bar(menu_primario->coluna_inicial,INICIAL_SUB_MENU+1,
						  menu_primario->coluna_sub_menu,menu_primario->linha_sub_menu);
				    setfillstyle(SOLID_FILL,menu_acionado);
				    bar(menu_primario->coluna_inicial,menu_secundario->linha_inicial+1,
						  menu_primario->coluna_sub_menu,menu_secundario->linha_final-1);
				    outtextxy(menu_primario->coluna_inicial+5,sub_menu_aux->linha_final-10,
						   sub_menu_aux->nome);
				    do{
				    setcolor(contorno);
				    rectangle(menu_primario->coluna_inicial,sub_menu_aux->linha_inicial+1,
						   menu_primario->coluna_sub_menu,sub_menu_aux->linha_final-1);
				    outtextxy(menu_primario->coluna_inicial+5,sub_menu_aux->linha_final-10,
						   sub_menu_aux->nome);
				    sub_menu_aux = sub_menu_aux->sub_prox;
				    }while(sub_menu_aux != menu_primario->sub_menu_prox);
			   } /* fim do if (menu_secundario != NULL) */
		     } /* fim do else if (menu_secundario_anterior!= NULL) */
			} /* fim do if (menu_primario ==  menu_primario_anterior */
			else
			{
			  if (menu_secundario != NULL)   /* para desenhar o sub menu
																	do novo respectivo
																	menu_principal */
			 {
				  sub_menu_aux = menu_secundario;
				  setfillstyle(SOLID_FILL,fundo_menu);
				  bar(menu_primario->coluna_inicial,INICIAL_SUB_MENU+1,
						menu_primario->coluna_sub_menu,menu_primario->linha_sub_menu);
				  setfillstyle(SOLID_FILL,menu_acionado);
				  bar(menu_primario->coluna_inicial,menu_secundario->linha_inicial+1,
						menu_primario->coluna_sub_menu,menu_secundario->linha_final-1);
				  outtextxy(menu_primario->coluna_inicial+5,sub_menu_aux->linha_final-10,
						 sub_menu_aux->nome);
				  do{
				  setcolor(contorno);
				  rectangle(menu_primario->coluna_inicial,sub_menu_aux->linha_inicial+1,
						 menu_primario->coluna_sub_menu,sub_menu_aux->linha_final-1);
				  outtextxy(menu_primario->coluna_inicial+5,sub_menu_aux->linha_final-10,
						 sub_menu_aux->nome);
				  sub_menu_aux = sub_menu_aux->sub_prox;
				  }while(sub_menu_aux != menu_primario->sub_menu_prox);
			 } /* fim do if (menu_secundario != NULL) */
		 } /* fim do else if (menu_primario == menu_primario_anterior) */

	  } /* fim do if (menu_secundario != menu_secundario_anterior) */
	  mostra_mensagem(menu_secundario->mensagem);
	}  /* fim do if menu_secundario != NULL  */
	else
		if (menu_primario != menu_primario_anterior)
	  mostra_mensagem(menu_primario->mensagem);
	}
	else
		mostra_mensagem(mensagem_default);

   mouse_on();
 }
}

void mostra_tecla (lista_botao *aux_botao, int tam)
{
   char aux[1];

   mouse_off();
   setcolor(contorno);
	sprintf(aux, "%c",aux_botao->dado[tam]);
   outtextxy(aux_botao->x1+5+tam*8,aux_botao->y1+8,aux);
   mouse_on();
}
void limpa_campo (lista_botao *aux_botao)
{
   mouse_off();
   setfillstyle(SOLID_FILL,fundo_tela);
   bar(aux_botao->x1+5,aux_botao->y1+8,
       aux_botao->x1+5+strlen(aux_botao->dado)*8+8,
       aux_botao->y1+8+7);
   mouse_on();
   free(aux_botao->dado);
   aux_botao->dado = NULL;

}
void tira_cursor (lista_botao *aux_botao)
{
   mouse_off();
   if ((aux_botao->indicacao == BOTAO)||(aux_botao->indicacao == BOTAO_DEFAULT))
   {
      def_color(fundo_menu);
      setcolor (normal);  /* era fundo_menu */
      rectangle(aux_botao->x1-1,aux_botao->y1-1,
                aux_botao->x1+aux_botao->x2+1,
                aux_botao->y1+aux_botao->y2+1);
      rectangle(aux_botao->x1-2,aux_botao->y1-2,
                aux_botao->x1+aux_botao->x2+2,
                aux_botao->y1+aux_botao->y2+2);

      setcolor (contorno);
      rectangle(campo_default->x1-1,campo_default->y1-1,
                campo_default->x1+campo_default->x2+1,
                campo_default->y1+campo_default->y2+1);
      rectangle(campo_default->x1-2,campo_default->y1-2,
                campo_default->x1+campo_default->x2+2,
                campo_default->y1+campo_default->y2+2);

   }
   else
   {
      setfillstyle(SOLID_FILL,fundo_tela);
      bar(aux_botao->x1+5+strlen(aux_botao->dado)*8,aux_botao->y1+8,
          aux_botao->x1+5+strlen(aux_botao->dado)*8+8,
          aux_botao->y1+8+7);
   }
   mouse_on();
}

void coloca_cursor (lista_botao *aux_botao)
{
 if(aux_botao != NULL)
 {
   mouse_off();
   if ((aux_botao->indicacao == BOTAO)||(aux_botao->indicacao == BOTAO_DEFAULT))
   {
      def_color(fundo_menu);
      setcolor (normal);  /* era fundo_menu */
      rectangle(campo_default->x1-1,campo_default->y1-1,
                campo_default->x1+campo_default->x2+1,
                campo_default->y1+campo_default->y2+1);
      rectangle(campo_default->x1-2,campo_default->y1-2,
                campo_default->x1+campo_default->x2+2,
                campo_default->y1+campo_default->y2+2);
      setcolor (contorno);
      rectangle(aux_botao->x1-1,aux_botao->y1-1,
                aux_botao->x1+aux_botao->x2+1,
                aux_botao->y1+aux_botao->y2+1);
      rectangle(aux_botao->x1-2,aux_botao->y1-2,
                aux_botao->x1+aux_botao->x2+2,
                aux_botao->y1+aux_botao->y2+2);
   }
   else
   {
      setcolor(contorno);
      outtextxy(aux_botao->x1+5+strlen(aux_botao->dado)*8,aux_botao->y1+8,"_");
   }
   mouse_on();
 }
}

void monta_janela (sub_menu *sub_menu_aux)
{
 lista_botao *aux_bot;
 int i=0,j=0;
 int co1,co2,li1,li2;

 if (sub_menu_aux != NULL)
 {
	co1 = JANELAX;
	co2 = sub_menu_aux->janelax;
	li1 = JANELAY;
	li2 = sub_menu_aux->janelay;

	aux_bot = sub_menu_aux->botoes;

	tela_janela(co1,li1,co2,li2,sub_menu_aux->nome_janela);
	mouse_off();
   li1 = li1 + 20;
   setcolor(contorno);
	do
	{
		if(aux_bot->indicacao == NAO_BOTAO)
		{
			outtextxy(co1+15,li1+30*i+45,aux_bot->nome);
         setfillstyle(SOLID_FILL,fundo_tela);
			bar(co1+15+strlen(aux_bot->nome)*8,li1+30*i+40,
				 co1+15+strlen(aux_bot->nome)*8+aux_bot->x2,
				 li1+30*i+40+aux_bot->y2);
			aux_bot->x1 = co1+15+strlen(aux_bot->nome)*8;
			aux_bot->y1 = li1+30*i+40;
         if (aux_bot->dado != NULL)
         {
            outtextxy(aux_bot->x1+5,aux_bot->y1+8,aux_bot->dado);
         }
			i++;
		}
		else
		{
			aux_bot->x1 = co1 + co2-100;
			aux_bot->y1 = li1+50*j + 15;
         mouse_on();
			larga_botao(aux_bot);
         mouse_off();
			j++;
		}
		aux_bot = aux_bot->prox;
	}while (aux_bot != sub_menu_aux->botoes);
   coloca_cursor(campo_escolhido);
   coloca_cursor(campo_default);
	mouse_on();
 }
}

void volta_para_menu_principal ( void )
{
   lista_botao *aux_bot;
   aux_bot = opcao_ativa->botoes;
   do
   {
      if (aux_bot->dado != NULL)
      {
         free(aux_bot->dado);
         aux_bot->dado = NULL;
      }
      aux_bot = aux_bot->prox;
   }while(aux_bot != opcao_ativa->botoes);
   opcao_ativa = NULL;
   desenha_tela_fundo();
}

void seleciona_opcao ( sub_menu *sub_menu_aux )
{
	opcao_ativa = sub_menu_aux;
   campo_escolhido = sub_menu_aux->botoes;
   if(campo_escolhido != NULL)
   {
      campo_default = acha_botao_default(campo_escolhido);
      monta_janela ( sub_menu_aux );
   }
}

void tira_mensagem_erro ( void )
{
   free(mensag_erro->nome_janela);
   mensag_erro->nome_janela = NULL;
   seleciona_opcao(quarda_opcao_ativa);
}
void mensagem_erro ( char *texto)
{
   quarda_opcao_ativa = opcao_ativa;
   mensag_erro->janelax = strlen(texto)*8+200;
   mensag_erro->nome_janela = malloc(strlen(texto)+1);
   strcpy(mensag_erro->nome_janela,texto);
   seleciona_opcao(mensag_erro);
}

void realiza_operacao (lista_botao *aux_botao)
{
   tipo_lhs *aux_lhs;
   tipo_rhs *aux_rhs;
   lista_botao *auxiliar_botao;

   switch(opcao_ativa->codigo)
   {
    case ERRO :
         switch(aux_botao->codigo)
         {
         case CANCEL :
              volta_para_menu_principal();
              break;
         case OK :
              tira_mensagem_erro();
              break;
         }
         break;
    case SUB_GRAMATICA_CRIAR :
         switch(aux_botao->codigo)
         {
         case CANCEL :
              volta_para_menu_principal();
              break;
         case OK :
              break;
         case MAIS_PRODUCAO :
              auxiliar_botao =acha_botao_codigo(aux_botao,ENTRADA_NT);
              if (auxiliar_botao->dado != NULL)  /* Se existir algo no campo */
              {
                 aux_lhs = acha_lhs (auxiliar_botao->dado[0]);
                 if (aux_lhs == NULL)
                 {
                    if (acha_botao_codigo(aux_botao,ENTRADA_PRODUCAO)->dado != NULL)
                    {
                       insere_lhs(auxiliar_botao->dado[0]);
                       auxiliar_botao =acha_botao_codigo(aux_botao,ENTRADA_PRODUCAO);
                       insere_rhs(descritor_lhs.ultimo,auxiliar_botao->dado);
                       limpa_campo(auxiliar_botao);
                       campo_escolhido = auxiliar_botao;
                       coloca_cursor(campo_escolhido);
                    }
                    else
                       mensagem_erro("Falta Producao");
                 }
                 else
                 {
                    if (acha_botao_codigo(aux_botao,ENTRADA_PRODUCAO)->dado != NULL)
                    {
                       auxiliar_botao =acha_botao_codigo(aux_botao,ENTRADA_PRODUCAO);
                       insere_rhs(aux_lhs,auxiliar_botao->dado);
                       limpa_campo(auxiliar_botao);
                       campo_escolhido = auxiliar_botao;
                       coloca_cursor(campo_escolhido);
                    }
                    else
                       mensagem_erro("Falta Producao");
                 }
              }
              else
                  mensagem_erro("Falta Nao Terminal");

              break;
         case MAIS_NT :
              auxiliar_botao =acha_botao_codigo(aux_botao,ENTRADA_NT);
              if (auxiliar_botao->dado != NULL)  /* Se existir algo no campo */
              {
                 aux_lhs = acha_lhs (auxiliar_botao->dado[0]);
                 if (aux_lhs == NULL)
                 {
                    if (acha_botao_codigo(aux_botao,ENTRADA_PRODUCAO)->dado != NULL)
                    {
                       insere_lhs(auxiliar_botao->dado[0]);
                       limpa_campo(auxiliar_botao);
                       campo_escolhido = auxiliar_botao;
                       auxiliar_botao =acha_botao_codigo(aux_botao,ENTRADA_PRODUCAO);
                       insere_rhs(descritor_lhs.ultimo,auxiliar_botao->dado);
                       limpa_campo(auxiliar_botao);
                       coloca_cursor(campo_escolhido);
                    }
                    else
                       mensagem_erro("Falta Producao");
                 }
                 else
                 {
                    if (acha_botao_codigo(aux_botao,ENTRADA_PRODUCAO)->dado != NULL)
                    {
                       limpa_campo(auxiliar_botao);
                       campo_escolhido = auxiliar_botao;
                       auxiliar_botao =acha_botao_codigo(aux_botao,ENTRADA_PRODUCAO);
                       insere_rhs(aux_lhs,auxiliar_botao->dado);
                       limpa_campo(auxiliar_botao);
                       coloca_cursor(campo_escolhido);
                    }
                    else
                       mensagem_erro("Falta Producao");
                 }
              }
              else
                  mensagem_erro("Falta Nao Terminal");
              break;
         }
         break;

    case SUB_GRAMATICA_CARREGAR :
         switch(aux_botao->codigo)
         {
         case CANCEL :
              volta_para_menu_principal();
              break;
         case OK :

              break;
         }
         break;
    case SUB_GRAMATICA_SALVAR :
         switch(aux_botao->codigo)
         {
         case CANCEL :
              volta_para_menu_principal();
              break;
         case OK :

              break;
         }
         break;
    case SUB_TEXTO_CRIAR :
         switch(aux_botao->codigo)
         {
         case CANCEL :
              volta_para_menu_principal();
              break;
         case OK :

              break;
         }
         break;

    case SUB_TEXTO_CARREGAR :
         switch(aux_botao->codigo)
         {
         case CANCEL :
              volta_para_menu_principal();
              break;
         case OK :

              break;
         }
         break;
    case SUB_TEXTO_SALVAR :
         switch(aux_botao->codigo)
         {
         case CANCEL :
              volta_para_menu_principal();
              break;
         case OK :

              break;
         }
         break;

    case SUB_EXECUTAR_RODAR :
         switch(aux_botao->codigo)
         {
         case CANCEL :
              volta_para_menu_principal();
              break;
         case OK :

              break;
         }
         break;

    case SUB_EXECUTAR_PASSO :
         switch(aux_botao->codigo)
         {
         case CANCEL :
              volta_para_menu_principal();
              break;
         case OK :

              break;
         }
         break;
    case SUB_OPCOES_COR :
         switch(aux_botao->codigo)
         {
         case CANCEL :
              volta_para_menu_principal();
              break;
         case OK :

              break;
         }
         break;
    case SUB_OPCOES_SAIR :
         switch(aux_botao->codigo)
         {
         case CANCEL :
              volta_para_menu_principal();
              break;
         case OK :
              fim = TRUE;
              break;
         }
         break;
   }
}

void executa_opcao ( void )
{
   lista_botao *campo;
   char *auxiliar;
   int i;

   switch (evento)
   {
   case TMOUSE :
               /* se estiver dentro da janela do sub_menu */
          if (( posicaox>JANELAX)&&
                   ( posicaox<JANELAX+(opcao_ativa->janelax))&&
                   ( posicaoy>JANELAY)&&
                   ( posicaoy<JANELAY+(opcao_ativa->janelay)))
          {
            campo = find_botao(opcao_ativa->botoes);
            if (campo!= NULL)  /* se existir um campo valido onde o
                                  o MOUSE foi acionada       */
            {
                if ((campo->indicacao == BOTAO)||
                    (campo->indicacao == BOTAO_DEFAULT))/* se for um botao */
                    realiza_operacao(campo);
                else
                {
                    tira_cursor(campo_escolhido);
                    campo_escolhido = campo;
                    coloca_cursor(campo_escolhido);
                }
            } /* end do  if (campo!=NULL) */
          } /* end do if JANELA */
			 break;
   case TTECLADO :
          pega_tecla_pressionada();
          switch(tecla1)
          {
            case ENTER :
                 if (campo_escolhido->indicacao == BOTAO)
                     realiza_operacao(campo_escolhido);
                 else
                     realiza_operacao(campo_default);
                 break;
            case ESC :
                     volta_para_menu_principal();
                 break;
            case TAB :
                 tira_cursor(campo_escolhido);
                 campo_escolhido = campo_escolhido->prox;
                 coloca_cursor(campo_escolhido);
                 break;
            case 0 :
                 switch (tecla2)
                 {
                  case SHIFT_TAB :
                       tira_cursor(campo_escolhido);
                       campo_escolhido = campo_escolhido->ant;
                       coloca_cursor(campo_escolhido);
                       break;
                 }
                 break;
            default :
                 if (campo_escolhido->indicacao == NAO_BOTAO)
                 {
                    if((tecla1 > 44 ) && (tecla1 < 122))
                    {
                       if (campo_escolhido->dado == NULL)
                       {
                          tira_cursor(campo_escolhido);
                          campo_escolhido->dado = malloc(2);
                          campo_escolhido->dado[0] = tecla1;
                          campo_escolhido->dado[1] = '\0';
                          mostra_tecla(campo_escolhido,0);
                          coloca_cursor(campo_escolhido);
                       }
                       else
                       {
                          if (campo_escolhido->max_tam >
                              strlen(campo_escolhido->dado))
                          {
                            tira_cursor(campo_escolhido);
                            auxiliar = malloc(strlen(campo_escolhido->dado)+2);
                            strcpy(auxiliar,campo_escolhido->dado);
                            free(campo_escolhido->dado);
                            i = strlen(auxiliar);
                            auxiliar[i] = tecla1;
                            auxiliar[i+1] = '\0';
                            campo_escolhido->dado = malloc(strlen(auxiliar)+1);
                            strcpy(campo_escolhido->dado,auxiliar);
                            free(auxiliar);
                            mostra_tecla(campo_escolhido,strlen(campo_escolhido->dado)-1);
                            coloca_cursor(campo_escolhido);
                          }
                       } /* fim do else do if (campo_escolhido->dado == NULL)*/
                    } /*fim do if((tecla1 > 44 ) && (tecla1 < 122))*/
                    else
                    {
                      if ((tecla1 == BACKSPACE)&&(campo_escolhido->dado != NULL)
                          &&(strlen(campo_escolhido->dado)>0))
                      {
                         tira_cursor(campo_escolhido);
                         if (strlen(campo_escolhido->dado) ==1)
                         {
                           free(campo_escolhido->dado);
                           campo_escolhido->dado = NULL;
                         }
                         else
                             campo_escolhido->dado[strlen(campo_escolhido->dado)-1] = '\0';
                         tira_cursor(campo_escolhido);
                         coloca_cursor(campo_escolhido);
                      }
                    }
                 }/*if (campo_escolhido->indicacao == NAO_BOTAO)*/

          }  /* end do switch (tecla1) */
			 break;
   } /* end switch(evento) */
}

int pega_proximo_evento( void )
{
    status_mouse();
    if (bot_esq)
				 return (TMOUSE);
    if (kbhit())
           return (TTECLADO);
	 return 0;
}

void trata_evento(int evento)
{
  int tecla_usada;   /* variavel que indica que a tecla ja foi consumida e
                        usada o seu objetivo    */
  switch( evento )
  {       case TMOUSE :           /* se for mouse */
          delay(5);               /* para dar tempo para largar o botao */
          status_mouse();
          if (bot_esq == 0)   /* para executar a opcao do sub menu escolhido
                                 quando e largado o botao do mouse em cima da
                                 da opcao selecinada */
          {
            if (menu_secundario != NULL)
	          if ((menu_secundario->linha_inicial <= posicaoy) &&
                 (menu_secundario->linha_final   >= posicaoy))
             {
              menu_primario_anterior = menu_primario;
				  menu_primario = NULL;
				  menu_secundario_anterior = menu_secundario;
				  menu_secundario = NULL;
				  onde = DEFAULT;
              desenha_menu();
				  seleciona_opcao (menu_secundario_anterior);
              break;
             }
          }
          switch( onde )
          {
				 case MENU_PRINCIPAL :
					  if ((posicaox > 7) && (posicaox < 634) &&
							(posicaoy > LINHA_INICIAL_MENU) && (posicaoy <LINHA_FINAL_MENU))   /* se esta na area
                                                             do menu */
                 {
                   menu_primario_anterior = menu_primario;
                   menu_primario = find_menu_principal(posicaox);
                                        /* acha qual menu precionado */
                   if (menu_primario != NULL) /* verefica se exite menu */
                   {
                     menu_secundario_anterior = menu_secundario;
                     menu_secundario = menu_primario->sub_menu_prox;
                     onde = SUB_MENU;
                   }
                   else
                   {
                     menu_secundario_anterior = menu_secundario;
							menu_secundario = NULL;
                     onde = DEFAULT;
                   }
					  }
                 else
					  {
                   menu_primario_anterior = menu_primario;
                   menu_secundario_anterior = menu_secundario;
                   menu_primario  = NULL;
                   menu_secundario= NULL;
                   onde = DEFAULT;
                 }
                  break;
             case SUB_MENU :
                 if ((posicaox > menu_primario->coluna_inicial) &&
                     (posicaox < menu_primario->coluna_sub_menu)&&
                     (posicaoy > INICIAL_SUB_MENU)              &&
                     (posicaoy < menu_primario->linha_sub_menu))
                 {
                   menu_primario_anterior = menu_primario;
						 menu_secundario_anterior = menu_secundario;
                   menu_secundario = find_sub_menu(posicaoy);

/**************         FALTA FAZER EXECUTAR A LINHA  ****************/

                   onde = SUB_MENU;
                   break;
                 }
                 if ((posicaox > 7) && (posicaox < 634) &&
                    (posicaoy > LINHA_INICIAL_MENU) && (posicaoy <LINHA_FINAL_MENU))
                 {
                   menu_primario_anterior = menu_primario;
                   menu_primario = find_menu_principal(posicaox);
                   if (menu_primario != NULL)
                   {
                     menu_secundario_anterior = menu_secundario;
                     menu_secundario = menu_primario->sub_menu_prox;
                     onde = SUB_MENU;
                   }
                   else
						 {
							menu_secundario_anterior = menu_secundario;
                     menu_secundario = NULL;
							onde = DEFAULT;
                   }
                 }
                 else
                 {
                   menu_primario_anterior = menu_primario;
                   menu_secundario_anterior = menu_secundario;
                   menu_primario  = NULL;
                   menu_secundario= NULL;
                   onde = DEFAULT;
                 }
                  break;
             default :
                 if ((posicaox > 7) && (posicaox < 634) &&
                     (posicaoy > LINHA_INICIAL_MENU) && (posicaoy <LINHA_FINAL_MENU))  /* verifica se esta
                                                            na area do menu */
                 {
						 menu_primario_anterior = menu_primario;
                   menu_primario = find_menu_principal(posicaox);
                   if (menu_primario != NULL)
                   {
                     menu_secundario_anterior = menu_secundario;
                     menu_secundario = menu_primario->sub_menu_prox;
                     onde = SUB_MENU;
                   }
                   else
                   {
                     menu_secundario_anterior = menu_secundario;
                     menu_secundario = NULL;
                     onde = DEFAULT;
                   }
                 }
                 else
                 {
                     menu_primario_anterior = menu_primario;  /* conserva-se
                                                                 sem menu */
							menu_secundario_anterior = menu_secundario;
							menu_primario =NULL;
                     menu_secundario = NULL;
                 }

          }
          desenha_menu ();
          break;
          case TTECLADO :    /* se for teclado */
               pega_tecla_pressionada();
               tecla_usada = FALSE;
               if (tecla1 == 0)
               {
                 switch(tecla2)
                 {
                     case F10 :
                             menu_secundario_anterior = menu_secundario;
                             menu_primario_anterior = menu_primario;
                             menu_secundario = NULL;
									  menu_primario = primeiro_menu;
                             tecla_usada = TRUE;
									  onde = MENU_PRINCIPAL;
                             break;
                 } /* final do switch (tecla2) */
               }  /* final do if (tecla1 == 0) */
               if (!tecla_usada)   /* se a tecla nao foi usada */
               switch( onde )
               {
                  case MENU_PRINCIPAL :
                       if (tecla1 == 0)
                       {
                         switch(tecla2)
                         {
                             case PARA_CIMA :
                                  break;
                             case PARA_BAIXO:
                                  menu_secundario_anterior = menu_secundario;
                                  menu_secundario = menu_primario->sub_menu_prox;
											 onde = SUB_MENU;
                                  break;
                             case DIREITA   :
											 menu_primario_anterior = menu_primario;
                                  menu_primario  = menu_primario->men_prox;
                                  menu_secundario_anterior = menu_secundario;
                                                           /* NULL;   nao sei */
                                  menu_secundario = NULL; /* menu_primario->sub_menu_prox; */
                                  onde = MENU_PRINCIPAL;
                                  break;
                             case ESQUERDA  :
                                  menu_primario_anterior = menu_primario;
                                  menu_primario  = menu_primario->men_ant;
                                  menu_secundario_anterior = menu_secundario;
                                                           /* NULL;   nao sei */
                                  menu_secundario = NULL; /* menu_primario->sub_menu_prox; */
                                  onde = MENU_PRINCIPAL;
                                  break;
                             default :
											 menu_secundario_anterior = menu_secundario;
                                  menu_primario_anterior = menu_primario;
                         }     /* final do switch (tecla2) */
                       }  /* final do if (tecla1==0)  */
							  else
                       {
                         switch(tecla1)
                         {
                             case ENTER :
                                  menu_secundario_anterior = menu_secundario;
                                  menu_secundario = menu_primario->sub_menu_prox;
                                  onde = SUB_MENU;
                                  break;
                             case ESC   :
                                  menu_primario_anterior = menu_primario;
                                  menu_primario = NULL;
                                  menu_secundario = NULL;
                                  onde = DEFAULT;
                                  break;

/*****************     IMPLEMENTAR MAIS TARDE A FUNCAO DE PEGAR A PRIMEIRA
                       LETRA DO MENU PRINCIPAL PARA SELECIONAR A OPCAO *****/
                             default :
                                  menu_secundario_anterior = menu_secundario;
											 menu_primario_anterior = menu_primario;
                         } /* final do switch(tecla1)  */
                       } /* final do else do if(tecla1 ==0) */
                       break;
                  case SUB_MENU :
                       if (tecla1 == 0)
                       {
                         switch(tecla2)
                         {
                             case PARA_CIMA :
                                  menu_primario_anterior = menu_primario;
                                  menu_secundario_anterior = menu_secundario;
                                  menu_secundario = menu_secundario->sub_ant;
                                  onde = SUB_MENU;
											 break;
                             case PARA_BAIXO:
                                  menu_primario_anterior = menu_primario;
                                  menu_secundario_anterior = menu_secundario;
                                  menu_secundario = menu_secundario->sub_prox;
                                  onde = SUB_MENU;
											 break;
                             case DIREITA   :
                                  menu_primario_anterior = menu_primario;
                                  menu_primario  = menu_primario->men_prox;
                                  menu_secundario_anterior = menu_secundario;
                                                           /* NULL;   nao sei */
                                  menu_secundario =  menu_primario->sub_menu_prox;
                                  onde = SUB_MENU;
                                  break;
                             case ESQUERDA  :
                                  menu_primario_anterior = menu_primario;
                                  menu_primario  = menu_primario->men_ant;
                                  menu_secundario_anterior = menu_secundario;
																			  /* NULL;   nao sei */
                                  menu_secundario = menu_primario->sub_menu_prox;
                                  onde = SUB_MENU;
                                  break;
                             default :
                                  menu_secundario_anterior = menu_secundario;
                                  menu_primario_anterior = menu_primario;
								 }     /* final do switch (tecla2) */
                       }  /* final do if (tecla1==0)  */
                       else
                       {
                         switch(tecla1)
                         {
                             case ENTER :
											 menu_primario_anterior = menu_primario;
											 menu_primario = NULL;
											 menu_secundario_anterior = menu_secundario;
											 menu_secundario = NULL;
											 onde = DEFAULT;
                                  desenha_menu();
											 seleciona_opcao (menu_secundario_anterior);
                                  break;
                             case ESC   :
                                  menu_primario_anterior = menu_primario;
                                  menu_primario = NULL;
                                  menu_secundario_anterior = menu_secundario;
                                  menu_secundario = NULL;
                                  onde = DEFAULT;
											 break;

/*****************     IMPLEMENTAR MAIS TARDE A FUNCAO DE PEGAR A PRIMEIRA
                       LETRA DO MENU PRINCIPAL PARA SELECIONAR A OPCAO *****/
                             default :
                                  menu_secundario_anterior = menu_secundario;
                                  menu_primario_anterior = menu_primario;
                         } /* final do switch(tecla1)  */
                       } /* final do else do if(tecla1 ==0) */
                       break;
                  default :
                       menu_secundario_anterior = menu_secundario;
                       menu_primario_anterior = menu_primario;
               } /* final do switch (onde) */
          if(opcao_ativa == NULL)
               desenha_menu ();
          break;  /* final da opcao TTECLADO */
  } /* final do primeiro switch (evento) */

}

