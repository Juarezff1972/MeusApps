uses Crt,Dos;

var _drive,_nsects,_lsect,jff : word;
    d,_byte                   : byte;
    _enddest,_endorig         : word;
    bootfile                  : text;
    _cilindro,_cabeca,_tipo   : byte;
    regs                      : registers;

{-----------------------------------------------------------------------}
Procedure Inicio;
begin
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90/$90);
   inline($90/$90/$90/$90);
end;
{-----------------------------------------------------------------------}
Procedure Guarde;
begin
   inline($8b/$1e/>_byte/        { mov bx,_BYTE    }
          $8b/$3e/>_enddest/     { mov di,_ENDDEST }
          $8a/$c3/               { mov al,bl       }
          $2e/$88/$85/$00/$00);  { mov cs:[di],al  }
end;
{-----------------------------------------------------------------------}
Procedure Zerar;
begin
   for _enddest:=ofs(Inicio)+10 to ofs(Inicio)+522 do
   begin
      _byte:=0;
      Guarde;
   end;
end;
{-----------------------------------------------------------------------}
Procedure Pegue;
begin
   inline($8b/$3e/>_endorig/ { mov di,_ENDORIG }
          $8a/$05/           { mov al,[di]     }
          $a2/>_byte);       { mov _BYTE,al    }
end;
{-----------------------------------------------------------------------}
Procedure LerSetor;
begin
   regs.ah:=2;
   regs.al:=1;
   regs.ch:=_cilindro;
   regs.cl:=_lsect;
   regs.dh:=_cabeca;
   regs.dl:=_drive;
   regs.es:=cseg;
   regs.bx:=ofs(Inicio)+10;
   intr($13,regs);
end;
{-----------------------------------------------------------------------}
Procedure GravarSetor;
begin
   regs.ah:=3;
   regs.al:=1;
   regs.ch:=_cilindro;
   regs.cl:=_lsect;
   regs.dh:=_cabeca;
   regs.dl:=_drive;
   regs.es:=cseg;
   regs.bx:=ofs(Inicio)+10;
   intr($13,regs);
end;
{-----------------------------------------------------------------------}
Procedure BootSector;
begin
   assign(bootfile,'c:\pascal\prog\bootfile.tbt');
   reset(bootfile);
   for _enddest:=ofs(Inicio)+10 to ofs(Inicio)+10+512 do
   begin
      readln(bootfile,_byte);
      Guarde;
      d:=_byte;
      write(d,' - ');
   end;
   writeln;
   close(bootfile);
   writeln;
   _drive:=1;
   _nsects:=1;
   _lsect:=1;
   _cilindro:=0;
   _cabeca:=0;
   GravarSetor;
   writeln(_drive,',',lo(_drive),',',hi(_drive));
end;
{-----------------------------------------------------------------------}
Procedure FATSector;
begin
   for _enddest:=ofs(Inicio)+10 to ofs(Inicio)+10+512 do
   begin
      _byte:=0;
      Guarde;
   end;
   _enddest:=ofs(Inicio)+10;
   _Byte:=$FD;
   _enddest:=ofs(Inicio)+11;
   _Byte:=$FF;
   _enddest:=ofs(Inicio)+12;
   _Byte:=$FF;
   _drive:=1;
   _nsects:=1;
   _lsect:=2;
   _cilindro:=0;
   _cabeca:=0;
   GravarSetor;
   _lsect:=4;
   _cilindro:=0;
   _cabeca:=0;
   GravarSetor;
end;
{-----------------------------------------------------------------------}
Procedure Formatar;
var i,entrada,lados,total : byte;
begin
   clrscr;
   writeln('Qual o tamanho do disco? ');
   writeln;
   writeln(' 1 - 160 Kb');
   writeln(' 2 - 180 Kb');
   writeln(' 3 - 320 Kb');
   writeln(' 4 - 360 Kb');
   writeln(' 5 - 1.2 Mb');
   writeln;
   write('--->');
   readln(entrada);
   _drive:=1;
   lados:=1;
   total:=39;
   if entrada=5 then _tipo:=15;
   if (entrada=2) or (entrada=4) then _tipo:=9;
   if (entrada=1) or (entrada=3) then _tipo:=8;
   if entrada>2 then lados:=2;
   if entrada=5 then total:=79;
   writeln;
   writeln(_tipo,' setores por triha');
   writeln;
   regs.al:=02;
   regs.dl:=_drive;
   regs.ah:=$17;
   intr($13,regs);
   for _cilindro:=0 to total do
   begin
      for i:=1 to lados do
      begin
         _drive:=1;
         _cabeca:=i-1;
         _lsect:=1;
         gotoxy(1,20);
         writeln('Formatando cilindro ',_cilindro,' - cabeca ',_cabeca);
         jff:=_lsect;
         _Byte:=_cilindro;
         _Enddest:=ofs(Inicio)+10+512+1;
         guarde;
         _Byte:=_cabeca;
         _Enddest:=ofs(Inicio)+10+512+2;
         guarde;
         _Byte:=jff;
         _Enddest:=ofs(Inicio)+10+512+3;
         guarde;
         _Byte:=2;
         _Enddest:=ofs(Inicio)+10+512+4;
         guarde;
         regs.ah:=$18;
         regs.ch:=total;
         regs.cl:=total+_tipo*32;
         regs.dl:=_drive;
         intr($13,regs);
         regs.ah:=5;
{         regs.al:=_tipo;}
         regs.cx:=_cilindro;
         regs.dh:=_cabeca;
         regs.dl:=_drive;
         regs.es:=cseg;
         regs.bx:=ofs(Inicio)+10+512+1;
         intr($13,regs);
         gotoxy(1,21);
         writeln('-->',regs.ah,' -->',regs.cl,'    ');
         regs.ah:=4;
         regs.al:=_tipo;
         regs.ch:=_cilindro;
         regs.cl:=jff;
         regs.dh:=_cabeca;
         regs.dl:=_drive;
         intr($13,regs);
         gotoxy(1,22);
         writeln('-->',regs.ah,'--->',regs.al,' -->',regs.cl,'    ');
      end;
   end;
end;
{-----------------------------------------------------------------------}
begin
   Inicio;
   _byte:=0;
   Zerar;
   regs.ah:=0;
   intr($13,regs);
   Formatar;
   Zerar;
   regs.ah:=0;
   intr($13,regs);
   _drive:=1;
   _nsects:=_tipo;
   _lsect:=1;
   for _cilindro:=0 to 39 do for _cabeca:=0 to 1 do
   begin
      gotoxy(1,20);
      writeln('Preparando trilha ',_cilindro,' - cabe‡a ',_cabeca,'   ');
      GravarSetor;
   end;
   BootSector;
   FATSector;
end.
