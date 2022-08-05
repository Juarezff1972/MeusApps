Program Conversion;
{$M 65520,0,131072}
Uses Crt,Dos;

var tecla : word;                        ch  : char;
    chave : string;                      dcm : string;
    i     : integer;                     bin : string;
    sair  : boolean;                     oct : string;
    opcoes: array[1..5] of string[12];   hex : string;
    menu  : byte;                        ltr : word;
    esc   : byte;                        opr : array[1..3] of word;
    sinal : char;                   saveintr : pointer;
    _dest : word;                       regs : registers;
    _orig : word;

{-----------------------------------------------------------------------}
Procedure Bin_Hexadecimal(var hexa : string; bin : string);
var k,adc : integer;
    number: string[4];

begin
   hexa:='';
   if length(bin)<16 then
   begin
      k:=length(bin);
      for adc:=1 to (16-k) do bin:='0'+bin;
   end;
   for k:=1 to 4 do
   begin
      number:=copy(bin,((k*4)-3),4);
      if number='0000' then hexa:=hexa+'0';
      if number='0001' then hexa:=hexa+'1';
      if number='0010' then hexa:=hexa+'2';
      if number='0011' then hexa:=hexa+'3';
      if number='0100' then hexa:=hexa+'4';
      if number='0101' then hexa:=hexa+'5';
      if number='0110' then hexa:=hexa+'6';
      if number='0111' then hexa:=hexa+'7';
      if number='1000' then hexa:=hexa+'8';
      if number='1001' then hexa:=hexa+'9';
      if number='1010' then hexa:=hexa+'A';
      if number='1011' then hexa:=hexa+'B';
      if number='1100' then hexa:=hexa+'C';
      if number='1101' then hexa:=hexa+'D';
      if number='1110' then hexa:=hexa+'E';
      if number='1111' then hexa:=hexa+'F';
   end;
end;
{-----------------------------------------------------------------------}
Procedure Bin_Octal(var octa : string; bin : string);
var k,adc : integer;
    number: string[3];
begin
   octa:='';
   if length(bin)<19 then
   begin
      k:=length(bin);
      for adc:=1 to (19-k) do bin:='0'+bin;
   end;
   for k:=1 to 6 do
   begin
      number:=copy(bin,((k*3)-1),3);
      if number='000' then octa:=octa+'0';
      if number='001' then octa:=octa+'1';
      if number='010' then octa:=octa+'2';
      if number='011' then octa:=octa+'3';
      if number='100' then octa:=octa+'4';
      if number='101' then octa:=octa+'5';
      if number='110' then octa:=octa+'6';
      if number='111' then octa:=octa+'7';
   end;
end;
{-----------------------------------------------------------------------}
Procedure Dec_Hexadecimal(var hexa : string; decimal : string);
var nd,d1,d2,d3,d4,n4,n3,n2,n1 : word;
    c1,c2,c3,c4                : char;
begin
   val(decimal,nd,nd);
   d1:=trunc(nd/16);
   d2:=trunc(d1/16);
   d3:=trunc(d2/16);
   d4:=trunc(d3/16);
   n4:=nd-d1*16;
   n3:=d1-d2*16;
   n2:=d2-d3*16;
   n1:=d3-d4*16;
   if (n4>=10) and (n4<=15) then n4:=n4+55;
   if (n3>=10) and (n3<=15) then n3:=n3+55;
   if (n2>=10) and (n2<=15) then n2:=n2+55;
   if (n1>=10) and (n1<=15) then n1:=n1+55;
   if (n4>= 0) and (n4< 10) then n4:=n4+48;
   if (n3>= 0) and (n3< 10) then n3:=n3+48;
   if (n2>= 0) and (n2< 10) then n2:=n2+48;
   if (n1>= 0) and (n1< 10) then n1:=n1+48;
   c1:=chr(n1);
   c2:=chr(n2);
   c3:=chr(n3);
   c4:=chr(n4);
   hexa:=c1+c2+c3+c4;
end;
{-----------------------------------------------------------------------}
Procedure Hex_Binario(var bin : string; hexa : string);
var hx : string;
    k  : integer;
begin
   bin:='';
   if length(hexa)<4 then for k:=1 to (4-length(hexa)) do hexa:='0'+hexa;
   for k:=1 to 4 do
   begin
      hx:=copy(hexa,k,1);
      if hx='0' then bin:=bin+'0000';
      if hx='1' then bin:=bin+'0001';
      if hx='2' then bin:=bin+'0010';
      if hx='3' then bin:=bin+'0011';
      if hx='4' then bin:=bin+'0100';
      if hx='5' then bin:=bin+'0101';
      if hx='6' then bin:=bin+'0110';
      if hx='7' then bin:=bin+'0111';
      if hx='8' then bin:=bin+'1000';
      if hx='9' then bin:=bin+'1001';
      if hx='A' then bin:=bin+'1010';
      if hx='B' then bin:=bin+'1011';
      if hx='C' then bin:=bin+'1100';
      if hx='D' then bin:=bin+'1101';
      if hx='E' then bin:=bin+'1110';
      if hx='F' then bin:=bin+'1111';
   end;
end;
{-----------------------------------------------------------------------}
Procedure Oct_Binario(var bin : string; octa : string);
var oc : string;
    k  : integer;
begin
   bin:='';
   if length(octa)<6 then for k:=1 to (6-length(octa)) do octa:='0'+octa;
   for k:=1 to 6 do
   begin
      oc:=copy(octa,k,1);
      if oc='0' then bin:=bin+'000';
      if oc='1' then bin:=bin+'001';
      if oc='2' then bin:=bin+'010';
      if oc='3' then bin:=bin+'011';
      if oc='4' then bin:=bin+'100';
      if oc='5' then bin:=bin+'101';
      if oc='6' then bin:=bin+'110';
      if oc='7' then bin:=bin+'111';
   end;
   bin:=copy(bin,3,16);
end;
{-----------------------------------------------------------------------}
Procedure Dec_Binario(var bin : string; decimal : string);
var hx : string;
begin
   Dec_Hexadecimal(hx,decimal);
   Hex_Binario(bin,hx);
end;
{-----------------------------------------------------------------------}
Procedure Oct_Hexadecimal(var hexa : string; octa : string);
var bin : string;
begin
   oct_binario(bin,octa);
   bin_hexadecimal(hexa,bin);
end;
{-----------------------------------------------------------------------}
Procedure Dec_Octal(var octa : string; decimal : string);
var bin : string;
begin
   dec_binario(bin,decimal);
   bin_octal(octa,bin);
end;
{-----------------------------------------------------------------------}
Procedure Hex_Octal(var octa : string; hexa : string);
var bin : string;
begin
   hex_binario(bin,hexa);
   bin_octal(octa,bin);
end;
{-----------------------------------------------------------------------}
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
{-----------------------------------------------------------------------}
procedure caracteres;
begin
   inline($b4/$08/               { MOV AH,0008 }
          $cd/$10/               { INT 10      }
          $b9/$01/$00/           { MOV CX,0001 }
          $b3/$03/               { MOV BL,0003 }
          $b4/$09/               { MOV AH,0009 }
          $cd/$10);              { INT 10      }
end;
{-----------------------------------------------------------------------}
procedure sombra(x1,y1,x2,y2 : integer);
var x,y : integer;
begin
   for x:=x1+2 to x2+1 do
      for y:=y1+1 to y2 do
      begin
         gotoxy(x,y);
         caracteres;
      end;
end;
{----------------------------------------------------------------------}
procedure Aperto;
begin
   inline($b4/$02/           { mov ah,0002  }
          $cd/$16/           { int 16       }
          $a2/>tecla);       { mov tecla,al }
end;
{----------------------------------------------------------------------}
Procedure Cursor(o : boolean);
begin
   if o=true then
      inline($b8/$68/$01/       { mov ax,0168 }
             $b9/$00/$19/       { mov cx,1900 }  { Desativa cursor }
             $cd/$10);          { int 10      }
   if o=false then
      inline($b8/$68/$01/       { mov ax,0168 }
             $b9/$07/$06/       { mov cx,0607 }  { Ativa cursor }
             $cd/$10);          { int 10      }
end;
{----------------------------------------------------------------------}
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
{-----------------------------------------------------------------------}
Procedure ConverterB(Base : byte;var Decimal,destino : string);
var n,x,i,j,k      : real;
    y              : byte;
    aux1,aux2,aux3 : string;
    aux4           : integer;
    aux5,aux6      : string;
    code           : integer;

begin
   n:=10;
   val(decimal,n,code);
   if n>=Base then n:=100;
   x:=0;
   for y:=1 to 20 do
   begin
      if n<=0 then
      begin
         j:=0;
         n:=0;
      end else j:=ln(n)/ln(base);
      if j<0 then j:=trunc(j)-1 else j:=trunc(j);
      if j=0 then x:=0 else x:=x+exp(n*ln(j));
      n:=n-exp(j*ln(base));
   end;
   str(trunc(x),aux1);
   str((x-trunc(x)):1:9,aux2);
   if base>10 then
   begin
      i:=length(aux1); { inteiro }
      j:=length(aux2); { fra‡„o  }
      k:=1;
      aux5:='';
      repeat
         aux3:=copy(aux1,trunc(i+1),2);
         val(aux3,aux4,aux4);
         if aux4>9 then aux5:=chr(aux4+55)+aux5 else aux5:=chr(aux4+48)+aux5;
         i:=i-2;
      until i<=0;
      repeat
         aux3:=copy(aux2,trunc(i),2);
         val(aux3,aux4,aux4);
         if aux4>9 then aux6:=chr(aux4+55)+aux6 else aux6:=chr(aux4+48)+aux6;
         k:=k+1;
      until k>=j;
      destino:=aux5+'.'+aux6;
   end else destino:=aux1+'.'+aux2;
end;
{-----------------------------------------------------------------------}
Procedure Converter10(Base : byte;var origem,decimal : string);
var inteiro,fracionario : real;
    y,i                 : byte;
    intg                : boolean;
    pot,aux             : real;
begin
   inteiro:=0;
   fracionario:=0;
   intg:=true;
   y:=0;
   for i:=length(origem) downto 1 do
   begin
      if (ord(origem[(length(origem)+1)-i]) >= 48) and (ord(origem[(length(origem)+1)-i]) <= 57) then
      begin
         if intg then
         begin
            pot:=exp((i-1)*ln(base));
            inteiro:=inteiro+((ord(origem[(length(origem)+1)-i])-48)*pot);
         end;
         if not(intg) then
         begin
            y:=y+1;
            pot:=1/exp(y*ln(base));
            fracionario:=fracionario+((ord(origem[i])-48)*pot);
         end;
      end;
      if (ord(origem[(length(origem)+1)-i]) >= 65) and (ord(origem[(length(origem)+1)-i]) <= 70) then
      begin
         if intg then
         begin
            pot:=exp((i-1)*ln(base));
            inteiro:=inteiro+((ord(origem[(length(origem)+1)-i])-55)*pot);
         end;
         if not(intg) then
         begin
            y:=y+1;
            pot:=1/exp(y*ln(base));
            fracionario:=fracionario+((ord(origem[i])-55)*pot);
         end;
      end;
      if origem[i]='.' then
      begin
         intg:=false;
         y:=1;
      end;
   end;
   Aux:=inteiro+fracionario;
   str(aux:1:9,decimal);
end;
{-----------------------------------------------------------------------}
Procedure Conversao(fl,cs,ip,ax,bx,cx,dx,si,di,ds,es,bp:word);
interrupt;
var esta : pointer;
begin
   if ax<>20292 then exit;
   getintvec($15,esta);
   setintvec($15,ptr($0ba7,$01ea));
   _orig:=$b800;
   _dest:=$8800;
   MovePagina;
   textcolor(15);
   textbackground(0);
   opr[1]:=0;
   opr[2]:=0;
   opr[3]:=0;
   sinal:=#0;
   chave:='';
   tecla:=0;
   sair:=false;
   esc:=1;
   bin:='';
   oct:='';
   dcm:='';
   hex:='';
   opcoes[1]:='Binario:    ';
   opcoes[2]:='Octal:      ';
   opcoes[3]:='Decimal:    ';
   opcoes[4]:='Hexadecimal:';
   opcoes[5]:='Limpar      ';
   Cursor(true);
   Sombra(10,5,70,15);
   Retangulo(10,5,70,15);
   gotoxy(15,13);
   writeln('Aperte <CTRL> + <SHIFT> esquerdo para sair...');
   for menu:=1 to 5 do
   begin
      gotoxy(12,6+menu);
      textcolor(15);
      textbackground(0);
      writeln(opcoes[menu]);
   end;
   gotoxy(12,6+esc);
   textcolor(0);
   textbackground(7);
   writeln(opcoes[esc]);
   repeat
     gotoxy(26,7);
     textcolor(15);
     textbackground(0);
     writeln(bin,'                  ');
     gotoxy(26,8);
     writeln(oct,'                  ');
     gotoxy(26,9);
     writeln(dcm,'                  ');
     val(dcm,ltr,ltr);
     if ltr>32767 then
     begin
        gotoxy(49,9);
        writeln('-',ltr-32768:1,'           ');
     end;
     ltr:=0;
     gotoxy(26,10);
     writeln(hex,'                  ');
     gotoxy(49,10);
     writeln('Opera‡„o: ',sinal,'     ');
     if keypressed then
     begin
        for menu:=1 to 5 do
        begin
           gotoxy(12,6+menu);
           textcolor(15);
           textbackground(0);
           writeln(opcoes[menu]);
        end;
        ch:=readkey;
        if (ch='+') or (ch='-') then
        begin
           val(dcm,opr[1],opr[1]);
           sinal:=ch;
        end;
        if ch='=' then
        begin
           val(dcm,opr[2],opr[2]);
           if sinal='+' then opr[3]:=opr[1]+opr[2];
           if sinal='-' then opr[3]:=opr[1]-opr[2];
           if opr[3]>65535 then opr[3]:=opr[3]-65535;
           if opr[3]<0     then opr[3]:=opr[3]+32768;
           str(opr[3],dcm);
           converterb(8,dcm,oct);
           converterb(16,dcm,hex);
           converterb(2,dcm,bin);
{           dec_octal(oct,dcm);
           dec_hexadecimal(hex,dcm);
           dec_binario(bin,dcm); }
           for i:=1 to 3 do opr[i]:=0;
           sinal:=#0;
        end;
        if ch=#0  then
        begin
           ch:=readkey;
           if ch=#72 then esc:=esc-1;
           if ch=#80 then esc:=esc+1;
           if ch=#68 then
           begin
              dcm:='';
              oct:='';
              hex:='';
              bin:='';
              gotoxy(49,9);
              writeln('           ');
           end;
           ch:=#0;
        end;
        if esc<1 then esc:=5;
        if esc>5 then esc:=1;
        if (esc=1) and ((ch='0') or (ch='1')) then
        begin
           if length(bin) <= 15 then bin:=bin+ch;
           Converter10(2,bin,dcm);
           bin_hexadecimal(hex,bin);
           bin_octal(oct,bin);
        end;
        if (esc=2) and ((ch>='0') and (ch<='7')) then
        begin
           if length(oct) <= 5 then oct:=oct+ch;
           Converter10(8,oct,dcm);
           oct_hexadecimal(hex,oct);
           oct_binario(bin,oct);
        end;
        val(dcm,ltr,ltr);
        if (esc=3) and ((ch>='0') and (ch<='9')) then
        begin
           if ltr <= 65535 then dcm:=dcm+ch;
           converterb(8,dcm,oct);
           converterb(16,dcm,hex);
           converterb(2,dcm,bin);
{           dec_octal(oct,dcm);
           dec_hexadecimal(hex,dcm);
           dec_binario(bin,dcm); }
        end;
        ch:=upcase(ch);
        if (esc=4)and(((ch>='0')and(ch<='9'))or((ch>='A')and(ch<='F'))) then
        begin
           if length(hex) <= 3 then hex:=hex+ch;
           hex_octal(oct,hex);
           Converter10(16,hex,dcm);
           hex_binario(bin,hex);
        end;
        if (esc=5) and (ch=#13) then
        begin
           dcm:='';
           oct:='';
           hex:='';
           bin:='';
           gotoxy(49,9);
           writeln('           ');
        end;
        gotoxy(12,6+esc);
        textcolor(0);
        textbackground(7);
        writeln(opcoes[esc]);
     end;
     Aperto;
     i:=tecla;
     chave:='';
     if i>=128 then i:=i-128; { Insert ligado }
     if i>=64 then  i:=i-64;  { Caps Lock     }
     if i>=32 then  i:=i-32;  { Num Lock      }
     if i>=16 then  i:=i-16;  { Scroll Lock   }
     if i>=8 then   i:=i-8;   { Alt           }
     if i=6 then sair:=true;
     if i>=4 then   i:=i-4;   { Ctrl          }
     if i>=2 then   i:=i-2;   { Shift esq.    }
     if i>=1 then   i:=i-1;   { Shift dir.    }
  until sair;
  cursor(false);
  gotoxy(1,25);
  textcolor(7);
  textbackground(0);
  _orig:=$8800;
  _dest:=$b800;
  MovePagina;
  inline($fb); { sti }
  setintvec($15,esta);
  halt(0);
end;
{-----------------------------------------------------------------------}
begin
   getintvec($15,SaveIntr);
   setintvec($15,@Conversao);
   writeln;
   writeln('Aperte <F10> para acionar a calculadora...');
   writeln;
   repeat
      write('.');
   until keypressed;
{   keep(768); }
end.


