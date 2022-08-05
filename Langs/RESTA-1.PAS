uses crt;

type matriz =array[1..9,1..9] of char;

var inicio  : matriz;
    destino : matriz;
    estado  : matriz;
    nivel   : word;
    iterat  : longint;
    max     : word;
    xx      : byte;
    yy      : byte;
    aux     : string;
{-----------------------------------------------------------------------}
function norte(x,y:byte;var estado:matriz) : boolean;
begin
   if estado[x,y]<>'þ' then begin norte:=false; exit; end;
   if y-2 < 1 then begin norte:=false; exit; end;
   if estado[x,y-2]<>' ' then begin norte:=false; exit; end;
   if estado[x,y-1]<>'þ' then begin norte:=false; exit; end;
   estado[x,y-2]:='þ';estado[x,y]:=' ';estado[x,y-1]:=' ';
   norte:=true;
end;
{-----------------------------------------------------------------------}
function sul(x,y:byte;var estado:matriz) : boolean;
begin
   if estado[x,y]<>'þ' then begin sul:=false; exit; end;
   if y+2 > 9 then begin sul:=false; exit; end;
   if estado[x,y+2]<>' ' then begin sul:=false; exit; end;
   if estado[x,y+1]<>'þ' then begin sul:=false; exit; end;
   estado[x,y+2]:='þ';estado[x,y]:=' ';estado[x,y+1]:=' ';
   sul:=true;
end;
{-----------------------------------------------------------------------}
function leste(x,y:byte;var estado:matriz) : boolean;
begin
   if estado[x,y]<>'þ' then begin leste:=false; exit; end;
   if x+2 > 9 then begin leste:=false; exit; end;
   if estado[x+2,y]<>' ' then begin leste:=false; exit; end;
   if estado[x+1,y]<>'þ' then begin leste:=false; exit; end;
   estado[x+2,y]:='þ';estado[x,y]:=' ';estado[x+1,y]:=' ';
   leste:=true;
end;
{-----------------------------------------------------------------------}
function oeste(x,y:byte;var estado:matriz) : boolean;
begin
   if estado[x,y]<>'þ' then begin oeste:=false; exit; end;
   if x-2 < 1 then begin oeste:=false; exit; end;
   if estado[x-2,y]<>' ' then begin oeste:=false; exit; end;
   if estado[x-1,y]<>'þ' then begin oeste:=false; exit; end;
   estado[x-2,y]:='þ';estado[x,y]:=' ';estado[x-1,y]:=' ';
   oeste:=true;
end;
{-----------------------------------------------------------------------}
function compara(e1,e2 : matriz) : boolean;
var x,y : byte;
begin
   compara:=true;
   for x:=1 to 9 do for y:=1 to 9 do if e1[x,y]<>e2[x,y] then compara:=false;
end;
{-----------------------------------------------------------------------}
function presa(e : matriz) : boolean;
var x,y : byte;
var h:string;
var v:string;
var hpresa:boolean;
var vpresa:boolean;
var px:byte;
var py:byte;
begin
   hpresa:=false;
   vpresa:=false;
{'°°°þþþ°°°'}
{'°°°þþþ°°°'}
{'°°°þþþ°°°'}
{'þþþþþþþþþ'}
{'þþþþ þþþþ'}
{'þþþþþþþþþ'}
{'°°°þþþ°°°'}
{'°°°þþþ°°°'}
{'°°°þþþ°°°'}
   x:=1;
   for y:=1 to 9 do
   begin
      if ((y>=1) and (y<4))or((y>=7) and (y<10)) then
      begin
         h:=e[x+2,y]+e[x+3,y]+e[x+4,y];
         if h='°þ ' then begin hpresa:=true; px:=x+3;py:=y; break; end;
         h:=e[x+3,y]+e[x+4,y]+e[x+5,y];
         if h=' þ ' then begin hpresa:=true; px:=x+4;py:=y; break; end;
         h:=e[x+4,y]+e[x+5,y]+e[x+6,y];
         if h=' þ°' then begin hpresa:=true; px:=x+5;py:=y; break; end;
      end;
      if (y>=4) and (y<7) then
      begin
         h:=e[x,y]+e[x+1,y];
         if h='þ ' then begin hpresa:=true; px:=x;py:=y; break; end;
         h:=e[x,y]+e[x+1,y]+e[x+2,y];
         if h=' þ ' then begin hpresa:=true; px:=x+1;py:=y; break; end;
         h:=e[x+1,y]+e[x+2,y]+e[x+3,y];
         if h=' þ ' then begin hpresa:=true; px:=x+2;py:=y; break; end;
         h:=e[x+2,y]+e[x+3,y]+e[x+4,y];
         if h=' þ ' then begin hpresa:=true; px:=x+3;py:=y; break; end;
         h:=e[x+3,y]+e[x+4,y]+e[x+5,y];
         if h=' þ ' then begin hpresa:=true; px:=x+4;py:=y; break; end;
         h:=e[x+4,y]+e[x+5,y]+e[x+6,y];
         if h=' þ ' then begin hpresa:=true; px:=x+5;py:=y; break; end;
         h:=e[x+5,y]+e[x+6,y]+e[x+7,y];
         if h=' þ ' then begin hpresa:=true; px:=x+6;py:=y; break; end;
         h:=e[x+6,y]+e[x+7,y]+e[x+8,y];
         if h=' þ ' then begin hpresa:=true; px:=x+7;py:=y; break; end;
         h:=e[x+7,y]+e[x+8,y];
         if h=' þ' then begin hpresa:=true; px:=x+8;py:=y; break; end;
      end;
   end;
   if py=1 then
   begin
      v:=e[px,py]+e[px,py+1];
      if v='þ ' then vpresa:=true;
   end;
   if py=2 then
   begin
      v:=e[px,py-1]+e[px,py]+e[px,py+1];
      if v=' þ ' then vpresa:=true;
   end;
   if py=3 then
   begin
      v:=e[px,py-1]+e[px,py]+e[px,py+1];
      if v=' þ ' then vpresa:=true;
   end;
   if (py>=4)and(py<7) then
   begin
      v:=e[px,py-1]+e[px,py]+e[px,py+1];
      if v=' þ ' then vpresa:=true;
      if v='°þ ' then vpresa:=true;
      if v=' þ°' then vpresa:=true;
   end;
   if py=7 then
   begin
      v:=e[px,py-1]+e[px,py]+e[px,py+1];
      if v=' þ ' then vpresa:=true;
   end;
   if py=8 then
   begin
      v:=e[px,py-1]+e[px,py]+e[px,py+1];
      if v=' þ ' then vpresa:=true;
   end;
   if py=9 then
   begin
      v:=e[px,py-1]+e[px,py];
      if v=' þ' then vpresa:=true;
   end;
   if hpresa and vpresa then presa:=true else presa:=false;
end;
{-----------------------------------------------------------------------}
procedure busca(estado : matriz);
var i,x,y : byte;
    n,s,l,o : boolean;
    n1,s1   : boolean;
    l1,o1   : boolean;
begin
   gotoxy(1,1);
   for y:=1 to 9 do
   begin
      for x:=1 to 9 do write(estado[x,y]);
      writeln;
   end;
   nivel:=nivel+1;
   iterat:=iterat+1;
   gotoxy(1,12);
{   for i:=1 to 9 do writeln(estado[i]);}
   writeln;
   writeln(iterat,'-----',nivel,'------',max,'         ');
{   readln;}
   if compara(estado,destino) then halt;
   if keypressed then halt;
   {if presa(estado) then
   begin
      gotoxy(nivel,15);writeln(' ');
      gotoxy(nivel,16);writeln(' ');
      gotoxy(nivel,17);writeln(' ');
      nivel:=nivel-1;
      if (nivel<max) then max:=nivel;
      gotoxy(max,18);writeln('|');
      exit;
   end;}
   n1:=false;s1:=false;l1:=false;o1:=false;
   for x:=1 to 9 do
   begin
      for y:=1 to 9 do
      begin
         gotoxy(nivel,16);writeln(x);
         gotoxy(nivel,17);writeln(y);
         gotoxy(nivel,15);
         n:=norte(x,y,estado);if n then begin writeln('n');busca(estado);n1:=true;end;
         s:=sul(x,y,estado);  if s then begin writeln('s');busca(estado);s1:=true;end;
         l:=leste(x,y,estado);if l then begin writeln('l');busca(estado);l1:=true;end;
         o:=oeste(x,y,estado);if o then begin writeln('o');busca(estado);o1:=true;end;
      end;
   end;

{   if (n1=false) and (s1=false) and (l1=false) and (o1=false) then
   begin
      gotoxy(1,1);
      for i:=1 to 9 do writeln(estado[i]);
   end;}
   gotoxy(nivel,15);writeln(' ');
   gotoxy(nivel,16);writeln(' ');
   gotoxy(nivel,17);writeln(' ');
   nivel:=nivel-1;
   if (nivel<max) then max:=nivel;
   gotoxy(max,18);writeln('|');
end;
{-----------------------------------------------------------------------}
begin
   clrscr;
   iterat:=0;
   max:=99;

   inicio[1]:='°°°þþþ°°°';
   inicio[2]:='°°°þþþ°°°';
   inicio[3]:='°°°þþþ°°°';
   inicio[4]:='þþþþþþþþþ';
   inicio[5]:='þþþþ þþþþ';
   inicio[6]:='þþþþþþþþþ';
   inicio[7]:='°°°þþþ°°°';
   inicio[8]:='°°°þþþ°°°';
   inicio[9]:='°°°þþþ°°°';

   destino[1]:='°°°   °°°';
   destino[2]:='°°°   °°°';
   destino[3]:='°°°   °°°';
   destino[4]:='         ';
   destino[5]:='    þ    ';
   destino[6]:='         ';
   destino[7]:='°°°   °°°';
   destino[8]:='°°°   °°°';
   destino[9]:='°°°   °°°';

   nivel:=0;
   for yy:=1 to 9 do
   begin
      aux:=inicio[yy];
      for xx:=1 to 9 do
      begin
         estado[xx,yy]:=aux[xx];
      end;
   end;
   estado:=inicio;
   busca(estado);

end.

