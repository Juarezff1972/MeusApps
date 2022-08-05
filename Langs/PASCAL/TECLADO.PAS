Program Keybored;

uses crt;

var tecla : word;
    chave : string;
    i     : integer;
    sair  : boolean;
{----------------------------------------------------------------------}
procedure Aperto;
begin
   inline($b4/$02/           { mov ah,0002  }
          $cd/$16/           { int 16       }
          $a2/>tecla);       { mov tecla,al }
end;
{----------------------------------------------------------------------}
begin
   clrscr;
   chave:='';
   tecla:=0;
   sair:=false;
   writeln('Aperte <CTRL> + <SHIFT> esquerdo para sair...');
   repeat
     Aperto;
     i:=tecla;
     chave:='';
     gotoxy(1,4);
     Writeln('Codigo usado: ',tecla,'    ');
     if i>=128 then begin
       chave:=chave+' Insert ligado -';
       i:=i-128;
     end;
     if i>=64 then begin
       chave:=chave+' Caps Lock -';
       i:=i-64;
     end;
     if i>=32 then begin
       chave:=chave+' Num Lock -';
       i:=i-32;
     end;
     if i>=16 then begin
       chave:=chave+' Scroll Lock -';
       i:=i-16;
     end;
     if i>=8 then begin
       chave:=chave+' Alt -';
       i:=i-8;
     end;
     if i=6 then sair:=true;
     if i>=4 then begin
       chave:=chave+' Ctrl -';
       i:=i-4;
     end;
     if i>=2 then begin
       chave:=chave+' Shift Esquerdo -';
       i:=i-2;
     end;
     if i>=1 then begin
       chave:=chave+' Shift Direito';
       i:=i-1;
     end;
     gotoxy(1,6);
     writeln('Teclas Pressionadas:',chave,'                                                                         ');
  until sair;
end.