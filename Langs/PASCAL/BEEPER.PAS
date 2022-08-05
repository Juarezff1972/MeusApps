{$M 8192,0,0}
uses CRT,DOS;
var abcd : pointer;
    botao,oldbot : word;
Procedure Beeper;
interrupt;
begin
   if Botao>0 then OldBot:=Botao;
   inline($50/$e4/$60/$a3/>Botao/$58);
   if Botao=OldBot then Botao:=0;
   if Botao<>0 then
   begin
      sound(random(2000));
      delay(3);
      nosound;
   end;
end;
begin
   getintvec($1b,abcd);
   setintvec($1b,SaveInt1B);
   getintvec($1c,abcd);
   setintvec($1c,@Beeper);
   keep(768);
end.