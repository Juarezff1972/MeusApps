Program PascalEnhancer;

Uses DOS,CRT;

var fyle          : string[8];
    fyle1,fyle2   : string[12];
    source,target : text;
    line1,line2   : string;
    spaces        : integer;
    distance      : string;

{-------------------------------------------------------------------------}
Procedure Add(quantity : integer);
begin
if spaces > 0 then insert('   ',distance,quantity);
if spaces < 0 then delete(distance,1,3);
end;
{-------------------------------------------------------------------------}
begin
clrscr;
write('Qual o nome do arquivo? ');
readln(fyle);
fyle1:=fyle+'.pas';
fyle2:=fyle+'.tpf';
writeln('Arrumando ',fyle1,'...');
assign(source,fyle1);
assign(target,fyle2);
reset(source);
rewrite(target);
spaces:=0;
distance:='';
line2:='';
repeat
delay(500);
readln(source,line1);
if (copy(line1,1,3)='end') or (copy(line1,1,5)='until') then begin
spaces:=-1;
add(spaces);
end;
line2:=distance+line1;
writeln(target,line2);
writeln(line2);
if (copy(line1,pos('begin' ,line1),5)='begin' ) or
   (copy(line1,pos('repeat',line1),6)='repeat') then begin
spaces:=1;
add(spaces);
end;
if (copy(line1,pos('begin' ,line1),5)='begin' ) or
   (copy(line1,pos('repeat',line1),6)='repeat') then line1:=line1+chr(13);
until eof(source);
writeln;
writeln('Arquivo melhorado foi denominado ',fyle2,'.');
end.