unit caract;
interface
uses dos,crt;
  Procedure ASCII179;
  Procedure ASCII194;
  Procedure ASCII196;
  Procedure ASCII197;
  Procedure ASCII202;
  Procedure ASCII203;
  Procedure ASCII209;
  Procedure ASCII210;
  Procedure ASCII213;
  Procedure ASCII216;
  Procedure ASCII222;
  Procedure ASCII132;
  Procedure ASCII186;
  Procedure ASCII247;
  Procedure ASCII201;
  Procedure ASCII187;
  Procedure ASCII188;
  Procedure ASCII200;
  Procedure ASCII208;
  Procedure ASCII205;
  Procedure ASCII199;
  Procedure ASCII195;
  Procedure ASCII211;
  Procedure ASCII212;
  Procedure ASCII01;
  Procedure ASCII02;
  Procedure ASCII03;
  Procedure ASCII04;
  Procedure ASCII10;
  Procedure CopyChar(code:byte);
var carac : array[1..16] of string[8];
implementation
var regs : registers;
    x    : ^byte;
    i    : word;
    t    : array[1..4096] of byte;
    v    : byte;
    aux  : byte;
    u    : integer;
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
Procedure CopyChar;
begin
  aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(carac[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(carac[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(carac[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(carac[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(carac[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(carac[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(carac[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(carac[i],8,1),v,u);
      aux:=v+aux;
      t[code*16+(i-1)]:=aux;
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
{--------------------------------------------------------------------------}
Procedure ASCII132;
begin
   carac[ 1]:='00000000';
   carac[ 2]:='00000000';
   carac[ 3]:='01110110';
   carac[ 4]:='11011100';
   carac[ 5]:='00000000';
   carac[ 6]:='01111000';
   carac[ 7]:='00001100';
   carac[ 8]:='01111100';
   carac[ 9]:='11001100';
   carac[10]:='11001100';
   carac[11]:='11011100';
   carac[12]:='01110110';
   carac[13]:='00000000';
   carac[14]:='00000000';
   carac[15]:='00000000';
   carac[16]:='00000000';
   copychar(132);
end;
{-----------------------------------------------------------------------}
Procedure ASCII186;
begin
   carac[ 1]:='11100000';
   carac[ 2]:='11100000';
   carac[ 3]:='11100000';
   carac[ 4]:='11100000';
   carac[ 5]:='11100000';
   carac[ 6]:='11100000';
   carac[ 7]:='11100000';
   carac[ 8]:='11100000';
   carac[ 9]:='11100000';
   carac[10]:='11100000';
   carac[11]:='11100000';
   carac[12]:='11100000';
   carac[13]:='11100000';
   carac[14]:='11100000';
   carac[15]:='11100000';
   carac[16]:='11100000';
   copychar(186);
end;
{-----------------------------------------------------------------------}
Procedure ASCII247;
begin
   carac[ 1]:='00000111';
   carac[ 2]:='00000111';
   carac[ 3]:='00000111';
   carac[ 4]:='00000111';
   carac[ 5]:='00000111';
   carac[ 6]:='00000111';
   carac[ 7]:='00000111';
   carac[ 8]:='00000111';
   carac[ 9]:='00000111';
   carac[10]:='00000111';
   carac[11]:='00000111';
   carac[12]:='00000111';
   carac[13]:='00000111';
   carac[14]:='00000111';
   carac[15]:='00000111';
   carac[16]:='00000111';
   copychar(247);
end;
{-----------------------------------------------------------------------}
Procedure ASCII201;
begin
   carac[ 1]:='11111111';
   carac[ 2]:='11111111';
   carac[ 3]:='11100000';
   carac[ 4]:='11100000';
   carac[ 5]:='11100000';
   carac[ 6]:='11100000';
   carac[ 7]:='11100000';
   carac[ 8]:='11100000';
   carac[ 9]:='11100000';
   carac[10]:='11100000';
   carac[11]:='11100000';
   carac[12]:='11100000';
   carac[13]:='11100000';
   carac[14]:='11100000';
   carac[15]:='11100000';
   carac[16]:='11100000';
   copychar(201);
end;
{-----------------------------------------------------------------------}
Procedure ASCII187;
begin
   carac[ 1]:='11111111';
   carac[ 2]:='11111111';
   carac[ 3]:='00000111';
   carac[ 4]:='00000111';
   carac[ 5]:='00000111';
   carac[ 6]:='00000111';
   carac[ 7]:='00000111';
   carac[ 8]:='00000111';
   carac[ 9]:='00000111';
   carac[10]:='00000111';
   carac[11]:='00000111';
   carac[12]:='00000111';
   carac[13]:='00000111';
   carac[14]:='00000111';
   carac[15]:='00000111';
   carac[16]:='00000111';
   copychar(187);
end;
{-----------------------------------------------------------------------}
Procedure ASCII188;
begin
   carac[ 1]:='00000111';
   carac[ 2]:='00000111';
   carac[ 3]:='00000111';
   carac[ 4]:='00000111';
   carac[ 5]:='00000111';
   carac[ 6]:='00000111';
   carac[ 7]:='00000111';
   carac[ 8]:='00000111';
   carac[ 9]:='00000111';
   carac[10]:='00000111';
   carac[11]:='00000111';
   carac[12]:='00000111';
   carac[13]:='00000111';
   carac[14]:='00000111';
   carac[15]:='11111111';
   carac[16]:='11111111';
   copychar(188);
end;
{-----------------------------------------------------------------------}
Procedure ASCII200;
begin
   carac[ 1]:='11100000';
   carac[ 2]:='11100000';
   carac[ 3]:='11100000';
   carac[ 4]:='11100000';
   carac[ 5]:='11100000';
   carac[ 6]:='11100000';
   carac[ 7]:='11100000';
   carac[ 8]:='11100000';
   carac[ 9]:='11100000';
   carac[10]:='11100000';
   carac[11]:='11100000';
   carac[12]:='11100000';
   carac[13]:='11100000';
   carac[14]:='11100000';
   carac[15]:='11111111';
   carac[16]:='11111111';
   copychar(200);
end;
{-----------------------------------------------------------------------}
Procedure ASCII208;
begin
   carac[ 1]:='00000000';
   carac[ 2]:='00000000';
   carac[ 3]:='00000000';
   carac[ 4]:='00000000';
   carac[ 5]:='00000000';
   carac[ 6]:='00000000';
   carac[ 7]:='00000000';
   carac[ 8]:='00000000';
   carac[ 9]:='00000000';
   carac[10]:='00000000';
   carac[11]:='00000000';
   carac[12]:='00000000';
   carac[13]:='00000000';
   carac[14]:='00000000';
   carac[15]:='11111111';
   carac[16]:='11111111';
   copychar(208);
end;
{-----------------------------------------------------------------------}
Procedure ASCII205;
begin
   carac[ 1]:='11111111';
   carac[ 2]:='11111111';
   carac[ 3]:='00000000';
   carac[ 4]:='00000000';
   carac[ 5]:='00000000';
   carac[ 6]:='00000000';
   carac[ 7]:='00000000';
   carac[ 8]:='00000000';
   carac[ 9]:='00000000';
   carac[10]:='00000000';
   carac[11]:='00000000';
   carac[12]:='00000000';
   carac[13]:='00000000';
   carac[14]:='00000000';
   carac[15]:='00000000';
   carac[16]:='00000000';
   copychar(205);
end;
{-----------------------------------------------------------------------}
Procedure ASCII199;
begin
   carac[ 1]:='00000000';
   carac[ 2]:='00000000';
   carac[ 3]:='00000000';
   carac[ 4]:='00000000';
   carac[ 5]:='00000000';
   carac[ 6]:='11111111';
   carac[ 7]:='00000000';
   carac[ 8]:='00000000';
   carac[ 9]:='00100000';
   carac[10]:='01000000';
   carac[11]:='11111111';
   carac[12]:='01000000';
   carac[13]:='00100000';
   carac[14]:='00000000';
   carac[15]:='00000000';
   carac[16]:='00000000';
   copychar(199);
end;
{-----------------------------------------------------------------------}
Procedure ASCII195;
begin
   carac[ 1]:='00000000';
   carac[ 2]:='00000000';
   carac[ 3]:='00000000';
   carac[ 4]:='00000100';
   carac[ 5]:='00000010';
   carac[ 6]:='11111111';
   carac[ 7]:='00000010';
   carac[ 8]:='00000100';
   carac[ 9]:='00000000';
   carac[10]:='00000000';
   carac[11]:='11111111';
   carac[12]:='00000000';
   carac[13]:='00000000';
   carac[14]:='00000000';
   carac[15]:='00000000';
   carac[16]:='00000000';
   copychar(195);
end;
{-----------------------------------------------------------------------}
Procedure ASCII01;
begin
   carac[ 1]:='11111000';
   carac[ 2]:='10000000';
   carac[ 3]:='10000000';
   carac[ 4]:='10000000';
   carac[ 5]:='11110000';
   carac[ 6]:='10000000';
   carac[ 7]:='10000000';
   carac[ 8]:='10000100';
   carac[ 9]:='10001100';
   carac[10]:='00010100';
   carac[11]:='00100100';
   carac[12]:='00000100';
   carac[13]:='00000100';
   carac[14]:='00000100';
   carac[15]:='00011111';
   carac[16]:='00000000';
   copychar(01);
end;
{-----------------------------------------------------------------------}
Procedure ASCII02;
begin
   carac[ 1]:='11111000';
   carac[ 2]:='10000000';
   carac[ 3]:='10000000';
   carac[ 4]:='10000000';
   carac[ 5]:='11110000';
   carac[ 6]:='10000000';
   carac[ 7]:='10000000';
   carac[ 8]:='10000000';
   carac[ 9]:='10011110';
   carac[10]:='00100001';
   carac[11]:='00000010';
   carac[12]:='00000100';
   carac[13]:='00001000';
   carac[14]:='00010000';
   carac[15]:='00111111';
   carac[16]:='00000000';
   copychar(02);
end;
{-----------------------------------------------------------------------}
Procedure ASCII03;
begin
   carac[ 1]:='11111000';
   carac[ 2]:='10000000';
   carac[ 3]:='10000000';
   carac[ 4]:='10000000';
   carac[ 5]:='11110000';
   carac[ 6]:='10000000';
   carac[ 7]:='10000000';
   carac[ 8]:='10000000';
   carac[ 9]:='10011110';
   carac[10]:='00100001';
   carac[11]:='00000001';
   carac[12]:='00001110';
   carac[13]:='00000001';
   carac[14]:='00100001';
   carac[15]:='00011110';
   carac[16]:='00000000';
   copychar(03);
end;
{-----------------------------------------------------------------------}
Procedure ASCII04;
begin
   carac[ 1]:='11111000';
   carac[ 2]:='10000000';
   carac[ 3]:='10000000';
   carac[ 4]:='10000000';
   carac[ 5]:='11110000';
   carac[ 6]:='10000000';
   carac[ 7]:='10000000';
   carac[ 8]:='10000110';
   carac[ 9]:='10001010';
   carac[10]:='00010010';
   carac[11]:='00100010';
   carac[12]:='00100010';
   carac[13]:='00111111';
   carac[14]:='00000010';
   carac[15]:='00000111';
   carac[16]:='00000000';
   copychar(04);
end;
{-----------------------------------------------------------------------}
Procedure ASCII10;
begin
   carac[ 1]:='11111000';
   carac[ 2]:='10000000';
   carac[ 3]:='10000000';
   carac[ 4]:='10000000';
   carac[ 5]:='11110000';
   carac[ 6]:='10000000';
   carac[ 7]:='10000000';
   carac[ 8]:='10000000';
   carac[ 9]:='10100110';
   carac[10]:='01101001';
   carac[11]:='10101001';
   carac[12]:='00101001';
   carac[13]:='00101001';
   carac[14]:='00101001';
   carac[15]:='01110110';
   carac[16]:='00000000';
   copychar(10);
end;
{-----------------------------------------------------------------------}
Procedure ASCII196;
begin
   carac[ 1]:='00000000';
   carac[ 2]:='00000000';
   carac[ 3]:='00000000';
   carac[ 4]:='00000000';
   carac[ 5]:='00000000';
   carac[ 6]:='00000000';
   carac[ 7]:='00000000';
   carac[ 8]:='00000000';
   carac[ 9]:='00000000';
   carac[10]:='00000000';
   carac[11]:='00000000';
   carac[12]:='00001111';
   carac[13]:='00111000';
   carac[14]:='01100000';
   carac[15]:='01100000';
   carac[16]:='01100000';
   copychar(196);
end;
{-----------------------------------------------------------------------}
Procedure ASCII194;
begin
   carac[ 1]:='00000000';
   carac[ 2]:='00000000';
   carac[ 3]:='01111111';
   carac[ 4]:='00001110';
   carac[ 5]:='00001110';
   carac[ 6]:='00001110';
   carac[ 7]:='00001110';
   carac[ 8]:='00001110';
   carac[ 9]:='00001110';
   carac[10]:='00001110';
   carac[11]:='00001110';
   carac[12]:='00001111';
   carac[13]:='00001110';
   carac[14]:='00001110';
   carac[15]:='00001110';
   carac[16]:='00001110';
   copychar(194);
end;
{-----------------------------------------------------------------------}
Procedure ASCII213;
begin
   carac[ 1]:='00000000';
   carac[ 2]:='00000000';
   carac[ 3]:='11110000';
   carac[ 4]:='10000000';
   carac[ 5]:='10000000';
   carac[ 6]:='10000000';
   carac[ 7]:='10000000';
   carac[ 8]:='10000000';
   carac[ 9]:='10000000';
   carac[10]:='10000000';
   carac[11]:='10000000';
   carac[12]:='10000000';
   carac[13]:='10000000';
   carac[14]:='10000000';
   carac[15]:='10000000';
   carac[16]:='10000000';
   copychar(213);
end;
{-----------------------------------------------------------------------}
Procedure ASCII197;
begin
   carac[ 1]:='00000000';
   carac[ 2]:='00000000';
   carac[ 3]:='10001111';
   carac[ 4]:='00000001';
   carac[ 5]:='00000001';
   carac[ 6]:='00000001';
   carac[ 7]:='00000001';
   carac[ 8]:='00000001';
   carac[ 9]:='00000001';
   carac[10]:='00000001';
   carac[11]:='00000001';
   carac[12]:='11111111';
   carac[13]:='00000001';
   carac[14]:='00000001';
   carac[15]:='00000001';
   carac[16]:='00000001';
   copychar(197);
end;
{-----------------------------------------------------------------------}
Procedure ASCII202;
begin
   carac[ 1]:='00000000';
   carac[ 2]:='00000000';
   carac[ 3]:='00000000';
   carac[ 4]:='00000000';
   carac[ 5]:='00000000';
   carac[ 6]:='00000000';
   carac[ 7]:='00000000';
   carac[ 8]:='00000000';
   carac[ 9]:='00000000';
   carac[10]:='00000000';
   carac[11]:='00000000';
   carac[12]:='11000100';
   carac[13]:='00111100';
   carac[14]:='00011100';
   carac[15]:='00001100';
   carac[16]:='00001100';
   CopyChar(202);
end;
{------------------------------------------------------------------------}
Procedure ASCII209;
begin
   carac[ 1]:='00001110';
   carac[ 2]:='00001110';
   carac[ 3]:='00001110';
   carac[ 4]:='00001110';
   carac[ 5]:='00111111';
   carac[ 6]:='00000000';
   carac[ 7]:='00000000';
   carac[ 8]:='00000000';
   carac[ 9]:='00000000';
   carac[10]:='00000000';
   carac[11]:='00000000';
   carac[12]:='00000000';
   carac[13]:='00000000';
   carac[14]:='00000000';
   carac[15]:='00000000';
   carac[16]:='00000000';
   copychar(209);
end;
{-----------------------------------------------------------------------}
Procedure ASCII210;
begin
   carac[ 1]:='00000001';
   carac[ 2]:='00000001';
   carac[ 3]:='00000001';
   carac[ 4]:='00000001';
   carac[ 5]:='10001111';
   carac[ 6]:='00000000';
   carac[ 7]:='00000000';
   carac[ 8]:='00000000';
   carac[ 9]:='00000000';
   carac[10]:='00000000';
   carac[11]:='00000000';
   carac[12]:='00000000';
   carac[13]:='00000000';
   carac[14]:='00000000';
   carac[15]:='00000000';
   carac[16]:='00000000';
   copychar(210);
end;
{-----------------------------------------------------------------------}
Procedure ASCII216;
begin
   carac[ 1]:='10000000';
   carac[ 2]:='10000000';
   carac[ 3]:='10000000';
   carac[ 4]:='10000000';
   carac[ 5]:='11110000';
   carac[ 6]:='00000000';
   carac[ 7]:='00000000';
   carac[ 8]:='00000000';
   carac[ 9]:='00000000';
   carac[10]:='00000000';
   carac[11]:='00000000';
   carac[12]:='00000000';
   carac[13]:='00000000';
   carac[14]:='00000000';
   carac[15]:='00000000';
   carac[16]:='00000000';
   copychar(216);
end;
{-----------------------------------------------------------------------}
Procedure ASCII222;
begin
   carac[ 1]:='01100000';
   carac[ 2]:='01110000';
   carac[ 3]:='00111110';
   carac[ 4]:='00011111';
   carac[ 5]:='00000111';
   carac[ 6]:='00000000';
   carac[ 7]:='00000000';
   carac[ 8]:='00000000';
   carac[ 9]:='00000000';
   carac[10]:='01000000';
   carac[11]:='01000000';
   carac[12]:='01100000';
   carac[13]:='01111000';
   carac[14]:='01001111';
   carac[15]:='00000000';
   carac[16]:='00000000';
   copychar(222);
end;
{-----------------------------------------------------------------------}
Procedure ASCII203;
begin
   carac[ 1]:='00000000';
   carac[ 2]:='00000000';
   carac[ 3]:='00000000';
   carac[ 4]:='11000000';
   carac[ 5]:='11110000';
   carac[ 6]:='11111000';
   carac[ 7]:='00111000';
   carac[ 8]:='00001100';
   carac[ 9]:='00001100';
   carac[10]:='00001100';
   carac[11]:='00001100';
   carac[12]:='00011000';
   carac[13]:='00110000';
   carac[14]:='11000000';
   carac[15]:='00000000';
   carac[16]:='00000000';
   copychar(203);
end;
{-----------------------------------------------------------------------}
Procedure ASCII212;
begin
   carac[ 1]:='11111111';
   carac[ 2]:='11111111';
   carac[ 3]:='11100000';
   carac[ 4]:='11100000';
   carac[ 5]:='11100000';
   carac[ 6]:='11100000';
   carac[ 7]:='11100111';
   carac[ 8]:='11100111';
   carac[ 9]:='11100111';
   carac[10]:='11100000';
   carac[11]:='11100000';
   carac[12]:='11100000';
   carac[13]:='11100000';
   carac[14]:='11111111';
   carac[15]:='11111111';
   carac[16]:='11100000';
   copychar(212);
end;
{-----------------------------------------------------------------------}
Procedure ASCII211;
begin
   carac[ 1]:='11111111';
   carac[ 2]:='11111111';
   carac[ 3]:='00000011';
   carac[ 4]:='00000011';
   carac[ 5]:='00000011';
   carac[ 6]:='00000011';
   carac[ 7]:='11110011';
   carac[ 8]:='11110011';
   carac[ 9]:='11110011';
   carac[10]:='00000011';
   carac[11]:='00000011';
   carac[12]:='00000011';
   carac[13]:='00000011';
   carac[14]:='11111111';
   carac[15]:='11111111';
   carac[16]:='00000000';
   copychar(211);
end;
{-----------------------------------------------------------------------}
Procedure ASCII179;
begin
   carac[ 1]:='00111100';
   carac[ 2]:='00111100';
   carac[ 3]:='00111100';
   carac[ 4]:='00111100';
   carac[ 5]:='00111100';
   carac[ 6]:='00111100';
   carac[ 7]:='00111100';
   carac[ 8]:='00111100';
   carac[ 9]:='00111100';
   carac[10]:='00111100';
   carac[11]:='00111100';
   carac[12]:='00111100';
   carac[13]:='00111100';
   carac[14]:='00111100';
   carac[15]:='00111100';
   carac[16]:='00111100';
   copychar(179);
end;
{-----------------------------------------------------------------------}begin
  GetAsciiTable;
  ASCII194;  ASCII196;  ASCII197;  ASCII202;  ASCII203;
  ASCII209;  ASCII210;  ASCII213;  ASCII216;  ASCII222;  ASCII132;
  ASCII186;  ASCII247;  ASCII201;  ASCII187;  ASCII188;  ASCII200;
  ASCII208;  ASCII205;  ASCII199;  ASCII195;  ASCII01;   ASCII02;
  ASCII03;   ASCII04;   ASCII10;   ASCII211;  ASCII212;  ASCII179;
end.
