uses printer;
var c : ^char;
    i : word;

begin
   writeln(lst,'Tipo  Cil  Cab  W-PComb  L-Zone  Capacidade');
   writeln(lst,'-------------------------------------------');
   for i:=$c747 to $ce76 do
   begin
      c:=ptr($f000,i);
      write(lst,c^);
   end;
end.
