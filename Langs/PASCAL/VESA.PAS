uses crt;
var x,y : word;
    c   : byte;
begin
   randomize;
   asm
      mov ax,$4f02
      mov bx,$104
      int $10
   end;
   for x:=0 to 639 do
   begin
      for y:=0 to 479 do
      begin
         c:=random(256);
         asm
            mov ah,$c
            mov al,c
            mov bx,0
            mov cx,x
            mov dx,y
            int $10
         end;
      end;
   end;
   repeat until keypressed;
   asm
      mov ax,$3
      int $10
   end;
end.