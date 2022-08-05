{$M 8192,0,0}
uses dos;
var x : ^byte;
    abcd : pointer;
Procedure Crazy;
interrupt;
begin
   x:=ptr($40,$17);
   x^:=not(x^);
end;
begin
   getintvec($1b,abcd);
   setintvec($1b,SaveInt1B);
   getintvec($1c,abcd);
   setintvec($1c,@Crazy);
   keep(768);
end.