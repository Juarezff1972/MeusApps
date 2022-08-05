uses dos,crt,graph;
var x : ^byte;
    y : file of byte;
    i : word;
    gd,gm : integer;
begin
   gd:=CGA;
   gm:=CGAHi;
   initgraph(gd,gm,'');
   assign(y,'screen.bmt');
{   rewrite(y);
   for i:=0 to 32768 do
   begin
      x:=ptr($b800,I);
      write(y,x^);
   end;
   close(y);}
   clrscr;
   readln;
   reset(y);
   for i:=0 to 32768 do
   begin
      x:=ptr($b800,I);
      read(y,x^);
   end;
   close(y);
end.