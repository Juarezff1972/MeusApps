uses crt,dos;
type chose = (Arquivo,Entrada,Calculo,Ajuda,Sair);
var x,y                                                       : integer;
    Fr,b,l,p1,k,z,i,fl,rpm,fdt,work,rps,f,s,f2,e,v,r1,x1,x2,n : real;
    p                                                         : byte;
    opcao                                                     : chose;
    _orig,_dest                                               : word;
    ch                                                        : char;
    arquivar                                                  : text;
    regs                                                      : registers;
    linhas                                                    : array[1..255] of string[80];
{--------------------------------------------------------------------------}
Procedure MovePagina;
begin
   inline($b9/$80/$3e/               { mov cx,3E80  }
          $33/$ff/                   { xor di,di    }
          $8b/$16/>_dest/            { mov dx,_DEST }
          $8e/$c2/                   { mov es,dx    }
          $33/$f6/                   { xor si,si    }
          $8b/$16/>_orig/            { mov dx,_ORIG }
          $1e/                       { push ds      }
          $8e/$da/                   { mov ds,dx    }
          $d1/$e9/                   { shr cx,1     }
          $f3/$a5/                   { rep movsw    }
          $1f);                      { pop ds       }

          { Pagina 0 : $b800 }
          { Pagina 1 : $8800 }
          { Pagina 2 : $8c00 }
          { Pagina 3 : $9000 }
          { Pagina 4 : $9400 }
end;
{--------------------------------------------------------------------------}
Procedure Arquivop;
var x11,y1,x2,y2 : integer;
    escolha     : byte;
    nome        : string[8];
begin
   x11:=2;
   y1:=2;
   x2:=16;
   y2:=6;
   escolha:=1;
   window(x11,y1,x2-2,y2-2);
   clrscr;
   window(x11,y1,x2,y2);
   textbackground(11);
   textcolor(15);
   gotoxy(1,1);             writeln('Ë');
   gotoxy(x2-x11,y2-y1);     writeln('¼');
   gotoxy(x2-x11,1);         writeln('Ë');
   gotoxy(1,y2-y1);         writeln('È');
   for y:=2 to y2-(y1+1) do
   begin
      gotoxy(1,y);         writeln('º');
      gotoxy(x2-x11,y);     writeln('º');
   end;
   for x:=2 to x2-(x11+1) do
   begin
      gotoxy(x,1);         writeln('Í');
      gotoxy(x,y2-y1);     writeln('Í');
   end;
   window(1,1,79,25);
   gotoxy(3,4);
   write('Ler dados   ');
   textcolor(15);
   textbackground(0);
   gotoxy(3,3);
   write('Gravar dados');
   repeat
      ch:=readkey;
      if ch=#77 then
      begin
         regs.ch:=77;
         regs.cl:=0;
         regs.ah:=5;
         intr($16,regs);
         regs.ch:=$1c;
         regs.cl:=$d;
         regs.ah:=5;
         intr($16,regs);
         ch:=#27;
      end;
      if ch=#75 then
      begin
         regs.ch:=75;
         regs.cl:=0;
         regs.ah:=5;
         intr($16,regs);
         ch:=#27;
      end;
      if (ch=#72) or (ch=#80) then escolha:=-escolha;
      if escolha=1 then
      begin
         textcolor(15);
         textbackground(0);
         gotoxy(3,3);
         write('Gravar dados');
         textbackground(11);
         textcolor(15);
         gotoxy(3,4);
         write('Ler dados   ');
      end;
      if escolha=255 then
      begin
         textcolor(15);
         textbackground(0);
         gotoxy(3,4);
         write('Ler dados   ');
         textbackground(11);
         textcolor(15);
         gotoxy(3,3);
         write('Gravar dados');
      end;
   until (ch=#13)or(ch=#27);
   _orig:=$8800;
   _dest:=$b800;
   MovePagina;
   if ch=#27 then
   begin
      textcolor(7);
      textbackground(0);
      exit;
   end;
   x11:=10;
   y1:=10;
   x2:=50;
   y2:=13;
   window(x11,y1,x2-2,y2-2);
   clrscr;
   window(x11,y1,x2,y2);
   textbackground(11);
   textcolor(15);
   gotoxy(1,1);             writeln('É');
   gotoxy(x2-x11,y2-y1);     writeln('¼');
   gotoxy(x2-x11,1);         writeln('»');
   gotoxy(1,y2-y1);         writeln('È');
   for y:=2 to y2-(y1+1) do
   begin
      gotoxy(1,y);         writeln('º');
      gotoxy(x2-x11,y);     writeln('º');
   end;
   for x:=2 to x2-(x11+1) do
   begin
      gotoxy(x,1);         writeln('Í');
      gotoxy(x,y2-y1);     writeln('Í');
   end;
   window(1,1,79,25);
   regs.ax:=$168;
   regs.cx:=$607;
   intr($10,regs);
   if escolha=1 then
   begin
      gotoxy(12,11);
      write('Nome a gravar?');
      readln(nome);
      assign(arquivar,nome+'.num');
      rewrite(arquivar);
      writeln(arquivar,I);
      writeln(arquivar,V);
      writeln(arquivar,f);
      writeln(arquivar,p);
      writeln(arquivar,l);
      writeln(arquivar,x1);
      writeln(arquivar,RPM);
      writeln(arquivar,fl);
      writeln(arquivar,z);
      writeln(arquivar,N);
      writeln(arquivar,s);
      writeln(arquivar,f2);
      writeln(arquivar,Fr);
      writeln(arquivar,p1);
      writeln(arquivar,K);
      writeln(arquivar,r1);
      writeln(arquivar,E);
      writeln(arquivar,fdt);
      writeln(arquivar,RPS);
      writeln(arquivar,work);
      close(arquivar);
   end;
   if escolha=255 then
   begin
      gotoxy(12,11);
      write('Nome a ler?');
      readln(nome);
      assign(arquivar,nome+'.num');
      reset(arquivar);
      readln(arquivar,I);
      readln(arquivar,V);
      readln(arquivar,f);
      readln(arquivar,p);
      readln(arquivar,l);
      readln(arquivar,x1);
      readln(arquivar,RPM);
      readln(arquivar,fl);
      readln(arquivar,z);
      readln(arquivar,N);
      readln(arquivar,s);
      readln(arquivar,f2);
      readln(arquivar,Fr);
      readln(arquivar,p1);
      readln(arquivar,K);
      readln(arquivar,r1);
      readln(arquivar,E);
      readln(arquivar,fdt);
      readln(arquivar,RPS);
      readln(arquivar,work);
      close(arquivar);
      textcolor(7);
      textbackground(0);
      gotoxy(4,5);
      write('For‡a de tra‡„o:        ',FDT:1:5,' HP                 ');
      gotoxy(4,6);
      write('Potencia:               ',Work:1:5,' watt              ');
      gotoxy(4,7);
      write('Rota‡”es por segundo:   ',rps:1:5,' RPS                ');
      gotoxy(4,10);
      write('Escorregamento de fase: ',(s*100):1:5,' %              ');
      gotoxy(4,11);
      write('Frequencia do rotor:    ',f2:1:5,' Hz                  ');
      gotoxy(4,12);
      write('For‡a  c.e.m.:          ',E:1:5,' V                    ');
      gotoxy(4,15);
      write('Resistencia do estator: ',r1:1:5,' ê                   ');
      gotoxy(4,17);
      write('For‡a sobre o condutor: ',Fr:1:5,' N                   ');
      gotoxy(4,18);
      write('Velocidade sincronismo: ',N:1:5,' RPM                  ');
      gotoxy(4,4);
      write('Rota‡”es por minuto:    ',RPM:1:5,' RPM                ');
      gotoxy(4,8);
      write('Frequencia do estator:  ',f:1:5,' Hz                   ');
      gotoxy(4,9);
      write('Numero de polos:        ',p,'                          ');
      gotoxy(4,13);
      write('Amperagem:              ',I:1:5,' A                    ');
      gotoxy(4,14);
      write('Voltagem no terminal:   ',V:1:5,' V                    ');
      gotoxy(4,16);
      write('Raio da espira:         ',x1:1:5,' cm                  ');
      gotoxy(4,19);
      write('Comprimento do condutor:',l:1:5,' cm                   ');
      _orig:=$b800;
      _dest:=$8800;
      MovePagina;
   end;
   regs.ax:=$168;
   regs.cx:=$1900;
   intr($10,regs);
   textcolor(7);
   textbackground(0);
end;
{--------------------------------------------------------------------------}
Procedure Entradap;
var x11,y1,x2,y2 : integer;
    escolha     : array[1..8] of string[25];
    select,z    : byte;
begin
   x11:=13;
   y1:=2;
   x2:=40;
   y2:=11;
   window(x11,y1,x2-2,y2-2);
   clrscr;
   window(x11,y1,x2,y2);
   textbackground(11);
   textcolor(15);
   gotoxy(1,1);             writeln('Ë');
   gotoxy(x2-x11,y2-y1);     writeln('¼');
   gotoxy(x2-x11,1);         writeln('Ë');
   gotoxy(1,y2-y1);         writeln('È');
   for y:=2 to y2-(y1+1) do
   begin
      gotoxy(1,y);         writeln('º');
      gotoxy(x2-x11,y);     writeln('º');
   end;
   for x:=2 to x2-(x11+1) do
   begin
      gotoxy(x,1);         writeln('Í');
      gotoxy(x,y2-y1);     writeln('Í');
   end;
   window(1,1,79,25);
   escolha[1]:=' Amperagem               ';
   escolha[2]:=' Voltagem no terminal    ';
   escolha[3]:=' Frequencia do estator   ';
   escolha[4]:=' Numero de polos         ';
   escolha[5]:=' Comprimento do condutor ';
   escolha[6]:=' Raio da espira          ';
   escolha[7]:=' Rota‡”es por minuto     ';
   for select:=1 to 7 do
   begin
      gotoxy(14,select+2); write(escolha[select]);
   end;
   textcolor(15);
   textbackground(0);
   gotoxy(14,3); write(escolha[1]);
   textcolor(15);
   textbackground(11);
   select:=1;
   repeat
      ch:=readkey;
      if ch=#77 then
      begin
         regs.ch:=77;
         regs.cl:=0;
         regs.ah:=5;
         intr($16,regs);
         ch:=#27;
      end;
      if ch=#75 then
      begin
         regs.ch:=75;
         regs.cl:=0;
         regs.ah:=5;
         intr($16,regs);
         regs.ch:=$1c;
         regs.cl:=$d;
         regs.ah:=5;
         intr($16,regs);
         ch:=#27;
      end;
      for z:=1 to 7 do
      begin
         gotoxy(14,z+2); write(escolha[z]);
      end;
      if ch=#72 then dec(select);
      if ch=#80 then inc(select);
      if select=0 then select:=7;
      if select=8 then select:=1;
      textcolor(15);
      textbackground(0);
      gotoxy(14,select+2); write(escolha[select]);
      textcolor(15);
      textbackground(11);
   until (ch=#13)or(ch=#27);
   _orig:=$8800;
   _dest:=$b800;
   MovePagina;
   if ch=#27 then
   begin
      textcolor(7);
      textbackground(0);
      exit;
   end;
   x11:=10;
   y1:=10;
   x2:=75;
   y2:=13;
   window(x11,y1,x2-2,y2-2);
   clrscr;
   window(x11,y1,x2,y2);
   textbackground(11);
   textcolor(15);
   gotoxy(1,1);             writeln('É');
   gotoxy(x2-x11,y2-y1);     writeln('¼');
   gotoxy(x2-x11,1);         writeln('»');
   gotoxy(1,y2-y1);         writeln('È');
   for y:=2 to y2-(y1+1) do
   begin
      gotoxy(1,y);         writeln('º');
      gotoxy(x2-x11,y);     writeln('º');
   end;
   for x:=2 to x2-(x11+1) do
   begin
      gotoxy(x,1);         writeln('Í');
      gotoxy(x,y2-y1);     writeln('Í');
   end;
   window(1,1,79,25);
   regs.ax:=$168;
   regs.cx:=$607;
   intr($10,regs);
   if select=1 then
   begin
      gotoxy(12,11);
      write('Qual a amperagem? ');
      readln(I);
      _orig:=$8800;
      _dest:=$b800;
      MovePagina;
      textcolor(7);
      textbackground(0);
      gotoxy(4,13);
      write('Amperagem:              ',I:1:5,' A     *              ');
   end;
   if select=2 then
   begin
      gotoxy(12,11);
      write('Qual a voltagem no terminal? ');
      readln(V);
      _orig:=$8800;
      _dest:=$b800;
      MovePagina;
      textcolor(7);
      textbackground(0);
      gotoxy(4,14);
      write('Voltagem no terminal:   ',V:1:5,' V     *              ');
   end;
   if select=3 then
   begin
      gotoxy(12,11);
      write('Qual a frequencia do estator? ');
      readln(f);
      _orig:=$8800;
      _dest:=$b800;
      MovePagina;
      textcolor(7);
      textbackground(0);
      gotoxy(4,8);
      write('Frequencia do estator:  ',f:1:5,' Hz    *              ');
   end;
   if select=4 then
   begin
      repeat
         gotoxy(12,11);
         write('Qual o numero de polos? ');
         readln(p);
      until p mod 2 = 0;
      _orig:=$8800;
      _dest:=$b800;
      MovePagina;
      textcolor(7);
      textbackground(0);
      gotoxy(4,9);
      write('Numero de polos:        ',p,'           *              ');
   end;
   if select=5 then
   begin
      gotoxy(12,11);
      write('Qual o comprimento do condutor? ');
      readln(l);
      _orig:=$8800;
      _dest:=$b800;
      MovePagina;
      textcolor(7);
      textbackground(0);
      gotoxy(4,19);
      write('Comprimento do condutor:',l:1:5,' cm    *              ');
   end;
   if select=6 then
   begin
      gotoxy(12,11);
      write('Qual o raio da espira? ');
      readln(x1);
      _orig:=$8800;
      _dest:=$b800;
      MovePagina;
      textcolor(7);
      textbackground(0);
      gotoxy(4,16);
      write('Raio da espira:         ',x1:1:5,' cm   *              ');
   end;
   if select=7 then
   begin
      gotoxy(12,11);
      write('Quantas rota‡”es por minuto? ');
      readln(RPM);
      _orig:=$8800;
      _dest:=$b800;
      MovePagina;
      textcolor(7);
      textbackground(0);
      gotoxy(4,4);
      write('Rota‡”es por minuto:    ',RPM:1:5,' RPM *              ');
   end;
   regs.ax:=$168;
   regs.cx:=$1900;
   intr($10,regs);
   _orig:=$b800;
   _dest:=$8800;
   MovePagina;
end;
{--------------------------------------------------------------------------}
Procedure Calculop;
begin
   N:=(f*120)/P;
   s:=(N-RPM)/N;
   f2:=s*f;
   Fr:=l*((I*I)/(x1*x1));
   r1:=(0.1*V)/I;
   E:=V-I*r1;
   fdt:=E*I*(12.73/9495);
   RPS:=RPM/60;
   work:=E*I;
   gotoxy(4,5);
   write('For‡a de tra‡„o:        ',FDT:1:5,' HP                 ');
   gotoxy(4,6);
   write('Potencia:               ',Work:1:5,' watt              ');
   gotoxy(4,7);
   write('Rota‡”es por segundo:   ',rps:1:5,' RPS                ');
   gotoxy(4,10);
   write('Escorregamento de fase: ',(s*100):1:5,' %              ');
   gotoxy(4,11);
   write('Frequencia do rotor:    ',f2:1:5,' Hz                  ');
   gotoxy(4,12);
   write('For‡a  c.e.m.:          ',E:1:5,' V                    ');
   gotoxy(4,15);
   write('Resistencia do estator: ',r1:1:5,' ê                   ');
   gotoxy(4,17);
   write('For‡a sobre o condutor: ',Fr:1:5,' N                   ');
   gotoxy(4,18);
   write('Velocidade sincronismo: ',N:1:5,' RPM                  ');
   gotoxy(4,4);
   write('Rota‡”es por minuto:    ',RPM:1:5,' RPM                ');
   gotoxy(4,8);
   write('Frequencia do estator:  ',f:1:5,' Hz                   ');
   gotoxy(4,9);
   write('Numero de polos:        ',p,'                          ');
   gotoxy(4,13);
   write('Amperagem:              ',I:1:5,' A                    ');
   gotoxy(4,14);
   write('Voltagem no terminal:   ',V:1:5,' V                    ');
   gotoxy(4,16);
   write('Raio da espira:         ',x1:1:5,' cm                  ');
   gotoxy(4,19);
   write('Comprimento do condutor:',l:1:5,' cm                   ');
   _orig:=$b800;
   _dest:=$8800;
   MovePagina;
end;
{--------------------------------------------------------------------------}
Procedure Ajudap;
var x1,x2,y1,y2,count,iii,max : byte;
begin
   x1:=1;
   y1:=5;
   x2:=79;
   y2:=20;
   window(x1,y1,x2-2,y2-2);
   clrscr;
   window(x1,y1,x2,y2);
   textbackground(11);
   textcolor(15);
   gotoxy(1,1);             writeln('Ì');
   gotoxy(x2-x1,y2-y1);     writeln('¹');
   gotoxy(x2-x1,1);         writeln('¹');
   gotoxy(1,y2-y1);         writeln('Ì');
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
   window(1,1,79,25);
   textcolor(7);
   textbackground(0);
   count:=1;
   assign(arquivar,'Motor.hlp');
   reset(arquivar);
   repeat
      readln(arquivar,linhas[count]);
      inc(count);
   until EOF(arquivar);
   close(arquivar);
   max:=count-1;
   for count:=1 to 13 do
   begin
      gotoxy(2,5+count);
      write(linhas[count]);
   end;
   count:=1;
   textcolor(7);
   textbackground(0);
   repeat
     ch:=readkey;
     if ch=#72 then
     begin
        dec(count);
        if count>0 then
        begin
           regs.ah:=7;
           regs.al:=1;
           regs.ch:=5;
           regs.cl:=1;
           regs.dl:=76;
           regs.dh:=17;
           regs.bh:=7;
           intr($10,regs);
        end;
        if count=0 then count:=1;
        gotoxy(2,6);
        write(linhas[count]);
     end;
     if ch=#80 then
     begin
        inc(count);
        if count<(max-12) then
        begin
           regs.ah:=6;
           regs.al:=1;
           regs.ch:=5;
           regs.cl:=1;
           regs.dl:=76;
           regs.dh:=17;
           regs.bh:=7;
           intr($10,regs);
        end;
        if count=(max-12) then dec(count);
        gotoxy(2,18);
        write(linhas[count+13]);
     end;
   until ch=#13;
end;
{--------------------------------------------------------------------------}
begin
   clrscr;
   regs.ax:=$168;
   regs.cx:=$1900;
   intr($10,regs);
   textbackground(11);
   textcolor(15);
   gotoxy(1,1);
   writeln('  Arquivo    Entrada    Calculo    Ajuda    Sair                              ');
   window(1,2,79,24);
   gotoxy(1,1);             writeln('É');
   gotoxy(79-1,24-2);     writeln('¼');
   gotoxy(79-1,1);         writeln('»');
   gotoxy(1,24-2);         writeln('È');
   for y:=2 to 24-(2+1) do
   begin
      gotoxy(1,y);         writeln('º');
      gotoxy(79-1,y);     writeln('º');
   end;
   for x:=2 to 79-(1+1) do
   begin
      gotoxy(x,1);         writeln('Í');
      gotoxy(x,24-2);     writeln('Í');
   end;
   window(1,1,79,25);
   textcolor(7);
   textbackground(0);
   I:=90;
   V:=120;
   f:=60;
   p:=4;
   l:=5000;
   x1:=30;
   RPM:=1730;
   N:=(f*120)/P;
   s:=(N-RPM)/N;
   f2:=s*f;
   Fr:=l*((I*I)/(x1*x1));
   r1:=(0.1*V)/I;
   E:=V-I*r1;
   fdt:=E*I*(12.73/9495);
   RPS:=RPM/60;
   work:=E*I;
   gotoxy(1,24);
   writeln('   Produzido por Juarez Fioravanti Filho');
   gotoxy(4,4);
   write('Rota‡”es por minuto:    ',RPM:1:5,' RPM');
   gotoxy(4,5);
   write('For‡a de tra‡„o:        ',FDT:1:5,' HP');
   gotoxy(4,6);
   write('Potencia:               ',Work:1:5,' watt');
   gotoxy(4,7);
   write('Rota‡”es por segundo:   ',rps:1:5,' RPS');
   gotoxy(4,8);
   write('Frequencia do estator:  ',f:1:5,' Hz');
   gotoxy(4,9);
   write('Numero de polos:        ',p);
   gotoxy(4,10);
   write('Escorregamento de fase: ',(s*100):1:5,' %');
   gotoxy(4,11);
   write('Frequencia do rotor:    ',f2:1:5,' Hz');
   gotoxy(4,12);
   write('For‡a  c.e.m.:          ',E:1:5,' V');
   gotoxy(4,13);
   write('Amperagem:              ',I:1:5,' A');
   gotoxy(4,14);
   write('Voltagem no terminal:   ',V:1:5,' V');
   gotoxy(4,15);
   write('Resistencia do estator: ',r1:1:5,' ê');
   gotoxy(4,16);
   write('Raio da espira:         ',x1:1:5,' cm');
   gotoxy(4,17);
   write('For‡a sobre o condutor: ',Fr:1:5,' N');
   gotoxy(4,18);
   write('Velocidade sincronismo: ',N:1:5,' RPM');
   gotoxy(4,19);
   write('Comprimento do condutor:',l:1:5,' cm');
   _orig:=$b800;
   _dest:=$8800;
   MovePagina;
   Opcao:=Arquivo;
   repeat
      if Opcao=Arquivo then
      begin
         gotoxy(2,1);
         write(' Arquivo ');
      end;
      if Opcao=Entrada then
      begin
         gotoxy(13,1);
         write(' Entrada ');
      end;
      if Opcao=Calculo then
      begin
         gotoxy(24,1);
         write(' Calculo ');
      end;
      if Opcao=Ajuda then
      begin
         gotoxy(35,1);
         write(' Ajuda ');
      end;
      if Opcao=Sair then
      begin
         gotoxy(44,1);
         write(' Sair ');
      end;
      ch:=readkey;
      if ch=#77 then inc(opcao);
      if ch=#75 then dec(opcao);
      if opcao>sair then opcao:=Arquivo;
      if opcao<Arquivo then opcao:=Sair;
      textbackground(11);
      textcolor(15);
      gotoxy(1,1);
      writeln('  Arquivo    Entrada    Calculo    Ajuda    Sair                              ');
      textcolor(7);
      textbackground(0);
      if (ch=#13) and (opcao=Arquivo) then
      begin
         Arquivop;
         _orig:=$8800;
         _dest:=$b800;
         MovePagina;
      end;
      if (ch=#13) and (opcao=Entrada) then
      begin
         Entradap;
         _orig:=$8800;
         _dest:=$b800;
         MovePagina;
      end;
      if (ch=#13) and (opcao=Ajuda) then
      begin
         Ajudap;
         _orig:=$8800;
         _dest:=$b800;
         MovePagina;
      end;
      if (ch=#13) and (opcao=Calculo) then
      begin
         Calculop;
         _orig:=$8800;
         _dest:=$b800;
         MovePagina;
      end;
   until (ch=#13)and(Opcao=Sair);
   clrscr;
end.

