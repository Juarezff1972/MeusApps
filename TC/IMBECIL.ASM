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

_TEXT	segment byte public 'CODE'
   ;
   ;	void inics1 (s1 *s)
   ;
_inics1	proc	near
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+4]
   ;
   ;	{
   ;	   s->i=s->j=0;
   ;
	xor	ax,ax
	mov	word ptr [si+2],ax
	mov	word ptr [si],ax
   ;
   ;	}
   ;
	pop	si
	pop	bp
	ret
_inics1	endp
   ;
   ;	void inics1tab(s1tab s[], int i)
   ;
_inics1tab	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;
   ;	{
   ;	   int n;
   ;	   for (n=0;n<i;n++) inics1(& s[n]);
   ;
	xor	si,si
	jmp	short @2@114
@2@58:
	mov	ax,si
	mov	dx,20
	imul	dx
	mov	dx,word ptr [bp+4]
	add	dx,ax
	push	dx
	call	near ptr _inics1
	pop	cx
	inc	si
@2@114:
	cmp	si,word ptr [bp+6]
	jl	short @2@58
   ;
   ;	}
   ;
	pop	si
	pop	bp
	ret
_inics1tab	endp
   ;
   ;	void inics2(s2 *s)
   ;
	assume	cs:_TEXT
_inics2	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	di,word ptr [bp+4]
   ;
   ;	{
   ;	   int i;
   ;	   inics1tab(s->t,sizeof(s1tab)/sizeof(s1));
   ;
	mov	ax,5
	push	ax
	push	di
	call	near ptr _inics1tab
	pop	cx
	pop	cx
   ;
   ;	   for(i=0;i<4;i++) inics1tab(& s->m[i], sizeof(s1tab)/sizeof(s1));
   ;
	xor	si,si
	jmp	short @3@114
@3@58:
	mov	ax,5
	push	ax
	mov	ax,si
	mov	dx,20
	imul	dx
	mov	dx,di
	add	dx,ax
	add	dx,20
	push	dx
	call	near ptr _inics1tab
	pop	cx
	pop	cx
	inc	si
@3@114:
	cmp	si,4
	jl	short @3@58
   ;
   ;	}
   ;
	pop	di
	pop	si
	pop	bp
	ret
_inics2	endp
   ;
   ;	void main()
   ;
_main	proc	near
	push	bp
	mov	bp,sp
	sub	sp,100
   ;
   ;	{
   ;	   s2 s;
   ;	   inics2(&s);
   ;
	lea	ax,word ptr [bp-100]
	push	ax
	call	near ptr _inics2
	pop	cx
   ;
   ;	}
   ;
	mov	sp,bp
	pop	bp
	ret
_main	endp
_TEXT	ends

_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends

	public	_main
	public	_inics2
	public	_inics1tab
	public	_inics1
_s@	equ	s@
	end
