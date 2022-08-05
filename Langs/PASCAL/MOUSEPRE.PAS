{$M 8192,0,0}
uses dos,crt;
var botao,oldbot : word;
    abcd : pointer;
Procedure Agora;
interrupt;
begin
   if Botao>0 then OldBot:=Botao;
   asm
      push ax
      in al,$60
      pop ax
   end;
   if Botao=OldBot then Botao:=0;
   if botao=130 then
   asm
      push ax
      push bx
      push cx
      push dx
      mov ax,0
      int $33
      mov ax,10
      mov bx,0
      mov cx,$ffff
      mov dx,$7700
      int $33
      pop dx
      pop cx
      pop bx
      pop ax
   end;
end;
begin
   getintvec($1b,abcd);
   setintvec($1b,SaveInt1B);
   getintvec($1c,abcd);
   setintvec($1c,@Agora);
   keep(768);
end.