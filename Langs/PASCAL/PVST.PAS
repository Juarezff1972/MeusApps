Program pvst;
uses crt,dos,printer,caract;
var op     : array[0..10] of string;        filer       : searchrec;
    i,ii   : byte;                          _dest,_orig : word;
    regs   : registers;
    choose : word;
    ch     : char;
    finis  : boolean;
    opch   : array[0..10] of integer;
    arqu   : text;
    Red    : array[0..255] of byte;
    Green  : array[0..255] of byte;
    Blue   : array[0..255] of byte;
    Red1   : array[0..255] of byte;
    Green1 : array[0..255] of byte;
    Blue1  : array[0..255] of byte;
{----------------------------------------------------------------}
procedure Acresentar;
var atr : byte;
begin
   inc(opch[choose]);
   gotoxy(10,23);
   atr:=textattr;
   textcolor(15);
   textbackground(11);
   writeln('Vocˆ escolheu a op‡„o ',choose,'. Obrigado.');
   repeat
      if keypressed then ch:=readkey;
      ch:=#255;
   until ch=#255;
   ch:=#1;
   delay(500);
   inc(opch[10]);
   rewrite(arqu);
   for i:=0 to 10 do writeln(arqu,opch[i]);
   close(arqu);
   gotoxy(10,23);
   textattr:=atr;
   writeln('                                  ');
end;
{----------------------------------------------------------------}
Procedure BarGraph;
var ii : word;
begin
   _dest:=$8c00;
   _orig:=$b800;
   inline($b9/$80/$3e/$33/$ff/$8b/$16/>_dest/$8e/$c2/$33/$f6/$8b/$16/>_orig/$1e/$8e/$da/$d1/$e9/$f3/$a5/$1f);
   clrscr;
   gotoxy(1,24);
   textcolor(0);
   textbackground(15);
   writeln('                Desenvolvido por Juarez Fioravanti Filho                       ');
   gotoxy(1,1);
   textcolor(15);
   textbackground(3);
   write  ('                 Programa Volvo de Seguran‡a no Trƒnsito                       ');
   textcolor(7);
   textbackground(0);
   write(' ');
   gotoxy(1,2);
   for ii:=0 to 9 do
   begin
      writeln(' ³');
      write  (ii,'Ã'); for i:=0 to trunc(opch[ii]*60/opch[10]) do write('±'); writeln(' ',opch[ii]);
   end;
   writeln(' ÀÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄÅÄ');
   writeln('  0 4   10  17  24  30  37  44  50  57  64  70  77  84  90  97  100    %    ');
   repeat until keypressed;
   ch:=readkey;
   ch:=#2;
   _dest:=$b800;
   _orig:=$8c00;
   inline($b9/$80/$3e/$33/$ff/$8b/$16/>_dest/$8e/$c2/$33/$f6/$8b/$16/>_orig/$1e/$8e/$da/$d1/$e9/$f3/$a5/$1f);
end;
{----------------------------------------------------------------}
Procedure VerTotais;
var chh : char;
begin
   _dest:=$8800;
   _orig:=$b800;
   inline($b9/$80/$3e/$33/$ff/$8b/$16/>_dest/$8e/$c2/$33/$f6/$8b/$16/>_orig/$1e/$8e/$da/$d1/$e9/$f3/$a5/$1f);
   clrscr;
   gotoxy(1,24);
   textcolor(0);
   textbackground(15);
   writeln('                Desenvolvido por Juarez Fioravanti Filho                       ');
   gotoxy(1,1);
   textcolor(15);
   textbackground(3);
   write  ('                 Programa Volvo de Seguran‡a no Trƒnsito                       ');
   textcolor(7);
   textbackground(0);
   write(' ');
   gotoxy(1,3);
   writeln('Pergunta   -   Quantidade   -    Porcentagem');
   gotoxy(1,5);
   for i:=0 to 9 do
   begin
      gotoxy(3,5+i);      writeln(i);
      gotoxy(19,5+i);     writeln(opch[i]);
      gotoxy(37,5+i);
      if opch[10]<>0 then writeln(opch[i]*100/opch[10]:1:3,' %');
      if opch[10]=0 then writeln('0 %');
   end;
   gotoxy(1,16);    writeln('Total:');
   gotoxy(19,16);   writeln(opch[10]);
   gotoxy(37,16);   writeln('100 %');
   gotoxy(1,18);
   writeln('Aperte Ctrl+F1 para zerar, Ctrl+F2 para gr fico de barra, ou ENTER para voltar');
   repeat
      chh:=readkey;
      if chh=#0 then
      begin
         chh:=readkey;
         if chh=#94 then
         begin
            writeln('Aperte Ctrl+F10 para confirmar se zerar');
            chh:=readkey;
            if chh=#0 then
            begin
               chh:=readkey;
               if chh=#103 then
               begin
                  for i:=0 to 10 do opch[i]:=0;
                  rewrite(arqu);
                  for i:=0 to 10 do writeln(arqu,opch[i]);
                  close(arqu);
               end;
            end;
         end;
         if (chh=#95)and(opch[10]<>0) then BarGraph;
         chh:=#13;
      end;
   until (chh=#13);
   _dest:=$b800;
   _orig:=$8800;
   inline($b9/$80/$3e/$33/$ff/$8b/$16/>_dest/$8e/$c2/$33/$f6/$8b/$16/>_orig/$1e/$8e/$da/$d1/$e9/$f3/$a5/$1f);
{ Pagina 0 : $b800 Pagina 1 : $8800 Pagina 2 : $8c00 Pagina 3 : $9000 Pagina 4 : $9400 }
end;
{----------------------------------------------------------------}
Procedure Inicio;
var ii,r,g,b : byte;
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
   for ii:=0 to 15 do
   begin
      regs.ah:=$10;
      regs.al:=$15;
      regs.bx:=ii;
      intr($10,regs);
      Red[ii]:=regs.dh;
      Green[ii]:=regs.ch;
      Blue[ii]:=regs.cl;
   end;
   textbackground(0);
   clrscr;
   gotoxy(1,24);
   textcolor(4);
   textbackground(7);
   writeln('                Desenvolvido por Juarez Fioravanti Filho                       ');
   gotoxy(1,1);
   textcolor(white);
   textbackground(4);
   write  ('                 Programa Volvo de Seguran‡a no Trƒnsito                       ');
   textcolor(7);
   textbackground(0);
   write(' ');
   gotoxy(1,3);
   textcolor(yellow);
   writeln('           " N„o corra para n„o fazer chorar quem te espera sorrindo "');
   textcolor(brown);
   writeln('                (Frase de p ra-choque de um camioneiro do Brasil)');
   writeln;
   writeln('  Este programa foi desenvolvido por Juarez Fioravanti Filho, para a realiza‡„o');
   writeln('de uma pesquisa de opini„o sobre o trƒnsito.');
   writeln;
   writeln('   As teclas de opera‡„o do programa s„o:');
   textcolor(yellow);
   writeln;
   writeln('             Ctrl + End - Termina o programa (Cuidado!)');
   writeln('             Ctrl + Home - Acessa a tela de estimativa');
   writeln('             Ctrl + PrtScr - Imprime o resultado da pesquisa');
   writeln;
   textcolor(cyan);
   writeln('   Na tela de estimativa, as seguintes teclas s„o v lidas: ');
   writeln;
   textcolor(lightred);
   writeln(' Ctrl + F1 - Zera todos os dados. Para confirmar, aperte Ctrl + F10 (Cuidado!)');
   writeln(' Ctrl + F2 - Acessa o gr fico de barra para compara‡„o. Para sair, aperte ENTER.');
   writeln(' ENTER ou ESPA€O  - volta ao modo pesquisa');
   textcolor(lightgreen);
   writeln;
   write('Aperte ENTER para continuar, ou ESC para sair do programa.');
   repeat
      ch:=readkey;
   until (ch=#13) or (ch=#27);
   if ch=#27 then
   begin
      clrscr;
      halt(0);
   end;
   for ii:=0 to 255 do
   begin
     repeat
      regs.ah:=$10;
      regs.al:=$15;
      regs.bx:=ii;
      intr($10,regs);
      r:=regs.dh;
      g:=regs.ch;
      b:=regs.cl;
      dec(r);dec(g);dec(b);
      if r=255 then r:=0;
      if g=255 then g:=0;
      if b=255 then b:=0;
      regs.ah:=$10;
      regs.al:=$10;
      regs.bx:=ii;
      regs.dh:=r;
      regs.ch:=g;
      regs.cl:=b;
      intr($10,regs);
     until (((r=0)and(b=0))and(g=0));
   end;
   window(1,2,79,24);
   clrscr;
   window(1,1,79,25);
end;
{----------------------------------------------------------------}
Procedure Imprime;
var ii : word;
begin
   writeln(lst,'             Programa Volvo de Seguranc',chr(8),',a no Tra',chr(8),'^nsito');
   writeln(lst,'  ');
   writeln(lst,'              Desenvolvido por Juarez Fioravanti FIlho');
   writeln(lst,'  ');
   writeln(lst,'As perguntas feitas sa',chr(8),'~o: ');
   write(lst,'1- Educac',chr(8),',a',chr(8),'~o de tra',chr(8),'^nsito como disciplina');
   writeln(lst,' curricular obrigato',chr(8),'''ria no 1o e 2o graus');
   writeln(lst,'2- Fiscalizac',chr(8),',a',chr(8),'~o mais rigorosa contra embriagez ao volante');
   writeln(lst,'3- Punic',chr(8),',a',chr(8),'~o pesada para os infratores de tra',chr(8),'^nsito');
   writeln(lst,'4- Recuperac',chr(8),',a',chr(8),'~o das rodovias');
   writeln(lst,'5- Atualizac',chr(8),',a',chr(8),'~o urgente do Co',chr(8),'''digo Nacional de Tra',chr(8),'^nsito');
   writeln(lst,'6- Campanhas mais efetivas e permanentes de seguranc',chr(8),',a no tra',chr(8),'^nsito');
   writeln(lst,'7- Eliminac',chr(8),',a',chr(8),'~o dos pontos negros rodovia',chr(8),'''rios e urbanos');
   writeln(lst,'8- Normas mais ri',chr(8),'''gidas na obtenc',chr(8),',a',chr(8),'~o da CNH');
   writeln(lst,'9- Reaparelhamento das poli',chr(8),'''cias de tra',chr(8),'^nsito urbano e rodovia',chr(8),'''rios');
   writeln(lst,'0- Uso obrigato',chr(8),'''rio de cinto de seguranc',chr(8),',a nas cidades');
   writeln(lst,'   ');
   writeln(lst,chr(14),'        Estimativa');
   writeln(lst,'   ');
   writeln(lst,'Pergunta   -   Quantidade   -    Porcentagem');
   for i:=0 to 9 do
   begin
      write(lst,'      ',i,'           ',opch[i],'                  ');
      if opch[10]<>0 then writeln(lst,opch[i]*100/opch[10]:1:3,' %');
      if opch[10]=0 then writeln(lst,'0 %');
   end;
   writeln(lst,'Total:           ',opch[10],'                  100 %');
   writeln(lst,'   ');
   writeln(lst,chr(14),'        Gra',chr(8),'''fico de Barra');
   writeln(lst,'   ');
   for ii:=0 to 9 do
   begin
      writeln(lst,' |');
      write  (lst,ii,'+'); for i:=0 to trunc(opch[ii]*60/opch[10]) do write(lst,'*'); writeln(lst,' ',opch[ii]);
   end;
   writeln(lst,' ++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-');
   writeln(lst,'  0 4   10  17  24  30  37  44  50  57  64  70  77  84  90  97  100    %    ');
   writeln(lst,chr(12),'  ');
end;
{----------------------------------------------------------------}
begin
   regs.ah:=$10;
   regs.al:=$03;
   regs.bl:=0;
   intr($10,regs);
   Inicio;
   for ii:=0 to 15 do
   begin
      Red1[ii]:=0;
      Green1[ii]:=0;
      Blue1[ii]:=0;
   end;
   op[1]:='Educa‡„o de trƒnsito como disciplina curricular obrigat¢ria no 1§ e 2§ graus';
   op[2]:='Fiscaliza‡„o mais rigorosa contra embriaguez ao volante';
   op[3]:='Puni‡„o pesada para os infratores de trƒnsito';
   op[4]:='Recupera‡„o das rodovias';
   op[5]:='Atualiza‡„o urgente do C¢digo Nacional de Trƒnsito';
   op[6]:='Campanhas mais efetivas e permanentes de seguran‡a no trƒnsito';
   op[7]:='Elimina‡„o dos pontos negros rodovi rios e urbanos';
   op[8]:='Normas mais r¡gidas na obten‡„o da CNH';
   op[9]:='Reaparelhamento das pol¡cias de trƒnsito urbano e rodovi rios';
   op[0]:='Uso obrigat¢rio de cinto de seguran‡a nas cidades';
   textcolor(15);
   gotoxy(1,3);
   writeln('                            Pesquisa de Opini„o');
   writeln;
   writeln;
   writeln('  O que vocˆ acha que deve ser prioridade no trƒnsito?');
   writeln;
   textcolor(7);
   for i:=0 to 9 do
   begin
      writeln(i,'- ',op[i]);
      opch[i]:=0;
   end;
   opch[10]:=0;
   doserror:=0;
   findfirst('pvst.dta',anyfile,filer);
   assign(arqu,'pvst.dta');
   if doserror=0 then
   begin
      reset(arqu);
      for i:=0 to 10 do readln(arqu,opch[i]);
      close(arqu);
   end;
   writeln;
   textcolor(15);
   writeln('   Para escolher, aperte o n£mero da escolha, ou ent„o utilize as setas');
   writeln('para cima e para baixo para marcar. Em seguida, aperte ENTER ou a tecla');
   writeln('de espa‡o para confirmar.');
   gotoxy(1,24);
   textcolor(0);
   textbackground(15);
   writeln('                Desenvolvido por Juarez Fioravanti Filho                       ');
   gotoxy(1,1);
   textcolor(15);
   textbackground(3);
   write  ('                 Programa Volvo de Seguran‡a no Trƒnsito                       ');
   textcolor(7);
   textbackground(0);
   write(' ');   
   regs.ax:=$168;
   regs.cx:=$1900;
   intr($10,regs);
   choose:=0;
   finis:=false;
   textcolor(0);
   textbackground(15);
   gotoxy(1,choose+8);
   writeln(choose,'- ',op[choose]);
   for ii:=0 to 255 do
   begin
      repeat
         regs.ah:=$10;
         regs.al:=$10;
         regs.bx:=ii;
         regs.dh:=Red1[ii];
         regs.ch:=Green1[ii];
         regs.cl:=Blue1[ii];
         intr($10,regs);
         inc(red1[ii]);inc(green1[ii]);inc(blue1[ii]);
      until ((Red[ii]=Red1[ii])and(Green[ii]=Green1[ii])and(Blue[ii]=Blue1[ii]));
   end;
   repeat
      ch:=readkey;
      textcolor(7);
      textbackground(0);
      gotoxy(1,choose+8);
      writeln(choose,'- ',op[choose]);
      if ch=#0 then
      begin
         ch:=readkey;
         if ch=#72 then dec(choose);
         if ch=#80 then inc(choose);
         if ch=#117 then finis:=true;
         if ch=#119 then vertotais;
         if ch=#114 then imprime;
      end;
      if (ch<='9') and (ch>='0') then val(ch,choose,choose);
      if (ch=#13) or (ch=#32) then acresentar;
      if choose<0 then choose:=9;
      if choose>9 then choose:=0;
      textcolor(0);
      textbackground(15);
      gotoxy(1,choose+8);
      writeln(choose,'- ',op[choose]);
   until finis;
   regs.ax:=$168;
   regs.cx:=$607;
   intr($10,regs);
   clrscr;
end.