DGROUP   group   _DATA,_BSS
   assume   cs:_TEXT,ds:DGROUP

_DATA   segment word public 'DATA'
_EstadoInicial   label   byte
   db   14,15,13,0,12,6,7,9,8,10,11,5,4,3,2,1
_EstadoObjetivo   label   byte
   db   1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0
_DATA   ends

_BSS   segment word public 'BSS'
_Raiz   label   word
   db   2 dup (?)
_Cabeca   label   word
   db   2 dup (?)
_Solucao   label   word
   db   2 dup (?)
_BSS   ends

_TEXT   segment byte public 'CODE'
;--------------------------------------------------------------------------
_Troca   proc   near
   push         bp
   mov          bp,sp
   push         si
   push         di
   mov          si,word ptr [bp+4]
   mov          di,word ptr [bp+6]
   mov          al,byte ptr [si]
   mov          ah,byte ptr [di]
   xchg         ah,al
   mov          byte ptr [si],al
   mov          byte ptr [di],ah
   pop          di
   pop          si
   pop          bp
   ret
_Troca   endp
;--------------------------------------------------------------------------
_EncontraPedra   proc   near
   push         bp
   mov          bp,sp
   push         si
   push         di
   mov          si,word ptr [bp+8]
   mov          di,word ptr [bp+10]
   mov          byte ptr [si],0
   jmp          @EncontraPedraBool1
@EncontraPedraSent1:
   mov          byte ptr [di],0
   jmp          @EncontraPedraBool2
@EncontraPedraSent2:
   mov          al,byte ptr [si]
   mov          ah,0
   shl          ax,2
   mov          bx,word ptr [bp+6]
   add          bx,ax
   mov          al,byte ptr [di]
   mov          ah,0
   add          bx,ax
   mov          al,byte ptr [bx]
   cmp          al,byte ptr [bp+4]
   je           @EncontraPedraFim
   inc          byte ptr [di]
@EncontraPedraBool2:
   cmp          byte ptr [di],4
   jb           @EncontraPedraSent2
   inc          byte ptr [si]
@EncontraPedraBool1:
   cmp          byte ptr [si],4
   jb           @EncontraPedraSent1
@EncontraPedraFim:
   pop          di
   pop          si
   pop          bp
   ret
_EncontraPedra   endp
;--------------------------------------------------------------------------
_AplicaOperador proc    near
	push         bp
	mov          bp,sp
	sub          sp,4
	push         si
	mov          si,word ptr [bp+4]
	lea          ax,word ptr [bp-4]
	push         ax
	lea          ax,word ptr [bp-2]
	push         ax
	push         si
	xor          al,al
	push         ax
	call         near ptr _EncontraPedra
	add          sp,8
	mov          bx,word ptr [bp+6]
	cmp          bx,3
	ja           @AplicaOperadorThen1
	shl          bx,1
	jmp          word ptr cs:@AplicaOperadorswitch[bx]
@AplicaOperadorCaso1:
	cmp          word ptr [bp-2],0
	jne          @AplicaOperadorElse1
@AplicaOperadorThen1:
	mov          ax,0
	jmp          @AplicaOperadorExit1
@AplicaOperadorElse1:
	mov          ax,word ptr [bp-2]
	dec          ax
@AplicaOperadorColunaP2:
	shl          ax,2
	mov          dx,si
	add          dx,ax
	add          dx,word ptr [bp-4]
	push         dx
	jmp          @AplicaOperadorParam1
@AplicaOperadorCaso2:
	cmp          word ptr [bp-2],3
	jne          @AplicaOperadorElse2
	jmp          @AplicaOperadorThen1
@AplicaOperadorElse2:
	mov          ax,word ptr [bp-2]
	inc          ax
	jmp          @AplicaOperadorColunaP2
@AplicaOperadorCaso3:
	cmp          word ptr [bp-4],0
	jne          @AplicaOperadorElse13
	jmp          @AplicaOperadorThen1
@AplicaOperadorElse13:
	mov          ax,word ptr [bp-4]
	dec          ax
	jmp          @AplicaOperadorParam2
@AplicaOperadorCaso4:
	cmp          word ptr [bp-4],3
	jne          @AplicaOperadorElse14
	jmp          @AplicaOperadorThen1
@AplicaOperadorElse14:
	mov          ax,word ptr [bp-4]
	inc          ax
@AplicaOperadorParam2:
	mov          dx,word ptr [bp-2]
	shl          dx,2
	mov          bx,si
	add          bx,dx
	add          bx,ax
	push         bx
@AplicaOperadorParam1:
	mov          ax,word ptr [bp-2]
	shl          ax,2
	mov          dx,si
	add          dx,ax
	add          dx,word ptr [bp-4]
	push         dx
	call         near ptr _Troca
	pop          cx
	pop          cx
	mov          ax,1
@AplicaOperadorExit1:
	pop          si
	mov          sp,bp
	pop          bp
	ret
_AplicaOperador endp
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@AplicaOperadorswitch       label   word
	dw      @AplicaOperadorCaso1
	dw      @AplicaOperadorCaso2
	dw      @AplicaOperadorCaso3
	dw      @AplicaOperadorCaso4
;--------------------------------------------------------------------------
_AjustaBranco	proc	near
	push	       bp
	mov	       bp,sp
	sub	       sp,8
	push	       si
	push	       di
	lea	       ax,word ptr [bp-4]
	push	       ax
	lea	       ax,word ptr [bp-2]
	push	       ax
	push	       word ptr [bp+4]
	mov	       al,0
	push	       ax
	call	       near ptr _EncontraPedra
	add	       sp,8
	lea	       ax,word ptr [bp-8]
	push	       ax
	lea	       ax,word ptr [bp-6]
	push	       ax
	push	       word ptr [bp+6]
	mov	       al,0
	push	       ax
	call	       near ptr _EncontraPedra
	add	       sp,8
	mov	       ax,word ptr [bp-6]
	sub	       ax,word ptr [bp-2]
	mov	       si,ax
	or	          si,si
	jle	       short @AjustaBrancoElse1
	jmp	       short @AjustaBrancoThen1
@AjustaBrancoFor1:
	mov	       ax,1
	push	       ax
	push	       word ptr [bp+4]
	call	       near ptr _AplicaOperador
	pop	       cx
	pop	       cx
	dec	       si
@AjustaBrancoThen1:
	or	          si,si
	jne	       short @AjustaBrancoFor1
	jmp	       short @AjustaBrancoFimIf1
@AjustaBrancoFor2:
	xor	       ax,ax
	push	       ax
	push	       word ptr [bp+4]
	call	       near ptr _AplicaOperador
	pop	       cx
	pop	       cx
	inc	       si
@AjustaBrancoElse1:
	or	          si,si
	jne	       short @AjustaBrancoFor2
@AjustaBrancoFimIf1:
	mov	       ax,word ptr [bp-8]
	sub	       ax,word ptr [bp-4]
	mov	       di,ax
	or	          di,di
	jle	       short @AjustaBrancoElse2
	jmp	       short @AjustaBrancoThen2
@AjustaBrancoFor3:
	mov	       ax,3
	push	       ax
	push	       word ptr [bp+4]
	call	       near ptr _AplicaOperador
	pop	       cx
	pop	       cx
	dec	       di
@AjustaBrancoThen2:
	or	          di,di
	jne	       short @AjustaBrancoFor3
	jmp	       short @AjustaBrancoFimIf2
@AjustaBrancoFor4:
	mov	       ax,2
	push	       ax
	push	       word ptr [bp+4]
	call	       near ptr _AplicaOperador
	pop	       cx
	pop	       cx
	inc	       di
@AjustaBrancoElse2:
	or	          di,di
	jne	       short @AjustaBrancoFor4
@AjustaBrancoFimIf2:
	pop	       di
	pop	       si
	mov	       sp,bp
	pop	       bp
	ret
_AjustaBranco  endp
;--------------------------------------------------------------------------
_VerificaParidade   proc   near
   push         bp
   mov          bp,sp
   sub          sp,22
   mov          byte ptr [bp-3],0
   mov          ax,16
   push         ax
   mov          ax,offset DGROUP:_EstadoInicial
   push         ax
   lea          ax,word ptr [bp-22]
   push         ax
   call         near ptr _memcpy
   add          sp,6
   mov          ax,offset DGROUP:_EstadoObjetivo
   push         ax
   lea          ax,word ptr [bp-22]
   push         ax
   call         near ptr _AjustaBranco
   pop          cx
   pop          cx
   mov          byte ptr [bp-1],0
   jmp          @VerificaParidadeBool1
@VerificaParidadeFor1:
   mov          byte ptr [bp-2],0
   jmp          @VerificaParidadeBool2
@VerificaParidadeFor2:
   mov          al,byte ptr [bp-1]
   mov          ah,0
   shl          ax,2
   mov          dl,byte ptr [bp-2]
   mov          dh,0
   add          ax,dx
   lea          dx,word ptr [bp-22]
   add          ax,dx
   mov          bx,ax
   mov          al,byte ptr [bx]
   mov          dl,byte ptr [bp-1]
   mov          dh,0
   shl          dx,2
   mov          bl,byte ptr [bp-2]
   mov          bh,0
   add          dx,bx
   mov          bx,dx
   cmp          al,byte ptr DGROUP:_EstadoObjetivo[bx]
   je           @VerificaParidadeElse1
   lea          ax,word ptr [bp-5]
   push         ax
   lea          ax,word ptr [bp-4]
   push         ax
   lea          ax,word ptr [bp-22]
   push         ax
   mov          al,byte ptr [bp-1]
   mov          ah,0
   shl          ax,2
   mov          dl,byte ptr [bp-2]
   mov          dh,0
   add          ax,dx
   mov          bx,ax
   mov          al,byte ptr DGROUP:_EstadoObjetivo[bx]
   push         ax
   call         near ptr _EncontraPedra
   add          sp,8
   mov          al,byte ptr [bp-4]
   mov          ah,0
   shl          ax,2
   mov          dl,byte ptr [bp-5]
   mov          dh,0
   add          ax,dx
   lea          dx,word ptr [bp-22]
   add          ax,dx
   push         ax
   mov          al,byte ptr [bp-1]
   mov          ah,0
   shl          ax,2
   mov          dl,byte ptr [bp-2]
   mov          dh,0
   add          ax,dx
   lea          dx,word ptr [bp-22]
   add          ax,dx
   push         ax
   call         near ptr _Troca
   pop          cx
   pop          cx
   inc          byte ptr [bp-3]
@VerificaParidadeElse1:
   inc          byte ptr [bp-2]
@VerificaParidadeBool2:
   cmp          byte ptr [bp-2],4
   jae          @VerificaParidadeSent1
   jmp          @VerificaParidadeFor2
@VerificaParidadeSent1:
   inc          byte ptr [bp-1]
@VerificaParidadeBool1:
   cmp          byte ptr [bp-1],4
   jae          @VerificaParidadeSent2
   jmp          @VerificaParidadeFor1
@VerificaParidadeSent2:
   mov          al,byte ptr [bp-3]
   xor          ah,ah
   mov          bx,2
   cwd
   idiv         bx
   cmp          dx,0
   je           @VerificaParidadeThen1
   xor          ax,ax
   jmp          @VerificaParidadeFim1
@VerificaParidadeThen1:
   mov          ax,1
@VerificaParidadeFim1:
   mov          sp,bp
   pop          bp
   ret
_VerificaParidade   endp
;--------------------------------------------------------------------------
_CalculaDistancia1  proc   near
   push         bp
   mov          bp,sp
   sub          sp,6
   lea          ax,word ptr [bp-3]
   push         ax
   lea          ax,word ptr [bp-1]
   push         ax
   push         word ptr [bp+4]
   mov          al,byte ptr [bp+8]
   push         ax
   call         near ptr _EncontraPedra
   add          sp,8
   lea          ax,word ptr [bp-4]
   push         ax
   lea          ax,word ptr [bp-2]
   push         ax
   push         word ptr [bp+6]
   mov          al,byte ptr [bp+8]
   push         ax
   call         near ptr _EncontraPedra
   add          sp,8
   mov          al,byte ptr [bp-1]
   cmp          al,byte ptr [bp-2]
   jbe          @CalculaDistancia1Else1
   mov          al,byte ptr [bp-1]
   sub          al,byte ptr [bp-2]
   jmp          @CalculaDistancia1Then1
@CalculaDistancia1Else1:
   mov          al,byte ptr [bp-2]
   sub          al,byte ptr [bp-1]
@CalculaDistancia1Then1:
   mov          byte ptr [bp-5],al
   mov          al,byte ptr [bp-3]
   cmp          al,byte ptr [bp-4]
   jbe          @CalculaDistancia1Else2
   mov          al,byte ptr [bp-3]
   sub          al,byte ptr [bp-4]
   jmp          @CalculaDistancia1Then2
@CalculaDistancia1Else2:
   mov          al,byte ptr [bp-4]
   sub          al,byte ptr [bp-3]
@CalculaDistancia1Then2:
   mov          byte ptr [bp-6],al
   mov          al,byte ptr [bp-5]
   xor          ah,ah
   mov          dl,byte ptr [bp-6]
   xor          dh,dh
   add          ax,dx
   mov          sp,bp
   pop          bp
   ret
_CalculaDistancia1   endp
;--------------------------------------------------------------------------
_CalculaDistancia2   proc   near
   push         bp
   mov          bp,sp
   sub          sp,6
   push         si
   mov          si,word ptr [bp+4]
   lea          ax,word ptr [bp-3]
   push         ax
   lea          ax,word ptr [bp-1]
   push         ax
   push         si
   mov          al,byte ptr [bp+6]
   push         ax
   call         near ptr _EncontraPedra
   add          sp,8
   lea          ax,word ptr [bp-4]
   push         ax
   lea          ax,word ptr [bp-2]
   push         ax
   push         si
   mov          al,byte ptr [bp+8]
   push         ax
   call         near ptr _EncontraPedra
   add          sp,8
   mov          al,byte ptr [bp-1]
   cmp          al,byte ptr [bp-2]
   jbe          @CalculaDistancia2Else1
   mov          al,byte ptr [bp-1]
   sub          al,byte ptr [bp-2]
   jmp          @CalculaDistancia2Then1
@CalculaDistancia2Else1:
   mov          al,byte ptr [bp-2]
   sub          al,byte ptr [bp-1]
@CalculaDistancia2Then1:
   mov          byte ptr [bp-5],al
   mov          al,byte ptr [bp-3]
   cmp          al,byte ptr [bp-4]
   jbe          @CalculaDistancia2Else2
   mov          al,byte ptr [bp-3]
   sub          al,byte ptr [bp-4]
   jmp          @CalculaDistancia2Then2
@CalculaDistancia2Else2:
   mov          al,byte ptr [bp-4]
   sub          al,byte ptr [bp-3]
@CalculaDistancia2Then2:
   mov          byte ptr [bp-6],al
   mov          al,byte ptr [bp-5]
   mov          ah,0
   mov          dl,byte ptr [bp-6]
   mov          dh,0
   add          ax,dx
   pop          si
   mov          sp,bp
   pop          bp
   ret
_CalculaDistancia2   endp
;--------------------------------------------------------------------------
_Avalia   proc   near
   push         bp
   mov          bp,sp
   sub          sp,2
   push         si
   mov          si,word ptr [bp+4]
   mov          word ptr [si+30],0
   mov          byte ptr [bp-1],0
   jmp          @AvaliaBool1
@AvaliaFor1:
   mov          byte ptr [bp-2],0
   jmp          @AvaliaBool2
@AvaliaFor2:
   mov          al,byte ptr [bp-1]
   mov          ah,0
   shl          ax,2
   mov          dl,byte ptr [bp-2]
   mov          dh,0
   add          ax,dx
   mov          bx,ax
   mov          al,byte ptr DGROUP:_EstadoObjetivo[bx]
   mov          dl,byte ptr [bp-1]
   mov          dh,0
   shl          dx,2
   mov          bx,si
   add          bx,dx
   mov          dl,byte ptr [bp-2]
   mov          dh,0
   add          bx,dx
   cmp          al,byte ptr [bx]
   je           @AvaliaElse1
   mov          al,byte ptr [bp-1]
   mov          ah,0
   shl          ax,2
   mov          bx,si
   add          bx,ax
   mov          al,byte ptr [bp-2]
   mov          ah,0
   add          bx,ax
   mov          al,byte ptr [bx]
   push         ax
   mov          ax,offset DGROUP:_EstadoObjetivo
   push         ax
   push         si
   call         near ptr _CalculaDistancia1
   add          sp,6
   add          word ptr [si+30],ax
@AvaliaElse1:
   inc          byte ptr [bp-2]
@AvaliaBool2:
   cmp          byte ptr [bp-2],4
   jb           @AvaliaFor2
   inc          byte ptr [bp-1]
@AvaliaBool1:
   cmp          byte ptr [bp-1],4
   jb           @AvaliaFor1
   mov          byte ptr [bp-1],0
   jmp          @AvaliaBool3
@AvaliaFor3:
   mov          byte ptr [bp-2],0
   jmp          @AvaliaBool4
@AvaliaFor4:
   cmp          byte ptr [bp-1],0
   jbe          @AvaliaElse2
   xor          ax,ax
   mov          bl,byte ptr [bp-1]
   shl          bl,2
   mov          dl,byte ptr [bp-2]
   add          bl,dl
   mov          al,byte ptr DGROUP:_EstadoObjetivo[bx-4]
   push         ax
   mov          al,byte ptr DGROUP:_EstadoObjetivo[bx]
   push         ax
   push         si
   call         near ptr _CalculaDistancia2
   add          sp,6
   cmp          ax,1
   je           @AvaliaElse2
   add          word ptr [si+30],3
@AvaliaElse2:
   cmp          byte ptr [bp-1],3
   jae          @AvaliaElse3
   xor          ax,ax
   mov          bl,byte ptr [bp-1]
   shl          bl,2
   mov          dl,byte ptr [bp-2]
   add          bl,dl
   mov          al,byte ptr DGROUP:_EstadoObjetivo[bx+4]
   push         ax
   mov          al,byte ptr DGROUP:_EstadoObjetivo[bx]
   push         ax
   push         si
   call         near ptr _CalculaDistancia2
   add          sp,6
   cmp          ax,1
   je           @AvaliaElse3
   add          word ptr [si+30],3
@AvaliaElse3:
   cmp          byte ptr [bp-2],0
   jbe          @AvaliaElse4
   xor          ax,ax
   mov          bl,byte ptr [bp-1]
   shl          bl,2
   mov          dl,byte ptr [bp-2]
   add          bl,dl
   mov          al,byte ptr DGROUP:_EstadoObjetivo[bx-1]
   push         ax
   mov          al,byte ptr DGROUP:_EstadoObjetivo[bx]
   push         ax
   push         si
   call         near ptr _CalculaDistancia2
   add          sp,6
   cmp          ax,1
   je           @AvaliaElse4
   add          word ptr [si+30],3
@AvaliaElse4:
   cmp          byte ptr [bp-2],3
   jae          @AvaliaElse5
   xor          ax,ax
   mov          bl,byte ptr [bp-1]
   shl          bl,2
   mov          dl,byte ptr [bp-2]
   add          bl,dl
   mov          al,byte ptr DGROUP:_EstadoObjetivo[bx+1]
   push         ax
   mov          al,byte ptr DGROUP:_EstadoObjetivo[bx]
   push         ax
   push         si
   call         near ptr _CalculaDistancia2
   add          sp,6
   cmp          ax,1
   je           @AvaliaElse5
   add          word ptr [si+30],3
@AvaliaElse5:
   inc          byte ptr [bp-2]
   jmp          @AvaliaBool4
@AvaliaInicFor4:
   jmp          @AvaliaFor4
@AvaliaBool4:
   cmp          byte ptr [bp-2],4
   jb           @AvaliaInicFor4
   inc          byte ptr [bp-1]
   jmp          @AvaliaBool3
@AvaliaInicFor3:
   jmp          @AvaliaFor3
@AvaliaBool3:
   cmp          byte ptr [bp-1],4
   jb           @AvaliaInicFor3
   mov          ax,word ptr [si+30]
   mov          dx,3
   imul         dx
   mov          dx,word ptr [si+28]
   add          dx,ax
   mov          word ptr [si+30],dx
   pop          si
   mov          sp,bp
   pop          bp
   ret
_Avalia   endp
;--------------------------------------------------------------------------
_ImprimePedra   proc   near
   push         bp
   mov          bp,sp
   push         ax
   push         bx
   push         cx
   push         dx
   mov          ax,0200H
   xor          bx,bx
   mov          dl,[bp+8]
   shl          dl,2
   add          dl,11
   mov          dh,[bp+6]
   shl          dh,1
   add          dh,6
   int          010H
   cmp          byte ptr [bp+4],0
   jne          @ImprimePedraIgual
   mov          cx,1
   mov          al,219
   jmp          @ImprimePedraDiferente
@ImprimePedraIgual:
   mov          cx,1
   mov          al,[bp+4]
   add          al,64
@ImprimePedraDiferente:
   mov          ah,0aH
   xor          bx,bx
   int          010H
   pop          dx
   pop          cx
   pop          bx
   pop          ax
   pop          bp
   ret
_ImprimePedra   endp
;--------------------------------------------------------------------------
_ImprimeEstado   proc   near
   push         bp
   mov          bp,sp
   sub          sp,2
   mov          byte ptr [bp-1],0
   jmp          @ImprimeEstadoBool1
@ImprimeEstadoFor1:
   mov          byte ptr [bp-2],0
   jmp          @ImprimeEstadoBool2
@ImprimeEstadoFor2:
   mov          al,byte ptr [bp-2]
   push         ax
   mov          al,byte ptr [bp-1]
   push         ax
   mov          al,byte ptr [bp-1]
   mov          ah,0
   shl          ax,2
   mov          bx,word ptr [bp+4]
   add          bx,ax
   mov          al,byte ptr [bp-2]
   mov          ah,0
   add          bx,ax
   mov          al,byte ptr [bx]
   push         ax
   call         near ptr _ImprimePedra
   add          sp,6
   inc          byte ptr [bp-2]
@ImprimeEstadoBool2:
   cmp          byte ptr [bp-2],4
   jb           @ImprimeEstadoFor2
   inc          byte ptr [bp-1]
@ImprimeEstadoBool1:
   cmp          byte ptr [bp-1],4
   jb           @ImprimeEstadoFor1
   mov          sp,bp
   pop          bp
   ret
_ImprimeEstado   endp
;--------------------------------------------------------------------------
_EncontraMenorFolha   proc   near
   push         bp
   mov          bp,sp
   push         si
   push         di
   mov          ax,word ptr DGROUP:_Cabeca
   mov          si,ax
   mov          di,ax
   jmp          @EncontraMenorFolhaBool1
@EncontraMenorFolhaFor1:
   mov          ax,word ptr [di+30]
   cmp          ax,word ptr [si+30]
   jbe          @EncontraMenorFolhaElse1
   mov          di,si
@EncontraMenorFolhaElse1:
   mov          si,word ptr [si+24]
@EncontraMenorFolhaBool1:
   cmp          si,0
   jne          @EncontraMenorFolhaFor1
   push         di
   call         near ptr _ImprimeEstado
   pop          cx
   mov          ax,di
   pop          di
   pop          si
   pop          bp
   ret
_EncontraMenorFolha   endp
;--------------------------------------------------------------------------
_ProcuraDuplicata   proc   near
   push         bp
   mov          bp,sp
   push         si
   push         di
   mov          si,word ptr [bp+4]
   mov          ax,16
   push         ax
   push         word ptr [bp+6]
   push         si
   call         near ptr _memcmp
   add          sp,6
   or           ax,ax
   je           @ProcuraDuplicataElse1
   xor          ax,ax
   jmp          @ProcuraDuplicataThen1
@ProcuraDuplicataElse1:
   mov          ax,1
@ProcuraDuplicataThen1:
   mov          di,ax
   cmp          word ptr [si+16],0
   je           @ProcuraDuplicataElse2
   or           di,di
   jne          @ProcuraDuplicataElse2
   push         word ptr [bp+6]
   push         word ptr [si+16]
   call         near ptr _ProcuraDuplicata
   pop          cx
   pop          cx
   mov          di,ax
@ProcuraDuplicataElse2:
   cmp          word ptr [si+18],0
   je           @ProcuraDuplicataElse3
   or           di,di
   jne          @ProcuraDuplicataElse3
   push         word ptr [bp+6]
   push         word ptr [si+18]
   call         near ptr _ProcuraDuplicata
   pop          cx
   pop          cx
   mov          di,ax
@ProcuraDuplicataElse3:
   cmp          word ptr [si+20],0
   je           @ProcuraDuplicataElse4
   or           di,di
   jne          @ProcuraDuplicataElse4
   push         word ptr [bp+6]
   push         word ptr [si+20]
   call         near ptr _ProcuraDuplicata
   pop          cx
   pop          cx
   mov          di,ax
@ProcuraDuplicataElse4:
   cmp          word ptr [si+22],0
   je           @ProcuraDuplicataElse5
   or           di,di
   jne          @ProcuraDuplicataElse5
   push         word ptr [bp+6]
   push         word ptr [si+22]
   call         near ptr _ProcuraDuplicata
   pop          cx
   pop          cx
   mov          di,ax
@ProcuraDuplicataElse5:
   mov          ax,di
   pop          di
   pop          si
   pop          bp
   ret
_ProcuraDuplicata   endp
;--------------------------------------------------------------------------
_CriaNo   proc   near
   push         bp
   mov          bp,sp
   push         si
   mov          ax,34
   push         ax
   call         near ptr _malloc
   pop          cx
   mov          si,ax
   or           si,si
   jne          @CriaNoElse1
   xor          ax,ax
   push         ax
   call         near ptr _exit
   pop          cx
@CriaNoElse1:
   mov          ax,16
   push         ax
   push         word ptr [bp+4]
   push         si
   call         near ptr _memcpy
   add          sp,6
   xor          ax,ax
   mov          word ptr [si+22],ax
   mov          word ptr [si+20],ax
   mov          word ptr [si+18],ax
   mov          word ptr [si+16],ax
   mov          ax,word ptr [bp+6]
   mov          word ptr [si+24],ax
   mov          word ptr [si+26],ax
   mov          ax,word ptr [bp+8]
   mov          word ptr [si+28],ax
   push         si
   call         near ptr _Avalia
   pop          cx
   mov          word ptr [si+32],1
   mov          ax,si
   pop          si
   pop          bp
   ret
_CriaNo   endp
;--------------------------------------------------------------------------
_InsereFolhas   proc   near
   push         bp
   mov          bp,sp
   push         si
   push         di
   mov          si,word ptr [bp+4]
   cmp          si,word ptr DGROUP:_Raiz
   jne          @InsereFolhasElse1
   mov          di,offset DGROUP:_Cabeca
@InsereFolhasElse1:
   cmp          word ptr [si+32],0
   jne          @InsereFolhasElse2
   cmp          word ptr [si+16],0
   je           @InsereFolhasElse3
   push         word ptr [si+16]
   call         near ptr _InsereFolhas
   pop          cx
@InsereFolhasElse3:
   cmp          word ptr [si+18],0
   je           @InsereFolhasElse4
   push         word ptr [si+18]
   call         near ptr _InsereFolhas
   pop          cx
@InsereFolhasElse4:
   cmp          word ptr [si+20],0
   je           @InsereFolhasElse5
   push         word ptr [si+20]
   call         near ptr _InsereFolhas
   pop          cx
@InsereFolhasElse5:
   cmp          word ptr [si+22],0
   je           @InsereFolhasElse6
   push         word ptr [si+22]
   call         near ptr _InsereFolhas
   pop          cx
@InsereFolhasElse6:
   jmp          @InsereFolhasFim
@InsereFolhasElse2:
   mov          ax,si
   mov          word ptr [di],ax
   add          ax,24
   mov          di,ax
@InsereFolhasFim:
   mov          ax,word ptr [si+24]
   mov          word ptr [bx],ax
   pop          di
   pop          si
   pop          bp
   ret
_InsereFolhas   endp
;--------------------------------------------------------------------------
_ExpandeFolha   proc   near
   push         bp
   mov          bp,sp
   sub          sp,16
   push         si
   mov          si,word ptr [bp+4]
   mov          ax,16
   push         ax
   push         si
   lea          ax,word ptr [bp-16]
   push         ax
   call         near ptr _memcpy
   add          sp,6
   mov          ax,2
   push         ax
   lea          ax,word ptr [bp-16]
   push         ax
   call         near ptr _AplicaOperador
   pop          cx
   pop          cx
   or           ax,ax
   je           short @ExpandeFolhaElse1
   lea          ax,word ptr [bp-16]
   push         ax
   push         word ptr DGROUP:_Raiz
   call         near ptr _ProcuraDuplicata
   pop          cx
   pop          cx
   or           ax,ax
   jne          short @ExpandeFolhaElse1
   mov          ax,word ptr [si+28]
   inc          ax
   push         ax
   push         si
   lea          ax,word ptr [bp-16]
   push         ax
   call         near ptr _CriaNo
   add          sp,6
   mov          word ptr [si+16],ax
@ExpandeFolhaElse1:
   mov          ax,16
   push         ax
   push         si
   lea          ax,word ptr [bp-16]
   push         ax
   call         near ptr _memcpy
   add          sp,6
   mov          ax,3
   push         ax
   lea          ax,word ptr [bp-16]
   push         ax
   call         near ptr _AplicaOperador
   pop          cx
   pop          cx
   or           ax,ax
   je           short @ExpandeFolhaElse2
   lea          ax,word ptr [bp-16]
   push         ax
   push         word ptr DGROUP:_Raiz
   call         near ptr _ProcuraDuplicata
   pop          cx
   pop          cx
   or           ax,ax
   jne          short @ExpandeFolhaElse2
   mov          ax,word ptr [si+28]
   inc          ax
   push         ax
   push         si
   lea          ax,word ptr [bp-16]
   push         ax
   call         near ptr _CriaNo
   add          sp,6
   mov          word ptr [si+18],ax
@ExpandeFolhaElse2:
   mov          ax,16
   push         ax
   push         si
   lea          ax,word ptr [bp-16]
   push         ax
   call         near ptr _memcpy
   add          sp,6
   xor          ax,ax
   push         ax
   lea          ax,word ptr [bp-16]
   push         ax
   call         near ptr _AplicaOperador
   pop          cx
   pop          cx
   or           ax,ax
   je           short @ExpandeFolhaElse3
   lea          ax,word ptr [bp-16]
   push         ax
   push         word ptr DGROUP:_Raiz
   call         near ptr _ProcuraDuplicata
   pop          cx
   pop          cx
   or           ax,ax
   jne          short @ExpandeFolhaElse3
   mov          ax,word ptr [si+28]
   inc          ax
   push         ax
   push         si
   lea          ax,word ptr [bp-16]
   push         ax
   call         near ptr _CriaNo
   add          sp,6
   mov          word ptr [si+20],ax
@ExpandeFolhaElse3:
   mov          ax,16
   push         ax
   push         si
   lea          ax,word ptr [bp-16]
   push         ax
   call         near ptr _memcpy
   add          sp,6
   mov          ax,1
   push         ax
   lea          ax,word ptr [bp-16]
   push         ax
   call         near ptr _AplicaOperador
   pop          cx
   pop          cx
   or           ax,ax
   lea          ax,word ptr [bp-16]
   push         ax
   push         word ptr DGROUP:_Raiz
   call         near ptr _ProcuraDuplicata
   pop          cx
   pop          cx
   or           ax,ax
   mov          ax,word ptr [si+28]
   inc          ax
   push         ax
   push         si
   lea          ax,word ptr [bp-16]
   push         ax
   call         near ptr _CriaNo
   add          sp,6
   mov          word ptr [si+22],ax
   cmp          word ptr [si+16],0
   jne          short @ExpandeFolhaElse4
   cmp          word ptr [si+18],0
   jne          short @ExpandeFolhaElse4
   cmp          word ptr [si+20],0
   jne          short @ExpandeFolhaElse4
   cmp          word ptr [si+22],0
   jne          short @ExpandeFolhaElse4
   mov          word ptr [si+30],32767
@ExpandeFolhaElse4:
   mov          word ptr [si+32],0
   push         word ptr DGROUP:_Raiz
   call         near ptr _InsereFolhas
   pop          cx
   pop          si
   mov          sp,bp
   pop          bp
   ret
_ExpandeFolha   endp
;--------------------------------------------------------------------------
_BuscaSolucao   proc   near
   push         bp
   mov          bp,sp
   push         si
   call         near ptr _EncontraMenorFolha
   mov          si,ax
   mov          ax,16
   push         ax
   mov          ax,offset DGROUP:_EstadoObjetivo
   push         ax
   push         si
   call         near ptr _memcmp
   add          sp,6
   cmp          ax,0
   jne          @BuscaSolucaoThen1
   mov          al,0
   jmp          @BuscaSolucaoThen2
@BuscaSolucaoThen1:
   mov          al,1
@BuscaSolucaoThen2:
   cmp          al,0
   je           @BuscaSolucaoElse1
   push         si
   call         near ptr _ExpandeFolha
   pop          cx
   jmp          @BuscaSolucaoFim
@BuscaSolucaoElse1:
   mov          word ptr DGROUP:_Solucao,si
@BuscaSolucaoFim:
   pop          si
   pop          bp
   ret
_BuscaSolucao   endp
;--------------------------------------------------------------------------
_ImprimeSolucao   proc   near
   push         bp
   mov          bp,sp
   push         si
   mov          si,word ptr [bp+4]
   cmp          word ptr [si+26],0
   je           @ImprimeSolucaoElse1
   push         word ptr [si+26]
   call         near ptr _ImprimeSolucao
   pop          cx
@ImprimeSolucaoElse1:
   push         si
   call         near ptr _ImprimeEstado
   pop          cx
   push         ax
   mov          ax,0800H
   int          021H
   pop          ax
   pop          si
   pop          bp
   ret
_ImprimeSolucao   endp
;--------------------------------------------------------------------------
_main	proc	near
	push	       bp
	mov	       bp,sp
	mov	       ax,1
	push	       ax
	xor	       ax,ax
	push	       ax
	mov	       ax,offset DGROUP:_EstadoInicial
	push	       ax
	call	       near ptr _CriaNo
	add	       sp,6
	mov	       word ptr DGROUP:_Raiz,ax
	mov	       word ptr DGROUP:_Cabeca,ax
   push         ax
	mov	       ax,3
	int	       010H
   pop          ax
	call	       near ptr _VerificaParidade
	or	          ax,ax
	je	          short @MainElse1
	mov	       ax,16
	push	       ax
	mov	       ax,offset DGROUP:_EstadoObjetivo
	push	       ax
	mov	       ax,offset DGROUP:_EstadoInicial
	push	       ax
	call	       near ptr _memcmp
	add	       sp,6
	or	          ax,ax
	je	          short @MainElse2
	mov	       al,0
	jmp	       short @MainThen2
@MainElse2:
	mov	       al,1
@MainThen2:
	or	          al,al
	je	          short @MainElse3
	mov	       ax,word ptr DGROUP:_Raiz
	mov	       word ptr DGROUP:_Solucao,ax
	jmp	       @MainFimIf1
@MainWhile1:
	call	       near ptr _BuscaSolucao
@MainElse3:
	cmp	       word ptr DGROUP:_Solucao,0
	je	          short @MainWhile1
	jmp	       @MainFimIf1
@MainElse1:
	mov	       ah,0eH
	mov	        al,13
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,10
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,78
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,132
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,111
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,32
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,101
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,120
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,105
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,115
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,116
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,101
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,32
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,115
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,111
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,108
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,117
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,135
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,132
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,111
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,13
	mov	        bh,0
	mov	        bl,11
	int	        010H
	mov	        ah,0eH
	mov	        al,10
	mov	        bh,0
	mov	        bl,11
	int	        010H
@MainFimIf1:
	cmp	        word ptr DGROUP:_Solucao,0
	je	          short @MainElse4
	push	       word ptr DGROUP:_Solucao
	call	       near ptr _ImprimeSolucao
	pop	       cx
@MainElse4:
	mov	       ax,0800H
	int	       021H
	pop	       bp
	ret
_main	endp
;--------------------------------------------------------------------------
_TEXT   ends
   public   _main
   public   _ImprimeSolucao
   public   _BuscaSolucao
   public   _ExpandeFolha
   public   _InsereFolhas
   public   _CriaNo
   public   _ProcuraDuplicata
   public   _EncontraMenorFolha
   public   _ImprimeEstado
   public   _ImprimePedra
   public   _Avalia
   public   _CalculaDistancia2
   public   _CalculaDistancia1
   public   _VerificaParidade
   public   _AjustaBranco
   public   _AplicaOperador
   public   _EncontraPedra
   public   _Troca
   public   _Solucao
   public   _Cabeca
   public   _Raiz
   public   _EstadoObjetivo
   public   _EstadoInicial
   extrn   _malloc:near
   extrn   _exit:near
   extrn   _memcpy:near
   extrn   _memcmp:near
end
