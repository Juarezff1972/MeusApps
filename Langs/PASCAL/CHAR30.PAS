
uses dos,crt;
type chtb = array[1..4096] of byte;
var regs : registers;
    x    : ^byte;
    y,z  : byte;
    i    : word;
    t    : chtb;
    v    : byte;

    char193,char196,char180,char255,char30,char31,char201,char197,
    char194 : array[1..16] of string[8];

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
      y:=x^;
      t[i]:=x^;
      if y>=128 then y:=y-128;
      if y>=64 then  y:=y-64;
      if y>=32 then  y:=y-32;
      if y>=16 then  y:=y-16;
      if y>=8 then   y:=y-8;
      if y>=4 then   y:=y-4;
      if y>=2 then   y:=y-2;
      if y>=1 then   y:=y-1;
   end;
end;
{-----------------------------------------------------------------------}
Procedure ASCII30;
begin
   char30[ 1]:='00011000';
   char30[ 2]:='00111100';
   char30[ 3]:='01111110';
   char30[ 4]:='11011011';
   char30[ 5]:='10011001';
   char30[ 6]:='00011000';
   char30[ 7]:='00011000';
   char30[ 8]:='00011000';
   char30[ 9]:='00011000';
   char30[10]:='00011000';
   char30[11]:='00111100';
   char30[12]:='01100110';
   char30[13]:='11000011';
   char30[14]:='10000001';
   char30[15]:='00000000';
   char30[16]:='00000000';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char30[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char30[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char30[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char30[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char30[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char30[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char30[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char30[i],8,1),v,u);
      aux:=v+aux;
      t[30*16+(i-1)]:=aux;
   end;
   for i:=30*16 to 30*16+15 do
   begin
      y:=t[i];
      write(y:3,' - ',i:4,' - ',(i div 16):3,' - ');
      if y>=128 then
      begin
         write('OO');
         y:=y-128;
      end
      else write('..');
      if y>=64 then
      begin
         write('OO');
         y:=y-64;
      end
      else write('..');
      if y>=32 then
      begin
         write('OO');
         y:=y-32;
      end
      else write('..');
      if y>=16 then
      begin
         write('OO');
         y:=y-16;
      end
      else write('..');
      if y>=8 then
      begin
         write('OO');
         y:=y-8;
      end
      else write('..');
      if y>=4 then
      begin
         write('OO');
         y:=y-4;
      end
      else write('..');
      if y>=2 then
      begin
         write('OO');
         y:=y-2;
      end
      else write('..');
      if y>=1 then
      begin
         writeln('OO');
         y:=y-1;
      end
      else writeln('..');
   end;
   readln;
   regs.ah:=$11;
   regs.al:=$0;
   regs.cx:=256;
   regs.dx:=0;
   regs.bh:=16;
   regs.es:=seg(t);
   regs.bp:=ofs(t)-1;
   intr($10,regs);
   writeln;
   writeln(chr(30));
   writeln;
   for i:=1 to 255 do
   begin
      textattr:=i;
      write(chr(30));
   end;
   textattr:=7;
end;
{-----------------------------------------------------------------------}
Procedure ASCII194;
begin
   char194[ 1]:='00000000';
   char194[ 2]:='11100111';
   char194[ 3]:='11100111';
   char194[ 4]:='00000000';
   char194[ 5]:='00000000';
   char194[ 6]:='01111110';
   char194[ 7]:='01111110';
   char194[ 8]:='00000000';
   char194[ 9]:='00000000';
   char194[10]:='11100111';
   char194[11]:='11100111';
   char194[12]:='00000000';
   char194[13]:='00000000';
   char194[14]:='01111110';
   char194[15]:='01111110';
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
   for i:=194*16 to 194*16+15 do
   begin
      y:=t[i];
      write(y:3,' - ',i:4,' - ',(i div 16):3,' - ');
      if y>=128 then
      begin
         write('OO');
         y:=y-128;
      end
      else write('..');
      if y>=64 then
      begin
         write('OO');
         y:=y-64;
      end
      else write('..');
      if y>=32 then
      begin
         write('OO');
         y:=y-32;
      end
      else write('..');
      if y>=16 then
      begin
         write('OO');
         y:=y-16;
      end
      else write('..');
      if y>=8 then
      begin
         write('OO');
         y:=y-8;
      end
      else write('..');
      if y>=4 then
      begin
         write('OO');
         y:=y-4;
      end
      else write('..');
      if y>=2 then
      begin
         write('OO');
         y:=y-2;
      end
      else write('..');
      if y>=1 then
      begin
         writeln('OO');
         y:=y-1;
      end
      else writeln('..');
   end;
   readln;
   regs.ah:=$11;
   regs.al:=$0;
   regs.cx:=256;
   regs.dx:=0;
   regs.bh:=16;
   regs.es:=seg(t);
   regs.bp:=ofs(t)-1;
   intr($10,regs);
   writeln;
   writeln(chr(194));
   writeln;
   for i:=1 to 2000 do
   begin
      textcolor(cyan);
      write(chr(194));
   end;
   textattr:=7;
end;
{-----------------------------------------------------------------------}
Procedure ASCII196;
begin
   char196[ 1]:='00001110';
   char196[ 2]:='00001000';
   char196[ 3]:='00001000';
   char196[ 4]:='00001000';
   char196[ 5]:='00111000';
   char196[ 6]:='00100000';
   char196[ 7]:='00100000';
   char196[ 8]:='00100000';
   char196[ 9]:='11100000';
   char196[10]:='10000000';
   char196[11]:='10000000';
   char196[12]:='10000000';
   char196[13]:='10000011';
   char196[14]:='00000010';
   char196[15]:='00000010';
   char196[16]:='00000010';
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
   for i:=196*16 to 196*16+15 do
   begin
      y:=t[i];
      write(y:3,' - ',i:4,' - ',(i div 16):3,' - ');
      if y>=128 then
      begin
         write('OO');
         y:=y-128;
      end
      else write('..');
      if y>=64 then
      begin
         write('OO');
         y:=y-64;
      end
      else write('..');
      if y>=32 then
      begin
         write('OO');
         y:=y-32;
      end
      else write('..');
      if y>=16 then
      begin
         write('OO');
         y:=y-16;
      end
      else write('..');
      if y>=8 then
      begin
         write('OO');
         y:=y-8;
      end
      else write('..');
      if y>=4 then
      begin
         write('OO');
         y:=y-4;
      end
      else write('..');
      if y>=2 then
      begin
         write('OO');
         y:=y-2;
      end
      else write('..');
      if y>=1 then
      begin
         writeln('OO');
         y:=y-1;
      end
      else writeln('..');
   end;
   readln;
   regs.ah:=$11;
   regs.al:=$0;
   regs.cx:=256;
   regs.dx:=0;
   regs.bh:=16;
   regs.es:=seg(t);
   regs.bp:=ofs(t)-1;
   intr($10,regs);
   writeln;
   writeln(chr(196));
   writeln;
   for i:=1 to 2000 do
   begin
      textcolor(cyan);
      write(chr(196));
   end;
   textattr:=7;
end;
{-----------------------------------------------------------------------}
Procedure ASCII180;
begin
   char180[ 1]:='10000001';
   char180[ 2]:='01000010';
   char180[ 3]:='00100100';
   char180[ 4]:='00011000';
   char180[ 5]:='00011000';
   char180[ 6]:='00100100';
   char180[ 7]:='01000010';
   char180[ 8]:='10000001';
   char180[ 9]:='10000001';
   char180[10]:='01000010';
   char180[11]:='00100100';
   char180[12]:='00011000';
   char180[13]:='00011000';
   char180[14]:='00100100';
   char180[15]:='01000010';
   char180[16]:='10000001';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char180[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char180[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char180[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char180[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char180[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char180[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char180[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char180[i],8,1),v,u);
      aux:=v+aux;
      t[180*16+(i-1)]:=aux;
   end;
   for i:=180*16 to 180*16+15 do
   begin
      y:=t[i];
      write(y:3,' - ',i:4,' - ',(i div 16):3,' - ');
      if y>=128 then
      begin
         write('OO');
         y:=y-128;
      end
      else write('..');
      if y>=64 then
      begin
         write('OO');
         y:=y-64;
      end
      else write('..');
      if y>=32 then
      begin
         write('OO');
         y:=y-32;
      end
      else write('..');
      if y>=16 then
      begin
         write('OO');
         y:=y-16;
      end
      else write('..');
      if y>=8 then
      begin
         write('OO');
         y:=y-8;
      end
      else write('..');
      if y>=4 then
      begin
         write('OO');
         y:=y-4;
      end
      else write('..');
      if y>=2 then
      begin
         write('OO');
         y:=y-2;
      end
      else write('..');
      if y>=1 then
      begin
         writeln('OO');
         y:=y-1;
      end
      else writeln('..');
   end;
   readln;
   regs.ah:=$11;
   regs.al:=$0;
   regs.cx:=256;
   regs.dx:=0;
   regs.bh:=16;
   regs.es:=seg(t);
   regs.bp:=ofs(t)-1;
   intr($10,regs);
   writeln;
   writeln(chr(180));
   writeln;
   for i:=1 to 2000 do
   begin
      textcolor(cyan);
      write(chr(180));
   end;
   textattr:=7;
end;
{-----------------------------------------------------------------------}
Procedure ASCII193;
begin
   char193[ 1]:='11111111';
   char193[ 2]:='01000010';
   char193[ 3]:='01000010';
   char193[ 4]:='01000010';
   char193[ 5]:='11111111';
   char193[ 6]:='01000010';
   char193[ 7]:='01000010';
   char193[ 8]:='01000010';
   char193[ 9]:='11111111';
   char193[10]:='01000010';
   char193[11]:='01000010';
   char193[12]:='01000010';
   char193[13]:='11111111';
   char193[14]:='01000010';
   char193[15]:='01000010';
   char193[16]:='01000010';
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
   for i:=193*16 to 193*16+15 do
   begin
      y:=t[i];
      write(y:3,' - ',i:4,' - ',(i div 16):3,' - ');
      if y>=128 then
      begin
         write('OO');
         y:=y-128;
      end
      else write('..');
      if y>=64 then
      begin
         write('OO');
         y:=y-64;
      end
      else write('..');
      if y>=32 then
      begin
         write('OO');
         y:=y-32;
      end
      else write('..');
      if y>=16 then
      begin
         write('OO');
         y:=y-16;
      end
      else write('..');
      if y>=8 then
      begin
         write('OO');
         y:=y-8;
      end
      else write('..');
      if y>=4 then
      begin
         write('OO');
         y:=y-4;
      end
      else write('..');
      if y>=2 then
      begin
         write('OO');
         y:=y-2;
      end
      else write('..');
      if y>=1 then
      begin
         writeln('OO');
         y:=y-1;
      end
      else writeln('..');
   end;
   readln;
   regs.ah:=$11;
   regs.al:=$0;
   regs.cx:=256;
   regs.dx:=0;
   regs.bh:=16;
   regs.es:=seg(t);
   regs.bp:=ofs(t)-1;
   intr($10,regs);
   writeln;
   writeln(chr(193));
   writeln;
   for i:=1 to 2000 do
   begin
      textcolor(cyan);
      write(chr(193));
   end;
   textattr:=7;
end;
{-----------------------------------------------------------------------}
Procedure ASCII197;
begin
   char197[ 1]:='11111111';
   char197[ 2]:='00011000';
   char197[ 3]:='00011000';
   char197[ 4]:='11111111';
   char197[ 5]:='11111111';
   char197[ 6]:='10000001';
   char197[ 7]:='10000001';
   char197[ 8]:='11111111';
   char197[ 9]:='11111111';
   char197[10]:='00011000';
   char197[11]:='00011000';
   char197[12]:='11111111';
   char197[13]:='11111111';
   char197[14]:='10000001';
   char197[15]:='10000001';
   char197[16]:='11111111';
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
   for i:=197*16 to 197*16+15 do
   begin
      y:=t[i];
      write(y:3,' - ',i:4,' - ',(i div 16):3,' - ');
      if y>=128 then
      begin
         write('OO');
         y:=y-128;
      end
      else write('..');
      if y>=64 then
      begin
         write('OO');
         y:=y-64;
      end
      else write('..');
      if y>=32 then
      begin
         write('OO');
         y:=y-32;
      end
      else write('..');
      if y>=16 then
      begin
         write('OO');
         y:=y-16;
      end
      else write('..');
      if y>=8 then
      begin
         write('OO');
         y:=y-8;
      end
      else write('..');
      if y>=4 then
      begin
         write('OO');
         y:=y-4;
      end
      else write('..');
      if y>=2 then
      begin
         write('OO');
         y:=y-2;
      end
      else write('..');
      if y>=1 then
      begin
         writeln('OO');
         y:=y-1;
      end
      else writeln('..');
   end;
   readln;
   regs.ah:=$11;
   regs.al:=$0;
   regs.cx:=256;
   regs.dx:=0;
   regs.bh:=16;
   regs.es:=seg(t);
   regs.bp:=ofs(t)-1;
   intr($10,regs);
   writeln;
   writeln(chr(197));
   writeln;
   for i:=1 to 2000 do
   begin
      textcolor(cyan);
      write(chr(197));
   end;
   textattr:=7;
end;
{-------------------------------}
Procedure ASCII31;
begin
   char31[16]:='00011000';
   char31[15]:='00111100';
   char31[14]:='01111110';
   char31[13]:='11011011';
   char31[12]:='10011001';
   char31[11]:='00011000';
   char31[10]:='00011000';
   char31[ 9]:='00011000';
   char31[ 8]:='00011000';
   char31[ 7]:='00011000';
   char31[ 6]:='00111100';
   char31[ 5]:='01100110';
   char31[ 4]:='11000011';
   char31[ 3]:='10000001';
   char31[ 2]:='00000000';
   char31[ 1]:='00000000';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char31[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char31[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char31[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char31[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char31[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char31[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char31[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char31[i],8,1),v,u);
      aux:=v+aux;
      t[31*16+(i-1)]:=aux;
   end;
   for i:=31*16 to 31*16+15 do
   begin
      y:=t[i];
      write(y:3,' - ',i:4,' - ',(i div 16):3,' - ');
      if y>=128 then
      begin
         write('OO');
         y:=y-128;
      end
      else write('..');
      if y>=64 then
      begin
         write('OO');
         y:=y-64;
      end
      else write('..');
      if y>=32 then
      begin
         write('OO');
         y:=y-32;
      end
      else write('..');
      if y>=16 then
      begin
         write('OO');
         y:=y-16;
      end
      else write('..');
      if y>=8 then
      begin
         write('OO');
         y:=y-8;
      end
      else write('..');
      if y>=4 then
      begin
         write('OO');
         y:=y-4;
      end
      else write('..');
      if y>=2 then
      begin
         write('OO');
         y:=y-2;
      end
      else write('..');
      if y>=1 then
      begin
         writeln('OO');
         y:=y-1;
      end
      else writeln('..');
   end;
   readln;
   regs.ah:=$11;
   regs.al:=$0;
   regs.cx:=256;
   regs.dx:=0;
   regs.bh:=16;
   regs.es:=seg(t);
   regs.bp:=ofs(t)-1;
   intr($10,regs);
   writeln;
   writeln(chr(31));
   writeln;
   for i:=1 to 255 do
   begin
      textattr:=i;
      write(chr(31));
   end;
   textattr:=7;
end;
{-----------------------------------------------------------------------}
Procedure ASCII201;
begin
   char201[ 1]:='00000000';
   char201[ 2]:='00000000';
   char201[ 3]:='00000000';
   char201[ 4]:='00000000';
   char201[ 5]:='00000000';
   char201[ 6]:='00001111';
   char201[ 7]:='00011000';
   char201[ 8]:='00011001';
   char201[ 9]:='00110011';
   char201[10]:='00110110';
   char201[11]:='00110110';
   char201[12]:='00110110';
   char201[13]:='00110110';
   char201[14]:='00110110';
   char201[15]:='00110110';
   char201[16]:='00110110';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(char201[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(char201[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(char201[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(char201[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(char201[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(char201[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(char201[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(char201[i],8,1),v,u);
      aux:=v+aux;
      t[201*16+(i-1)]:=aux;
   end;
   for i:=201*16 to 201*16+15 do
   begin
      y:=t[i];
      write(y:3,' - ',i:4,' - ',(i div 16):3,' - ');
      if y>=128 then
      begin
         write('OO');
         y:=y-128;
      end
      else write('..');
      if y>=64 then
      begin
         write('OO');
         y:=y-64;
      end
      else write('..');
      if y>=32 then
      begin
         write('OO');
         y:=y-32;
      end
      else write('..');
      if y>=16 then
      begin
         write('OO');
         y:=y-16;
      end
      else write('..');
      if y>=8 then
      begin
         write('OO');
         y:=y-8;
      end
      else write('..');
      if y>=4 then
      begin
         write('OO');
         y:=y-4;
      end
      else write('..');
      if y>=2 then
      begin
         write('OO');
         y:=y-2;
      end
      else write('..');
      if y>=1 then
      begin
         writeln('OO');
         y:=y-1;
      end
      else writeln('..');
   end;
   readln;
   regs.ah:=$11;
   regs.al:=$0;
   regs.cx:=256;
   regs.dx:=0;
   regs.bh:=16;
   regs.es:=seg(t);
   regs.bp:=ofs(t)-1;
   intr($10,regs);
   writeln;
   writeln(chr(201));
   writeln;
   for i:=1 to 255 do
   begin
      textattr:=i;
      write(chr(201));
   end;
   textattr:=7;
end;
{-----------------------------------------------------------------------}
begin
   GetAsciiTable;
   writeln;
   write('Escolha o caracter: ');readln(z);
   writeln;
   writeln('Caracter ',z,': ',chr(z));
   writeln;
   for i:=z*16 to z*16+15 do
   begin
      y:=t[i];
      write(y:3,' - ',i:4,' - ',(i div 16):3,' - ');
      if y>=128 then
      begin
         write('OO');
         y:=y-128;
      end
      else write('..');
      if y>=64 then
      begin
         write('OO');
         y:=y-64;
      end
      else write('..');
      if y>=32 then
      begin
         write('OO');
         y:=y-32;
      end
      else write('..');
      if y>=16 then
      begin
         write('OO');
         y:=y-16;
      end
      else write('..');
      if y>=8 then
      begin
         write('OO');
         y:=y-8;
      end
      else write('..');
      if y>=4 then
      begin
         write('OO');
         y:=y-4;
      end
      else write('..');
      if y>=2 then
      begin
         write('OO');
         y:=y-2;
      end
      else write('..');
      if y>=1 then
      begin
         writeln('OO');
         y:=y-1;
      end
      else writeln('..');
   end;
  readln;
   ASCII30;
   readln;
   ASCII31;
   readln;
   ASCII201;
   readln;
   ascii194;
   readln;
   ascii197;
   readln;
   ascii196;
   readln;
   ascii180;
   readln;
   ascii193;
   readln;
end.
