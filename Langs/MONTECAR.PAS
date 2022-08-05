uses crt;
var i,j : integer;
var x : array[0..4,0..7] of integer;
var indice : array[0..4] of integer;
var porcnt : array[0..4] of real;
var soma : array[0..4] of integer;
var ordem : array[0..7] of integer;
var contagem : array[0..4] of integer;
var total : integer;
var aux : real;
var aux2 : integer;
var z,zz : string;
begin
   clrscr;
   randomize;
   for i:=0 to 3 do
   begin
      for j:=0 to 6 do
      begin
         x[i,j]:=random(10)+1;
      end;
   end;
   total:=0;
   for i:=0 to 3 do
   begin
      soma[i]:=0;
      indice[i]:=i;
      for j:=0 to 6 do
      begin
         soma[i]:=soma[i]+x[i,j];
      end;
      total:=total+soma[i];
      porcnt[i]:=0;
   end;
   porcnt[0]:=soma[0]/total;
   porcnt[1]:=soma[1]/total;
   porcnt[2]:=soma[2]/total;
   porcnt[3]:=soma[3]/total;
   for i:=0 to 3 do
   begin
      str(porcnt[i],z);
      writeln(z);
   end;
   for i:=2 downto 0 do
   begin
      for j:=0 to i do
      begin
         if porcnt[j]>porcnt[j+1] then
         begin
            aux:=porcnt[j+1];
            aux2:=indice[j+1];
            porcnt[j+1]:=porcnt[j];
            indice[j+1]:=indice[j];
            porcnt[j]:=aux;
            indice[j]:=aux2;
         end;
      end;
   end;
   writeln('--------------------');
   for i:=0 to 3 do
   begin
      str(porcnt[i],z);
      str(indice[i],zz);
      writeln(zz+' - '+z);
   end;
   porcnt[0]:=porcnt[0]+0;
   porcnt[1]:=porcnt[1]+porcnt[0];
   porcnt[2]:=porcnt[2]+porcnt[1];
   porcnt[3]:=porcnt[3]+porcnt[2];
   for i:=0 to 6 do
   begin
      aux:=random(1000)/1000;
      str(aux,zz);
      write(zz+' - ');
      if (aux>=0) and (aux<=porcnt[0]) then
      begin
         ordem[i]:=indice[0];
      end;
      if (aux>porcnt[0]) and (aux<=porcnt[1]) then
      begin
         ordem[i]:=indice[1];
      end;
      if (aux>porcnt[1]) and (aux<=porcnt[2]) then
      begin
         ordem[i]:=indice[2];
      end;
      if (aux>porcnt[2]) and (aux<=porcnt[3]) then
      begin
         ordem[i]:=indice[3];
      end;
      str(ordem[i],zz);
      writeln(zz);
   end;
   for i:=0 to 6 do
   begin
      contagem[ordem[i]]:=contagem[ordem[i]]+1;
   end;
   writeln('------------');
   for i:=0 to 3 do
   begin
      str(contagem[i],zz);
      str(i,z);
      writeln(z+' - '+zz);
   end;
end.