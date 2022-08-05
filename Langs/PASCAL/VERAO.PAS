uses dos;
var h,m,s,c : word;
    v       : integer;
{----------------------------------------------------------------------}
Function CMOSInfo(valor:byte) : byte;
var aux : byte;
label a,b,c;
begin
   asm
      mov al,valor
      cli
      jmp c
   c: out $70,al
      jmp a
   a: in al,$71
      jmp b
   b: sti
      mov aux,al
   end;
   CMOSInfo:=aux;
end;
{----------------------------------------------------------------------}
Procedure WriteCMOSInfo(indice,valor:byte);
var aux : byte;
label a,b,c;
begin
   asm
      mov al,indice
      cli
      jmp c
   c: out $70,al
      mov al,valor
   a: out $71,al
      jmp b
   b: sti
   end;
end;
{----------------------------------------------------------------------}
begin
   writeln;
   if (CMOSInfo($b) mod 2)=0 then
   begin
      writeln('Hor rio de ver„o agora ativo.');
      v:=1;
   end else
   begin
      writeln('Hor rio de ver„o agora inativo.');
      v:=-1;
   end;
   writeln;
   WriteCMOSInfo($b,CMOSInfo($b) xor 1);
   gettime(h,m,s,c);
   writeln('Hora atual: ',h);
   if (h=0) and (v=-1) then begin h:=23; v:=0; end;
   if (h=23) and (v=1) then begin h:=0;  v:=0; end;
   writeln('Nova Hora: ',h+v);
   settime(h+v,m,s,c);
end.