Program Fatorial2;
{$M 64000,0,655360}
var i : integer;
Function FAT(n : integer):extended;
var m,o : extended;
begin
   if n<2 then
   begin
      FAT:=1;
      exit;
   end;
   o:=FAT(n-1);
   m:=n*o;
   FAT:=m;
end;

begin
   for i:=1 to 1754 do writeln(i,'! = ',FAT(i):1:0);
end.