Program ASCII;
{$M 8192,0,0}
Uses DOS,CRT;
var regs        : registers;    abcd        : pointer;
    _dest,_orig : word;         botao,oldbot: word;
    i,tecla     : byte;         x,y,z,t     : byte;
    c,jff       : byte;         efgh        : pointer;
    x1,x2,y1,y2 : byte;         buffer      : char;
    cursor,zzo1 : word;         zzs,zzo,zzs1: word;
    sair        : boolean;      origem      : pointer;
    from,fin    : ^byte;        selchar     : char;
    selx,sely   : byte;         xx,inicio   : ^byte;
    final,zz    : ^byte;        q,w         : byte;
{--------------------------------------------------------------------------}
procedure ativar;
interrupt;
begin
   regs.ah:=2;
   intr($16,regs);
   tecla:=regs.al;
   i:=tecla;
   oldbot:=234;
   if i>=128 then i:=i-128;
   if i>=64  then i:=i-64;
   if i>=32  then i:=i-32;
   if i>=16  then i:=i-16;
   if i>=8   then i:=i-8;
   if i=6 then
   begin
      textattr:=15;
      regs.ah:=3;
      intr($10,regs);
      cursor:=regs.cx;
      regs.ah:=1;
      regs.cx:=$1900;
      intr($10,regs);
      _orig:=$b800;
      _dest:=$9400;
      inline($b9/$80/$3e/$33/$ff/$8b/$16/>_dest/$8e/$c2/$33/$f6/
             $8b/$16/>_orig/$1e/$8e/$da/$d1/$e9/$f3/$a5/$1f);
      inline($50/$06/$b8/$40/$00/$8e/$c0/$26/$a1/$17/$00/$24/$fb/
             $26/$a3/$17/$00/$07/$58); { Elimina Ctrl }
      inline($50/$06/$b8/$40/$00/$8e/$c0/$26/$a1/$17/$00/$24/$fd/
             $26/$a3/$17/$00/$07/$58); { Elimina Shift }
      regs.ah:=2;
      intr($16,regs);
      tecla:=regs.al;
      if tecla>=128 then tecla:=tecla-128;
      if tecla>=64  then tecla:=tecla-64;
      if tecla>=32  then tecla:=tecla-32;
      if tecla>=16  then tecla:=tecla-16;
      if tecla>=8   then tecla:=tecla-8;
      if tecla>=4   then
      begin
         tecla:=tecla-4;
         inline($50/$06/$b8/$40/$00/$8e/$c0/$26/$a1/$17/$00/$24/$fb/
                $26/$a3/$17/$00/$07/$58); { Elimina Ctrl }
      end;
      if tecla>=2   then
      begin
         tecla:=tecla-2;
         inline($50/$06/$b8/$40/$00/$8e/$c0/$26/$a1/$17/$00/$24/$fd/
                $26/$a3/$17/$00/$07/$58); { Elimina Shift }
      end;
      if tecla>=1   then tecla:=tecla-1;
      x:=wherex;
      y:=wherey;
      x1:=10;
      y1:=5;
      x2:=70;
      y2:=21;
      for z:=x1+2 to x2+1 do
      for t:=y2-1 to y2 do
      begin
         gotoxy(z,t);
         inline($b4/$08/$cd/$10/$b9/$01/$00/$b3/$03/$b4/$09/$cd/$10);
      end;
      for z:=x2 to x2+1 do
      for t:=y1+1 to y2 do
      begin
         gotoxy(z,t);
         inline($b4/$08/$cd/$10/$b9/$01/$00/$b3/$03/$b4/$09/$cd/$10);
      end;
      window(x1,y1,x2,y2);
      gotoxy(1,1);             writeln('É');
      gotoxy(x2-x1,y2-y1);     writeln('¼');
      gotoxy(x2-x1,1);         writeln('»');
      gotoxy(1,y2-y1);         writeln('È');
      for z:=2 to y2-(y1+1) do
      begin
         gotoxy(1,z);         writeln('º');
         gotoxy(x2-x1,z);     writeln('º');
      end;
      for t:=2 to x2-(x1+1) do
      begin
         gotoxy(t,1);         writeln('Í');
         gotoxy(t,y2-y1);     writeln('Í');
      end;
      window(x1+1,y1+1,x2-2,y2-2);
      clrscr;
      window(1,1,80,25);
      selx:=11;
      sely:=10;
      botao:=0;
      c:=0;
      repeat
         if botao>0 then oldbot:=botao;
         inline($50/$e4/$60/$a3/>botao/$58);
         if botao=oldbot then botao:=0;
         textattr:=15;
         gotoxy(12,6);
         writeln('       Tabela ASCII - Por Juarez Fioravanti Filho');
         gotoxy(12,7);
         writeln('       Aperte <F10> para sair - Espa‡o para mudar');
         gotoxy(12,8);
         writeln('       Aperte <F9> para enviar caracter escolhido');
         gotoxy(12,9);
         x1:=12;
         y1:=10;
         while keypressed do buffer:=readkey;
         selchar:=chr(((8*(sely-10))+((selx-11) div 7))+72*((c div 72)-1));
         if (72*((c div 72)-1))=-72 then selchar:=chr(((8*(sely-10))+((selx-11) div 7))+72*((288 div 72)-1));
         if ((botao=72)or(botao=80)or(botao=75)or(botao=77)) then
         begin
            textattr:=15+(112*(ord(selchar) mod 2));
            gotoxy(selx+7,sely);
            write(' ');
            textattr:=7;
         end;
         if botao=72 then dec(sely);
         if botao=80 then inc(sely);
         if botao=75 then selx:=selx-7;
         if botao=77 then selx:=selx+7;
         if selx=67 then selx:=11;
         if selx=4  then selx:=60;
         if sely=9  then sely:=18;
         if sely=19 then sely:=10;
         textattr:=15+(112*(ord(selchar) mod 2));
         gotoxy(selx+7,sely);
         write('<');
         textattr:=7;
         if botao=67 then
         begin
            inicio:=ptr($40,$1a);
            final:=ptr($40,$1c);
            inicio^:=$1e;
            final^:=2*length(selchar)+$1e;
            w:=0;
            repeat
               xx:=ptr($40,$1e+w);
               zz:=ptr($40,$1e+w+1);
               zz^:=0;
               q:=ord(selchar);
               xx^:=q;
               w:=w+2;
            until w=2*length(selchar);
            botao:=68;
         end;
         if botao=57 then
         begin
         window(11,10,68,19);
         clrscr;
         window(1,1,80,25);
         repeat
            while keypressed do buffer:=readkey;
            botao:=0;
            gotoxy(x1,y1);
            textattr:=15+(112*(c mod 2));
            write(c:3,'- ');
            gotoxy(x1+5,y1);
            regs.ah:=9;
            regs.cx:=1;
            regs.al:=c;
            regs.bx:=15+(112*(c mod 2));
            intr($10,regs);
            gotoxy(x1+6,y1);
            write(' ');
            gotoxy(selx+7,sely);
            write('<');
            x1:=x1+7;
            inc(c);
            if c=256 then c:=0;
            if x1=68 then
            begin
               x1:=12;
               inc(y1);
            end;
         until (c mod 72) = 0;
      end;
      until botao=68;
      textattr:=7;
      _orig:=$9400;
      _dest:=$b800;
      inline($b9/$80/$3e/$33/$ff/$8b/$16/>_dest/$8e/$c2/$33/$f6/
             $8b/$16/>_orig/$1e/$8e/$da/$d1/$e9/$f3/$a5/$1f);
      regs.ah:=1;
      regs.cx:=cursor;
      intr($10,regs);
      gotoxy(x,y);
   end;
   if i>=4   then i:=i-4;
   if i>=2   then i:=i-2;
   if i>=1   then i:=i-1;
end;
{--------------------------------------------------------------------------}
begin
   getintvec($1b,abcd);
   setintvec($1b,SaveInt1B);
   getintvec($28,abcd);
   origem:=@ativar;
   sair:=false;
   jff:=0;
   repeat
      zzs:=seg(abcd^)+jff;
      zzo:=ofs(abcd^)+jff;
      zzs1:=seg(origem^)+jff;
      zzo1:=ofs(origem^)+jff;
      inc(jff);
      if jff>=10 then sair:=true;
      from:=ptr(zzs,zzo);
      fin :=ptr(zzs1,zzo1);
      if ((from^)<>(fin^)) then jff:=50;
   until sair;
   if sair and (jff = 10) then
   begin
      writeln;
      writeln('Tabela ASCII residente j  estava instalada.');
      writeln;
      writeln('Para ativar, aperte <Ctrl> + <Shift> esquerdo.');
      writeln;
      writeln('Para remover da mem¢ria, aperte <Ctrl> + <Alt> + <Del> ou');
      writeln('aperte a tecla RESET do seu micro.');
      writeln;
      exit;
   end;
   writeln;
   writeln('Tabela ASCII residente instalada.');
   writeln;
   writeln('Aperte <Ctrl> + <Shift> esquerdo para ativar.');
   writeln;
   getintvec($28,abcd);
   setintvec($28,@ativar);
   keep(768);
end.




