var a : char;
begin
   repeat
      asm
         push ax
         push bx
         mov bx,$3f8
         in bx,al
         mov a,al
         pop bx
         pop ax
      end;
      write(a);
   until 1=1;
end.