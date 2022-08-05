Program CannotStop;
uses dos,crt;
var esc,brk,cad       : integer;
    botao,mn,oldbot   : byte;
    break,c,a,d       : boolean;
    abcd              : pointer;
    regs              : registers;
procedure nostop;
interrupt;
begin
   ESC:=0;
   BRK:=0;
   CAD:=0;
   mn:=0;
   c:=false;
   a:=false;
   d:=false;
   setcbreak(false);
   clrscr;
   writeln('Tente sair deste programa! (Apertar RESET ou desligar o micro n„o vale!)');
   repeat
      if Botao>0 then OldBot:=Botao;
      inline($50/$e4/$60/$a3/>Botao/$58);     { push ax; in al,60; mov Botao,ax; pop ax }
      if Botao=OldBot then Botao:=0;
      if (botao=83) then c:=true;
      if (botao=56) then a:=true;
      if (botao=29) then d:=true;
      if botao=224 then botao:=0;
      if ((botao<>83) and (botao<>56) and (botao<>29) and (botao<>0) and (botao>127)) then
         begin
            c:=false;
            a:=false;
            d:=false;
         end;
      if botao=198 then
      begin
         c:=false;
         a:=false;
         d:=false;
      end;
      if botao=1 then ESC:=ESC+1;
      if ((c) and (a) and (d)) then
      begin
         CAD:=CAD+1;
         c:=false;
         a:=false;
         d:=false;
      end;
      if botao=198 then brk:=brk+1;
      gotoxy(10,10);
      writeln('Tentativas da tecla ESC: ',ESC);
      gotoxy(10,12);
      writeln('Tentativas da tecla BREAK: ',BRK);
      gotoxy(10,14);
      writeln('Tentativas das teclas CTRL+ALT+DEL: ',CAD);
   until botao=79;
   setintvec($1c,abcd);
   setcbreak(true);
end;
begin
   regs.ax:=$168;
   regs.cx:=$1900;
   intr($10,regs);
   getintvec($1c,abcd);
   setintvec($1c,@nostop);
   inline($cd/$1c);                { int 1c }
   setintvec($1c,abcd);
   clrscr;
   gotoxy(10,16);
   writeln('Voce acertou!!! - <END> apenas sai!!!');
   regs.ax:=$168;
   regs.cx:=$607;
   intr($10,regs);
end.

