#include <conio.h>
#include <mem.h>
#include <alloc.h>
#include <stdio.h>
/************************************************************************/
#define boolean int
#define false 0
#define true !false

#define branco 0

enum Operador {ParaCima, ParaBaixo, ParaEsquerda, ParaDireita};

#define MaxLin 4
#define MaxCol 4

#define Pedra unsigned int
typedef Pedra Estado [MaxLin][MaxCol];

Estado EstadoInicial =  {1,3,5,7,9,2,4,6,8,10,11,12,13,14,branco,15};
Estado EstadoObjetivo = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,branco};

typedef struct NO {
	Estado estado;
	struct NO *Esquerda, *Direita, *Cima, *Baixo;
	struct NO *Seguinte;
	struct NO *Pai;
	unsigned Profundidade, Avaliacao;
	boolean Folha;
} NO;
/************************************************************************/
void EncontraPedra(Pedra pedra, Estado estado, register unsigned *Linha, register unsigned *Coluna);
boolean AplicaOperador(Estado estado, enum Operador operador);
void AjustaBranco(register Estado EstadoInicial, register Estado EstadoObjetivo);
boolean VerificaParidade(void);
void Avalia(NO *No);
NO *EncontraMenorFolha(void);
boolean ProcuraDuplicata(NO *Raiz, Estado estado);
NO *CriaNo(Estado estado, NO *Pai, unsigned Prof);
void InsereFolhas(NO *RaizTmp);
void ExpandeFolha(NO *No);
void BuscaSolucao(void);
void Troca(register Pedra *x, register Pedra *y);
/************************************************************************/
#define ComparaEstados(a,b) memcmp(a,b,sizeof(Estado)) ? false : true
#define CopiaEstado(a,b)    memcpy(a,b,sizeof(Estado))

NO *Raiz, *Cabeca, *Solucao;

unsigned int Counter=0;

char *Tabua = "*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e"
				  "*\x1e \x1e \x1e \x1e*\x1e \x1e \x1e \x1e*\x1e \x1e \x1e \x1e*\x1e \x1e \x1e \x1e*\x1e"
              "*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e"
				  "*\x1e \x1e \x1e \x1e*\x1e \x1e \x1e \x1e*\x1e \x1e \x1e \x1e*\x1e \x1e \x1e \x1e*\x1e"
              "*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e"
				  "*\x1e \x1e \x1e \x1e*\x1e \x1e \x1e \x1e*\x1e \x1e \x1e \x1e*\x1e \x1e \x1e \x1e*\x1e"
              "*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e"
				  "*\x1e \x1e \x1e \x1e*\x1e \x1e \x1e \x1e*\x1e \x1e \x1e \x1e*\x1e \x1e \x1e \x1e*\x1e"
				  "*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e*\x1e";
/************************************************************************/
void Troca(register Pedra *x, register Pedra *y)
{
	Pedra z=*x;*x=*y;*y=z;
}
/************************************************************************/
void EncontraPedra(Pedra pedra, Estado estado, register unsigned *Linha, register unsigned *Coluna)
{
	for (*Linha=0; *Linha<MaxLin; (*Linha)++)
		for (*Coluna=0; *Coluna<MaxCol; (*Coluna)++)
			if (estado [*Linha][*Coluna]==pedra)
				return;
}
/************************************************************************/
void LeEstado(Estado estado)
{
	register unsigned Linha,Coluna;

	for (Linha=0;Linha<MaxLin;Linha++)
		for (Coluna=0;Coluna<MaxCol;Coluna++)
			cscanf("%u",&estado [Linha][Coluna]);
}
/************************************************************************/
boolean AplicaOperador(Estado estado, enum Operador operador)
{
	unsigned Linha,Coluna;
	EncontraPedra(branco,estado,&Linha,&Coluna);
	switch(operador)
	{
	case ParaCima:
				if (Linha==0) return false;
				Troca(&estado [Linha][Coluna],&estado [Linha-1][Coluna]);
				return true;
	case ParaBaixo:
				if (Linha==MaxLin-1) return false;
				Troca(&estado [Linha][Coluna],&estado [Linha+1][Coluna]);
				return true;
	case ParaEsquerda:
				if (Coluna==0) return false;
				Troca(&estado [Linha][Coluna],&estado [Linha][Coluna-1]);
				return true;
	case ParaDireita:
				if (Coluna==MaxCol-1) return false;
				Troca(&estado [Linha][Coluna],&estado [Linha][Coluna+1]);
				return true;
	default:
				return false;
	};
}
/************************************************************************/
void AjustaBranco(register Estado EstadoInicial, register Estado EstadoObjetivo)
{
	unsigned LinhaInicial,ColunaInicial;
	unsigned LinhaObjetivo,ColunaObjetivo;
	int Linhas,Colunas;

	EncontraPedra(branco,EstadoInicial,&LinhaInicial,&ColunaInicial);
	EncontraPedra(branco,EstadoObjetivo,&LinhaObjetivo,&ColunaObjetivo);

	Linhas=LinhaObjetivo-LinhaInicial;
	if (Linhas)
		if (Linhas>0)
			for(;Linhas;Linhas--) AplicaOperador(EstadoInicial,ParaBaixo);
		else
			for(;Linhas;Linhas++) AplicaOperador(EstadoInicial,ParaCima);

	Colunas=ColunaObjetivo-ColunaInicial;
	if (Colunas)
		if (Colunas>0)
			for(;Colunas;Colunas--) AplicaOperador(EstadoInicial,ParaDireita);
		else
			for(;Colunas;Colunas++) AplicaOperador(EstadoInicial,ParaEsquerda);
}
/************************************************************************/
boolean VerificaParidade(void)
{
	register unsigned Linha,Coluna;
	unsigned Trocas=0;
	Estado EstadoAuxiliar;

	CopiaEstado(EstadoAuxiliar,EstadoInicial);
	AjustaBranco(EstadoAuxiliar,EstadoObjetivo);
	for (Linha=0;Linha<MaxLin;Linha++)
		for (Coluna=0;Coluna<MaxCol;Coluna++)
			if (EstadoAuxiliar [Linha][Coluna] != EstadoObjetivo [Linha][Coluna])
			{
				unsigned LinhaAux,ColunaAux;

				EncontraPedra(EstadoObjetivo [Linha][Coluna],EstadoAuxiliar, &LinhaAux, &ColunaAux);
				Troca(&EstadoAuxiliar [Linha][Coluna],&EstadoAuxiliar [LinhaAux][ColunaAux]);
				Trocas++;
			};
	return (Trocas % 2) ? false : true;
}
/************************************************************************/
unsigned CalculaDistancia(Estado est1,Estado est2, Pedra pedra)
{
	unsigned l1,l2,c1,c2,l,c;

	EncontraPedra(pedra,est1,&l1,&c1);
	EncontraPedra(pedra,est2,&l2,&c2);

	l=l1>l2?l1-l2:l2-l1;
	c=c1>c2?c1-c2:c2-c1;

	return l+c;
}
/************************************************************************/
void Avalia(NO *No)
{
	register unsigned Linha,Coluna;

	No->Avaliacao=0;
	for (Linha=0;Linha<MaxLin;Linha++)
		for (Coluna=0;Coluna<MaxCol;Coluna++)
			if (No->estado [Linha][Coluna] != EstadoObjetivo [Linha][Coluna])
				No->Avaliacao+=CalculaDistancia(No->estado,EstadoObjetivo,No->estado[Linha][Coluna]);
	No->Avaliacao=No->Profundidade*2+No->Avaliacao*5;
}
/************************************************************************/
NO *EncontraMenorFolha(void)
{
	NO *MenorFolha, *Folha;
	int Coluna,Linha;
	for (MenorFolha=Folha=Cabeca;Folha!=NULL;Folha=Folha->Seguinte)
	{
		if (MenorFolha->Avaliacao>Folha->Avaliacao)
			MenorFolha=Folha;
		for(Coluna=0;Coluna<MaxCol;Coluna++)
			for(Linha=0;Linha<MaxLin;Linha++)
			{
				gotoxy(11+Coluna*4,6+Linha*2);
				if (Folha->estado[Linha][Coluna]) cprintf(" %X",Folha->estado[Linha][Coluna]); else cprintf("ÛÛ");
			}
	}

	return MenorFolha;
}
/************************************************************************/
boolean ProcuraDuplicata(NO *Raiz,Estado estado)
{
	boolean Achou=ComparaEstados(Raiz->estado,estado);

	if (Raiz->Esquerda != NULL && !Achou)
		Achou=ProcuraDuplicata(Raiz->Esquerda,estado);
   if (Raiz->Direita != NULL && !Achou)
		Achou=ProcuraDuplicata(Raiz->Direita,estado);
   if (Raiz->Cima != NULL && !Achou)
		Achou=ProcuraDuplicata(Raiz->Cima,estado);
   if (Raiz->Baixo != NULL && !Achou)
		Achou=ProcuraDuplicata(Raiz->Baixo,estado);
	return Achou;
}
/************************************************************************/
NO *CriaNo(Estado estado, NO *Pai, unsigned Prof)
{
	NO *No=(NO *) malloc (sizeof(NO));

	CopiaEstado(No->estado,estado);
	No->Esquerda=No->Direita=No->Cima=No->Baixo=NULL;
	No->Pai=No->Seguinte=Pai;
	No->Profundidade=Prof;
	Avalia(No);
	No->Folha=true;
	return No;
}
/************************************************************************/
void InsereFolhas(NO *RaizTmp)
{
	static NO ** Folha;

	if (RaizTmp==Raiz) Folha = &Cabeca;

	if (!RaizTmp->Folha)
	{
		if (RaizTmp->Esquerda != NULL) InsereFolhas(RaizTmp->Esquerda);
		if (RaizTmp->Direita != NULL) InsereFolhas(RaizTmp->Direita);
		if (RaizTmp->Cima != NULL) InsereFolhas(RaizTmp->Cima);
		if (RaizTmp->Baixo != NULL) InsereFolhas(RaizTmp->Baixo);
	}
	else
		Folha=&((*Folha=RaizTmp)->Seguinte);

	*Folha=RaizTmp->Seguinte;

}
/************************************************************************/
void ExpandeFolha(NO *No)
{
	static Estado estado;
	CopiaEstado(estado,No->estado);
	if (AplicaOperador(estado,ParaEsquerda))
		if (!ProcuraDuplicata(Raiz,estado))
			No->Esquerda=CriaNo(estado,No,No->Profundidade+1);
	CopiaEstado(estado,No->estado);
	if (AplicaOperador(estado,ParaDireita))
		if (!ProcuraDuplicata(Raiz,estado))
			No->Direita=CriaNo(estado,No,No->Profundidade+1);
	CopiaEstado(estado,No->estado);
	if (AplicaOperador(estado,ParaCima))
		if (!ProcuraDuplicata(Raiz,estado))
			No->Cima=CriaNo(estado,No,No->Profundidade+1);
	CopiaEstado(estado,No->estado);
	if (AplicaOperador(estado,ParaBaixo))
		if (!ProcuraDuplicata(Raiz,estado))
			No->Baixo=CriaNo(estado,No,No->Profundidade+1);

	No->Folha=false;

	InsereFolhas(Raiz);

}
/************************************************************************/
void BuscaSolucao(void)
{
	NO *No=EncontraMenorFolha();

	if (!ComparaEstados (No->estado,EstadoObjetivo))
		ExpandeFolha(No);
	else
		Solucao=No;

}
/************************************************************************/
void ImprimePedra(Pedra pedra,unsigned Linha,unsigned Coluna)
{
	gotoxy(11+Coluna*4,6+Linha*2);
	if (pedra) cprintf(" %X",pedra); else cprintf("ÛÛ");
}
/************************************************************************/
void ImprimeEstado(Estado estado)
{
	register unsigned Linha,Coluna;

	for (Linha=0;Linha<MaxLin;Linha++)
		for(Coluna=0;Coluna<MaxCol;Coluna++)
			ImprimePedra(estado[Linha][Coluna],Linha,Coluna);
}
/************************************************************************/
void ImprimeSolucao(NO *No)
{
	if (No->Pai!=NULL) ImprimeSolucao(No->Pai);
	ImprimeEstado(No->estado);
	getch();
}
/************************************************************************/
void main()
{
	Cabeca=Raiz=CriaNo(EstadoInicial,NULL,1);

   clrscr();
	puttext(10,5,10+MaxCol*4,5+MaxLin*2,Tabua);

	if (VerificaParidade())
		if (ComparaEstados(EstadoInicial,EstadoObjetivo))
			Solucao=Raiz;
		else
			while(Solucao==NULL)
			{
				Counter++;
				if (Counter==1766)
					printf("\n\nAGORA!\n\n");
				BuscaSolucao();
			}
	else
		cputs("\nN„o existe solu‡„o.\n");

	if (Solucao!=NULL)
	{
      clrscr();
		puttext(10,5,10+MaxCol*4,5+MaxLin*2,Tabua);
		ImprimeSolucao(Solucao);
	}
}