#include <conio.h>
#include <mem.h>
#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <values.h>
////////////////////////////////////////////////////////////////////////////
#define boolean int
#define FALSE 0
#define TRUE !FALSE
#define BRANCO 0
#define MAXLIN 4
#define MAXCOL 4
#define PEDRA unsigned char
#define ComparaEstados(a,b) memcmp (a, b, sizeof (ESTADO)) ? FALSE : TRUE
#define CopiaEstado(a,b)    memcpy (a, b, sizeof (ESTADO))
////////////////////////////////////////////////////////////////////////////
enum OPERADOR {PARACIM, PARABAI, PARAESQ, PARADIR};
typedef PEDRA ESTADO [MAXLIN][MAXCOL];
typedef struct NO {
	ESTADO Estado;
	struct NO * Esquerda, * Direita, * Cima, * Baixo;
	struct NO * Seguinte;
	struct NO * Pai;
	unsigned Profundidade, Avaliacao;
	boolean Folha;
} NO;
ESTADO EstadoInicial  = {{15,14,13,BRANCO},{12,6,7,9},{8,10,11,5},{4,3,2,1}};
ESTADO EstadoObjetivo = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,BRANCO}};
NO * Raiz, * Cabeca, * Solucao;
//////////////////////////////////////////////////////////////////////////
void Troca (register PEDRA *x, register PEDRA *y)
{
   PEDRA z=*x;
   *x=*y;
   *y=z;
}
////////////////////////////////////////////////////////////////////////////
void EncontraPedra(PEDRA Pedra,ESTADO Estado,register unsigned *Linha,register unsigned *Coluna)
{
   for (*Linha=0;*Linha<MAXLIN;(*Linha)++)
      for (*Coluna=0;*Coluna<MAXCOL;(*Coluna)++)
	      if (Estado[*Linha][*Coluna]==Pedra) return;
}
////////////////////////////////////////////////////////////////////////////
/*void LeEstado(ESTADO Estado)
{
   register unsigned Linha,Coluna;
   for (Linha=0;Linha<MAXLIN;Linha++)
      for (Coluna=0;Coluna<MAXCOL;Coluna++)
	      cscanf ("%u", &Estado[Linha][Coluna]);
}*/
////////////////////////////////////////////////////////////////////////////
boolean AplicaOperador(ESTADO Estado,enum OPERADOR Operador)
{
   unsigned Linha,Coluna;
   EncontraPedra(BRANCO,Estado,&Linha,&Coluna);
   switch (Operador)
   {
      case PARACIM: if (Linha==0) return FALSE;
	                 Troca(&Estado[Linha][Coluna],&Estado[Linha-1][Coluna]);
	                 return TRUE;
      case PARABAI: if (Linha==MAXLIN-1) return FALSE;
	                 Troca(&Estado[Linha][Coluna],&Estado[Linha+1][Coluna]);
	                 return TRUE;
      case PARAESQ: if (Coluna==0) return FALSE;
	                 Troca(&Estado[Linha][Coluna],&Estado[Linha][Coluna-1]);
	                 return TRUE;
      case PARADIR: if (Coluna==MAXCOL-1) return FALSE;
	                 Troca(&Estado[Linha][Coluna],&Estado[Linha][Coluna+1]);
	                 return TRUE;
      default:      return FALSE;
   }
}
////////////////////////////////////////////////////////////////////////////
void AjustaBranco(register ESTADO EstadoInicial,register ESTADO EstadoObjetivo)
{
   unsigned LinhaInicial,ColunaInicial,LinhaObjetivo,ColunaObjetivo;
   int Linhas,Colunas;
   EncontraPedra(BRANCO,EstadoInicial,&LinhaInicial,&ColunaInicial);
   EncontraPedra(BRANCO,EstadoObjetivo,&LinhaObjetivo,&ColunaObjetivo);
   Linhas=LinhaObjetivo-LinhaInicial;
   if (Linhas>0)
     for (;Linhas;Linhas--) AplicaOperador(EstadoInicial,PARABAI);
   else
      for (;Linhas;Linhas++) AplicaOperador(EstadoInicial,PARACIM);
   Colunas=ColunaObjetivo-ColunaInicial;
   if (Colunas>0)
      for (;Colunas;Colunas--) AplicaOperador(EstadoInicial,PARADIR);
   else
	   for (;Colunas;Colunas++) AplicaOperador(EstadoInicial,PARAESQ);
}
////////////////////////////////////////////////////////////////////////////
boolean VerificaParidade(void)
{
   register unsigned Linha,Coluna;
   unsigned Trocas=0,LinhaAux,ColunaAux;
   ESTADO EstadoAuxiliar;
   CopiaEstado(EstadoAuxiliar,EstadoInicial);
   AjustaBranco(EstadoAuxiliar,EstadoObjetivo);
   for (Linha=0;Linha<MAXLIN;Linha++)
      for (Coluna=0;Coluna<MAXCOL;Coluna++)
	      if (EstadoAuxiliar[Linha][Coluna]!=EstadoObjetivo[Linha][Coluna])
	      {
	         EncontraPedra(EstadoObjetivo[Linha][Coluna],EstadoAuxiliar,&LinhaAux,&ColunaAux);
	         Troca(&EstadoAuxiliar[Linha][Coluna],&EstadoAuxiliar[LinhaAux][ColunaAux]);
	         Trocas++;
	      }
   return(Trocas%2)?FALSE:TRUE;
}
////////////////////////////////////////////////////////////////////////////
unsigned CalculaDistancia1(ESTADO Est1,ESTADO Est2,PEDRA Pedra)
{
   unsigned L1,L2,C1,C2,L,C;
   EncontraPedra(Pedra,Est1,&L1,&C1);
   EncontraPedra(Pedra,Est2,&L2,&C2);
   L=L1>L2?L1-L2:L2-L1;
   C=C1>C2?C1-C2:C2-C1;
   return L+C;
}
////////////////////////////////////////////////////////////////////////////
unsigned CalculaDistancia2(ESTADO Est,PEDRA Pedra1,PEDRA Pedra2)
{
   unsigned L1,L2,C1,C2,L,C;
   EncontraPedra(Pedra1,Est,&L1,&C1);
   EncontraPedra(Pedra2,Est,&L2,&C2);
   L=L1>L2?L1-L2:L2-L1;
   C=C1>C2?C1-C2:C2-C1;
   return L+C;
}
////////////////////////////////////////////////////////////////////////////
void Avalia(NO *No)
{
   register unsigned Linha,Coluna;
   int d;
   No->Avaliacao = 0;
   for (Linha=0;Linha<MAXLIN;Linha++)
      for (Coluna=0;Coluna<MAXCOL;Coluna++)
	      if (No->Estado[Linha][Coluna]!=EstadoObjetivo[Linha][Coluna])
	         No->Avaliacao+=CalculaDistancia1(No->Estado,EstadoObjetivo,No->Estado[Linha][Coluna]);
   for (Linha=0;Linha<MAXLIN;Linha++)
      for (Coluna=0;Coluna<MAXCOL;Coluna++)
      {
	      if (Linha>0)
	         if (CalculaDistancia2(No->Estado,EstadoObjetivo[Linha][Coluna],EstadoObjetivo[Linha-1][Coluna])!=1)
	            No->Avaliacao+=3;
	      if (Linha<3)
	         if (CalculaDistancia2(No->Estado,EstadoObjetivo[Linha][Coluna],EstadoObjetivo[Linha+1][Coluna])!=1)
	            No->Avaliacao += 3;
	      if (Coluna>0)
	         if (CalculaDistancia2(No->Estado,EstadoObjetivo[Linha][Coluna],EstadoObjetivo[Linha][Coluna-1])!=1)
	            No->Avaliacao += 3;
	      if (Coluna<3)
	         if (CalculaDistancia2(No->Estado,EstadoObjetivo[Linha][Coluna],EstadoObjetivo[Linha][Coluna+1])!=1)
	            No->Avaliacao += 3;
      }
   No->Avaliacao=No->Profundidade+No->Avaliacao*3;
}
////////////////////////////////////////////////////////////////////////////
void ImprimePedra (PEDRA Pedra,unsigned char Linha,unsigned char Coluna)
{
/*   gotoxy (11+Coluna*4,6+Linha*2);
   if (Pedra) cprintf (" %X ",Pedra); else cprintf ("лл");*/
   asm{
      mov ax,0x200
      xor bx,bx
      mov dh,Coluna
      mul dh,4
      add dh,11
      mov dl,Linha
      mul dl,2
      add dl,6
      int 0x10

      cmp Pedra,0
      je  ImprimeIgual
      mov cx,3
      mov al,219
      jmp ImprimeDiferente
   }
ImprimeIgual:
   asm{
      mov cx,1
      mov al,Pedra
      add Pedra,64
   }
ImprimeDiferente:
   asm{
      mov ah,0xa
      mov bx,0
      int 0x10
   }
}
////////////////////////////////////////////////////////////////////////////
void ImprimeEstado (ESTADO Estado)
{
   register unsigned Linha,Coluna;
   for (Linha=0;Linha<MAXLIN;Linha++)
      for (Coluna=0;Coluna<MAXCOL;Coluna++)
	      ImprimePedra (Estado[Linha][Coluna],Linha,Coluna);
}
////////////////////////////////////////////////////////////////////////////
NO *EncontraMenorFolha(void)
{
   NO *MenorFolha,*Folha;
   for (MenorFolha=Folha=Cabeca;Folha!=NULL;Folha=Folha->Seguinte)
     if (MenorFolha->Avaliacao>Folha->Avaliacao) MenorFolha=Folha;
   ImprimeEstado (MenorFolha->Estado);
   return MenorFolha;
}
////////////////////////////////////////////////////////////////////////////
boolean ProcuraDuplicata (NO *Raiz,ESTADO Estado)
{
   boolean Achou=ComparaEstados (Raiz->Estado,Estado);
   if (Raiz->Esquerda!=NULL && !Achou) Achou=ProcuraDuplicata(Raiz->Esquerda,Estado);
   if (Raiz->Direita!=NULL && !Achou) Achou=ProcuraDuplicata(Raiz->Direita,Estado);
   if (Raiz->Cima!=NULL && !Achou) Achou=ProcuraDuplicata(Raiz->Cima,Estado);
   if (Raiz->Baixo!=NULL && !Achou) Achou=ProcuraDuplicata(Raiz->Baixo,Estado);
   return Achou;
}
////////////////////////////////////////////////////////////////////////////
NO *CriaNo(ESTADO Estado,NO *Pai,unsigned Prof)
{
   NO *No=(NO *)malloc(sizeof(NO));
   if (No==NULL) exit(0);
   CopiaEstado(No->Estado,Estado);
   No->Esquerda=No->Direita=No->Cima=No->Baixo=NULL;
   No->Pai=No->Seguinte=Pai;
   No->Profundidade=Prof;
   Avalia(No);
   No->Folha=TRUE;
   return No;
}
////////////////////////////////////////////////////////////////////////////
void InsereFolhas(NO *RaizTmp)
{
   static NO **Folha;
   if (RaizTmp==Raiz) Folha=&Cabeca;
   if (!RaizTmp->Folha)
   {
     if (RaizTmp->Esquerda!=NULL) InsereFolhas(RaizTmp->Esquerda);
     if (RaizTmp->Direita!=NULL) InsereFolhas(RaizTmp->Direita);
     if (RaizTmp->Cima!=NULL) InsereFolhas(RaizTmp->Cima);
     if (RaizTmp->Baixo!=NULL) InsereFolhas(RaizTmp->Baixo);
   }
   else Folha=&((*Folha=RaizTmp)->Seguinte);
   *Folha=RaizTmp->Seguinte;
}
////////////////////////////////////////////////////////////////////////////
void ExpandeFolha(NO *No)
{
   static ESTADO Estado;
   CopiaEstado(Estado,No->Estado);
   if (AplicaOperador(Estado,PARAESQ))
      if (!ProcuraDuplicata(Raiz,Estado))
	      No->Esquerda=CriaNo(Estado,No,No->Profundidade+1);
   CopiaEstado(Estado,No->Estado);
   if (AplicaOperador(Estado,PARADIR))
      if (!ProcuraDuplicata(Raiz,Estado))
	      No->Direita=CriaNo(Estado,No,No->Profundidade+1);
   CopiaEstado(Estado,No->Estado);
   if (AplicaOperador(Estado,PARACIM))
      if (!ProcuraDuplicata(Raiz,Estado))
	      No->Cima=CriaNo(Estado,No,No->Profundidade+1);
   CopiaEstado(Estado,No->Estado);
   if (AplicaOperador(Estado,PARABAI));
      if (!ProcuraDuplicata(Raiz,Estado));
	      No->Baixo=CriaNo(Estado,No,No->Profundidade+1);
   if (No->Esquerda==NULL && No->Direita==NULL && No->Cima==NULL && No->Baixo==NULL)
      No->Avaliacao=MAXINT;
   No->Folha=FALSE;
   InsereFolhas(Raiz);
}
////////////////////////////////////////////////////////////////////////////
void BuscaSolucao (void)
{
   NO *No=EncontraMenorFolha();
   if (!ComparaEstados(No->Estado,EstadoObjetivo)) ExpandeFolha(No);
   else Solucao=No;
}
////////////////////////////////////////////////////////////////////////////
void ImprimeSolucao(NO *No)
{
   if (No->Pai!=NULL) ImprimeSolucao(No->Pai);
   ImprimeEstado(No->Estado);
   getch();
}
////////////////////////////////////////////////////////////////////////////
void main ()
{
   Cabeca=Raiz=CriaNo(EstadoInicial,NULL,1);
   clrscr();
   if (VerificaParidade ())
      if (ComparaEstados(EstadoInicial,EstadoObjetivo)) Solucao=Raiz;
      else
	      while (Solucao==NULL) BuscaSolucao();
   else cputs("\nNao existe solucao\n");
   puttext(10,5,10+MAXCOL*4,5+MAXLIN*2,Tabua);
   if (Solucao!=NULL) ImprimeSolucao(Solucao);
   getch();
}
