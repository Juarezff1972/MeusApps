cseg    segment para public 'CODE'
driver  proc far
	assume cs:cseg,ds:cseg,es:cseg
	dd      -1
	dw      32768
	dw      dev_strategy
	dw      dev_interrupt
	db      "PASSWORD"
	rh_address      dd      1 dup ( ? )
	rh_offset       equ     word ptr rh_address
	rh_segment      equ     word ptr rh_address + 2
	db      13,10,"Entre com a senha:",13,10,0
dev_strategy     proc    far
		mov     cs:rh_offset,bx
		mov     cs:rh_segment,es
		retf
dev_strategy     endp
dev_interrupt   proc    far
		pushf
		push    ax
		push    si
      push    di
		push    ds
		push    bp
		push    cs
		 pop    ds
		 les    di,rh_address
		 cmp    es:byte ptr [di+2],0
		 jnz    parte1
		 push   di
		 push   es
		 call   parte2
		 pop    es
		 pop    di
		 mov    es:byte ptr [di+13],0
		 mov    es:word ptr [di+14],0
		 mov    es:word ptr [di+16],cs
parte1:
		 mov    es:word ptr [di+3],256
		pop    bp
		pop    ds
		pop    di
		pop    si
		pop    ax
		popf
		retf
dev_interrupt   endp
initialize proc near
parte2:
		 mov    si,22
		 cld
parte4:
		 lodsb
		 or     al,al
		 jz     parte3
		 mov    ah,14
		 int    16
		 jmp    parte4
parte3:
		 jmp parte5

sen1   db     "BCDEFGHIJKLMNOPQRSTU"
sen2   db     "bcdefghijklmnopqrstu"
sen3   db     "12345678901234567890"
sen4   db     "                    "

parte5:
		 mov     bp,0
		 push    cs
		 pop     ds
parte6:
		 mov     al,byte ptr [sen1+bp]
		 dec     al
		 mov     byte ptr [sen1+bp],al
		 inc     bp
		 cmp     bp,20
		 jne     parte6
		 mov     bp,0
parte7:
		 mov     al,byte ptr [sen2+bp]
		 dec     al
		 mov     byte ptr [sen2+bp],al
		 inc     bp
		 cmp     bp,20
		 jne     parte7
		 mov     bp,0
parte8:
		 mov     al,byte ptr [sen3+bp]
		 dec     al
		 mov     byte ptr [sen3+bp],al
		 inc     bp
		 cmp     bp,20
		 jne     parte8
		 mov     bp,0
		 xor     dx,dx
parte10:
		 mov     ah,0
		 int     22
		 cmp     al,13
		 je      parte9
		 push    ax
		 push    bx
		 push    cx
		 push    dx
		 mov     ah,14
		 mov     al,"*"
		 xor     bx,bx
		 mov     cx,1
		 int     16
		 pop     dx
		 pop     cx
		 pop     bx
		 pop     ax
		 mov     byte ptr [sen4+bp],al
		 inc     bp
		 cmp     bp,20
		 je      parte9
		 inc     bx
		 jmp     parte10
parte9:
		 push    cs
		 pop     ds
		 push    cs
		 pop     es
		 mov     si,0
parte11:
		 mov     bh,byte ptr [sen1+si]
		 mov     bl,byte ptr [sen4+si]
		 inc     si
		 cmp     si,20
		 je      parte16
		 cmp     bh,bl
		 je      parte11
parte13:
		 mov     si,0
parte12:
		 mov     bh,byte ptr [sen2+si]
		 mov     bl,byte ptr [sen4+si]
		 inc     si
		 cmp     si,20
		 je      parte16
		 cmp     bh,bl
		 je      parte12
parte14:
		 mov     si,0
parte15:
		 mov     bh,byte ptr [sen3+si]
		 mov     bl,byte ptr [sen4+si]
		 inc     si
		 cmp     si,20
		 je      parte16
		 cmp     bh,bl
		 je      parte15
parte17:
		 inc     dl
		 cmp     dl,4
		 je      parte18
		 push    ax
		 push    bx
		 push    cx
		 push    dx
		 mov     ah,6
		 mov     al,4
		 mov     bh,7
		 mov     ch,0
		 mov     cl,0
		 mov     dh,5
		 mov     dl,70
		 int     16
		 mov     ah,2
		 mov     bh,0
		 mov     dx,0
		 int     16
       mov    si,22
		 cld
parte20:
		 lodsb
		 or     al,al
		 jz     parte21
		 mov    ah,14
		 int    16
		 mov    bp,0
		 jmp    parte20
parte21:
		 pop     dx
		 pop     cx
		 pop     bx
		 pop     ax
		 jmp     parte10
parte18:
		 jmp     parte18
parte16:
		 mov     ah,6
		 mov     al,4
		 mov     bh,7
		 mov     ch,0
		 mov     cl,0
		 mov     dh,5
		 mov     dl,70
		 int     16
		 mov     ah,2
		 mov     bh,0
		 mov     dx,0
		 int     16
		 ret
		 db      "SENHA.SYS - Produzido por Juarez Fioravanti Filho"
initialize endp
driver endp
cseg ends
end
