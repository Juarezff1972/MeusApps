#include <conio.h>
#include <stdlib.h>

#define Q	1		/* estado normal */
#define B   2		/* estado backtracking */
#define T   3		/* estado final de aceitacao */
#define N   4		/* estado final de nao aceitacao */

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

struct {
	struct no_lhs *primeiro;
	struct no_lhs *ultimo;
}descritor_lhs;


typedef struct no_hist			/* pilha HIST */
{
	char symb;						/* contem o nao terminal, ou terminal */
	struct no_rhs *p;				/* Aj ou NULL */
	struct no_hist *proximo;   /* proximo */
}tipo_hist;

char *aux_sent, *sent; 			/* pilha beta SENT  */
char *string_entrada;			/*	string de entrada */
int estado;							/* estado atual */

//tipo_lhs *lhs;                /* Estrutura que contem as producoes */
tipo_hist *hist;              /* pilha alfa HIST */


tipo_rhs *p;
char *texto_de_entrada;

char s,
	  t;
int  n,
/*	  parse, */
	  posicao;  /* indica a posicao da string de entrada que esta sendo analisada */

/***********************************************************************
Serve par zerar toda a estrutura que contem a gramtatica
***********************************************************************/
void anula_gramatica ( void )
{
   tipo_lhs *aux_lhs;
   tipo_rhs *aux_rhs;

   while(descritor_lhs.primeiro != NULL)
   {
      while(descritor_lhs.primeiro->first != NULL)
      {
         aux_rhs = descritor_lhs.primeiro->first;
         descritor_lhs.primeiro->first = descritor_lhs.primeiro->first->proximo;
         free(aux_rhs);
      }
      aux_lhs = descritor_lhs.primeiro;
      descritor_lhs.primeiro = descritor_lhs.primeiro->proximo;
      free(aux_lhs);
   }
}

/***********************************************************************
 Serve par zerar toda a estrutura que contem as pilhas do parsing
***********************************************************************/
void anula_pilhas_do_parsing (void)
{
 tipo_hist *aux_hist;

 if (aux_sent != NULL)
    free(aux_sent);
 if (sent != NULL)
    free(sent);
 while(hist != NULL)
 {
   aux_hist = hist;
   hist = hist->proximo;
   free(aux_hist);
 }
}
/***********************************************************************
Funcao que retorna o ponteriro da producao do dado nao terminal,
       se exitir, se nao retorna NULL.
***********************************************************************/
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

/***********************************************************************
Funcao que retorna o ponteriro do nao terminal,
       se exitir, se nao retorna NULL.
***********************************************************************/
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


/***********************************************************************
Funcao que insere o nao terminal na estrutura lhs
***********************************************************************/
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

/***********************************************************************
Funcao que insere a producao na estrutura apontada pela ponterio lhs.
***********************************************************************/
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

void inicializar_parsing ( char *string )
{
	tipo_hist *aux_hist;

	hist = NULL;
	estado = Q;
	n = strlen(string);
	string_entrada = malloc(strlen(string)+2);
	strcpy(string_entrada,string);
	strcat(string_entrada,"#");
	posicao = 0;

	aux_hist = (tipo_hist *)malloc(sizeof(tipo_hist));
	aux_hist->symb = '\0';
	aux_hist->p = NULL;
	aux_hist->proximo = NULL;
	hist = aux_hist;

	sent = malloc(3);
	sent[0] = descritor_lhs.primeiro->nt;
	sent[1] = '\0';
	strcat(sent,"#");

   aux_sent = NULL;

}


int configuracao ( void )
{

  p = hist->p;
  s = hist->symb;
  t = sent[0];

  if ((estado == Q)&&(posicao == n )&&(t == '#'))
		return( 3 );  /* caso 3 */
  else
		if (estado == Q)
		{
			if (funcao_de_disponibilidade(t) != NULL)
				return ( 1 );  /* caso 1 */
			else
				if ( t == string_entrada[posicao] )
					return ( 2 );  /* caso  2 */
				else
					return ( 4 );  /* caso  4 */
		}
		else
		{
			if (funcao_de_disponibilidade(s) == NULL)
				return ( 5 );     /* caso  5 */
			else
				if (p->proximo != NULL)
					return ( 6 );   /* caso  6 */
				else
					if ((posicao==0)&&(s==descritor_lhs.primeiro->nt))
					{
                  estado = N;
						return ( 8 );
					}
					else
						return ( 7 );   /* caso  7 */
		}
}

int executa ( int caso )
{
	tipo_hist *aux_hist;

	switch(caso)
	{
		case 1 :
				  aux_hist = (tipo_hist *)malloc(sizeof(tipo_hist));
				  aux_hist->symb = t;
				  aux_hist->p = funcao_de_disponibilidade ( t );
				  aux_hist->proximo = hist;
				  hist = aux_hist;

				  aux_sent = malloc(strlen(hist->p->producao)+strlen(sent));
				  strcpy(aux_sent,hist->p->producao);
				  strcat(aux_sent,sent+1);
				  free(sent);
				  sent = malloc(strlen(aux_sent)+1);
				  strcpy(sent,aux_sent);
				  free(aux_sent);
              aux_sent = NULL;

				  break;
		case 2 :
				  aux_hist = (tipo_hist *)malloc(sizeof(tipo_hist));
				  aux_hist->symb = t;
				  aux_hist->p = NULL;
				  aux_hist->proximo = hist;
				  hist = aux_hist;
				  posicao++;

				  aux_sent = malloc(strlen(sent));
				  strcpy(aux_sent,sent+1);
				  free(sent);
				  sent =malloc(strlen(aux_sent)+1);
				  strcpy(sent,aux_sent);
				  free(aux_sent);
              aux_sent = NULL;

				  break;
		case 3 :
				  estado = T;
				  strcpy(sent," ");
				  break;
		case 4 :
				  estado = B;
				  break;
		case 5 :
				  posicao = posicao - 1;
				  aux_hist = hist;
				  hist = hist->proximo;
				  free(aux_hist);

				  aux_sent = malloc(strlen(sent)+2);
				  aux_sent[0] = hist->symb;
				  aux_sent[1] = '\0';
				  strcat(aux_sent,sent);
				  free(sent);
				  sent =malloc(strlen(aux_sent)+1);
				  strcpy(sent,aux_sent);
				  free(aux_sent);
              aux_sent = NULL;
				  break;
		case 6 :
				  estado = Q;
				  hist->p = p->proximo;

				  aux_sent = malloc(strlen(hist->p->producao)+strlen(sent));
				  strcpy(aux_sent,hist->p->producao);
				  strcat(aux_sent,sent+strlen(p->producao));
				  free(sent);
				  sent = malloc(strlen(aux_sent)+1);
				  strcpy(sent,aux_sent);
				  free(aux_sent);
              aux_sent = NULL;
				  break;
		case 7 :

				  aux_sent = malloc(strlen(sent)+2);
				  aux_sent[0] = s;
				  aux_sent[1] = '\0';
				  strcat(aux_sent,sent + strlen(p->producao));
				  free(sent);
				  sent = malloc(strlen(aux_sent)+1);
				  strcpy(sent,aux_sent);
				  free(aux_sent);
              aux_sent = NULL;

				  aux_hist = hist;
				  hist = hist->proximo;
				  free(aux_hist);
				  break;
	}
		return (estado);
}

void tira_recursividade (tipo_lhs *rec_lhs)
{
   char *novo_nt;
   tipo_rhs *recursivo=NULL, *nao_recursivo=NULL
	    , *aux_rhs, *aux1_rhs, *aux2_rhs;
   tipo_lhs *aux_lhs;

   randomize();
   aux_rhs = rec_lhs->first;
   while(aux_rhs != NULL)
   {
      if(rec_lhs->nt == aux_rhs->producao[0])
      {
	 aux1_rhs = (tipo_rhs *)malloc(sizeof(tipo_rhs));
	 aux1_rhs->producao = malloc(strlen(aux_rhs->producao)+2);
	 strcpy(aux1_rhs->producao,aux_rhs->producao+1);
	 aux1_rhs->proximo = recursivo;
	 recursivo = aux1_rhs;
      }
      else
      {
	 aux1_rhs = (tipo_rhs *)malloc(sizeof(tipo_rhs));
	 aux1_rhs->producao = malloc(strlen(aux_rhs->producao)+1);
	 strcpy(aux1_rhs->producao,aux_rhs->producao);
	 aux1_rhs->proximo = nao_recursivo;
	 nao_recursivo = aux1_rhs;
      }
//      free(aux_rhs->producao);
      aux2_rhs = aux_rhs;
      aux_rhs = aux_rhs->proximo;
      free(aux2_rhs);
   }

   novo_nt = malloc(3);
   while(funcao_de_disponibilidade(novo_nt[0] = 170+random(50))!=NULL);
   novo_nt[1] = '\0';
   aux_lhs = (tipo_lhs *)malloc(sizeof(tipo_lhs));
   aux_lhs->nt = novo_nt[0];

   aux_lhs->first = aux_lhs->last = rec_lhs->last = rec_lhs->first = NULL;
   aux_lhs->max =  rec_lhs->max = 0;
   aux_rhs = nao_recursivo;
   while(aux_rhs != NULL)
   {
     aux2_rhs = (tipo_rhs *)malloc(sizeof(tipo_rhs));
     if (rec_lhs->first == NULL)
     {
	 rec_lhs->first = aux2_rhs;
	 rec_lhs->last = aux2_rhs;
	 aux2_rhs->proximo = NULL;
     }
     else
     {
	 rec_lhs->last->proximo = aux2_rhs;
	 rec_lhs->last = aux2_rhs;
     }
     rec_lhs->max++;
     aux2_rhs->proximo = NULL;
     aux2_rhs->producao = malloc(strlen(aux_rhs->producao)+1);
     strcpy(aux2_rhs->producao,aux_rhs->producao);
     aux_rhs = aux_rhs->proximo;
   }

   aux_rhs = nao_recursivo;
   while(aux_rhs != NULL)
   {
     aux2_rhs = (tipo_rhs *)malloc(sizeof(tipo_rhs));
     rec_lhs->last->proximo = aux2_rhs;
     rec_lhs->max++;
     aux2_rhs->proximo = NULL;
     rec_lhs->last = aux2_rhs;
     aux2_rhs->producao = malloc(strlen(aux_rhs->producao)+strlen(novo_nt)+1);     aux2_rhs->producao = malloc(strlen(aux_rhs->producao)+strlen(novo_nt)+1);
     strcpy(aux2_rhs->producao,aux_rhs->producao);
     strcat(aux2_rhs->producao,novo_nt);
     aux_rhs = aux_rhs->proximo;
   }

   aux_rhs = recursivo;
   while(aux_rhs != NULL)
   {
     aux2_rhs = (tipo_rhs *)malloc(sizeof(tipo_rhs));
     if (aux_lhs->first == NULL)
     {
	 aux_lhs->first = aux2_rhs;
	 aux_lhs->last = aux2_rhs;
	 aux2_rhs->proximo = NULL;
     }
     else
     {
	 aux_lhs->last->proximo = aux2_rhs;
	 aux_lhs->last = aux2_rhs;
     }
     aux_lhs->max++;
     aux2_rhs->producao = malloc(strlen(aux_rhs->producao)+1);
     strcpy(aux2_rhs->producao,aux_rhs->producao);
     aux_rhs = aux_rhs->proximo;
   }

   aux_rhs = recursivo; /* */
   while(aux_rhs != NULL)
   {
     aux2_rhs = (tipo_rhs *)malloc(sizeof(tipo_rhs));
     aux2_rhs->proximo = NULL;
     aux_lhs->last->proximo = aux2_rhs;
     aux_lhs->last = aux2_rhs;
     aux_lhs->max++;
     aux2_rhs->producao = malloc(strlen(aux_rhs->producao)+strlen(novo_nt)+1);
     strcpy(aux2_rhs->producao,aux_rhs->producao);
     strcat(aux2_rhs->producao,novo_nt);
     aux_rhs = aux_rhs->proximo;
   }
   aux_lhs->proximo = rec_lhs->proximo;
   rec_lhs->proximo = aux_lhs;
}

void verifica_recursividade ( void )
{
   tipo_lhs *aux_lhs;
   tipo_rhs *aux_rhs;

   aux_lhs = descritor_lhs.primeiro;
   while(aux_lhs != NULL)
   {
      aux_rhs = aux_lhs->first;
      while(aux_rhs!=NULL)
      {
	 if (aux_lhs->nt == aux_rhs->producao[0])
	     {
	       tira_recursividade(aux_lhs);
	       break;
	     }
	 aux_rhs = aux_rhs->proximo;
      }
      aux_lhs = aux_lhs->proximo;
   }
}