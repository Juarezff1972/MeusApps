// Este arquivo contem prototypes e dados globais das funcoes usadas em
// CENTRAL.PRJ

#ifndef _CENTRAL_
#define _CENTRAL_

// Rotinas de NETWAREC.C

int InicializaConexaoEscuta(void);
void GerenciaFilaDeConexao(void);
void GerenciaFilaDeTimeouts(void);
void AtendeTimeouts(void);
void GerenciaFilaDeRequisicao(void);
void PreparaTransAckConexao(void);
void EnviaNack(struct request_queue *r_queue);
void EnviaAck(struct request_queue *r_queue);
int EnviaPacote(struct request_queue *r_q);
int ReconheceAck(struct request_queue *r_q);
void PreparaTransmissao(void);
void PreparaReceiveConexao(void);
void PreparaReceiveRequisicao(void);
void PreparaReceiveAck(void);
int Transmite(void);

// Rotinas ESR

void far cdecl ConectionESRHandler();
void far cdecl RequestESRHandler();
void far cdecl AckESRHandler();
void cdecl TransmiteAckConexao(void);
void cdecl RecebePacoteAcessoDados(void);
void cdecl AguardaPacote(void);


// Rotinas de INTERFAC.C

void InicializaCentral(void);
void AtualizaConexoes(BYTE *addr,int operacao,int num_msg);
int VerificaConexao(struct request_queue *r_q);
void AtualizaProdutoRequisitado(struct request_queue *r_q);
void ConsultaProdutoRequisitado(struct request_queue *r_q);
void DeletaProdutoRequisitado(struct request_queue *r_q);

// Dados globais: Filas e listas encadeadas

struct conection_queue {
  int Flag;
  BYTE addr[10];
  int num_msg;
};

struct conection_list {
  int numero;
  BYTE addr[10];
  int ultimo_num_msg;
  struct conection_list *proximo;
};

struct request_queue {
  BYTE addr[10];
  unsigned numero;
  unsigned tipo;
  enum operacoes operacao;
  unsigned long ref_prod;
  unsigned char item_prod[20];
  unsigned char nome_prod[20];
  unsigned qtde_prod;
  unsigned char info_prod[255];
};

struct timeout_queue {
  BYTE addr[10];
  unsigned numero;
  unsigned tipo;
  enum operacoes operacao;
  unsigned long ref_prod;
  unsigned char item_prod[20];
  unsigned char nome_prod[20];
  unsigned qtde_prod;
  unsigned char info_prod[255];
};

#endif