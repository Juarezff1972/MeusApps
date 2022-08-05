{$N+,E+}
program Fib8087;

var
  i : integer;

function Fib(N : integer) : extended;
{ calcula a seqÅància de fibonacci para N }
var F1, F2 : extended;
begin
  if N = 0 then
    Fib := 0.0
  else
    if N = 1 then
      Fib := 1.0
    else
    begin
      F1 := Fib(N - 1);
      F2 := Fib(N - 2);
      Fib := F1 + F2;
    end;
end;

begin
  for i := 0 to 30 do
    Writeln(i, '. ', Fib(i):1:0);
end.
