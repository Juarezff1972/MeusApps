Program Fib2;
{$N+,E+}
var i : integer;
function Fib(n:integer) : extended;
var z,x,y :  extended;
    i : byte;
    q : integer;
begin
   i:=1;   x:=0;   y:=1;
   if n=1 then
   begin
      Fib:=1;
      exit;
   end;
   for q:=2 to n do
   begin
      z:=x+y;
      x:=y;
      y:=z;
   end;
   Fib:=z;
end;

begin
   for i := 0 to 9999 do Writeln(i, '. ', Fib(i):1:0);
end.