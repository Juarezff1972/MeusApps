Program Senha;
uses crt,dos;
var code  : string[8];
    try   : string[8];
    take,a: char;
    check,verify : word;
    regs  : registers;
    OK    : boolean;
    x1,y1,x2,y2,i : byte;
{---------------------------------------------------------------------------}
Procedure boot;
begin
   inline($ba/$40/$00/$8e/$da/$bb/$72/$00/$c7/$07/$34/$12/$ea/$00/$00/$ff/$ff/$90);
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
procedure Moldura(x1,y1,x2,y2 : integer);
var x,y : integer;
begin
   window(x1,y1,x2,y2);
   gotoxy(1,1);             writeln('Û');
   gotoxy(x2-x1-1,y2-y1);     writeln('ß ');
   gotoxy(x2-x1-1,1);         writeln('Û ');
   gotoxy(1,y2-y1);         writeln('ß');
   for y:=2 to y2-(y1+1) do
   begin
      gotoxy(1,y);         writeln('Û ');
      gotoxy(x2-x1-1,y);     writeln('Û ');
   end;
   for x:=2 to x2-(x1+1)-1 do
   begin
      gotoxy(x,1);         writeln('ß');
      gotoxy(x,y2-y1);     writeln('ß');
   end;
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
Procedure DirectWrite(Char,Atributo:Byte);
Begin
   InLine($06/                 {    push es               }
          $56/                 {    push di               }
          $57/                 {    push si               }
          $8a/$66/<Atributo/   {    mov  ah,Atributo[bp]  }
          $8a/$46/<Char/       {    mov  al,Char[bp]      }
          $b9/$a4/$0f/         {    mov  cx,fa4h          }
          $bb/$00/$b8/         {    mov  bx,b800h         }
          $8e/$c3/             {    mov  es,bx            }
          $33/$db/             {    xor  bx,bx            }
          $26/$89/$07/         { LB:mov  es:[bx],ax       }
          $83/$c3/$02/         {    add  bx,2             }
          $49/                 {    dec  cx               }
          $e2/$f7/             {    loop LB               }
          $5f/                 {    pop  si               }
          $5e/                 {    pop  di               }
          $07);                {    pop  es               }
End;
{---------------------------------------------------------------------------}

begin
   regs.ax:=$168;
   regs.cx:=$1900;
   intr($10,regs);
   code:='PASSWORD';
   clrscr;
   code[1]:=chr(65);     code[5]:=chr(49);
   code[2]:=chr(66);     code[6]:=chr(50);
   code[3]:=chr(67);     code[7]:=chr(51);
   code[4]:=chr(68);     code[8]:=chr(52);
   check:=65+66+67+68+49+50+51+52;
   verify:=0;
   for i:=1 to 8 do inc(verify,ord(code[i]));
   if verify<>check then boot;
   DirectWrite(176,cyan*16+lightcyan);
   x1:=17;
   x2:=57;
   y1:=10;
   y2:=15;
   Sombra(x1,y1,x2,y2,cyan);
   Sombra(x1+1,y1+1,x1+2,y1+2,cyan);
   textcolor(15);
   Moldura(3,2,79,25);
   Retangulo(x1,y1,x2,y2);
   gotoxy(x1+2,y1+2);
   writeln('Entre com a senha correta:  ');
   ok:=false;
   x1:=1;
   x2:=1;
   try:='';
   while keypressed do a:=readkey;
   repeat
      if keypressed then
      begin
         take:=upcase(readkey);
         inc(x1);
         try:=try+take;
         gotoxy(x1+45,y1+2);
         writeln('*');
      end;
      if x1=9 then Ok:=true;
      if x2=4 then
      begin
         gotoxy(x1+46,y1+2);
         writeln('O');
         x2:=1;
      end;
      if x2=3 then
      begin
         gotoxy(x1+46,y1+2);
         writeln('o');
         inc(x2);
      end;
      if x2=2 then
      begin
         gotoxy(x1+46,y1+2);
         writeln('ø');
         inc(x2);
      end;
      if x2=1 then
      begin
         gotoxy(x1+46,y1+2);
         writeln('ú');
         inc(x2);
      end;
      delay(50);
   until Ok;
   delay(50);
   if try<>code then boot;
   clrscr;
   writeln('Sistema ativado, carregado e pronto para uso!');
   writeln;
   writeln;
   delay(50);
   regs.ax:=$168;
   regs.cx:=$607;
   intr($10,regs);
end.