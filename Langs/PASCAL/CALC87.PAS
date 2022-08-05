{$M 8192,0,0}
uses DOS,CRT;
var Base,w,q,x,y,z,t,OldBot,X2,Y1,Y2,Tecla,JFF,Botao,X1,ZzS,ZzO,ZzS1,ZzO1 : byte;
    Buffer,Operacao                                                       : char;
    Regs                                                                  : registers;
    AbCd,Origem                                                           : pointer;
    Cursor,_dest,_orig                                                    : word;
    Sair,Neg,FlagNegative                                                 : boolean;
    From,Fin,Comeco,Final,XX,ZZ                                           : ^byte;
    AUX,i,XXa,l                                                           : integer;
    NumeroVisor,NumeroMemoria,AUX2,IntgPart,OldNumber,NewNumber           : string;
    XA,n,NumMemCalc,NumMemVis,AUX3,OldBase,NewBase                        : extended;
{-------------------------------------------------------------------------}
Procedure Inicio;
interrupt;
begin
   Regs.ah:=2;
   intr($16,Regs);
   Tecla:=Regs.al;
   i:=Tecla;
   OldBot:=234;
   Botao:=0;
   NumeroVisor:='';
   NumeroMemoria:='';
   NumMemCalc:=0;
   FlagNegative:=false;
   if i>=128 then i:=i-128; { Insert ligado }
   if i>=64 then  i:=i-64;  { Caps Lock     }
   if i>=32 then  i:=i-32;  { Num Lock      }
   if i>=16 then  i:=i-16;  { Scroll Lock   }
   if i>=8 then   i:=i-8;   { Alt           }
   if i=6 then
   begin
      textattr:=15;
      Regs.ah:=3;
      intr($10,Regs);
      Cursor:=Regs.cx;
      Regs.ah:=1;
      Regs.cx:=$1900;
      intr($10,Regs);
      _orig:=$b800;
      _dest:=$9400;
      inline($b9/$80/$3e/$33/$ff/$8b/$16/>_dest/$8e/$c2/$33/$f6/
             $8b/$16/>_orig/$1e/$8e/$da/$d1/$e9/$f3/$a5/$1f);
      inline($50/$06/$b8/$40/$00/$8e/$c0/$26/$a1/$17/$00/$24/$fb/
             $26/$a3/$17/$00/$07/$58); { Elimina Ctrl }
      inline($50/$06/$b8/$40/$00/$8e/$c0/$26/$a1/$17/$00/$24/$fd/
             $26/$a3/$17/$00/$07/$58); { Elimina Shift }
      Regs.ah:=2;
      intr($16,Regs);
      Tecla:=Regs.al;
      if Tecla>=128 then Tecla:=Tecla-128;
      if Tecla>=64  then Tecla:=Tecla-64;
      if Tecla>=32  then Tecla:=Tecla-32;
      if Tecla>=16  then Tecla:=Tecla-16;
      if Tecla>=8   then Tecla:=Tecla-8;
      if Tecla>=4   then
      begin
         Tecla:=Tecla-4;
         inline($50/$06/$b8/$40/$00/$8e/$c0/$26/$a1/$17/$00/$24/$fb/
                $26/$a3/$17/$00/$07/$58); { Elimina Ctrl }
      end;
      if Tecla>=2   then
      begin
         Tecla:=Tecla-2;
         inline($50/$06/$b8/$40/$00/$8e/$c0/$26/$a1/$17/$00/$24/$fd/
                $26/$a3/$17/$00/$07/$58); { Elimina Shift }
      end;
      if Tecla>=1   then Tecla:=Tecla-1;
      x:=wherex;
      y:=wherey;
      X1:=10;
      Y1:=5;
      X2:=70;
      Y2:=21;
      for z:=X1+2 to X2+1 do
      for t:=Y2-1 to Y2 do
      begin
         gotoxy(z,t);
         inline($b4/$08/$cd/$10/$b9/$01/$00/$b3/$03/$b4/$09/$cd/$10);
      end;
      for z:=X2 to X2+1 do
      for t:=Y1+1 to Y2 do
      begin
         gotoxy(z,t);
         inline($b4/$08/$cd/$10/$b9/$01/$00/$b3/$03/$b4/$09/$cd/$10);
      end;
      window(X1,Y1,X2,Y2);
      gotoxy(1,1);             writeln('É');
      gotoxy(X2-X1,Y2-Y1);     writeln('¼');
      gotoxy(X2-X1,1);         writeln('»');
      gotoxy(1,Y2-Y1);         writeln('È');
      for z:=2 to Y2-(Y1+1) do
      begin
         gotoxy(1,z);         writeln('º');
         gotoxy(X2-X1,z);     writeln('º');
      end;
      for t:=2 to X2-(X1+1) do
      begin
         gotoxy(t,1);         writeln('Í');
         gotoxy(t,Y2-Y1);     writeln('Í');
      end;
      window(X1+1,Y1+1,X2-2,Y2-2);
      clrscr;
      window(1,1,80,25);
      textattr:=15;
      window(12,6,68,9);
      gotoxy(1,1);           writeln('Ú');
      gotoxy(68-12,9-6);     writeln('Ù');
      gotoxy(68-12,1);       writeln('¿');
      gotoxy(1,9-6);         writeln('À');
      for z:=2 to 9-(6+1) do
      begin
         gotoxy(1,z);         writeln('³');
         gotoxy(68-12,z);     writeln('³');
      end;
      for t:=2 to 68-(12+1) do
      begin
         gotoxy(t,1);         writeln('Ä');
         gotoxy(t,9-6);       writeln('Ä');
      end;
      window(12+1,6+1,68-2,9-2);
      clrscr;
      window(1,1,80,25);
      Operacao:=#0;
      Base:=10;
      repeat
         if Botao>0 then OldBot:=Botao;
         inline($50/$e4/$60/$a3/>Botao/$58);
         if Botao=OldBot then Botao:=0;
         if Botao<>0 then
         begin
            window(12+1,6+1,68-2,9-2);
            clrscr;
            window(1,1,80,25);
         end;
         if NumeroVisor='' then
         begin
            gotoxy(14,7);
            write(NumMemCalc:1:18,'   ');
         end;
         gotoxy(62,7);
         write('B=',Base);
         gotoxy(60,7);
         write(Operacao);
         if NumeroVisor<>'' then
         begin
            gotoxy(14,7);
            write(NumeroVisor,'   ');
         end;
         FlagNegative:=false;
         if copy(NumeroVisor,1,1)='-' then
         begin
            FlagNegative:=true;
            delete(NumeroVisor,1,1);
         end;
         if Base=10 then
         begin
            NumeroMemoria:=NumeroVisor;
            val(NumeroMemoria,NumMemVis,AUX);
         end;
         if (Base<>10)and(NumMemVis<>0) then
         begin
            IntgPart:='';
            if pos('.',NumeroVisor)=0 then IntgPart:=NumeroVisor;
            if pos('.',NumeroVisor)=length(NumeroVisor) then
            begin
               IntgPart:=NumeroVisor;
               delete(IntgPart,length(IntgPart),1);
            end;
            if (pos('.',NumeroVisor)<length(NumeroVisor))
            and(pos('.',NumeroVisor)>0) then IntgPart:=copy(NumeroVisor,1,pos('.',NumeroVisor)-1);
            OldNumber:=IntgPart;
            OldBase:=Base;
            NewBase:=10;
            begin
               n:=0;
               XA:=0;
               XXa:=0;
               l:=0;
               NewNumber:='';
               Neg:=false;
               if copy(OldNumber,1,1)='-' then
               begin
                  delete(OldNumber,1,1);
                  Neg:=true;
               end;
               for i:=1 to length(OldNumber) do
               begin
                  XA:=ord(OldNumber[i])-48;
                  if XA>9 then XA:=XA-7;
                  n:=n*OldBase+XA;
               end;
               n:=n+0.5;
               l:=trunc(ln(n)/ln(NewBase));
               for i:=l downto 0 do
               begin
                  XXa:=trunc(n/exp(i*ln(NewBase)));
                  n:=n-XXa*exp(i*ln(NewBase));
                  if XXa>9 then NewNumber:=NewNumber+chr(48+XXa+7);
                  if XXa<=9 then NewNumber:=NewNumber+chr(48+XXa);
               end;
               if Neg then NewNumber:='-'+NewNumber;
               if OldNumber='' then NewNumber:='';
               if OldNumber='0' then NewNumber:='0';
            end;
            AUX2:=NewNumber;
            IntgPart:=AUX2;
            NumeroMemoria:=IntgPart;
            val(NumeroMemoria,NumMemVis,AUX);
            IntgPart:='';
            if pos('.',NumeroMemoria)=0 then IntgPart:=NumeroMemoria;
            if pos('.',NumeroMemoria)=length(NumeroMemoria) then
            begin
               IntgPart:=NumeroMemoria;
               delete(IntgPart,length(IntgPart),1);
            end;
            if (pos('.',NumeroMemoria)<length(NumeroMemoria))
            and(pos('.',NumeroVisor)>0) then IntgPart:=copy(NumeroMemoria,1,pos('.',NumeroMemoria)-1);
            OldNumber:=IntgPart;
            OldBase:=10;
            NewBase:=Base;
            begin
               n:=0;
               XA:=0;
               XXa:=0;
               l:=0;
               NewNumber:='';
               Neg:=false;
               if copy(OldNumber,1,1)='-' then
               begin
                  delete(OldNumber,1,1);
                  Neg:=true;
               end;
               for i:=1 to length(OldNumber) do
               begin
                  XA:=ord(OldNumber[i])-48;
                  if XA>9 then XA:=XA-7;
                  n:=n*OldBase+XA;
               end;
               n:=n+0.5;
               l:=trunc(ln(n)/ln(NewBase));
               for i:=l downto 0 do
               begin
                  XXa:=trunc(n/exp(i*ln(NewBase)));
                  n:=n-XXa*exp(i*ln(NewBase));
                  if XXa>9 then NewNumber:=NewNumber+chr(48+XXa+7);
                  if XXa<=9 then NewNumber:=NewNumber+chr(48+XXa);
               end;
               if Neg then NewNumber:='-'+NewNumber;
               if OldNumber='' then NewNumber:='';
               if OldNumber='0' then NewNumber:='0';
            end;
            AUX2:=NewNumber;
            IntgPart:=AUX2;
            NumeroVisor:=IntgPart;
         end;
         if FlagNegative then
         begin
            NumeroVisor:='-'+NumeroVisor;
            NumeroMemoria:='-'+NumeroMemoria;
            NumMemVis:=-NumMemVis;
         end;
         textattr:=15;
         gotoxy(13,10);
         writeln(' lOg Intg ''(abs) Ln 1/X Q(xý) R(ûx) P(xü) W(ñ) M(cos)');
         gotoxy(13,12);
         writeln(' Sin Tan K(exp) U(frac) Hex J(dec) [(bin) V(!) \(rad)');
         gotoxy(13,14);
         writeln(' ](oct) ;(%) clY `(clr)  deG    7   8   9   /  *  -  +');
         gotoxy(13,16);
         writeln('   F10 -> Sair    A  B  C  D    4   5   6   .  ,  =');
         gotoxy(13,18);
         writeln('   F9  -> Enviar valor  E  F    1   2   3  0   bkspc');
         X1:=12;
         Y1:=10;
         while keypressed do Buffer:=readkey;
         textattr:=7;
         if (Operacao='*') and (abs(NumMemVis)>=1.1e4900/NumMemCalc) then
         begin NumMemVis:=0; NumeroVisor:=''; NumMemCalc:=0; Base:=10; end;
         if (Operacao='+') and (abs(NumMemVis)>=1.1e4900-NumMemCalc) then
         begin NumMemVis:=0; NumeroVisor:=''; NumMemCalc:=0; Base:=10; end;
         if (Botao=37) and (abs(NumMemVis)>=ln(1.1e4900)) then
         begin NumMemVis:=0; NumeroVisor:=''; NumMemCalc:=0; Base:=10; end;
         if (Botao=16) and (abs(NumMemVis)>=sqrt(1.1e4900)) then
         begin NumMemVis:=0; NumeroVisor:=''; NumMemCalc:=0; Base:=10; end;
         if (Operacao=';') and (abs(NumMemVis)>=(1.1e4900)*100/NumMemCalc) then
         begin NumMemVis:=0; NumeroVisor:=''; NumMemCalc:=0; Base:=10; end;
         if (Operacao='P') and (abs(NumMemVis)>=exp((1/NumMemCalc)*ln(1.1e4900))) then
         begin NumMemVis:=0; NumeroVisor:=''; NumMemCalc:=0; Base:=10; end;
         if abs(NumMemVis)=1.1e4900 then
         begin NumMemVis:=0; NumeroVisor:=''; NumMemCalc:=0; Base:=10; end;
         case Botao of
            24 : begin NumMemVis:=ln(NumMemVis)/ln(10);str(NumMemVis:1:18,NumeroVisor);end;
            22 : begin NumMemVis:=frac(NumMemVis);str(NumMemVis:1:18,NumeroVisor);end;
            38 : begin if NumMemVis>0 then NumMemVis:=ln(NumMemVis);str(NumMemVis:1:18,NumeroVisor);end;
            23 : begin NumMemVis:=int(NumMemVis);str(NumMemVis:1:18,NumeroVisor);end;
            16 : begin NumMemVis:=NumMemVis*NumMemVis;str(NumMemVis:1:18,NumeroVisor);end;
            19 : begin NumMemVis:=sqrt(NumMemVis);str(NumMemVis:1:18,NumeroVisor);end;
            25 : begin
                    if Operacao='+' then begin NumMemVis:=NumMemVis+NumMemCalc;end;
                    if Operacao='P' then begin
                                            if NumMemCalc>0 then NumMemVis:=exp(NumMemVis*ln(NumMemCalc))
                                               else NumMemVis:=1;
                                         end;
                    if Operacao='-' then begin NumMemVis:=NumMemCalc-NumMemVis;end;
                    if Operacao='*' then begin NumMemVis:=NumMemCalc*NumMemVis;end;
                    if Operacao='/' then begin NumMemVis:=NumMemCalc/NumMemVis;end;
                    if Operacao=';' then begin NumMemVis:=NumMemCalc*NumMemVis/100;end;
                    Operacao:='P';
                    NumMemCalc:=NumMemVis;
                    NumMemVis:=0;
                    NumeroVisor:='';
                 end;
            31 : begin NumMemVis:=sin(NumMemVis);str(NumMemVis:1:18,NumeroVisor);end;
            37 : begin NumMemVis:=exp(NumMemVis);str(NumMemVis:1:18,NumeroVisor);end;
            39 : begin
                    if Operacao='+' then begin NumMemVis:=NumMemVis+NumMemCalc;end;
                    if Operacao='P' then begin
                                            if NumMemCalc>0 then NumMemVis:=exp(NumMemVis*ln(NumMemCalc))
                                              else NumMemVis:=1;
                                         end;
                    if Operacao='-' then begin NumMemVis:=NumMemCalc-NumMemVis;end;
                    if Operacao='*' then begin NumMemVis:=NumMemCalc*NumMemVis;end;
                    if Operacao='/' then begin NumMemVis:=NumMemCalc/NumMemVis;end;
                    if Operacao=';' then begin NumMemVis:=NumMemCalc*NumMemVis/100;end;
                    Operacao:=';';
                    NumMemCalc:=NumMemVis;
                    NumMemVis:=0;
                    NumeroVisor:='';
                 end;
            41 : begin
                    NumMemVis:=0;
                    NumeroVisor:='';
                    NumMemCalc:=0;
                    Base:=10;
                 end;
            20 : begin if cos(NumMemVis)<>0 then NumMemVis:=sin(NumMemVis)/cos(NumMemVis);str(NumMemVis:1:18,NumeroVisor);end;
            36 : begin
                    FlagNegative:=false;
                    if copy(NumeroVisor,1,1)='-' then
                    begin
                       FlagNegative:=true;
                       delete(NumeroVisor,1,1);
                    end;
                    IntgPart:='';
                    if pos('.',NumeroVisor)=0 then IntgPart:=NumeroVisor;
                    if pos('.',NumeroVisor)=length(NumeroVisor) then
                    begin
                       IntgPart:=NumeroVisor;
                       delete(IntgPart,length(IntgPart),1);
                    end;
                    if (pos('.',NumeroVisor)<length(NumeroVisor))
                    and(pos('.',NumeroVisor)>0) then IntgPart:=copy(NumeroVisor,1,pos('.',NumeroVisor)-1);
                    OldNumber:=IntgPart;
                    OldBase:=Base;
                    NewBase:=10;
                    begin
                       n:=0;
                       XA:=0;
                       XXa:=0;
                       l:=0;
                       NewNumber:='';
                       Neg:=false;
                       if copy(OldNumber,1,1)='-' then
                       begin
                          delete(OldNumber,1,1);
                          Neg:=true;
                       end;
                       for i:=1 to length(OldNumber) do
                       begin
                          XA:=ord(OldNumber[i])-48;
                          if XA>9 then XA:=XA-7;
                          n:=n*OldBase+XA;
                        end;
                        n:=n+0.5;
                        l:=trunc(ln(n)/ln(NewBase));
                        for i:=l downto 0 do
                        begin
                           XXa:=trunc(n/exp(i*ln(NewBase)));
                           n:=n-XXa*exp(i*ln(NewBase));
                           if XXa>9 then NewNumber:=NewNumber+chr(48+XXa+7);
                           if XXa<=9 then NewNumber:=NewNumber+chr(48+XXa);
                        end;
                        if Neg then NewNumber:='-'+NewNumber;
                        if OldNumber='' then NewNumber:='';
                        if OldNumber='0' then NewNumber:='0';
                    end;
                    AUX2:=NewNumber;
                    IntgPart:=AUX2;
                    NumeroVisor:=IntgPart;
                    if FlagNegative then NumeroVisor:='-'+NumeroVisor;
                    val(NumeroVisor,NumMemVis,AUX);
                    Base:=10;
                 end;
            50 : begin NumMemVis:=cos(NumMemVis);str(NumMemVis:1:18,NumeroVisor);end;
            35 : begin
                    FlagNegative:=false;
                    if copy(NumeroVisor,1,1)='-' then
                    begin
                       FlagNegative:=true;
                       delete(NumeroVisor,1,1);
                    end;
                    IntgPart:='';
                    if pos('.',NumeroVisor)=0 then IntgPart:=NumeroVisor;
                    if pos('.',NumeroVisor)=length(NumeroVisor) then
                    begin
                       IntgPart:=NumeroVisor;
                       delete(IntgPart,length(IntgPart),1);
                    end;
                    if (pos('.',NumeroVisor)<length(NumeroVisor))
                    and(pos('.',NumeroVisor)>0) then IntgPart:=copy(NumeroVisor,1,pos('.',NumeroVisor)-1);
                    OldNumber:=IntgPart;
                    OldBase:=Base;
                    NewBase:=16;
                    begin
                       n:=0;
                       XA:=0;
                       XXa:=0;
                       l:=0;
                       NewNumber:='';
                       Neg:=false;
                       if copy(OldNumber,1,1)='-' then
                       begin
                          delete(OldNumber,1,1);
                          Neg:=true;
                       end;
                       for i:=1 to length(OldNumber) do
                       begin
                          XA:=ord(OldNumber[i])-48;
                          if XA>9 then XA:=XA-7;
                          n:=n*OldBase+XA;
                        end;
                        n:=n+0.5;
                        l:=trunc(ln(n)/ln(NewBase));
                        for i:=l downto 0 do
                        begin
                           XXa:=trunc(n/exp(i*ln(NewBase)));
                           n:=n-XXa*exp(i*ln(NewBase));
                           if XXa>9 then NewNumber:=NewNumber+chr(48+XXa+7);
                           if XXa<=9 then NewNumber:=NewNumber+chr(48+XXa);
                        end;
                        if Neg then NewNumber:='-'+NewNumber;
                        if OldNumber='' then NewNumber:='';
                        if OldNumber='0' then NewNumber:='0';
                    end;
                    AUX2:=NewNumber;
                    IntgPart:=AUX2;
                    NumeroVisor:=IntgPart;
                    if FlagNegative then NumeroVisor:='-'+NumeroVisor;
                    Base:=16;
                 end;
            40 : begin NumMemVis:=abs(NumMemVis);str(NumMemVis:1:18,NumeroVisor);end;
            26 : begin
                    FlagNegative:=false;
                    if copy(NumeroVisor,1,1)='-' then
                    begin
                       FlagNegative:=true;
                       delete(NumeroVisor,1,1);
                    end;
                    IntgPart:='';
                    if pos('.',NumeroVisor)=0 then IntgPart:=NumeroVisor;
                    if pos('.',NumeroVisor)=length(NumeroVisor) then
                    begin
                       IntgPart:=NumeroVisor;
                       delete(IntgPart,length(IntgPart),1);
                    end;
                    if (pos('.',NumeroVisor)<length(NumeroVisor))
                    and(pos('.',NumeroVisor)>0) then IntgPart:=copy(NumeroVisor,1,pos('.',NumeroVisor)-1);
                    OldNumber:=IntgPart;
                    OldBase:=Base;
                    NewBase:=2;
                    begin
                       n:=0;
                       XA:=0;
                       XXa:=0;
                       l:=0;
                       NewNumber:='';
                       Neg:=false;
                       if copy(OldNumber,1,1)='-' then
                       begin
                          delete(OldNumber,1,1);
                          Neg:=true;
                       end;
                       for i:=1 to length(OldNumber) do
                       begin
                          XA:=ord(OldNumber[i])-48;
                          if XA>9 then XA:=XA-7;
                          n:=n*OldBase+XA;
                        end;
                        n:=n+0.5;
                        l:=trunc(ln(n)/ln(NewBase));
                        for i:=l downto 0 do
                        begin
                           XXa:=trunc(n/exp(i*ln(NewBase)));
                           n:=n-XXa*exp(i*ln(NewBase));
                           if XXa>9 then NewNumber:=NewNumber+chr(48+XXa+7);
                           if XXa<=9 then NewNumber:=NewNumber+chr(48+XXa);
                        end;
                        if Neg then NewNumber:='-'+NewNumber;
                        if OldNumber='' then NewNumber:='';
                        if OldNumber='0' then NewNumber:='0';
                    end;
                    AUX2:=NewNumber;
                    IntgPart:=AUX2;
                    NumeroVisor:=IntgPart;
                    if FlagNegative then NumeroVisor:='-'+NumeroVisor;
                    Base:=2;
                 end;
            34 : begin NumMemVis:=NumMemVis*180/3.1415926535897932384654;str(NumMemVis:1:18,NumeroVisor);end;
            43 : begin NumMemVis:=NumMemVis*3.1415926535897932384654/180;str(NumMemVis:1:18,NumeroVisor);end;
            27 : begin
                    FlagNegative:=false;
                    if copy(NumeroVisor,1,1)='-' then
                    begin
                       FlagNegative:=true;
                       delete(NumeroVisor,1,1);
                    end;
                    IntgPart:='';
                    if pos('.',NumeroVisor)=0 then IntgPart:=NumeroVisor;
                    if pos('.',NumeroVisor)=length(NumeroVisor) then
                    begin
                       IntgPart:=NumeroVisor;
                       delete(IntgPart,length(IntgPart),1);
                    end;
                    if (pos('.',NumeroVisor)<length(NumeroVisor))
                    and(pos('.',NumeroVisor)>0) then IntgPart:=copy(NumeroVisor,1,pos('.',NumeroVisor)-1);
                    OldNumber:=IntgPart;
                    OldBase:=Base;
                    NewBase:=8;
                    begin
                       n:=0;
                       XA:=0;
                       XXa:=0;
                       l:=0;
                       NewNumber:='';
                       Neg:=false;
                       if copy(OldNumber,1,1)='-' then
                       begin
                          delete(OldNumber,1,1);
                          Neg:=true;
                       end;
                       for i:=1 to length(OldNumber) do
                       begin
                          XA:=ord(OldNumber[i])-48;
                          if XA>9 then XA:=XA-7;
                          n:=n*OldBase+XA;
                        end;
                        n:=n+0.5;
                        l:=trunc(ln(n)/ln(NewBase));
                        for i:=l downto 0 do
                        begin
                           XXa:=trunc(n/exp(i*ln(NewBase)));
                           n:=n-XXa*exp(i*ln(NewBase));
                           if XXa>9 then NewNumber:=NewNumber+chr(48+XXa+7);
                           if XXa<=9 then NewNumber:=NewNumber+chr(48+XXa);
                        end;
                        if Neg then NewNumber:='-'+NewNumber;
                        if OldNumber='' then NewNumber:='';
                        if OldNumber='0' then NewNumber:='0';
                    end;
                    AUX2:=NewNumber;
                    IntgPart:=AUX2;
                    NumeroVisor:=IntgPart;
                    if FlagNegative then NumeroVisor:='-'+NumeroVisor;
                    Base:=8;
                 end;
            21 : begin
                    NumMemVis:=0;
                    NumeroVisor:='';
                 end;
            47 : begin
                    AUX3:=1;
                    if (NumMemVis<1754)and(NumMemVis>0) then for i:=1 to trunc(NumMemVis) do AUX3:=AUX3*i;
                    NumMemVis:=AUX3;
                    str(NumMemVis:1:18,NumeroVisor);
                 end;
            45 : begin if NumMemVis<>0 then NumMemVis:=1/NumMemVis;str(NumMemVis:1:18,NumeroVisor);end;
            13 : begin
                    if Operacao='+' then begin NumMemVis:=NumMemVis+NumMemCalc;end;
                    if Operacao='-' then begin NumMemVis:=NumMemCalc-NumMemVis;end;
                    if Operacao='P' then begin
                                            if NumMemCalc>0 then NumMemVis:=exp(NumMemVis*ln(NumMemCalc))
                                               else NumMemVis:=1;
                                         end;
                    if Operacao='*' then begin NumMemVis:=NumMemCalc*NumMemVis;end;
                    if Operacao='/' then begin NumMemVis:=NumMemCalc/NumMemVis;end;
                    if Operacao=';' then begin NumMemVis:=NumMemCalc*NumMemVis/100;end;
                    Operacao:=#0;
                    NumMemCalc:=NumMemVis;
                    str(NumMemVis:1:18,NumeroVisor);
                 end;
            17 : begin NumMemVis:=-NumMemVis;str(NumMemVis:1:18,NumeroVisor);end;
            78 : begin
                    if Operacao='P' then begin
                                            if NumMemCalc>0 then NumMemVis:=exp(NumMemVis*ln(NumMemCalc))
                                               else NumMemVis:=1;
                                         end;
                    if Operacao='+' then begin NumMemVis:=NumMemVis+NumMemCalc;end;
                    if Operacao='-' then begin NumMemVis:=NumMemCalc-NumMemVis;end;
                    if Operacao='*' then begin NumMemVis:=NumMemCalc*NumMemVis;end;
                    if Operacao='/' then begin NumMemVis:=NumMemCalc/NumMemVis;end;
                    if Operacao=';' then begin NumMemVis:=NumMemCalc*NumMemVis/100;end;
                    Operacao:='+';
                    NumMemCalc:=NumMemVis;
                    NumMemVis:=0;
                    NumeroVisor:='';
                 end;
            55 : begin
                    if Operacao='+' then begin NumMemVis:=NumMemVis+NumMemCalc;end;
                    if Operacao='P' then begin
                                            if NumMemCalc>0 then NumMemVis:=exp(NumMemVis*ln(NumMemCalc))
                                               else NumMemVis:=1;
                                         end;
                    if Operacao='-' then begin NumMemVis:=NumMemCalc-NumMemVis;end;
                    if Operacao='*' then begin NumMemVis:=NumMemCalc*NumMemVis;end;
                    if Operacao='/' then begin NumMemVis:=NumMemCalc/NumMemVis;end;
                    if Operacao=';' then begin NumMemVis:=NumMemCalc*NumMemVis/100;end;
                    Operacao:='*';
                    NumMemCalc:=NumMemVis;
                    NumMemVis:=0;
                    NumeroVisor:='';
                 end;
            74 : begin
                    if Operacao='P' then begin
                                            if NumMemCalc>0 then NumMemVis:=exp(NumMemVis*ln(NumMemCalc))
                                               else NumMemVis:=1;
                                         end;
                    if Operacao='+' then begin NumMemVis:=NumMemVis+NumMemCalc;end;
                    if Operacao='-' then begin NumMemVis:=NumMemCalc-NumMemVis;end;
                    if Operacao='*' then begin NumMemVis:=NumMemCalc*NumMemVis;end;
                    if Operacao='/' then begin NumMemVis:=NumMemCalc/NumMemVis;end;
                    if Operacao=';' then begin NumMemVis:=NumMemCalc*NumMemVis/100;end;
                    Operacao:='-';
                    NumMemCalc:=NumMemVis;
                    NumMemVis:=0;
                    NumeroVisor:='';
                 end;
            12 : begin
                    if Operacao='P' then begin
                                            if NumMemCalc>0 then NumMemVis:=exp(NumMemVis*ln(NumMemCalc))
                                               else NumMemVis:=1;
                                         end;
                    if Operacao='+' then begin NumMemVis:=NumMemVis+NumMemCalc;end;
                    if Operacao='-' then begin NumMemVis:=NumMemCalc-NumMemVis;end;
                    if Operacao='*' then begin NumMemVis:=NumMemCalc*NumMemVis;end;
                    if Operacao='/' then begin NumMemVis:=NumMemCalc/NumMemVis;end;
                    if Operacao=';' then begin NumMemVis:=NumMemCalc*NumMemVis/100;end;
                    Operacao:='-';
                    NumMemCalc:=NumMemVis;
                    NumMemVis:=0;
                    NumeroVisor:='';
                 end;
            53 : begin
                    if Operacao='+' then begin NumMemVis:=NumMemVis+NumMemCalc;end;
                    if Operacao='P' then begin
                                            if NumMemCalc>0 then NumMemVis:=exp(NumMemVis*ln(NumMemCalc))
                                               else NumMemVis:=1;
                                         end;
                    if Operacao='-' then begin NumMemVis:=NumMemCalc-NumMemVis;end;
                    if Operacao='*' then begin NumMemVis:=NumMemCalc*NumMemVis;end;
                    if Operacao='/' then begin NumMemVis:=NumMemCalc/NumMemVis;end;
                    if Operacao=';' then begin NumMemVis:=NumMemCalc*NumMemVis/100;end;
                    if NumMemVis<>0 then
                    begin
                       Operacao:='/';
                       NumMemCalc:=NumMemVis;
                    end;
                    NumMemVis:=0;
                    NumeroVisor:='';
                 end;
         end;
         if ((Base<>10)and(NumMemVis<>0)) then
         begin
            FlagNegative:=false;
            str(NumMemVis:1:18,NumeroVisor);
            if copy(NumeroVisor,1,1)='-' then
            begin
               FlagNegative:=false;
               delete(NumeroVisor,1,1);
            end;
            IntgPart:='';
            if pos('.',NumeroVisor)=0 then IntgPart:=NumeroVisor;
            if pos('.',NumeroVisor)=length(NumeroVisor) then
            begin
               IntgPart:=NumeroVisor;
               delete(IntgPart,length(IntgPart),1);
            end;
            if (pos('.',NumeroVisor)<length(NumeroVisor))
            and(pos('.',NumeroVisor)>0) then IntgPart:=copy(NumeroVisor,1,pos('.',NumeroVisor)-1);
            OldNumber:=IntgPart;
            OldBase:=10;
            NewBase:=Base;
            begin
               n:=0;
               XA:=0;
               XXa:=0;
               l:=0;
               NewNumber:='';
               Neg:=false;
               if copy(OldNumber,1,1)='-' then
               begin
                  delete(OldNumber,1,1);
                  Neg:=true;
               end;
               for i:=1 to length(OldNumber) do
               begin
                  XA:=ord(OldNumber[i])-48;
                  if XA>9 then XA:=XA-7;
                  n:=n*OldBase+XA;
               end;
               n:=n+0.5;
               l:=trunc(ln(n)/ln(NewBase));
               for i:=l downto 0 do
               begin
                  XXa:=trunc(n/exp(i*ln(NewBase)));
                  n:=n-XXa*exp(i*ln(NewBase));
                  if XXa>9 then NewNumber:=NewNumber+chr(48+XXa+7);
                  if XXa<=9 then NewNumber:=NewNumber+chr(48+XXa);
               end;
               if Neg then NewNumber:='-'+NewNumber;
               if OldNumber='' then NewNumber:='';
               if OldNumber='0' then NewNumber:='0';
            end;
            AUX2:=NewNumber;
            NumeroVisor:=AUX2;
            if FlagNegative then NumeroVisor:='-'+NumeroVisor;
         end;
         if ((Botao=8)or(Botao=71)) and (Base>7) then NumeroVisor:=NumeroVisor+'7';
         if ((Botao=9)or(Botao=72)) and (Base>8) then NumeroVisor:=NumeroVisor+'8';
         if ((Botao=10)or(Botao=73)) and (Base>9) then NumeroVisor:=NumeroVisor+'9';
         if ((Botao=5)or(Botao=75)) and (Base>4) then NumeroVisor:=NumeroVisor+'4';
         if ((Botao=6)or(Botao=76)) and (Base>5) then NumeroVisor:=NumeroVisor+'5';
         if ((Botao=7)or(Botao=77)) and (Base>6) then NumeroVisor:=NumeroVisor+'6';
         if ((Botao=2)or(Botao=79)) and (Base>1) then NumeroVisor:=NumeroVisor+'1';
         if ((Botao=3)or(Botao=80)) and (Base>2) then NumeroVisor:=NumeroVisor+'2';
         if ((Botao=4)or(Botao=81)) and (Base>3) then NumeroVisor:=NumeroVisor+'3';
         if ((Botao=11)or(Botao=82)) then NumeroVisor:=NumeroVisor+'0';
         if (Botao=30) and (Base>10) then NumeroVisor:=NumeroVisor+'A';
         if (Botao=48) and (Base>11)then NumeroVisor:=NumeroVisor+'B';
         if (Botao=46) and (Base>12)then NumeroVisor:=NumeroVisor+'C';
         if (Botao=32) and (Base>13)then NumeroVisor:=NumeroVisor+'D';
         if (Botao=18) and (Base>14)then NumeroVisor:=NumeroVisor+'E';
         if (Botao=33) and (Base>15)then NumeroVisor:=NumeroVisor+'F';
         if (((Botao=83)or(Botao=52)or(Botao=51))and(Base=10)) then NumeroVisor:=NumeroVisor+'.';
         if Botao=14 then delete(NumeroVisor,length(NumeroVisor),1);
         if Botao=67 then
         begin
            Comeco:=ptr($40,$1a);
            Final:=ptr($40,$1c);
            Comeco^:=$1e;
            if length(NumeroVisor)>14 then delete(NumeroVisor,14,length(NumeroVisor)-14);
            Final^:=2*length(NumeroVisor)+$1e;
            w:=0;
            repeat
               XX:=ptr($40,$1e+w);
               ZZ:=ptr($40,$1e+w+1);
               ZZ^:=0;
               q:=ord(NumeroVisor[(w div 2)+1]);
               XX^:=q;
               w:=w+2;
            until w=2*length(NumeroVisor);
            Botao:=68;
         end;
      until Botao=68;
      textattr:=7;
      _orig:=$9400;
      _dest:=$b800;
      inline($b9/$80/$3e/$33/$ff/$8b/$16/>_dest/$8e/$c2/$33/$f6/
             $8b/$16/>_orig/$1e/$8e/$da/$d1/$e9/$f3/$a5/$1f);
      Regs.ah:=1;
      Regs.cx:=Cursor;
      intr($10,Regs);
      gotoxy(x,y);
   end;
   if i>=4 then   i:=i-4;   { Ctrl          }
   if i>=2 then   i:=i-2;   { Shift esq.    }
   if i>=1 then   i:=i-1;   { Shift dir.    }
end;
{--------------------------------------------------------------------------}
begin
   if test8087=0 then
   begin
      writeln;
      writeln('Esta vers„o da Calculadora necessita do 80x87 instalado.');
      writeln;
      halt(123);
   end;
   getintvec($1b,abcd);
   setintvec($1b,SaveInt1B);
   getintvec($28,AbCd);
   Origem:=@inicio;
   Sair:=false;
   JFF:=0;
   repeat
      ZZS:=seg(AbCd^)+JFF;
      ZZO:=ofs(AbCd^)+JFF;
      ZZS1:=seg(Origem^)+JFF;
      ZZO1:=ofs(Origem^)+JFF;
      inc(JFF);
      if JFF>=10 then Sair:=true;
      From:=ptr(ZZS,ZZO);
      Fin :=ptr(ZZS1,ZZO1);
      if ((From^)<>(Fin^)) then JFF:=50;
   until Sair;
   if Sair and (JFF = 10) then
   begin
      writeln;
      writeln('Calculadora residente j  estava instalada.');
      writeln;
      writeln('Para ativar, aperte <Ctrl> + <Shift> esquerdo.');
      writeln;
      writeln('Para remover da mem¢ria, aperte <Ctrl> + <Alt> + <Del> ou');
      writeln('aperte a Tecla RESET do seu micro.');
      writeln;
      exit;
   end;
   writeln;
   writeln('Calculadora residente instalada.');
   writeln;
   writeln('Aperte <Ctrl> + <Shift> esquerdo para ativar.');
   writeln;
   getintvec($28,AbCd);
   setintvec($28,@inicio);
   keep(768);
end.
