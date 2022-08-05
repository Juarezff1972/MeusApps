{$M 8192,0,0}
uses CRT,DOS,drivers;
var abcd : pointer;
    botao,oldbot : word;
    x,y,z : byte;
    regs : registers;
Procedure Beeper;
interrupt;
begin
   if Botao>0 then OldBot:=Botao;
   inline($50/$e4/$60/$a3/>Botao/$58);
   if Botao=OldBot then Botao:=0;
   if Botao<>0 then
   begin
     regs.ah:=$10;
     regs.al:=$1a;
     intr($10,regs);
     x:=wherex;
     y:=wherey;
     gotoxy(1,1); write(regs.al,' ');
     gotoxy(1,2); write(regs.ah,' ');
     gotoxy(1,3); write(regs.bl,' ');
     gotoxy(1,4); write(regs.bh,' ');
     gotoxy(1,5); write(regs.cl,' ');
     gotoxy(1,6); write(regs.ch,' ');
     gotoxy(1,7); write(regs.dl,' ');
     gotoxy(1,8); write(regs.dh,' ');
     gotoxy(x,y);
   end;
end;
begin
   getintvec($1b,abcd);
   setintvec($1b,SaveInt1B);
   getintvec($1c,abcd);
   setintvec($1c,@Beeper);
   keep(768);
end.                                     
