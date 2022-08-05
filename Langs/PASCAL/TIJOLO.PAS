uses dos,crt,tools;
type chtb = array[1..4096] of byte;
var regs : registers;
    x    : ^byte;
    y,z  : byte;
    i    : word;
    t    : chtb;
    v    : byte;

    char193,char196,char197,char194 : array[1..16] of string[8];

    aux  : byte;
    u    : integer;
    ch   : char;
    r,g,b,r1,g1,b1: byte;
    a,h,c,d,e,f : byte;
{-----------------------------------------------------------------------}
Procedure GetAsciiTable;
begin
   regs.ah:=$11;
   regs.al:=4;
   regs.bl:=0;
   intr($10,regs);
   regs.ah:=$11;
   regs.al:=$30;
   regs.bh:=6;
   intr($10,regs);
   for i:=0 to 255*16+15 do
   begin
      x:=ptr(regs.es,regs.bp+i);
      t[i]:=x^;
   end;
end;
{-----------------------------------------------------------------------}
Procedure ASCII196;
begin
   char196[ 1]:='00000000';
   char196[ 2]:='00000000';
   char196[ 3]:='11000011';
   char196[ 4]:='11000011';
   char196[ 5]:='11000011';
   char196[ 6]:='11000011';
   char196[ 7]:='11000011';
   char196[ 8]:='11000011';
   char196[ 9]:='11000011';
   char196[10]:='11000011';
   char196[11]:='11000011';
   char196[12]:='11000011';
   char196[13]:='11000011';
   char196[14]:='11000011';
   char196[15]:='00000000';
   char196[16]:='00000000';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char196[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char196[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char196[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char196[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char196[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char196[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char196[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char196[i],8,1),v,u);
      aux:=v+aux;
      t[196*16+(i-1)]:=aux;
   end;
   regs.ah:=$11;
   regs.al:=$0;
   regs.cx:=256;
   regs.dx:=0;
   regs.bh:=16;
   regs.es:=seg(t);
   regs.bp:=ofs(t)-1;
   intr($10,regs);
end;
{-----------------------------------------------------------------------}
Procedure ASCII194;
begin
   char194[ 1]:='00000000';
   char194[ 2]:='00000000';
   char194[ 3]:='11111111';
   char194[ 4]:='11111111';
   char194[ 5]:='11111111';
   char194[ 6]:='11111111';
   char194[ 7]:='11111111';
   char194[ 8]:='11111111';
   char194[ 9]:='11111111';
   char194[10]:='11111111';
   char194[11]:='11111111';
   char194[12]:='11111111';
   char194[13]:='11111111';
   char194[14]:='11111111';
   char194[15]:='00000000';
   char194[16]:='00000000';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char194[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char194[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char194[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char194[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char194[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char194[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char194[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char194[i],8,1),v,u);
      aux:=v+aux;
      t[194*16+(i-1)]:=aux;
   end;
   regs.ah:=$11;
   regs.al:=$0;
   regs.cx:=256;
   regs.dx:=0;
   regs.bh:=16;
   regs.es:=seg(t);
   regs.bp:=ofs(t)-1;
   intr($10,regs);
end;
{-----------------------------------------------------------------------}
Procedure ASCII193;
begin
   char193[ 1]:='00000000';
   char193[ 2]:='00000000';
   char193[ 3]:='11111100';
   char193[ 4]:='11111100';
   char193[ 5]:='11111100';
   char193[ 6]:='11111100';
   char193[ 7]:='11111100';
   char193[ 8]:='11111100';
   char193[ 9]:='11111100';
   char193[10]:='11111100';
   char193[11]:='11111100';
   char193[12]:='11111100';
   char193[13]:='11111100';
   char193[14]:='11111100';
   char193[15]:='00000000';
   char193[16]:='00000000';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char193[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char193[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char193[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char193[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char193[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char193[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char193[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char193[i],8,1),v,u);
      aux:=v+aux;
      t[193*16+(i-1)]:=aux;
   end;
   regs.ah:=$11;
   regs.al:=$0;
   regs.cx:=256;
   regs.dx:=0;
   regs.bh:=16;
   regs.es:=seg(t);
   regs.bp:=ofs(t)-1;
   intr($10,regs);
end;
{-----------------------------------------------------------------------}
Procedure ASCII197;
begin
   char197[ 1]:='00000000';
   char197[ 2]:='00000000';
   char197[ 3]:='00111111';
   char197[ 4]:='00111111';
   char197[ 5]:='00111111';
   char197[ 6]:='00111111';
   char197[ 7]:='00111111';
   char197[ 8]:='00111111';
   char197[ 9]:='00111111';
   char197[10]:='00111111';
   char197[11]:='00111111';
   char197[12]:='00111111';
   char197[13]:='00111111';
   char197[14]:='00111111';
   char197[15]:='00000000';
   char197[16]:='00000000';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char197[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char197[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char197[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char197[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char197[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char197[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char197[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char197[i],8,1),v,u);
      aux:=v+aux;
      t[197*16+(i-1)]:=aux;
   end;
   regs.ah:=$11;
   regs.al:=$0;
   regs.cx:=256;
   regs.dx:=0;
   regs.bh:=16;
   regs.es:=seg(t);
   regs.bp:=ofs(t)-1;
   intr($10,regs);
end;
{-------------------------------}
{-----------------------------------------------------------------------}
begin
   GetAsciiTable;
   a:=0;h:=0;c:=0;d:=0;e:=0;f:=0;
   ascii197;
   ascii196;
   ascii194;
   ascii193;
   r:=39;
   g:=23;
   b:=17;
   r1:=31;
   g1:=31;
   b1:=31;
   regs.ah:=$10;
   regs.al:=$10;
   regs.bx:=red;
   regs.dh:=0;
   regs.ch:=0;
   regs.cl:=0;
   intr($10,regs);
   regs.ah:=$10;
   regs.al:=$10;
   regs.bx:=lightgray;
   regs.dh:=0;
   regs.ch:=0;
   regs.cl:=0;
   intr($10,regs);
   textcolor(red);
   textbackground(lightgray);
   y:=1;
   repeat
   z:=1;
   repeat
      gotoxy(z,y);
      write('ÅÂÂÂÁ');
      gotoxy(z,y+1);
      write('ÂÂÄÂÂ');
      inc(z,5);
   until z>79;
   inc(y,2);
   until y>24;
   repeat
   regs.ah:=$10;
   regs.al:=$10;
   regs.bx:=red;
   regs.dh:=a;
   regs.ch:=h;
   regs.cl:=c;
   intr($10,regs);
   regs.ah:=$10;
   regs.al:=$10;
   regs.bx:=lightgray;
   regs.dh:=d;
   regs.ch:=e;
   regs.cl:=f;
   intr($10,regs);
   inc(a);inc(h);inc(c);inc(d);inc(e);inc(f);
   if a>r then a:=r;
   if h>g then h:=g;
   if c>b then c:=b;
   if d>r1 then d:=r1;
   if e>g1 then e:=g1;
   if f>b1 then f:=b1;
   delay(50);
   until a=39;
{   sombra(10,5,60,20,red);
   textcolor(blue);
   textbackground(lightgray);
   retangulo(10,5,60,20);}
   repeat
      ch:=readkey;
      
      if upcase(ch)='Q' then inc(r);
      if upcase(ch)='W' then inc(g);
      if upcase(ch)='E' then inc(b);
      if upcase(ch)='A' then dec(r);
      if upcase(ch)='S' then dec(g);
      if upcase(ch)='D' then dec(b);

      if upcase(ch)='1' then inc(r1);
      if upcase(ch)='2' then inc(g1);
      if upcase(ch)='3' then inc(b1);
      if upcase(ch)='4' then dec(r1);
      if upcase(ch)='5' then dec(g1);
      if upcase(ch)='6' then dec(b1);

   regs.ah:=$10;
   regs.al:=$10;
   regs.bx:=red;
   regs.dh:=r;
   regs.ch:=g;
   regs.cl:=b;
   intr($10,regs);
   regs.ah:=$10;
   regs.al:=$10;
   regs.bx:=lightgray;
   regs.dh:=r1;
   regs.ch:=g1;
   regs.cl:=b1;
   intr($10,regs);
   gotoxy(12,6); writeln('Vermelho= ',r,'   ');
   gotoxy(12,7); writeln('Verde=    ',g,'   ');
   gotoxy(12,8); writeln('Azul=     ',b,'   ');
   gotoxy(12,9); writeln('Vermelho= ',r1,'   ');
   gotoxy(12,10); writeln('Verde=    ',g1,'   ');
   gotoxy(12,11); writeln('Azul=     ',b1,'   ');
   until ch=#27;
end.
