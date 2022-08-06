// Prototypes das funcoes dos modulos do TERMINAL

#ifndef _TERMINAL_
#define _TERMINAL_

// #include <nxt.h>
#include "pulldown.h"
#include "netgloba.h"

// Funcoes em NETWARET.C

int ConectaDesconectaComCentral(void);
void PreparaTransBroadcast(void);
void PreparaRecepcao(void);
void PreparaTransmissao(void);
int Conexao(void);
int ComparaEnderecos(BYTE *networkaddr);
int ReconheceAckDeConexao(void);
void CarregaEnderecoDaCentral(void);
int EnviaPacoteAT_DEL(mensagem pacote,WORD soquete);
void CarregaPacote(mensagem pacote);
int ReconheceAck(void);
int Transmite(void);

// Rotinas ESR

void far cdecl ReceiveESRHandler();
void cdecl AguardaPacote(void);

// Funcoes em INTERFAT.C

int Handler(int errval,int ax,int bp,int si);
void Exit(void);
void Mensagem(char *string);
int CaixaDeDialogo(char *string);
int BuscaConexao(void);
void Menu(void);
int BuscaListaDeProdutos(void);
void ListaItens(void);
void CriaItem(void);
void AjustaItensNosProdutos(void);
void ListaProdutos(int num_item);
int CriaProduto(int num_item);
void EditaInfoProd(struct product_list *q);
void CarregaConfiguracao(void);
void EditaConfiguracao(void);
void IndiceAjuda(void);

// Prototypes de PASSWORD.C

int CmpPassword(unsigned long);

// Prototypes das fucoes que estao nos objetos

void Window(int,int,int,int);
void CloseWindow(void);
int Quadro(int,int,int,int);
int Capc(char*,int,int,int,int,int);
int Botao(int,int,int,int,char*);
char *in_str(int,int,int,char);
int PullDown(menu_info*,char*[]);
void Rolagem(char *);

// Lista encadeada de produtos

struct product_list {
  struct product_list *anterior;
  unsigned long ref_prod;
  unsigned char item_prod[20];
  unsigned char nome_prod[20];
  unsigned qtde_prod;
  unsigned char info_prod[255];
  enum operacoes operacao;
  int mod_flag;
  struct product_list *proximo;
};

// Lista encadeada de itens

struct item_list {
  struct item_list *anterior;
  unsigned char item[20];
  int mod_flag;
  struct product_list *p_list;
  struct item_list *proximo;
};

#endif
