Program Programa;
uses dos,unidade;
{$M 8192,0,0}
var x : pointer;

Procedure Procedimento;
interrupt;
begin
   TabelaASCII;
end;

begin
   getintvec($05,x);
   setintvec($05,@Procedimento);
   keep(768);
end.