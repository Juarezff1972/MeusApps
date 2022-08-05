Program CodeOff;
{$M 8192,0,0}
uses dos,crt;

var x : pointer;

Procedure Inicio;
interrupt;
begin
   asm
      pushf
      push ax
      push bx
      push cx
      push dx
      push si
      push di
      push es
      push ds
      push ss
      push sp
      push bp
      mov ah,[ss:$7464]
      cmp ah,$0f
      jne @abc
      mov ah,$02
      mov [ss:$7464],ah
 @abc:pop bp
      pop sp
      pop ss
      pop ss
      pop ds
      pop es
      pop di
      pop si
      pop dx
      pop cx
      pop bx
      pop ax
      popf
  end;
end;

begin
   getintvec($1c,x);
   setintvec($1c,@inicio);
   swapvectors;
   exec(getenv('COMSPEC'),'/c prince');
   swapvectors;
end.