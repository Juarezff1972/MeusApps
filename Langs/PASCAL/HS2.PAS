uses dos,crt;
type chtb = array[1..4096] of byte;
var regs : registers;
    x    : ^byte;
    y,z  : byte;
    i    : word;
    t    : chtb;
    v,q  : byte;

    char202,char209,char210,char216,char195,char203,
    char214,char207,char193,char182,char212,char204,
    char213,char196,char197,char194 : array[1..16] of string[8];

    aux  : byte;
    u    : integer;
    ch   : char;
    r,g,b,r1,g1,b1: byte;
    a,h,c,d,e,f : byte;
    w    : string;
    rr1,rr2,gg1,gg2,bb1,bb2 : byte;
{-----------------------------------------------------------------------}
Procedure Cores(cor_fundo,cor_texto:byte);
begin
  textbackground(cor_fundo);
  textcolor(cor_texto);
end;
{---------------------------------------------------------------------------}
procedure retangulo(x1,y1,x2,y2 : integer);
var x,y : integer;
begin
   window(x1,y1,x2,y2);
   gotoxy(1,1);             writeln('É');
   gotoxy(x2-x1,y2-y1);     writeln('¼');
   gotoxy(x2-x1,1);         writeln('»');
   gotoxy(1,y2-y1);         writeln('È');
   for y:=2 to y2-(y1+1) do
   begin
      gotoxy(1,y);         writeln('º');
      gotoxy(x2-x1,y);     writeln('º');
   end;
   for x:=2 to x2-(x1+1) do
   begin
      gotoxy(x,1);         writeln('Í');
      gotoxy(x,y2-y1);     writeln('Í');
   end;
   window(x1+1,y1+1,x2-2,y2-2);
   clrscr;
   window(1,1,80,25);
end;
{---------------------------------------------------------------------------}
procedure sombra(x1,y1,x2,y2,sombraatrib : integer);
var x,y : integer;
begin
   for x:=x1+2 to x2+1 do
   begin
      gotoxy(x,y2);
      regs.ah:=8;
      intr($10,regs);
      regs.cx:=1;
      regs.bx:=sombraatrib;
      regs.ah:=9;
      intr($10,regs);
   end;
   for x:=x2 to x2+1 do
      for y:=y1+1 to y2 do
      begin
         gotoxy(x,y);
         regs.ah:=8;
         intr($10,regs);
         regs.cx:=1;
         regs.bx:=sombraatrib;
         regs.ah:=9;
         intr($10,regs);
      end;
end;
{---------------------------------------------------------------------------}
procedure Cursor(o : boolean; cursoroff : word);
begin
   if o=true then
   begin
      regs.ax:=$168;
      regs.cx:=cursoroff; { Desativa cursor }
      intr($10,regs);
   end;
   if o=false then
   begin
      regs.ax:=$168;
      regs.cx:=$607;      { Ativa cursor }
      intr($10,regs);
   end;
end;
{---------------------------------------------------------------------------}
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
   char196[ 3]:='00000000';
   char196[ 4]:='00000000';
   char196[ 5]:='00000000';
   char196[ 6]:='00000000';
   char196[ 7]:='00000000';
   char196[ 8]:='00000000';
   char196[ 9]:='00000000';
   char196[10]:='00000000';
   char196[11]:='00000000';
   char196[12]:='00001111';
   char196[13]:='00111000';
   char196[14]:='01100000';
   char196[15]:='01100000';
   char196[16]:='01100000';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char196[i],1,1),v,u);      aux:=2*2*2*2*2*2*2*v;
      val(copy(char196[i],2,1),v,u);      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char196[i],3,1),v,u);      aux:=2*2*2*2*2*v+aux;
      val(copy(char196[i],4,1),v,u);      aux:=2*2*2*2*v+aux;
      val(copy(char196[i],5,1),v,u);      aux:=2*2*2*v+aux;
      val(copy(char196[i],6,1),v,u);      aux:=2*2*v+aux;
      val(copy(char196[i],7,1),v,u);      aux:=2*v+aux;
      val(copy(char196[i],8,1),v,u);      aux:=v+aux;
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
   char194[ 3]:='01111111';
   char194[ 4]:='00001110';
   char194[ 5]:='00001110';
   char194[ 6]:='00001110';
   char194[ 7]:='00001110';
   char194[ 8]:='00001110';
   char194[ 9]:='00001110';
   char194[10]:='00001110';
   char194[11]:='00001110';
   char194[12]:='00001111';
   char194[13]:='00001110';
   char194[14]:='00001110';
   char194[15]:='00001110';
   char194[16]:='00001110';
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
Procedure ASCII213;
begin
   char213[ 1]:='00000000';
   char213[ 2]:='00000000';
   char213[ 3]:='11110000';
   char213[ 4]:='10000000';
   char213[ 5]:='10000000';
   char213[ 6]:='10000000';
   char213[ 7]:='10000000';
   char213[ 8]:='10000000';
   char213[ 9]:='10000000';
   char213[10]:='10000000';
   char213[11]:='10000000';
   char213[12]:='10000000';
   char213[13]:='10000000';
   char213[14]:='10000000';
   char213[15]:='10000000';
   char213[16]:='10000000';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char213[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char213[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char213[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char213[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char213[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char213[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char213[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char213[i],8,1),v,u);
      aux:=v+aux;
      t[213*16+(i-1)]:=aux;
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
   char197[ 3]:='10001111';
   char197[ 4]:='00000001';
   char197[ 5]:='00000001';
   char197[ 6]:='00000001';
   char197[ 7]:='00000001';
   char197[ 8]:='00000001';
   char197[ 9]:='00000001';
   char197[10]:='00000001';
   char197[11]:='00000001';
   char197[12]:='11111111';
   char197[13]:='00000001';
   char197[14]:='00000001';
   char197[15]:='00000001';
   char197[16]:='00000001';
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
{-----------------------------------------------------------------------}
Procedure ASCII202;
begin
   char202[ 1]:='00000000';
   char202[ 2]:='00000000';
   char202[ 3]:='00000000';
   char202[ 4]:='00000000';
   char202[ 5]:='00000000';
   char202[ 6]:='00000000';
   char202[ 7]:='00000000';
   char202[ 8]:='00000000';
   char202[ 9]:='00000000';
   char202[10]:='00000000';
   char202[11]:='00000000';
   char202[12]:='11000100';
   char202[13]:='00111100';
   char202[14]:='00011100';
   char202[15]:='00001100';
   char202[16]:='00001100';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char202[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char202[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char202[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char202[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char202[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char202[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char202[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char202[i],8,1),v,u);
      aux:=v+aux;
      t[202*16+(i-1)]:=aux;
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
{------------------------------------------------------------------------}
Procedure ASCII209;
begin
   char209[ 1]:='00001110';
   char209[ 2]:='00001110';
   char209[ 3]:='00001110';
   char209[ 4]:='00001110';
   char209[ 5]:='00111111';
   char209[ 6]:='00000000';
   char209[ 7]:='00000000';
   char209[ 8]:='00000000';
   char209[ 9]:='00000000';
   char209[10]:='00000000';
   char209[11]:='00000000';
   char209[12]:='00000000';
   char209[13]:='00000000';
   char209[14]:='00000000';
   char209[15]:='00000000';
   char209[16]:='00000000';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char209[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char209[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char209[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char209[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char209[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char209[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char209[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char209[i],8,1),v,u);
      aux:=v+aux;
      t[209*16+(i-1)]:=aux;
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
Procedure ASCII210;
begin
   char210[ 1]:='00000001';
   char210[ 2]:='00000001';
   char210[ 3]:='00000001';
   char210[ 4]:='00000001';
   char210[ 5]:='10001111';
   char210[ 6]:='00000000';
   char210[ 7]:='00000000';
   char210[ 8]:='00000000';
   char210[ 9]:='00000000';
   char210[10]:='00000000';
   char210[11]:='00000000';
   char210[12]:='00000000';
   char210[13]:='00000000';
   char210[14]:='00000000';
   char210[15]:='00000000';
   char210[16]:='00000000';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char210[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char210[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char210[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char210[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char210[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char210[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char210[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char210[i],8,1),v,u);
      aux:=v+aux;
      t[210*16+(i-1)]:=aux;
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
Procedure ASCII216;
begin
   char216[ 1]:='10000000';
   char216[ 2]:='10000000';
   char216[ 3]:='10000000';
   char216[ 4]:='10000000';
   char216[ 5]:='11110000';
   char216[ 6]:='00000000';
   char216[ 7]:='00000000';
   char216[ 8]:='00000000';
   char216[ 9]:='00000000';
   char216[10]:='00000000';
   char216[11]:='00000000';
   char216[12]:='00000000';
   char216[13]:='00000000';
   char216[14]:='00000000';
   char216[15]:='00000000';
   char216[16]:='00000000';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char216[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char216[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char216[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char216[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char216[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char216[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char216[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char216[i],8,1),v,u);
      aux:=v+aux;
      t[216*16+(i-1)]:=aux;
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
Procedure ASCII195;
begin
   char195[ 1]:='01100000';
   char195[ 2]:='01110000';
   char195[ 3]:='00111110';
   char195[ 4]:='00011111';
   char195[ 5]:='00000111';
   char195[ 6]:='00000000';
   char195[ 7]:='00000000';
   char195[ 8]:='00000000';
   char195[ 9]:='00000000';
   char195[10]:='01000000';
   char195[11]:='01000000';
   char195[12]:='01100000';
   char195[13]:='01111000';
   char195[14]:='01001111';
   char195[15]:='00000000';
   char195[16]:='00000000';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char195[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char195[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char195[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char195[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char195[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char195[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char195[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char195[i],8,1),v,u);
      aux:=v+aux;
      t[195*16+(i-1)]:=aux;
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
Procedure ASCII203;
begin
   char203[ 1]:='00000000';
   char203[ 2]:='00000000';
   char203[ 3]:='00000000';
   char203[ 4]:='11000000';
   char203[ 5]:='11110000';
   char203[ 6]:='11111000';
   char203[ 7]:='00111000';
   char203[ 8]:='00001100';
   char203[ 9]:='00001100';
   char203[10]:='00001100';
   char203[11]:='00001100';
   char203[12]:='00011000';
   char203[13]:='00110000';
   char203[14]:='11000000';
   char203[15]:='00000000';
   char203[16]:='00000000';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char203[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char203[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char203[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char203[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char203[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char203[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char203[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char203[i],8,1),v,u);
      aux:=v+aux;
      t[203*16+(i-1)]:=aux;
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
Procedure botao1;
begin
   char214[ 1]:='11111111';
   char214[ 2]:='10000000';
   char214[ 3]:='10000000';
   char214[ 4]:='10000000';
   char214[ 5]:='10000000';
   char214[ 6]:='10000000';
   char214[ 7]:='10000000';
   char214[ 8]:='10000000';
   char214[ 9]:='10000000';
   char214[10]:='10000000';
   char214[11]:='10000000';
   char214[12]:='10000000';
   char214[13]:='10000000';
   char214[14]:='10000000';
   char214[15]:='10000000';
   char214[16]:='10000000';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char214[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char214[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char214[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char214[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char214[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char214[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char214[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char214[i],8,1),v,u);
      aux:=v+aux;
      t[214*16+(i-1)]:=aux;
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
Procedure botao2;
begin
   char207[ 1]:='00000001';
   char207[ 2]:='00000011';
   char207[ 3]:='00000111';
   char207[ 4]:='00001111';
   char207[ 5]:='00011111';
   char207[ 6]:='00111111';
   char207[ 7]:='01111111';
   char207[ 8]:='11111111';
   char207[ 9]:='11111111';
   char207[10]:='11111111';
   char207[11]:='11111111';
   char207[12]:='11111111';
   char207[13]:='11111111';
   char207[14]:='11111111';
   char207[15]:='11111111';
   char207[16]:='11111111';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char207[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char207[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char207[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char207[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char207[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char207[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char207[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char207[i],8,1),v,u);
      aux:=v+aux;
      t[207*16+(i-1)]:=aux;
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
Procedure botao3;
begin
   char193[ 1]:='00000000';
   char193[ 2]:='00000000';
   char193[ 3]:='00000000';
   char193[ 4]:='00000000';
   char193[ 5]:='00000000';
   char193[ 6]:='00000000';
   char193[ 7]:='00000000';
   char193[ 8]:='00000000';
   char193[ 9]:='00000001';
   char193[10]:='00000011';
   char193[11]:='00000111';
   char193[12]:='00001111';
   char193[13]:='00011111';
   char193[14]:='00111111';
   char193[15]:='01111111';
   char193[16]:='11111111';
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
Procedure botao4;
begin
   char182[ 1]:='10000000';
   char182[ 2]:='10000000';
   char182[ 3]:='10000000';
   char182[ 4]:='10000000';
   char182[ 5]:='10000000';
   char182[ 6]:='10000000';
   char182[ 7]:='10000000';
   char182[ 8]:='10000000';
   char182[ 9]:='10000000';
   char182[10]:='10000000';
   char182[11]:='10000000';
   char182[12]:='10000000';
   char182[13]:='10000000';
   char182[14]:='10000000';
   char182[15]:='10000000';
   char182[16]:='10000000';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char182[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char182[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char182[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char182[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char182[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char182[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char182[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char182[i],8,1),v,u);
      aux:=v+aux;
      t[182*16+(i-1)]:=aux;
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
Procedure botao5;
begin
   char212[ 1]:='00000000';
   char212[ 2]:='00000000';
   char212[ 3]:='00000000';
   char212[ 4]:='00000000';
   char212[ 5]:='00000000';
   char212[ 6]:='00000000';
   char212[ 7]:='00000000';
   char212[ 8]:='00000000';
   char212[ 9]:='00000001';
   char212[10]:='00000011';
   char212[11]:='00000111';
   char212[12]:='00001111';
   char212[13]:='00011111';
   char212[14]:='00111111';
   char212[15]:='01111111';
   char212[16]:='11111111';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char212[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char212[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char212[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char212[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char212[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char212[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char212[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char212[i],8,1),v,u);
      aux:=v+aux;
      t[212*16+(i-1)]:=aux;
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
Procedure botao6;
begin
   char204[ 1]:='00000001';
   char204[ 2]:='00000011';
   char204[ 3]:='00000111';
   char204[ 4]:='00001111';
   char204[ 5]:='00011111';
   char204[ 6]:='00111111';
   char204[ 7]:='01111111';
   char204[ 8]:='11111111';
   char204[ 9]:='11111111';
   char204[10]:='11111111';
   char204[11]:='11111111';
   char204[12]:='11111111';
   char204[13]:='11111111';
   char204[14]:='11111111';
   char204[15]:='11111111';
   char204[16]:='11111111';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char204[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char204[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char204[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char204[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char204[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char204[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char204[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char204[i],8,1),v,u);
      aux:=v+aux;
      t[204*16+(i-1)]:=aux;
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
Procedure TipoDeVideo;
begin
   regs.ah:=$1a;
   regs.al:=0;
   intr($10,regs);
   if regs.bl=0 then begin writeln(chr(7)); halt(1); end;
   if regs.bl=1 then
   begin
      writeln;
      writeln('Este programa n„o funciona em um MDA. Precisa de um EGA ou VGA');
      writeln;
      halt(1);
   end;
   if regs.bl=2 then
   begin
      writeln;
      writeln('Este programa n„o funciona em um CGA. Precisa de um EGA ou VGA');
      writeln;
      halt(1);
   end;
   if regs.bl=5 then
   begin
      writeln;
      writeln('Compre um EGA colorido, por favor! (EGA monocrom tico... Argh!)');
      writeln;
      halt(1);
   end;
   if regs.bl=7 then
   begin
      writeln;
      writeln('Compre um VGA colorido, por favor! (VGA monocrom tico... Argh!)');
      writeln;
      halt(1);
   end;
   if regs.bl=$b then
   begin
      writeln;
      writeln('Compre um MCGA colorido, por favor! (MCGA monocrom tico... Argh!)');
      writeln;
      halt(1);
   end;
   if regs.bl=$FF then
   begin
      writeln;
      writeln('N„o conhe‡o este monitor.');
      writeln;
      halt(1);
   end;
end;
{-----------------------------------------------------------------------}
begin
   TipoDeVideo;
   regs.ah:=$10;
   regs.al:=$15;
   regs.bx:=red;
   intr($10,regs);
   rr1:=regs.dh;
   gg1:=regs.ch;
   bb1:=regs.cl;
   regs.ah:=$10;
   regs.al:=$15;
   regs.bx:=lightgray;
   intr($10,regs);
   rr2:=regs.dh;
   gg2:=regs.ch;
   bb2:=regs.cl;
   cursor(true,6400);
   regs.ah:=$10;
   regs.al:=$03;
   regs.bl:=0;
   intr($10,regs);
   GetAsciiTable;
   a:=0;h:=0;c:=0;d:=0;e:=0;f:=0;
   ascii197;
   ascii196;
   ascii194;
   ascii213;
   ascii202;
   ascii209;
   ascii210;
   ascii216;
   ascii195;
   ascii203;
   botao1;
   botao2;
   botao3;
   botao4;
   botao5;
   botao6;
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
      write('ÂÅÕÄÊ');
      gotoxy(z,y+1);
      write('ÑÒØÃË');
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
   cores(magenta,white);
   Sombra(23,5,59,21,8);
  Retangulo(23,5,59,21);
  gotoxy(25,5); write('( )');
  gotoxy(26,5);
  regs.ah:=9;
  regs.al:=7;
  regs.bl:=magenta*16+white;
  regs.bh:=0;
  regs.cx:=1;
  intr($10,regs);
  cores(blue,white);
  gotoxy(24,6); write(' ':34);
  gotoxy(33,6); write('Sobre o Super Fade');
  cores(magenta,white);
  gotoxy(26,8);  write('ÂÅÕÄÊ');
  gotoxy(26,9);  write('ÑÒØÃË');
  gotoxy(35,8); write('Super Fade');
  gotoxy(36,10); write('Vers„o 1');
  gotoxy(29,12);write('Copyright (C) 1990 - 1992');
  gotoxy(31,14);write('Juarez Fioravanti F.');
  cores(white,56);
  gotoxy(44,15);  write('ÔÌ');
  for q:=0 to 2 do begin gotoxy(44,16+q);write('ÛÛ');end;
  gotoxy(36,19);  write('ÁÏ');
  cores(magenta,darkgray);
  gotoxy(38,19);  write('ÛÛÛÛÛÛÛÛ');
  cores(magenta,white);
  gotoxy(36,15);  write('ÛÛÛÛÛÛÛÛ');
  for q:=0 to 2 do begin gotoxy(36,16+q); write('ÛÛ');end;
  cores(lightgray,black);
  gotoxy(38,17);  write('  Ok  ');
  gotoxy(38,16);  write('      ');
  gotoxy(38,18);  write('      ');
  textcolor(white);
  textbackground(black);
  retangulo(1,5,17,13);
  sombra(1,5,17,13,8);
  gotoxy(3,6); writeln('Vermelho= ',r,' ');
  gotoxy(3,7); writeln('Verde=    ',g,' ');
  gotoxy(3,8); writeln('Azul=     ',b,' ');
  gotoxy(3,9); writeln('Vermelho= ',r1,' ');
  gotoxy(3,10); writeln('Verde=    ',g1,' ');
  gotoxy(3,11); writeln('Azul=     ',b1,' ');
  gotoxy(1,24);
  write('  Q, W, E, 1, 2, 3 para aumentar e A, S, D, 4, 5, 6 para reduzir a cor de fundo ');
  repeat
   gotoxy(1,1);
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
   if r>63 then r:=0; if g>63 then g:=0; if b>63 then b:=0;
   if r1>63 then r1:=0; if g1>63 then g1:=0; if b1>63 then b1:=0;
   if r=255 then r:=63; if g=255 then g:=63; if b=255 then b:=63;
   if r1=255 then r1:=63; if g1=255 then g1:=63; if b1=255 then b1:=63;
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
   textcolor(white);
   textbackground(black);
   retangulo(1,5,17,13);
   sombra(1,5,17,13,8);
   gotoxy(3,6); writeln('Vermelho= ',r,' ');
   gotoxy(3,7); writeln('Verde=    ',g,' ');
   gotoxy(3,8); writeln('Azul=     ',b,' ');
   gotoxy(3,9); writeln('Vermelho= ',r1,' ');
   gotoxy(3,10); writeln('Verde=    ',g1,' ');
   gotoxy(3,11); writeln('Azul=     ',b1,' ');
   gotoxy(1,24);
   write('  Q, W, E, 1, 2, 3 para aumentar e A, S, D, 4, 5, 6 para reduzir a cor de fundo ');
   gotoxy(1,1);
  until ch=#27;
  regs.ah:=$10;
  regs.al:=$03;
  regs.bl:=1;
  intr($10,regs);
  regs.ah:=$11;
  regs.al:=4;
  regs.bl:=0;
  intr($10,regs);
  regs.ah:=$10;
  regs.al:=$10;
  regs.bx:=red;
  regs.dh:=rr1;
  regs.ch:=gg1;
  regs.cl:=bb1;
  intr($10,regs);
  regs.ah:=$10;
  regs.al:=$15;
  regs.bx:=lightgray;
  regs.dh:=rr2;
  regs.ch:=gg2;
  regs.cl:=bb2;
  intr($10,regs);
  clrscr;
end.
