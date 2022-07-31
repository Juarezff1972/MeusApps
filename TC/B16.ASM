	ifndef	??version
?debug	macro
	endm
$comm	macro	name,dist,size,count
	comm	dist name:BYTE:count*size
	endm
	else
$comm	macro	name,dist,size,count
	comm	dist name[size]:BYTE:count
	endm
	endif
	?debug	S "b16.cpp"
	?debug	C E97D7E1A1D076231362E637070
	?debug	C E90018521815433A5C54435C494E434C5544455C636F6E696F2E68
	?debug	C E90018521815433A5C54435C494E434C5544455C5F646566732E68
	?debug	C E90018521813433A5C54435C494E434C5544455C6D656D2E68
	?debug	C E90018521815433A5C54435C494E434C5544455C5F6E756C6C2E68
	?debug	C E90018521815433A5C54435C494E434C5544455C616C6C6F632E68
	?debug	C E90018521815433A5C54435C494E434C5544455C737464696F2E68
	?debug	C E90018521816433A5C54435C494E434C5544455C7374646C69622E+
	?debug	C 68
	?debug	C E90018521816433A5C54435C494E434C5544455C76616C7565732E+
	?debug	C 68
_TEXT	segment byte public 'CODE'
_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
_DATA	segment word public 'DATA'
_EstadoInicial	label	byte
	db	15
	db	14
	db	13
	db	0
	db	12
	db	6
	db	7
	db	9
	db	8
	db	10
	db	11
	db	5
	db	4
	db	3
	db	2
	db	1
_EstadoObjetivo	label	byte
	db	1
	db	2
	db	3
	db	4
	db	5
	db	6
	db	7
	db	8
	db	9
	db	10
	db	11
	db	12
	db	13
	db	14
	db	15
	db	0
_Raiz	label	word
	db	2 dup (0)
_Cabeca	label	word
	db	2 dup (0)
_Solucao	label	word
	db	2 dup (0)
_DATA	ends
_TEXT	segment byte public 'CODE'
   ;	
   ;	void Troca (register PEDRA *x, register PEDRA *y)
   ;	
	assume	cs:_TEXT
@Troca$qpuct1	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
	mov	si,word ptr [bp+4]
	mov	di,word ptr [bp+6]
   ;	
   ;	{
   ;	   PEDRA z=*x;
   ;	
	mov	al,byte ptr [si]
	mov	byte ptr [bp-1],al
   ;	
   ;	   *x=*y;
   ;	
	mov	al,byte ptr [di]
	mov	byte ptr [si],al
   ;	
   ;	   *y=z;
   ;	
	mov	al,byte ptr [bp-1]
	mov	byte ptr [di],al
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@Troca$qpuct1	endp
   ;	
   ;	void EncontraPedra(PEDRA Pedra,ESTADO Estado,register unsigned *Linha,register unsigned *Coluna)
   ;	
	assume	cs:_TEXT
@EncontraPedra$qucpa4$ucpuit3	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	si,word ptr [bp+8]
	mov	di,word ptr [bp+10]
   ;	
   ;	{
   ;	   for (*Linha=0;*Linha<MAXLIN;(*Linha)++)
   ;	
	mov	word ptr [si],0
	jmp	short @2@226
@2@58:
   ;	
   ;	      for (*Coluna=0;*Coluna<MAXCOL;(*Coluna)++)
   ;	
	mov	word ptr [di],0
	jmp	short @2@170
@2@86:
   ;	
   ;		      if (Estado[*Linha][*Coluna]==Pedra) return;
   ;	
	mov	ax,word ptr [si]
	shl	ax,1
	shl	ax,1
	mov	dx,word ptr [bp+6]
	add	dx,ax
	add	dx,word ptr [di]
	mov	bx,dx
	mov	al,byte ptr [bx]
	cmp	al,byte ptr [bp+4]
	jne	short @2@142
	jmp	short @2@254
@2@142:
	inc	word ptr [di]
@2@170:
	cmp	word ptr [di],4
	jb	short @2@86
	inc	word ptr [si]
@2@226:
	cmp	word ptr [si],4
	jb	short @2@58
@2@254:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
@EncontraPedra$qucpa4$ucpuit3	endp
   ;	
   ;	boolean AplicaOperador(ESTADO Estado,enum OPERADOR Operador)
   ;	
	assume	cs:_TEXT
@AplicaOperador$qpa4$uc8OPERADOR	proc	near
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	   unsigned Linha,Coluna;
   ;	   EncontraPedra(BRANCO,Estado,&Linha,&Coluna);
   ;	
	lea	ax,word ptr [bp-4]
	push	ax
	lea	ax,word ptr [bp-2]
	push	ax
	push	si
	mov	al,0
	push	ax
	call	near ptr @EncontraPedra$qucpa4$ucpuit3
	add	sp,8
   ;	
   ;	   switch (Operador)
   ;	
	mov	bx,word ptr [bp+6]
	cmp	bx,3
	ja	short @3@590
	shl	bx,1
	jmp	word ptr cs:@3@C498[bx]
@3@142:
   ;	
   ;	   {
   ;	      case PARACIM: if (Linha==0) return FALSE;
   ;	
	cmp	word ptr [bp-2],0
	jne	short @3@226
@3@170:
	xor	ax,ax
@3@198:
	jmp	short @3@618
@3@226:
   ;	
   ;				 Troca(&Estado[Linha][Coluna],&Estado[Linha-1][Coluna]);
   ;	
	mov	ax,word ptr [bp-2]
	dec	ax
@3@254:
	shl	ax,1
	shl	ax,1
	mov	dx,si
	add	dx,ax
	add	dx,word ptr [bp-4]
	push	dx
	jmp	short @3@562
@3@282:
   ;	
   ;				 return TRUE;
   ;	      case PARABAI: if (Linha==MAXLIN-1) return FALSE;
   ;	
	cmp	word ptr [bp-2],3
	jne	short @3@338
	jmp	short @3@170
@3@338:
   ;	
   ;				 Troca(&Estado[Linha][Coluna],&Estado[Linha+1][Coluna]);
   ;	
	mov	ax,word ptr [bp-2]
	inc	ax
	jmp	short @3@254
@3@366:
   ;	
   ;				 return TRUE;
   ;	      case PARAESQ: if (Coluna==0) return FALSE;
   ;	
	cmp	word ptr [bp-4],0
	jne	short @3@422
	jmp	short @3@170
@3@422:
   ;	
   ;				 Troca(&Estado[Linha][Coluna],&Estado[Linha][Coluna-1]);
   ;	
	mov	ax,word ptr [bp-4]
	dec	ax
	jmp	short @3@534
@3@450:
   ;	
   ;				 return TRUE;
   ;	      case PARADIR: if (Coluna==MAXCOL-1) return FALSE;
   ;	
	cmp	word ptr [bp-4],3
	jne	short @3@506
	jmp	short @3@170
@3@506:
   ;	
   ;				 Troca(&Estado[Linha][Coluna],&Estado[Linha][Coluna+1]);
   ;	
	mov	ax,word ptr [bp-4]
	inc	ax
@3@534:
	mov	dx,word ptr [bp-2]
	shl	dx,1
	shl	dx,1
	mov	bx,si
	add	bx,dx
	add	bx,ax
	push	bx
@3@562:
	mov	ax,word ptr [bp-2]
	shl	ax,1
	shl	ax,1
	mov	dx,si
	add	dx,ax
	add	dx,word ptr [bp-4]
	push	dx
	call	near ptr @Troca$qpuct1
	pop	cx
	pop	cx
   ;	
   ;				 return TRUE;
   ;	
	mov	ax,1
	jmp	short @3@198
@3@590:
	jmp	short @3@170
@3@618:
   ;	
   ;	      default:      return FALSE;
   ;	   }
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@AplicaOperador$qpa4$uc8OPERADOR	endp
@3@C498	label	word
	dw	@3@142
	dw	@3@282
	dw	@3@366
	dw	@3@450
   ;	
   ;	void AjustaBranco(register ESTADO EstadoInicial,register ESTADO EstadoObjetivo)
   ;	
	assume	cs:_TEXT
@AjustaBranco$qpa4$uct1	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
	push	di
   ;	
   ;	{
   ;	   unsigned LinhaInicial,ColunaInicial,LinhaObjetivo,ColunaObjetivo;
   ;	   int Linhas,Colunas;
   ;	   EncontraPedra(BRANCO,EstadoInicial,&LinhaInicial,&ColunaInicial);
   ;	
	lea	ax,word ptr [bp-4]
	push	ax
	lea	ax,word ptr [bp-2]
	push	ax
	push	word ptr [bp+4]
	mov	al,0
	push	ax
	call	near ptr @EncontraPedra$qucpa4$ucpuit3
	add	sp,8
   ;	
   ;	   EncontraPedra(BRANCO,EstadoObjetivo,&LinhaObjetivo,&ColunaObjetivo);
   ;	
	lea	ax,word ptr [bp-8]
	push	ax
	lea	ax,word ptr [bp-6]
	push	ax
	push	word ptr [bp+6]
	mov	al,0
	push	ax
	call	near ptr @EncontraPedra$qucpa4$ucpuit3
	add	sp,8
   ;	
   ;	   Linhas=LinhaObjetivo-LinhaInicial;
   ;	
	mov	ax,word ptr [bp-6]
	sub	ax,word ptr [bp-2]
	mov	si,ax
   ;	
   ;	   if (Linhas>0)
   ;	
	or	si,si
	jle	short @4@198
	jmp	short @4@142
@4@86:
   ;	
   ;	     for (;Linhas;Linhas--) AplicaOperador(EstadoInicial,PARABAI);
   ;	
	mov	ax,1
	push	ax
	push	word ptr [bp+4]
	call	near ptr @AplicaOperador$qpa4$uc8OPERADOR
	pop	cx
	pop	cx
	dec	si
@4@142:
	or	si,si
	jne	short @4@86
	jmp	short @4@310
@4@198:
	jmp	short @4@282
@4@226:
   ;	
   ;	   else
   ;	      for (;Linhas;Linhas++) AplicaOperador(EstadoInicial,PARACIM);
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [bp+4]
	call	near ptr @AplicaOperador$qpa4$uc8OPERADOR
	pop	cx
	pop	cx
	inc	si
@4@282:
	or	si,si
	jne	short @4@226
@4@310:
   ;	
   ;	   Colunas=ColunaObjetivo-ColunaInicial;
   ;	
	mov	ax,word ptr [bp-8]
	sub	ax,word ptr [bp-4]
	mov	di,ax
   ;	
   ;	   if (Colunas>0)
   ;	
	or	di,di
	jle	short @4@478
	jmp	short @4@422
@4@366:
   ;	
   ;	      for (;Colunas;Colunas--) AplicaOperador(EstadoInicial,PARADIR);
   ;	
	mov	ax,3
	push	ax
	push	word ptr [bp+4]
	call	near ptr @AplicaOperador$qpa4$uc8OPERADOR
	pop	cx
	pop	cx
	dec	di
@4@422:
	or	di,di
	jne	short @4@366
	jmp	short @4@590
@4@478:
	jmp	short @4@562
@4@506:
   ;	
   ;	   else
   ;		   for (;Colunas;Colunas++) AplicaOperador(EstadoInicial,PARAESQ);
   ;	
	mov	ax,2
	push	ax
	push	word ptr [bp+4]
	call	near ptr @AplicaOperador$qpa4$uc8OPERADOR
	pop	cx
	pop	cx
	inc	di
@4@562:
	or	di,di
	jne	short @4@506
@4@590:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@AjustaBranco$qpa4$uct1	endp
   ;	
   ;	boolean VerificaParidade(void)
   ;	
	assume	cs:_TEXT
@VerificaParidade$qv	proc	near
	push	bp
	mov	bp,sp
	sub	sp,22
	push	si
	push	di
   ;	
   ;	{
   ;	   register unsigned Linha,Coluna;
   ;	   unsigned Trocas=0,LinhaAux,ColunaAux;
   ;	
	mov	word ptr [bp-2],0
   ;	
   ;	   ESTADO EstadoAuxiliar;
   ;	   CopiaEstado(EstadoAuxiliar,EstadoInicial);
   ;	
	mov	ax,16
	push	ax
	mov	ax,offset DGROUP:_EstadoInicial
	push	ax
	lea	ax,word ptr [bp-22]
	push	ax
	call	near ptr _memcpy
	add	sp,6
   ;	
   ;	   AjustaBranco(EstadoAuxiliar,EstadoObjetivo);
   ;	
	mov	ax,offset DGROUP:_EstadoObjetivo
	push	ax
	lea	ax,word ptr [bp-22]
	push	ax
	call	near ptr @AjustaBranco$qpa4$uct1
	pop	cx
	pop	cx
   ;	
   ;	   for (Linha=0;Linha<MAXLIN;Linha++)
   ;	
	xor	si,si
	jmp	short @5@226
@5@58:
   ;	
   ;	      for (Coluna=0;Coluna<MAXCOL;Coluna++)
   ;	
	xor	di,di
	jmp	short @5@170
@5@86:
   ;	
   ;		      if (EstadoAuxiliar[Linha][Coluna]!=EstadoObjetivo[Linha][Coluna])
   ;	
	mov	bx,si
	shl	bx,1
	shl	bx,1
	add	bx,di
	lea	ax,word ptr [bp-22]
	add	bx,ax
	mov	al,byte ptr [bx]
	mov	bx,si
	shl	bx,1
	shl	bx,1
	cmp	al,byte ptr DGROUP:_EstadoObjetivo[bx+di]
	je	short @5@142
   ;	
   ;		      {
   ;			 EncontraPedra(EstadoObjetivo[Linha][Coluna],EstadoAuxiliar,&LinhaAux,&ColunaAux);
   ;	
	lea	ax,word ptr [bp-6]
	push	ax
	lea	ax,word ptr [bp-4]
	push	ax
	lea	ax,word ptr [bp-22]
	push	ax
	mov	bx,si
	shl	bx,1
	shl	bx,1
	mov	al,byte ptr DGROUP:_EstadoObjetivo[bx+di]
	push	ax
	call	near ptr @EncontraPedra$qucpa4$ucpuit3
	add	sp,8
   ;	
   ;			 Troca(&EstadoAuxiliar[Linha][Coluna],&EstadoAuxiliar[LinhaAux][ColunaAux]);
   ;	
	mov	ax,word ptr [bp-4]
	shl	ax,1
	shl	ax,1
	add	ax,word ptr [bp-6]
	lea	dx,word ptr [bp-22]
	add	ax,dx
	push	ax
	mov	ax,si
	shl	ax,1
	shl	ax,1
	add	ax,di
	lea	dx,word ptr [bp-22]
	add	ax,dx
	push	ax
	call	near ptr @Troca$qpuct1
	pop	cx
	pop	cx
   ;	
   ;			 Trocas++;
   ;	
	inc	word ptr [bp-2]
@5@142:
	inc	di
@5@170:
	cmp	di,4
	jb	short @5@86
	inc	si
@5@226:
	cmp	si,4
	jb	short @5@58
   ;	
   ;		      }
   ;	   return(Trocas%2)?FALSE:TRUE;
   ;	
	test	word ptr [bp-2],1
	je	short @5@310
	xor	ax,ax
	jmp	short @5@338
@5@310:
	mov	ax,1
@5@338:
	jmp	short @5@366
@5@366:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@VerificaParidade$qv	endp
   ;	
   ;	unsigned CalculaDistancia1(ESTADO Est1,ESTADO Est2,PEDRA Pedra)
   ;	
	assume	cs:_TEXT
@CalculaDistancia1$qpa4$uct1uc	proc	near
	push	bp
	mov	bp,sp
	sub	sp,12
   ;	
   ;	{
   ;	   unsigned L1,L2,C1,C2,L,C;
   ;	   EncontraPedra(Pedra,Est1,&L1,&C1);
   ;	
	lea	ax,word ptr [bp-6]
	push	ax
	lea	ax,word ptr [bp-2]
	push	ax
	push	word ptr [bp+4]
	mov	al,byte ptr [bp+8]
	push	ax
	call	near ptr @EncontraPedra$qucpa4$ucpuit3
	add	sp,8
   ;	
   ;	   EncontraPedra(Pedra,Est2,&L2,&C2);
   ;	
	lea	ax,word ptr [bp-8]
	push	ax
	lea	ax,word ptr [bp-4]
	push	ax
	push	word ptr [bp+6]
	mov	al,byte ptr [bp+8]
	push	ax
	call	near ptr @EncontraPedra$qucpa4$ucpuit3
	add	sp,8
   ;	
   ;	   L=L1>L2?L1-L2:L2-L1;
   ;	
	mov	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp-4]
	jbe	short @6@86
	mov	ax,word ptr [bp-2]
	sub	ax,word ptr [bp-4]
	jmp	short @6@114
@6@86:
	mov	ax,word ptr [bp-4]
	sub	ax,word ptr [bp-2]
@6@114:
	mov	word ptr [bp-10],ax
   ;	
   ;	   C=C1>C2?C1-C2:C2-C1;
   ;	
	mov	ax,word ptr [bp-6]
	cmp	ax,word ptr [bp-8]
	jbe	short @6@170
	mov	ax,word ptr [bp-6]
	sub	ax,word ptr [bp-8]
	jmp	short @6@198
@6@170:
	mov	ax,word ptr [bp-8]
	sub	ax,word ptr [bp-6]
@6@198:
	mov	word ptr [bp-12],ax
   ;	
   ;	   return L+C;
   ;	
	mov	ax,word ptr [bp-10]
	add	ax,word ptr [bp-12]
	jmp	short @6@226
@6@226:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
@CalculaDistancia1$qpa4$uct1uc	endp
   ;	
   ;	unsigned CalculaDistancia2(ESTADO Est,PEDRA Pedra1,PEDRA Pedra2)
   ;	
	assume	cs:_TEXT
@CalculaDistancia2$qpa4$ucucuc	proc	near
	push	bp
	mov	bp,sp
	sub	sp,12
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	   unsigned L1,L2,C1,C2,L,C;
   ;	   EncontraPedra(Pedra1,Est,&L1,&C1);
   ;	
	lea	ax,word ptr [bp-6]
	push	ax
	lea	ax,word ptr [bp-2]
	push	ax
	push	si
	mov	al,byte ptr [bp+6]
	push	ax
	call	near ptr @EncontraPedra$qucpa4$ucpuit3
	add	sp,8
   ;	
   ;	   EncontraPedra(Pedra2,Est,&L2,&C2);
   ;	
	lea	ax,word ptr [bp-8]
	push	ax
	lea	ax,word ptr [bp-4]
	push	ax
	push	si
	mov	al,byte ptr [bp+8]
	push	ax
	call	near ptr @EncontraPedra$qucpa4$ucpuit3
	add	sp,8
   ;	
   ;	   L=L1>L2?L1-L2:L2-L1;
   ;	
	mov	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp-4]
	jbe	short @7@86
	mov	ax,word ptr [bp-2]
	sub	ax,word ptr [bp-4]
	jmp	short @7@114
@7@86:
	mov	ax,word ptr [bp-4]
	sub	ax,word ptr [bp-2]
@7@114:
	mov	word ptr [bp-10],ax
   ;	
   ;	   C=C1>C2?C1-C2:C2-C1;
   ;	
	mov	ax,word ptr [bp-6]
	cmp	ax,word ptr [bp-8]
	jbe	short @7@170
	mov	ax,word ptr [bp-6]
	sub	ax,word ptr [bp-8]
	jmp	short @7@198
@7@170:
	mov	ax,word ptr [bp-8]
	sub	ax,word ptr [bp-6]
@7@198:
	mov	word ptr [bp-12],ax
   ;	
   ;	   return L+C;
   ;	
	mov	ax,word ptr [bp-10]
	add	ax,word ptr [bp-12]
	jmp	short @7@226
@7@226:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@CalculaDistancia2$qpa4$ucucuc	endp
   ;	
   ;	void Avalia(NO *No)
   ;	
	assume	cs:_TEXT
@Avalia$qp2NO	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
   ;	
   ;	{
   ;	   register unsigned Linha,Coluna;
   ;	   int d;
   ;	   No->Avaliacao = 0;
   ;	
	mov	bx,word ptr [bp+4]
	mov	word ptr [bx+30],0
   ;	
   ;	   for (Linha=0;Linha<MAXLIN;Linha++)
   ;	
	xor	si,si
	jmp	short @8@226
@8@58:
   ;	
   ;	      for (Coluna=0;Coluna<MAXCOL;Coluna++)
   ;	
	xor	di,di
	jmp	short @8@170
@8@86:
   ;	
   ;		      if (No->Estado[Linha][Coluna]!=EstadoObjetivo[Linha][Coluna])
   ;	
	mov	ax,si
	shl	ax,1
	shl	ax,1
	mov	bx,word ptr [bp+4]
	add	bx,ax
	mov	al,byte ptr [bx+di]
	mov	bx,si
	shl	bx,1
	shl	bx,1
	cmp	al,byte ptr DGROUP:_EstadoObjetivo[bx+di]
	je	short @8@142
   ;	
   ;			 No->Avaliacao+=CalculaDistancia1(No->Estado,EstadoObjetivo,No->Estado[Linha][Coluna]);
   ;	
	mov	ax,si
	shl	ax,1
	shl	ax,1
	mov	bx,word ptr [bp+4]
	add	bx,ax
	mov	al,byte ptr [bx+di]
	push	ax
	mov	ax,offset DGROUP:_EstadoObjetivo
	push	ax
	push	word ptr [bp+4]
	call	near ptr @CalculaDistancia1$qpa4$uct1uc
	add	sp,6
	mov	bx,word ptr [bp+4]
	add	word ptr [bx+30],ax
@8@142:
	inc	di
@8@170:
	cmp	di,4
	jb	short @8@86
	inc	si
@8@226:
	cmp	si,4
	jb	short @8@58
   ;	
   ;	   for (Linha=0;Linha<MAXLIN;Linha++)
   ;	
	xor	si,si
	jmp	@8@730
@8@282:
   ;	
   ;	      for (Coluna=0;Coluna<MAXCOL;Coluna++)
   ;	
	xor	di,di
	jmp	@8@674
@8@310:
   ;	
   ;	      {
   ;		      if (Linha>0)
   ;	
	or	si,si
	jbe	short @8@394
   ;	
   ;			 if (CalculaDistancia2(No->Estado,EstadoObjetivo[Linha][Coluna],EstadoObjetivo[Linha-1][Coluna])!=1)
   ;	
	mov	bx,si
	dec	bx
	shl	bx,1
	shl	bx,1
	mov	al,byte ptr DGROUP:_EstadoObjetivo[bx+di]
	push	ax
	mov	bx,si
	shl	bx,1
	shl	bx,1
	mov	al,byte ptr DGROUP:_EstadoObjetivo[bx+di]
	push	ax
	push	word ptr [bp+4]
	call	near ptr @CalculaDistancia2$qpa4$ucucuc
	add	sp,6
	cmp	ax,1
	je	short @8@394
   ;	
   ;			    No->Avaliacao+=3;
   ;	
	mov	bx,word ptr [bp+4]
	add	word ptr [bx+30],3
@8@394:
   ;	
   ;		      if (Linha<3)
   ;	
	cmp	si,3
	jae	short @8@478
   ;	
   ;			 if (CalculaDistancia2(No->Estado,EstadoObjetivo[Linha][Coluna],EstadoObjetivo[Linha+1][Coluna])!=1)
   ;	
	mov	bx,si
	inc	bx
	shl	bx,1
	shl	bx,1
	mov	al,byte ptr DGROUP:_EstadoObjetivo[bx+di]
	push	ax
	mov	bx,si
	shl	bx,1
	shl	bx,1
	mov	al,byte ptr DGROUP:_EstadoObjetivo[bx+di]
	push	ax
	push	word ptr [bp+4]
	call	near ptr @CalculaDistancia2$qpa4$ucucuc
	add	sp,6
	cmp	ax,1
	je	short @8@478
   ;	
   ;			    No->Avaliacao += 3;
   ;	
	mov	bx,word ptr [bp+4]
	add	word ptr [bx+30],3
@8@478:
   ;	
   ;		      if (Coluna>0)
   ;	
	or	di,di
	jbe	short @8@562
   ;	
   ;			 if (CalculaDistancia2(No->Estado,EstadoObjetivo[Linha][Coluna],EstadoObjetivo[Linha][Coluna-1])!=1)
   ;	
	mov	ax,di
	dec	ax
	mov	bx,si
	shl	bx,1
	shl	bx,1
	add	bx,ax
	mov	al,byte ptr DGROUP:_EstadoObjetivo[bx]
	push	ax
	mov	bx,si
	shl	bx,1
	shl	bx,1
	mov	al,byte ptr DGROUP:_EstadoObjetivo[bx+di]
	push	ax
	push	word ptr [bp+4]
	call	near ptr @CalculaDistancia2$qpa4$ucucuc
	add	sp,6
	cmp	ax,1
	je	short @8@562
   ;	
   ;			    No->Avaliacao += 3;
   ;	
	mov	bx,word ptr [bp+4]
	add	word ptr [bx+30],3
@8@562:
   ;	
   ;		      if (Coluna<3)
   ;	
	cmp	di,3
	jae	short @8@646
   ;	
   ;			 if (CalculaDistancia2(No->Estado,EstadoObjetivo[Linha][Coluna],EstadoObjetivo[Linha][Coluna+1])!=1)
   ;	
	mov	ax,di
	inc	ax
	mov	bx,si
	shl	bx,1
	shl	bx,1
	add	bx,ax
	mov	al,byte ptr DGROUP:_EstadoObjetivo[bx]
	push	ax
	mov	bx,si
	shl	bx,1
	shl	bx,1
	mov	al,byte ptr DGROUP:_EstadoObjetivo[bx+di]
	push	ax
	push	word ptr [bp+4]
	call	near ptr @CalculaDistancia2$qpa4$ucucuc
	add	sp,6
	cmp	ax,1
	je	short @8@646
   ;	
   ;			    No->Avaliacao += 3;
   ;	
	mov	bx,word ptr [bp+4]
	add	word ptr [bx+30],3
@8@646:
	inc	di
@8@674:
	cmp	di,4
	jae	@@0
	jmp	@8@310
@@0:
	inc	si
@8@730:
	cmp	si,4
	jae	@@1
	jmp	@8@282
@@1:
   ;	
   ;	      }
   ;	   No->Avaliacao=No->Profundidade+No->Avaliacao*3;
   ;	
	mov	bx,word ptr [bp+4]
	mov	ax,word ptr [bx+30]
	mov	dx,3
	imul	dx
	mov	bx,word ptr [bp+4]
	mov	dx,word ptr [bx+28]
	add	dx,ax
	mov	bx,word ptr [bp+4]
	mov	word ptr [bx+30],dx
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
@Avalia$qp2NO	endp
   ;	
   ;	void ImprimePedra (PEDRA Pedra,unsigned char Linha,unsigned char Coluna)
   ;	
	assume	cs:_TEXT
@ImprimePedra$qucucuc	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	   gotoxy (11+Coluna*4,6+Linha*2);
   ;	
	mov	al,byte ptr [bp+6]
	mov	ah,0
	shl	ax,1
	add	ax,6
	push	ax
	mov	al,byte ptr [bp+8]
	mov	ah,0
	shl	ax,1
	shl	ax,1
	add	ax,11
	push	ax
	call	near ptr _gotoxy
	pop	cx
	pop	cx
   ;	
   ;	   if (Pedra) cprintf (" %X ",Pedra); else cprintf ("лл");
   ;	
	cmp	byte ptr [bp+4],0
	je	short @9@86
	mov	al,byte ptr [bp+4]
	mov	ah,0
	push	ax
	mov	ax,offset DGROUP:s@
	push	ax
	call	near ptr _cprintf
	pop	cx
	pop	cx
	jmp	short @9@114
@9@86:
	mov	ax,offset DGROUP:s@+5
	push	ax
	call	near ptr _cprintf
	pop	cx
@9@114:
   ;	
   ;	}
   ;	
	pop	bp
	ret	
@ImprimePedra$qucucuc	endp
   ;	
   ;	void ImprimeEstado (ESTADO Estado)
   ;	
	assume	cs:_TEXT
@ImprimeEstado$qpa4$uc	proc	near
	push	bp
	mov	bp,sp
	sub	sp,4
   ;	
   ;	{
   ;	   register unsigned Linha,Coluna;
   ;	   for (Linha=0;Linha<MAXLIN;Linha++)
   ;	
	mov	word ptr [bp-2],0
	jmp	short @10@198
@10@58:
   ;	
   ;	      for (Coluna=0;Coluna<MAXCOL;Coluna++)
   ;	
	mov	word ptr [bp-4],0
	jmp	short @10@142
@10@86:
   ;	
   ;		      ImprimePedra (Estado[Linha][Coluna],Linha,Coluna);
   ;	
	mov	al,byte ptr [bp-4]
	push	ax
	mov	al,byte ptr [bp-2]
	push	ax
	mov	ax,word ptr [bp-2]
	shl	ax,1
	shl	ax,1
	mov	bx,word ptr [bp+4]
	add	bx,ax
	add	bx,word ptr [bp-4]
	mov	al,byte ptr [bx]
	push	ax
	call	near ptr @ImprimePedra$qucucuc
	add	sp,6
	inc	word ptr [bp-4]
@10@142:
	cmp	word ptr [bp-4],4
	jb	short @10@86
	inc	word ptr [bp-2]
@10@198:
	cmp	word ptr [bp-2],4
	jb	short @10@58
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
@ImprimeEstado$qpa4$uc	endp
   ;	
   ;	NO *EncontraMenorFolha(void)
   ;	
	assume	cs:_TEXT
@EncontraMenorFolha$qv	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
   ;	
   ;	{
   ;	   NO *MenorFolha,*Folha;
   ;	   for (MenorFolha=Folha=Cabeca;Folha!=NULL;Folha=Folha->Seguinte)
   ;	
	mov	ax,word ptr DGROUP:_Cabeca
	mov	si,ax
	mov	di,ax
	jmp	short @11@142
@11@58:
   ;	
   ;	     if (MenorFolha->Avaliacao>Folha->Avaliacao) MenorFolha=Folha;
   ;	
	mov	ax,word ptr [di+30]
	cmp	ax,word ptr [si+30]
	jbe	short @11@114
	mov	di,si
@11@114:
	mov	si,word ptr [si+24]
@11@142:
	or	si,si
	jne	short @11@58
   ;	
   ;	   ImprimeEstado (MenorFolha->Estado);
   ;	
	push	di
	call	near ptr @ImprimeEstado$qpa4$uc
	pop	cx
   ;	
   ;	   return MenorFolha;
   ;	
	mov	ax,di
	jmp	short @11@198
@11@198:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
@EncontraMenorFolha$qv	endp
   ;	
   ;	boolean ProcuraDuplicata (NO *Raiz,ESTADO Estado)
   ;	
	assume	cs:_TEXT
@ProcuraDuplicata$qp2NOpa4$uc	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	   boolean Achou=ComparaEstados (Raiz->Estado,Estado);
   ;	
	mov	ax,16
	push	ax
	push	word ptr [bp+6]
	push	si
	call	near ptr _memcmp
	add	sp,6
	or	ax,ax
	je	short @12@86
	xor	ax,ax
	jmp	short @12@114
@12@86:
	mov	ax,1
@12@114:
	mov	di,ax
   ;	
   ;	   if (Raiz->Esquerda!=NULL && !Achou) Achou=ProcuraDuplicata(Raiz->Esquerda,Estado);
   ;	
	cmp	word ptr [si+16],0
	je	short @12@198
	or	di,di
	jne	short @12@198
	push	word ptr [bp+6]
	push	word ptr [si+16]
	call	near ptr @ProcuraDuplicata$qp2NOpa4$uc
	pop	cx
	pop	cx
	mov	di,ax
@12@198:
   ;	
   ;	   if (Raiz->Direita!=NULL && !Achou) Achou=ProcuraDuplicata(Raiz->Direita,Estado);
   ;	
	cmp	word ptr [si+18],0
	je	short @12@282
	or	di,di
	jne	short @12@282
	push	word ptr [bp+6]
	push	word ptr [si+18]
	call	near ptr @ProcuraDuplicata$qp2NOpa4$uc
	pop	cx
	pop	cx
	mov	di,ax
@12@282:
   ;	
   ;	   if (Raiz->Cima!=NULL && !Achou) Achou=ProcuraDuplicata(Raiz->Cima,Estado);
   ;	
	cmp	word ptr [si+20],0
	je	short @12@366
	or	di,di
	jne	short @12@366
	push	word ptr [bp+6]
	push	word ptr [si+20]
	call	near ptr @ProcuraDuplicata$qp2NOpa4$uc
	pop	cx
	pop	cx
	mov	di,ax
@12@366:
   ;	
   ;	   if (Raiz->Baixo!=NULL && !Achou) Achou=ProcuraDuplicata(Raiz->Baixo,Estado);
   ;	
	cmp	word ptr [si+22],0
	je	short @12@450
	or	di,di
	jne	short @12@450
	push	word ptr [bp+6]
	push	word ptr [si+22]
	call	near ptr @ProcuraDuplicata$qp2NOpa4$uc
	pop	cx
	pop	cx
	mov	di,ax
@12@450:
   ;	
   ;	   return Achou;
   ;	
	mov	ax,di
	jmp	short @12@478
@12@478:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
@ProcuraDuplicata$qp2NOpa4$uc	endp
   ;	
   ;	NO *CriaNo(ESTADO Estado,NO *Pai,unsigned Prof)
   ;	
	assume	cs:_TEXT
@CriaNo$qpa4$ucp2NOui	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	   NO *No=(NO *)malloc(sizeof(NO));
   ;	
	mov	ax,34
	push	ax
	call	near ptr _malloc
	pop	cx
	mov	si,ax
   ;	
   ;	   if (No==NULL) exit(0);
   ;	
	or	si,si
	jne	short @13@86
	xor	ax,ax
	push	ax
	call	near ptr _exit
	pop	cx
@13@86:
   ;	
   ;	   CopiaEstado(No->Estado,Estado);
   ;	
	mov	ax,16
	push	ax
	push	word ptr [bp+4]
	push	si
	call	near ptr _memcpy
	add	sp,6
   ;	
   ;	   No->Esquerda=No->Direita=No->Cima=No->Baixo=NULL;
   ;	
	xor	ax,ax
	mov	word ptr [si+22],ax
	mov	word ptr [si+20],ax
	mov	word ptr [si+18],ax
	mov	word ptr [si+16],ax
   ;	
   ;	   No->Pai=No->Seguinte=Pai;
   ;	
	mov	ax,word ptr [bp+6]
	mov	word ptr [si+24],ax
	mov	word ptr [si+26],ax
   ;	
   ;	   No->Profundidade=Prof;
   ;	
	mov	ax,word ptr [bp+8]
	mov	word ptr [si+28],ax
   ;	
   ;	   Avalia(No);
   ;	
	push	si
	call	near ptr @Avalia$qp2NO
	pop	cx
   ;	
   ;	   No->Folha=TRUE;
   ;	
	mov	word ptr [si+32],1
   ;	
   ;	   return No;
   ;	
	mov	ax,si
	jmp	short @13@114
@13@114:
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
@CriaNo$qpa4$ucp2NOui	endp
   ;	
   ;	void InsereFolhas(NO *RaizTmp)
   ;	
	assume	cs:_TEXT
@InsereFolhas$qp2NO	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	   NO **Folha;
   ;	   if (RaizTmp==Raiz) Folha=&Cabeca;
   ;	
	cmp	si,word ptr DGROUP:_Raiz
	jne	short @14@86
	mov	di,offset DGROUP:_Cabeca
@14@86:
   ;	
   ;	   if (!RaizTmp->Folha)
   ;	
	cmp	word ptr [si+32],0
	jne	short @14@366
   ;	
   ;	   {
   ;	     if (RaizTmp->Esquerda!=NULL) InsereFolhas(RaizTmp->Esquerda);
   ;	
	cmp	word ptr [si+16],0
	je	short @14@170
	push	word ptr [si+16]
	call	near ptr @InsereFolhas$qp2NO
	pop	cx
@14@170:
   ;	
   ;	     if (RaizTmp->Direita!=NULL) InsereFolhas(RaizTmp->Direita);
   ;	
	cmp	word ptr [si+18],0
	je	short @14@226
	push	word ptr [si+18]
	call	near ptr @InsereFolhas$qp2NO
	pop	cx
@14@226:
   ;	
   ;	     if (RaizTmp->Cima!=NULL) InsereFolhas(RaizTmp->Cima);
   ;	
	cmp	word ptr [si+20],0
	je	short @14@282
	push	word ptr [si+20]
	call	near ptr @InsereFolhas$qp2NO
	pop	cx
@14@282:
   ;	
   ;	     if (RaizTmp->Baixo!=NULL) InsereFolhas(RaizTmp->Baixo);
   ;	
	cmp	word ptr [si+22],0
	je	short @14@338
	push	word ptr [si+22]
	call	near ptr @InsereFolhas$qp2NO
	pop	cx
@14@338:
   ;	
   ;	   }
   ;	
	jmp	short @14@394
@14@366:
   ;	
   ;	   else Folha=&((*Folha=RaizTmp)->Seguinte);
   ;	
	mov	ax,si
	mov	word ptr [di],ax
	add	ax,24
	mov	di,ax
@14@394:
   ;	
   ;	   *Folha=RaizTmp->Seguinte;
   ;	
	mov	ax,word ptr [si+24]
	mov	word ptr [di],ax
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
@InsereFolhas$qp2NO	endp
   ;	
   ;	void ExpandeFolha(NO *No)
   ;	
	assume	cs:_TEXT
@ExpandeFolha$qp2NO	proc	near
	push	bp
	mov	bp,sp
	sub	sp,16
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	   ESTADO Estado;
   ;	   CopiaEstado(Estado,No->Estado);
   ;	
	mov	ax,16
	push	ax
	push	si
	lea	ax,word ptr [bp-16]
	push	ax
	call	near ptr _memcpy
	add	sp,6
   ;	
   ;	   if (AplicaOperador(Estado,PARAESQ))
   ;	
	mov	ax,2
	push	ax
	lea	ax,word ptr [bp-16]
	push	ax
	call	near ptr @AplicaOperador$qpa4$uc8OPERADOR
	pop	cx
	pop	cx
	or	ax,ax
	je	short @15@114
   ;	
   ;	      if (!ProcuraDuplicata(Raiz,Estado))
   ;	
	lea	ax,word ptr [bp-16]
	push	ax
	push	word ptr DGROUP:_Raiz
	call	near ptr @ProcuraDuplicata$qp2NOpa4$uc
	pop	cx
	pop	cx
	or	ax,ax
	jne	short @15@114
   ;	
   ;		      No->Esquerda=CriaNo(Estado,No,No->Profundidade+1);
   ;	
	mov	ax,word ptr [si+28]
	inc	ax
	push	ax
	push	si
	lea	ax,word ptr [bp-16]
	push	ax
	call	near ptr @CriaNo$qpa4$ucp2NOui
	add	sp,6
	mov	word ptr [si+16],ax
@15@114:
   ;	
   ;	   CopiaEstado(Estado,No->Estado);
   ;	
	mov	ax,16
	push	ax
	push	si
	lea	ax,word ptr [bp-16]
	push	ax
	call	near ptr _memcpy
	add	sp,6
   ;	
   ;	   if (AplicaOperador(Estado,PARADIR))
   ;	
	mov	ax,3
	push	ax
	lea	ax,word ptr [bp-16]
	push	ax
	call	near ptr @AplicaOperador$qpa4$uc8OPERADOR
	pop	cx
	pop	cx
	or	ax,ax
	je	short @15@198
   ;	
   ;	      if (!ProcuraDuplicata(Raiz,Estado))
   ;	
	lea	ax,word ptr [bp-16]
	push	ax
	push	word ptr DGROUP:_Raiz
	call	near ptr @ProcuraDuplicata$qp2NOpa4$uc
	pop	cx
	pop	cx
	or	ax,ax
	jne	short @15@198
   ;	
   ;		      No->Direita=CriaNo(Estado,No,No->Profundidade+1);
   ;	
	mov	ax,word ptr [si+28]
	inc	ax
	push	ax
	push	si
	lea	ax,word ptr [bp-16]
	push	ax
	call	near ptr @CriaNo$qpa4$ucp2NOui
	add	sp,6
	mov	word ptr [si+18],ax
@15@198:
   ;	
   ;	   CopiaEstado(Estado,No->Estado);
   ;	
	mov	ax,16
	push	ax
	push	si
	lea	ax,word ptr [bp-16]
	push	ax
	call	near ptr _memcpy
	add	sp,6
   ;	
   ;	   if (AplicaOperador(Estado,PARACIM))
   ;	
	xor	ax,ax
	push	ax
	lea	ax,word ptr [bp-16]
	push	ax
	call	near ptr @AplicaOperador$qpa4$uc8OPERADOR
	pop	cx
	pop	cx
	or	ax,ax
	je	short @15@282
   ;	
   ;	      if (!ProcuraDuplicata(Raiz,Estado))
   ;	
	lea	ax,word ptr [bp-16]
	push	ax
	push	word ptr DGROUP:_Raiz
	call	near ptr @ProcuraDuplicata$qp2NOpa4$uc
	pop	cx
	pop	cx
	or	ax,ax
	jne	short @15@282
   ;	
   ;		      No->Cima=CriaNo(Estado,No,No->Profundidade+1);
   ;	
	mov	ax,word ptr [si+28]
	inc	ax
	push	ax
	push	si
	lea	ax,word ptr [bp-16]
	push	ax
	call	near ptr @CriaNo$qpa4$ucp2NOui
	add	sp,6
	mov	word ptr [si+20],ax
@15@282:
   ;	
   ;	   CopiaEstado(Estado,No->Estado);
   ;	
	mov	ax,16
	push	ax
	push	si
	lea	ax,word ptr [bp-16]
	push	ax
	call	near ptr _memcpy
	add	sp,6
   ;	
   ;	   if (AplicaOperador(Estado,PARABAI));
   ;	
	mov	ax,1
	push	ax
	lea	ax,word ptr [bp-16]
	push	ax
	call	near ptr @AplicaOperador$qpa4$uc8OPERADOR
	pop	cx
	pop	cx
	or	ax,ax
	je	short @15@310
@15@310:
   ;	
   ;	      if (!ProcuraDuplicata(Raiz,Estado));
   ;	
	lea	ax,word ptr [bp-16]
	push	ax
	push	word ptr DGROUP:_Raiz
	call	near ptr @ProcuraDuplicata$qp2NOpa4$uc
	pop	cx
	pop	cx
	or	ax,ax
	jne	short @15@338
@15@338:
   ;	
   ;		      No->Baixo=CriaNo(Estado,No,No->Profundidade+1);
   ;	
	mov	ax,word ptr [si+28]
	inc	ax
	push	ax
	push	si
	lea	ax,word ptr [bp-16]
	push	ax
	call	near ptr @CriaNo$qpa4$ucp2NOui
	add	sp,6
	mov	word ptr [si+22],ax
   ;	
   ;	   if (No->Esquerda==NULL && No->Direita==NULL && No->Cima==NULL && No->Baixo==NULL)
   ;	
	cmp	word ptr [si+16],0
	jne	short @15@478
	cmp	word ptr [si+18],0
	jne	short @15@478
	cmp	word ptr [si+20],0
	jne	short @15@478
	cmp	word ptr [si+22],0
	jne	short @15@478
   ;	
   ;	      No->Avaliacao=MAXINT;
   ;	
	mov	word ptr [si+30],32767
@15@478:
   ;	
   ;	   No->Folha=FALSE;
   ;	
	mov	word ptr [si+32],0
   ;	
   ;	   InsereFolhas(Raiz);
   ;	
	push	word ptr DGROUP:_Raiz
	call	near ptr @InsereFolhas$qp2NO
	pop	cx
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@ExpandeFolha$qp2NO	endp
   ;	
   ;	void BuscaSolucao (void)
   ;	
	assume	cs:_TEXT
@BuscaSolucao$qv	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	   NO *No=EncontraMenorFolha();
   ;	
	call	near ptr @EncontraMenorFolha$qv
	mov	si,ax
   ;	
   ;	   if (!ComparaEstados(No->Estado,EstadoObjetivo)) ExpandeFolha(No);
   ;	
	mov	ax,16
	push	ax
	mov	ax,offset DGROUP:_EstadoObjetivo
	push	ax
	push	si
	call	near ptr _memcmp
	add	sp,6
	or	ax,ax
	jne	short @16@86
	mov	al,0
	jmp	short @16@114
@16@86:
	mov	al,1
@16@114:
	or	al,al
	je	short @16@170
	push	si
	call	near ptr @ExpandeFolha$qp2NO
	pop	cx
	jmp	short @16@198
@16@170:
   ;	
   ;	   else Solucao=No;
   ;	
	mov	word ptr DGROUP:_Solucao,si
@16@198:
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
@BuscaSolucao$qv	endp
   ;	
   ;	void ImprimeSolucao(NO *No)
   ;	
	assume	cs:_TEXT
@ImprimeSolucao$qp2NO	proc	near
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	   if (No->Pai!=NULL) ImprimeSolucao(No->Pai);
   ;	
	cmp	word ptr [si+26],0
	je	short @17@86
	push	word ptr [si+26]
	call	near ptr @ImprimeSolucao$qp2NO
	pop	cx
@17@86:
   ;	
   ;	   ImprimeEstado(No->Estado);
   ;	
	push	si
	call	near ptr @ImprimeEstado$qpa4$uc
	pop	cx
   ;	
   ;	   getch();
   ;	
	call	near ptr _getch
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
@ImprimeSolucao$qp2NO	endp
   ;	
   ;	void main ()
   ;	
	assume	cs:_TEXT
_main	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	   Cabeca=Raiz=CriaNo(EstadoInicial,NULL,1);
   ;	
	mov	ax,1
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,offset DGROUP:_EstadoInicial
	push	ax
	call	near ptr @CriaNo$qpa4$ucp2NOui
	add	sp,6
	mov	word ptr DGROUP:_Raiz,ax
	mov	word ptr DGROUP:_Cabeca,ax
   ;	
   ;	   asm{
   ;	      mov ax,3
   ;	
	mov	 ax,3
   ;	
   ;	      int 0x10
   ;	
	int	 010H
   ;	
   ;	   }
   ;	   clrscr();
   ;	
	call	near ptr _clrscr
   ;	
   ;	   if (VerificaParidade ())
   ;	
	call	near ptr @VerificaParidade$qv
	or	ax,ax
	je	short @18@394
   ;	
   ;	      if (ComparaEstados(EstadoInicial,EstadoObjetivo)) Solucao=Raiz;
   ;	
	mov	ax,16
	push	ax
	mov	ax,offset DGROUP:_EstadoObjetivo
	push	ax
	mov	ax,offset DGROUP:_EstadoInicial
	push	ax
	call	near ptr _memcmp
	add	sp,6
	or	ax,ax
	je	short @18@198
	mov	al,0
	jmp	short @18@226
@18@198:
	mov	al,1
@18@226:
	or	al,al
	je	short @18@282
	mov	ax,word ptr DGROUP:_Raiz
	mov	word ptr DGROUP:_Solucao,ax
	jmp	short @18@366
@18@282:
	jmp	short @18@338
@18@310:
   ;	
   ;	      else
   ;		      while (Solucao==NULL) BuscaSolucao();
   ;	
	call	near ptr @BuscaSolucao$qv
@18@338:
	cmp	word ptr DGROUP:_Solucao,0
	je	short @18@310
@18@366:
	jmp	@18@3474
@18@394:
   ;	
   ;	   else
   ;	      asm{
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,13
   ;	
	mov	 al,13
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,10
   ;	
	mov	 al,10
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,78
   ;	
	mov	 al,78
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,132
   ;	
	mov	 al,132
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,111
   ;	
	mov	 al,111
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,32
   ;	
	mov	 al,32
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,101
   ;	
	mov	 al,101
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,120
   ;	
	mov	 al,120
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,105
   ;	
	mov	 al,105
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,115
   ;	
	mov	 al,115
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,116
   ;	
	mov	 al,116
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,101
   ;	
	mov	 al,101
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,32
   ;	
	mov	 al,32
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,115
   ;	
	mov	 al,115
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,111
   ;	
	mov	 al,111
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,108
   ;	
	mov	 al,108
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,117
   ;	
	mov	 al,117
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,135
   ;	
	mov	 al,135
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,132
   ;	
	mov	 al,132
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,111
   ;	
	mov	 al,111
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,13
   ;	
	mov	 al,13
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
   ;	
   ;	         mov ah,0xe
   ;	
	mov	 ah,0eH
   ;	
   ;	         mov al,10
   ;	
	mov	 al,10
   ;	
   ;	         mov bh,0
   ;	
	mov	 bh,0
   ;	
   ;	         mov bl,11
   ;	
	mov	 bl,11
   ;	
   ;	         int 0x10
   ;	
	int	 010H
@18@3474:
   ;	
   ;	      }
   ;	   if (Solucao!=NULL) ImprimeSolucao(Solucao);
   ;	
	cmp	word ptr DGROUP:_Solucao,0
	je	short @18@3530
	push	word ptr DGROUP:_Solucao
	call	near ptr @ImprimeSolucao$qp2NO
	pop	cx
@18@3530:
   ;	
   ;	   asm{
   ;	      mov ax,0x800
   ;	
	mov	 ax,0800H
   ;	
   ;	      int 0x21
   ;	
	int	 021H
   ;	
   ;	   }
   ;	}
   ;	
	pop	bp
	ret	
_main	endp
	?debug	C E9
_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	' %X '
	db	0
	db	-37
	db	-37
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	public	_main
	public	@ImprimeSolucao$qp2NO
	public	@BuscaSolucao$qv
	public	@ExpandeFolha$qp2NO
	public	@InsereFolhas$qp2NO
	public	@CriaNo$qpa4$ucp2NOui
	public	@ProcuraDuplicata$qp2NOpa4$uc
	public	@EncontraMenorFolha$qv
	public	@ImprimeEstado$qpa4$uc
	public	@ImprimePedra$qucucuc
	public	@Avalia$qp2NO
	public	@CalculaDistancia2$qpa4$ucucuc
	public	@CalculaDistancia1$qpa4$uct1uc
	public	@VerificaParidade$qv
	public	@AjustaBranco$qpa4$uct1
	public	@AplicaOperador$qpa4$uc8OPERADOR
	public	@EncontraPedra$qucpa4$ucpuit3
	public	@Troca$qpuct1
	public	_Solucao
	public	_Cabeca
	public	_Raiz
	public	_EstadoObjetivo
	public	_EstadoInicial
	extrn	_exit:near
_atoi	equ	atoi
_abs	equ	abs
	extrn	_malloc:near
	extrn	_memcpy:near
	extrn	_memcmp:near
	extrn	_cprintf:near
	extrn	_getch:near
	extrn	_gotoxy:near
	extrn	_clrscr:near
_s@	equ	s@
	end
