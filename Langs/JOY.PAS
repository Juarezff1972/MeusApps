uses dos,crt;
var bte:word;
begin
   asm
      mov dx,$201
      mov ax,65535
      out dx,ax
   end;
   repeat
      asm
         mov dx,$201
         in ax,dx
         mov bte,ax
      end;
      writeln('Valor = ',bte);
   until keypressed;
end.