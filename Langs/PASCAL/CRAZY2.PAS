{$M 8192,0,0}
uses crt,dos;
var x : ^byte;
    a:byte;
    b:char;
Procedure Crazy;
begin
b:=#1;
repeat
   if keypressed then b:=readkey;
   randomize;
   x:=ptr($40,$17);
   a:=random(4);
   if a=0 then x^:=16;
   if a=1 then x^:=32;
   if a=2 then x^:=64;
   if a=3 then x^:=128;
   delay((ord(b)*2));
   a:=random(ord(b));
   textattr:=a;
   gotoxy(random(80),random(25));
   write(b);
until b=#27;
end;
begin
 crazy
end.