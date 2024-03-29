uses dos,crt;
var IOaddr     : word;
    raw        : array[1..64000] of byte;
    arq        : string;
    f          : file of byte;
    s          : Searchrec;
    zzz        : boolean;
    aux,tmp    : byte;
    i,finis,spd: word;
label d;
{-------------------------------------------------------------------}
Procedure SetVoice(x : boolean);
var aux : byte;
begin
   repeat
      asm
         mov dx,IOaddr;
         add dx,$c;
         in  al,dx
         and al,$80
         mov aux,al
      end;
   until aux=0;
   if not(x) then aux:=$d3;
   if x then aux:=$d1;
   asm
      mov dx,IOaddr;
      add dx,$c
      mov al,aux
      out dx,al
   end;
end;
{-------------------------------------------------------------------}
Procedure SetSampleRate(rate : word);
var aux,tc : byte;
begin
   tc:=trunc(256-(1000000/rate));
   repeat
      asm
         mov dx,IOaddr;
         add dx,$c;
         in  al,dx
         and al,$80
         mov aux,al
      end;
   until aux=0;
   asm
      mov dx,IOaddr;
      add dx,$c
      mov al,$40
      out dx,al
   end;
   repeat
      asm
         mov dx,IOaddr;
         add dx,$c;
         in  al,dx
         and al,$80
         mov aux,al
      end;
   until aux=0;
   asm
      mov dx,IOaddr;
      add dx,$c
      mov al,tc
      out dx,al
   end;
end;
{-------------------------------------------------------------------}
Function InitSb : boolean;
var saida : word;
label A,B,C;
begin
   asm
      mov al,1
      mov dx,IOaddr
      add dx,6
      out dx,al
      in al,dx
      in al,dx
      in al,dx
      in al,dx
      mov al,0
      out dx,al
      add dx,4
      mov cx,100
    A:in al,dx
      cmp al,$aa
      je B
      loop A
      mov ax,1
      jmp C
    B:mov ax,0
    C:mov saida,ax
   end;
   if saida=0 then InitSb:=true;
   if saida<>0 then InitSb:=false;
end;
{-------------------------------------------------------------------}
begin
   if ParamCount<>1 then
   begin
      writeln;
      writeln('Uso: SBOUT nome_de_arquivo');
      halt(1);
   end;
{-----------------------}
   IOAddr:=$220;
{-----------------------}
   if not(InitSb) then
   begin
      writeln;
      writeln('N�o encontrei a Soundblaster!');
      halt(2);
   end;
   if InitSb then
   begin
      writeln;
      writeln('Soundblaster no endere�o ',IOAddr,'.');
   end;
   arq:=Paramstr(1);
   assign(f,arq);
   findfirst(arq,anyfile,s);
   if doserror<>0 then
   begin
      writeln;
      writeln('Erro ao abrir arquivo!');
      halt(3);
   end;
   Reset(f);
   SetVoice(true);
   i:=1;
   repeat
      read(f,raw[i]);
      inc(i);
   until eof(f)or(i>64000);
   write('Velocidade: ');
   readln(spd);
   finis:=i;
   i:=1;
   repeat
      repeat
         asm
            mov dx,IOaddr;
            add dx,$c;
            in  al,dx
            and al,$80
            mov aux,al
         end;
      until aux=0;
      asm
         mov dx,IOaddr;
         add dx,$c
         mov al,$10
         out dx,al
      end;
      repeat
         asm
            mov dx,IOaddr;
            add dx,$c;
            in  al,dx
            and al,$80
            mov aux,al
         end;
      until aux=0;
      tmp:=raw[i];
      asm
         mov dx,IOaddr;
         add dx,$c
         mov al,tmp
         out dx,al
      end;
      repeat
         asm
            mov dx,IOaddr;
            add dx,$c;
            in  al,dx
            and al,$80
            mov aux,al
         end;
      until aux=0;
      inc(i);
      asm
         mov cx,spd       {250, 50}
       d:loop d
      end;
   until i=finis;
   SetVoice(false);
   close(f);
   zzz:=InitSb;
end.
