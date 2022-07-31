#include <mem.h>
#include <stdlib.h>
#include <values.h>
////////////////////////////////////////////////////////////////////////////
#define ComparaEstados(a,b) memcmp (a, b, sizeof (ESTADO)) ? 0 : 1
#define CopiaEstado(a,b)    memcpy (a, b, sizeof (ESTADO))
////////////////////////////////////////////////////////////////////////////
enum OPERADOR {PARACIM, PARABAI, PARAESQ, PARADIR};
typedef unsigned char ESTADO [4][4];
typedef struct NO {
	ESTADO Estado;
	struct NO * Esquerda, * Direita, * Cima, * Baixo;
	struct NO * Seguinte;
	struct NO * Pai;
	unsigned Profundidade, Avaliacao;
	int Folha;
} NO;
ESTADO EstadoInicial  = {{15,14,13,0},{12,6,7,9},{8,10,11,5},{4,3,2,1}};
ESTADO EstadoObjetivo = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,0}};
NO * Raiz, * Cabeca, * Solucao;
//////////////////////////////////////////////////////////////////////////
void Troca (unsigned char *x, unsigned char *y)
{
   asm{
	   mov     si,word ptr [bp+4]
	   mov     di,word ptr [bp+6]
	   mov     al,byte ptr [si]
      mov     ah,byte ptr [di]
      xchg    ah,al
    	mov     byte ptr [si],al
	   mov     byte ptr [di],ah
   }
}
////////////////////////////////////////////////////////////////////////////
void EncontraPedra(unsigned char Pedra,ESTADO Estado,unsigned char *Linha,unsigned char *Coluna)
{
   asm{
	   mov	si,word ptr [bp+8]
	   mov	di,word ptr [bp+10]
	   mov	byte ptr [si],0
	   jmp	EncontraPedraBool1
   }
EncontraPedraSent1:
   asm{
   	mov	byte ptr [di],0
	   jmp	EncontraPedraBool2
   }
EncontraPedraSent2:
   asm{
	   mov	al,byte ptr [si]
	   mov	ah,0
	   shl	ax,2
	   mov	bx,word ptr [bp+6]
	   add	bx,ax
	   mov	al,byte ptr [di]
	   mov	ah,0
	   add	bx,ax
	   mov	al,byte ptr [bx]
	   cmp	al,byte ptr [bp+4]
	   je	   EncontraPedraFim
	   inc	byte ptr [di]
   }
EncontraPedraBool2:
   asm{
	   cmp	byte ptr [di],4
	   jb	   EncontraPedraSent2
	   inc	byte ptr [si]
   }
EncontraPedraBool1:
   asm{
	   cmp	byte ptr [si],4
	   jb	EncontraPedraSent1
   }
EncontraPedraFim:
}
////////////////////////////////////////////////////////////////////////////
int AplicaOperador(ESTADO Estado,enum OPERADOR Operador)
{
   unsigned char Linha,Coluna;
   EncontraPedra(0,Estado,&Linha,&Coluna);
   switch (Operador)
   {
      case PARACIM: if (Linha==0) return 0;
	                 Troca(&Estado[Linha][Coluna],&Estado[Linha-1][Coluna]);
	                 return 1;
      case PARABAI: if (Linha==4-1) return 0;
	                 Troca(&Estado[Linha][Coluna],&Estado[Linha+1][Coluna]);
	                 return 1;
      case PARAESQ: if (Coluna==0) return 0;
	                 Troca(&Estado[Linha][Coluna],&Estado[Linha][Coluna-1]);
	                 return 1;
      case PARADIR: if (Coluna==4-1) return 0;
	                 Troca(&Estado[Linha][Coluna],&Estado[Linha][Coluna+1]);
	                 return 1;
      default:      return 0;
   }
}
////////////////////////////////////////////////////////////////////////////
void AjustaBranco(ESTADO EstadoInicial,ESTADO EstadoObjetivo)
{
   asm{
      sub     sp,8
	   push    si
	   push    di
	   lea     ax,word ptr [bp-4]
	   push    ax
	   lea     ax,word ptr [bp-2]
	   push    ax
	   push    word ptr [bp+4]
	   mov     al,0
	   push    ax
	   call    near ptr _EncontraPedra
	   add     sp,8
	   lea     ax,word ptr [bp-8]
	   push    ax
	   lea     ax,word ptr [bp-6]
	   push    ax
	   push    word ptr [bp+6]
	   mov     al,0
	   push    ax
   	call    near ptr _EncontraPedra
	   add     sp,8
	   mov     ax,word ptr [bp-6]
	   sub     ax,word ptr [bp-2]
	   mov     si,ax
	   cmp     ax,0
	   jle     short AjustaBrancoElse1
	   jmp     short AjustaBrancoThen1
   }
AjustaBrancoFor1:
   asm{
	   mov     ax,1
	   push    ax
	   push    word ptr [bp+4]
	   call    near ptr _AplicaOperador
	   pop     cx
	   pop     cx
	   dec     si
   }
AjustaBrancoThen1:
   asm{
 	   or      si,si
	   jne     short AjustaBrancoFor1
	   jmp     short AjustaBrancoFimFor1
   }
AjustaBrancoFor2:
   asm{
	   xor     ax,ax
	   push    ax
	   push    word ptr [bp+4]
	   call    near ptr _AplicaOperador
	   pop     cx
	   pop     cx
	   inc     si
   }
AjustaBrancoElse1:
   asm{
	   or      si,si
	   jne     short AjustaBrancoFor2
   }
AjustaBrancoFimFor1:
   asm{
	   mov     ax,word ptr [bp-8]
	   sub     ax,word ptr [bp-4]
	   mov     di,ax
	   or      di,di
	   jle     short AjustaBrancoElse2
	   jmp     short AjustaBrancoThen2
   }
AjustaBrancoFor3:
   asm{
   	mov     ax,3
	   push    ax
	   push    word ptr [bp+4]
	   call    near ptr _AplicaOperador
	   pop     cx
	   pop     cx
	   dec     di
   }
AjustaBrancoThen2:
   asm{
	   or      di,di
	   jne     short AjustaBrancoFor3
	   jmp     short AjustaBrancoFim
   }
AjustaBrancoFor4:
   asm{
	   mov     ax,2
    	push    ax
	   push    word ptr [bp+4]
    	call    near ptr _AplicaOperador
	   pop     cx
	   pop     cx
	   inc     di
   }
AjustaBrancoElse2:
   asm{
   	or      di,di
	   jne     short AjustaBrancoFor4
   }
AjustaBrancoFim:
   asm{
	   pop     di
	   pop     si
	   mov     sp,bp
   }
}
////////////////////////////////////////////////////////////////////////////
int VerificaParidade(void)
{
   asm{
      sub	sp,22
   	mov	byte ptr [bp-3],0
	   mov	ax,16
	   push	ax
	   mov	ax,offset DGROUP:_EstadoInicial
	   push	ax
	   lea	ax,word ptr [bp-22]
	   push	ax
	   call	near ptr _memcpy
	   add	sp,6
	   mov	ax,offset DGROUP:_EstadoObjetivo
	   push	ax
	   lea	ax,word ptr [bp-22]
	   push	ax
	   call	near ptr _AjustaBranco
	   pop	cx
	   pop	cx
	   mov	byte ptr [bp-1],0
	   jmp	VerificaParidadeBool1
VerificaParidadeFor1:
	   mov	byte ptr [bp-2],0
	   jmp	VerificaParidadeBool2
VerificaParidadeFor2:
	   mov	al,byte ptr [bp-1]
	   mov	ah,0
	   shl	ax,2
	   mov	dl,byte ptr [bp-2]
	   mov	dh,0
	   add	ax,dx
	   lea	dx,word ptr [bp-22]
	   add	ax,dx
	   mov	bx,ax
	   mov	al,byte ptr [bx]
	   mov	dl,byte ptr [bp-1]
	   mov	dh,0
	   shl	dx,2
	   mov	bl,byte ptr [bp-2]
	   mov	bh,0
	   add	dx,bx
	   mov	bx,dx
	   cmp	al,byte ptr DGROUP:_EstadoObjetivo[bx]
	   je 	VerificaParidadeElse1
	   lea	ax,word ptr [bp-5]
	   push	ax
	   lea	ax,word ptr [bp-4]
	   push	ax
	   lea	ax,word ptr [bp-22]
	   push	ax
	   mov	al,byte ptr [bp-1]
	   mov	ah,0
	   shl	ax,2
	   mov	dl,byte ptr [bp-2]
	   mov	dh,0
	   add	ax,dx
	   mov	bx,ax
	   mov	al,byte ptr DGROUP:_EstadoObjetivo[bx]
	   push	ax
	   call	near ptr _EncontraPedra
	   add	sp,8
	   mov	al,byte ptr [bp-4]
	   mov	ah,0
	   shl	ax,2
   	mov	dl,byte ptr [bp-5]
	   mov	dh,0
	   add	ax,dx
	   lea	dx,word ptr [bp-22]
	   add	ax,dx
	   push	ax
	   mov	al,byte ptr [bp-1]
	   mov	ah,0
	   shl	ax,2
	   mov	dl,byte ptr [bp-2]
	   mov	dh,0
	   add	ax,dx
	   lea	dx,word ptr [bp-22]
	   add	ax,dx
	   push	ax
	   call	near ptr _Troca
	   pop	cx
	   pop	cx
	   inc	byte ptr [bp-3]
VerificaParidadeElse1:
	   inc	byte ptr [bp-2]
VerificaParidadeBool2:
	   cmp	byte ptr [bp-2],4
	   jae	VerificaParidadeSent1
	   jmp	VerificaParidadeFor2
VerificaParidadeSent1:
	   inc	byte ptr [bp-1]
VerificaParidadeBool1:
	   cmp	byte ptr [bp-1],4
	   jae	VerificaParidadeSent2
	   jmp	VerificaParidadeFor1
VerificaParidadeSent2:
	   mov	al,byte ptr [bp-3]
	   xor	ah,ah
	   mov	bx,2
	   cwd
	   idiv	bx
	   cmp   dx,0
	   je 	short VerificaParidadeThen1
	   xor	ax,ax
	   jmp	short VerificaParidadeFim1
VerificaParidadeThen1:
	   mov	ax,1
VerificaParidadeFim1:
	   mov	sp,bp
   }
}
////////////////////////////////////////////////////////////////////////////
unsigned CalculaDistancia1(ESTADO Est1,ESTADO Est2,unsigned char Pedra)
{
   unsigned char L1,L2,C1,C2,L,C;
   EncontraPedra(Pedra,Est1,&L1,&C1);
   EncontraPedra(Pedra,Est2,&L2,&C2);
   L=L1>L2?L1-L2:L2-L1;
   C=C1>C2?C1-C2:C2-C1;
   return L+C;
}
////////////////////////////////////////////////////////////////////////////
unsigned CalculaDistancia2(ESTADO Est,unsigned char Pedra1,unsigned char Pedra2)
{
   unsigned char L1,L2,C1,C2,L,C;
   EncontraPedra(Pedra1,Est,&L1,&C1);
   EncontraPedra(Pedra2,Est,&L2,&C2);
   L=L1>L2?L1-L2:L2-L1;
   C=C1>C2?C1-C2:C2-C1;
   return L+C;
}
////////////////////////////////////////////////////////////////////////////
void Avalia(NO *No)
{
   unsigned char Linha,Coluna;
   int d;
   No->Avaliacao = 0;
   for (Linha=0;Linha<4;Linha++)
      for (Coluna=0;Coluna<4;Coluna++)
	      if (No->Estado[Linha][Coluna]!=EstadoObjetivo[Linha][Coluna])
	         No->Avaliacao+=CalculaDistancia1(No->Estado,EstadoObjetivo,No->Estado[Linha][Coluna]);
   for (Linha=0;Linha<4;Linha++)
      for (Coluna=0;Coluna<4;Coluna++)
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
void ImprimePedra (unsigned char Pedra,unsigned char Linha,unsigned char Coluna)
{
   asm{
      mov ax,0x200
      xor bx,bx
      mov dl,Coluna
      shl dl,2
      add dl,11
      mov dh,Linha
      shl dh,1
      add dh,6
      int 0x10
      cmp Pedra,0
      jne ImprimePedraIgual
      mov cx,1
      mov al,219
      jmp ImprimePedraDiferente
   }
ImprimePedraIgual:
   asm{
      mov cx,1
      mov al,Pedra
      add al,64
   }
ImprimePedraDiferente:
   asm{
      mov ah,0xa
      mov bx,0
      int 0x10
   }
}
////////////////////////////////////////////////////////////////////////////
void ImprimeEstado (ESTADO Estado)
{
   unsigned char Linha,Coluna;
   for (Linha=0;Linha<4;Linha++)
      for (Coluna=0;Coluna<4;Coluna++)
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
int ProcuraDuplicata (NO *Raiz,ESTADO Estado)
{
   int Achou=ComparaEstados (Raiz->Estado,Estado);
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
   No->Folha=1;
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
   No->Folha=0;
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
   asm{
      mov ax,0x800
      int 0x21
   }
}
////////////////////////////////////////////////////////////////////////////
void main ()
{
   char string[]="  Nao existe solu‡„o  $";
   Cabeca=Raiz=CriaNo(EstadoInicial,NULL,1);
   asm{
      mov ax,3
      int 0x10
   }
   if (VerificaParidade ())
      if (ComparaEstados(EstadoInicial,EstadoObjetivo)) Solucao=Raiz;
      else
	      while (Solucao==NULL) BuscaSolucao();
   else
      asm{
         push ds
         mov ax,0x900
         mov dx,OFFSET string
         mov ax,SEG string
         mov ds,ax
         int 0x21
         pop ds
      }
   if (Solucao!=NULL) ImprimeSolucao(Solucao);
   asm{
      mov ax,0x800
      int 0x21
   }
}
