#include <conio.h>
#include <stdlib.h>

#define Q	1		/* estado normal */
#define B   2		/* estado backtracking */
#define T   3		/* estado final */

#define FALSE 0
#define TRUE  1

typedef struct no_rhs			/* estrutura que contem o RHS */
{
	char *producao;				/* producao correspondente */
	struct no_rhs *proximo;		/* ponteiro para a proxima producao */
}tipo_rhs;

typedef struct no_lhs			/* estrutura que armazena o LHS */
{
	char nt;							/* contem o nao terminal */
	int max;
	struct no_rhs *first;		/* ponteiro para a primeira producao */
	struct no_rhs *last ;		/* ponteiro para a ultima   producao */
	struct no_lhs *proximo;		/* ponteiro para o proximo nao terminal */
}tipo_lhs;

typedef struct no_hist			/* pilha HIST */
{
	char symb;						/* contem o nao terminal, ou terminal */
	struct no_rhs *p;				/* Aj ou NULL */
	struct no_hist *proximo;   /* proximo */
}tipo_hist;

struct {
	struct no_lhs *primeiro;
	struct no_lhs *ultimo;
}descritor_lhs;

char *aux_sent, *sent; 			/* pilha SENT  */
char *string_entrada;			/*	string de entrada */
int estado;							/* estado atual */

tipo_lhs *lhs;
tipo_hist *hist;


tipo_rhs *p;
char s,
	  t;
int  n,
	  parse,
	  posicao;


tipo_rhs *funcao_de_disponibilidade ( char nao_t )
{
	tipo_lhs *aux_lhs;

	aux_lhs = descritor_lhs.primeiro;

	while((aux_lhs != NULL)&&(aux_lhs->nt != nao_t))
	{
		aux_lhs =aux_lhs->proximo;
	}
	if (aux_lhs->nt == nao_t)
		return(aux_lhs->first);
	else
			return(NULL);
}

tipo_lhs *acha_lhs ( char nao_t )
{
	tipo_lhs *aux_lhs;

	aux_lhs = descritor_lhs.primeiro;

	while((aux_lhs != NULL)&&(aux_lhs->nt != nao_t))
	{
		aux_lhs =aux_lhs->proximo;
	}
	if (aux_lhs->nt == nao_t)
		return(aux_lhs);
	else
			return(NULL);
}


void insere_lhs (char nt)
{
	char producao[10];
	tipo_lhs *aux_lhs;

	aux_lhs = (tipo_lhs *)malloc(sizeof(tipo_lhs));
	if (descritor_lhs.primeiro == NULL)
	{
		descritor_lhs.primeiro = aux_lhs;
		descritor_lhs.ultimo   = aux_lhs;
	}
	else
	{
		descritor_lhs.ultimo->proximo = aux_lhs;
		descritor_lhs.ultimo = aux_lhs;
	}

	aux_lhs->proximo = NULL;
	aux_lhs->nt      = nt;
	aux_lhs->max     = 0;
	aux_lhs->first   = NULL;
	aux_lhs->last    = NULL;
}

void insere_rhs (tipo_lhs *aux_lhs, char *producao)
{
	tipo_rhs *aux_rhs;
   aux_lhs->max++;
   aux_rhs = (tipo_rhs *)malloc(sizeof(tipo_rhs));
   if (aux_lhs->first == NULL)
   {
	   aux_lhs->first = aux_rhs;
	   aux_lhs->last 	= aux_rhs;
   }
   else
   {
	   aux_lhs->last->proximo = aux_rhs;
	   aux_lhs->last = aux_rhs;
   }
   aux_rhs->proximo = NULL;
   aux_rhs->producao = malloc(strlen(producao)+1);
   strcpy(aux_rhs->producao,producao);
}

