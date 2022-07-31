unsigned char Compara(unsigned orig,unsigned dest,unsigned tam)
{
   unsigned char aux;
   asm{
      push bx
      push di
      push cx
      push dx
      push ax
      mov bx,orig
      mov di,dest
      mov cx,tam
lupe: add bx,cx
      mov dx,word ptr [bx]
      cmp dx,word ptr [di]
      je igual
      mov ax,0
      jmp final
igual:mov ax,1
final:loop lupe
      mov aux,ax
      pop ax
      pop dx
      pop cx
      pop di
      pop bx
   }
   return aux;
}
