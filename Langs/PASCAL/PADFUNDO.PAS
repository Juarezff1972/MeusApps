uses dos,crt;
type chtb = array[1..4096] of byte;
var regs : registers;
    x    : ^byte;
    y,z  : byte;
    i    : word;
    t    : chtb;
    v    : byte;
    w    : array[1..16] of string[8];
    aux  : byte;
    u    : integer;
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
   w[ 1]:='10001111';
   w[ 2]:='10000111';
   w[ 3]:='10100011';
   w[ 4]:='10100001';
   w[ 5]:='10101000';
   w[ 6]:='10101000';
   w[ 7]:='10101010';
   w[ 8]:='11111111';
   w[ 9]:='11111111';
   w[10]:='10101010';
   w[11]:='10101000';
   w[12]:='10101000';
   w[13]:='10100001';
   w[14]:='10100011';
   w[15]:='10000111';
   w[16]:='10001111';
   aux:=0;
   for i:=1 to 16 do
   begin
      val(copy(w[i],1,1),v,u);
      aux:=2*2*2*2*2*2*2*v;
      val(copy(w[i],2,1),v,u);
      aux:=2*2*2*2*2*2*v+aux;
      val(copy(w[i],3,1),v,u);
      aux:=2*2*2*2*2*v+aux;
      val(copy(w[i],4,1),v,u);
      aux:=2*2*2*2*v+aux;
      val(copy(w[i],5,1),v,u);
      aux:=2*2*2*v+aux;
      val(copy(w[i],6,1),v,u);
      aux:=2*2*v+aux;
      val(copy(w[i],7,1),v,u);
      aux:=2*v+aux;
      val(copy(w[i],8,1),v,u);
      aux:=v+aux;
      t[176*16+(i-1)]:=aux;
   end;
   for i:=176*16 to 176*16+15 do
   begin
      y:=t[i];
      write(y:3,' - ',i:4,' - ',(i div 16):3,' - ');
      if y>=128 then
      begin
         write('O');
         y:=y-128;
      end
      else write('.');
      if y>=64 then
      begin
         write('O');
         y:=y-64;
      end
      else write('.');
      if y>=32 then
      begin
         write('O');
         y:=y-32;
      end
      else write('.');
      if y>=16 then
      begin
         write('O');
         y:=y-16;
      end
      else write('.');
      if y>=8 then
      begin
         write('O');
         y:=y-8;
      end
      else write('.');
      if y>=4 then
      begin
         write('O');
         y:=y-4;
      end
      else write('.');
      if y>=2 then
      begin
         write('O');
         y:=y-2;
      end
      else write('.');
      if y>=1 then
      begin
         writeln('O');
         y:=y-1;
      end
      else writeln('.');
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
   writeln(chr(176));
   writeln;
   for i:=1 to 255 do
   begin
      textattr:=i;
      write(chr(176));
   end;
   textattr:=7;
   writeln;
   readln;
end.
