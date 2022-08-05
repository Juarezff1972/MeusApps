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
    chrt : byte;
begin
   chrt:=255;
   regs.ah:=$11;  { Reseta Tabela }
   regs.al:=4;    { 4 = 8x16(VGA), 2 = 8x8(EGA/VGA),  1 = 8x14(EGA/VGA) }
   regs.bl:=0;
   intr($10,regs);
   regs.ah:=$11;
   regs.al:=$30; { Pega Informa‡„o da Tabela}
   regs.bh:=6;   { 8 = 8x16, 7 = 9x16, 5 = 9x14, 3 = 8x8, 2 = 8x14 }
   intr($10,regs);
   for i:=0 to 255*16+15 do
   begin
      x:=ptr(regs.es,regs.bp+i);
      t[i]:=x^;
   end;
   w[ 1]:='00111000';
   w[ 2]:='01000100';
   w[ 3]:='10000010';
   w[ 4]:='10000010';
   w[ 5]:='10000010';
   w[ 6]:='01000100';
   w[ 7]:='00111000';
   w[ 8]:='01010000';
   w[ 9]:='11010110';
   w[10]:='11010110';
   w[11]:='00010000';
   w[12]:='00010000';
   w[13]:='00010000';
   w[14]:='01010100';
   w[15]:='00111000';
   w[16]:='00010000';
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
      t[chrt*16+(i-1)]:=aux;
   end;
   for i:=chrt*16 to chrt*16+15 do
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
   regs.al:=$0; { $10 = Carrega e ajusta altura, $0 = Carrega }
   regs.cx:=256;{ No. de caracteres da tabela }
   regs.dx:=0;   { Codigo ASCII do primeiro caracter da tabela }
   regs.bh:=16;  { No. de bytes por caracter }
   regs.es:=seg(t); { Segmento da Tabela }
   regs.bp:=ofs(t)-1; { Offset da Tabela }
   intr($10,regs);
   writeln;
   writeln(chr(chrt));
   writeln;
   for i:=1 to 255 do
   begin
      textattr:=i;
      write(chr(chrt));
   end;
   textattr:=7;
   writeln;
   readln;
end.
