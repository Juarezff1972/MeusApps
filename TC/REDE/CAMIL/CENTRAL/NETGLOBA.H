// Estruturas de dados globais para os programas: TERMINAL e CENTRAL

#ifndef _NETGLOBA_
#define _NETGLOBA_

// Codigos de erro: OK - sucesso, TRANS - falha de transmissao, RECEP - fa-
// lha de recepcao, TIME - timeout, NACK - Nack, NIPX - IPX nao instalado,
// SOCK - soquete nao pode ser aberta, MSGD - mensagem desconhecida.

enum errorcode {OK,TRANS,RECEP,TIME,NACK,NIPX,SOCK,MSGD,NCON};

// Operacoes: atualizacao, novo produto, novo item de produtos, requisicao
// de informacao sobre produtos.

enum operacoes {AT,REQ,DEL};

// Dados da mensagem a ser enviada:

typedef struct {
  unsigned numero;
  unsigned tipo;
  enum operacoes operacao;
  unsigned long ref_prod;
  unsigned char item_prod[20];
  unsigned char nome_prod[20];
  unsigned qtde_prod;
  unsigned char info_prod[255];
} mensagem;

#endif