{ Consumo - Volvo - Data: 17 de Mar‡o de 1991 }
{                                             }
{  Vers„o IBM-PC por Juarez Fioravanti Filho  }
{                                             }
{ Vers„o Apple original criado em 1985        }
{ por Juarez Fioravanti                       }

Program Consumo;

Uses DOS,CRT;

var n,p,i,y,nt     : integer;                     vq,f,fa : real;
    m,cm,u,vn,pbt  : real;
    gb             : array[1..200] of string[4];
    dd,mm,yy,ds,TI : word;
    TS,TS1,TS2     : string[63];
    VS,MTS,ESS     : string;
    a,r,b,cfm,rdm  : real;
    d,s,t,vm,rr    : array[1..200] of real;
    rq,f1,f2,f3,fm : real;
    pps            : array[1..200] of char;


{-------------------------------------------------------------------------}
Procedure Torque101F;
begin
   if n>1900 then m:=107;
   if (n<=1900) and (n>1800) then m:=112;
   if (n<=1800) and (n>1700) then m:=115;
   if (n<=1700) and (n>1600) then m:=117.5;
   if (n<=1600) and (n>1500) then m:=120;
   if (n<=1500) and (n>1400) then m:=123;
   if (n<=1400) and (n>1300) then m:=125;
   if n<1300 then m:=117;
end;
{-------------------------------------------------------------------------}
Procedure Motor101F;
label prossegue1,prossegue2,prossegue3,
      prossegue4,prossegue5,prossegue6;
begin
   if n>1900 then goto prossegue1;
   if (n<=1900) and (n>1800) then goto prossegue1;
   if (n<=1800) and (n>1700) then goto prossegue2;
   if (n<=1700) and (n>1600) then goto prossegue3;
   if (n<=1600) and (n>1500) then goto prossegue4;
   if (n<=1500) and (n>1400) then goto prossegue5;
   if (n<=1400) and (n>1300) then goto prossegue6;
   if n<1300 then goto prossegue6;
prossegue1:
   if  p>=185 then cm:=40.5;
   if (p<185) and (p>=170) then cm:=40  ;
   if (p<170) and (p>=160) then cm:=39.5;
   if (p<160) and (p>=145) then cm:=39  ;
   if (p<145) and (p>=135) then cm:=38.5;
   if (p<135) and (p>=125) then cm:=38  ;
   if (p<125) and (p>=115) then cm:=37.5;
   if (p<115) and (p>=105) then cm:=37  ;
   if  p<105 then cm:=36;
   exit;
prossegue2:
   if  p>=280 then cm:=41;
   if (p<280) and (p>=250) then cm:=41.3;
   if (p<250) and (p>=235) then cm:=41.2;
   if (p<235) and (p>=185) then cm:=41  ;
   if (p<185) and (p>=170) then cm:=40.7;
   if (p<170) and (p>=155) then cm:=40.3;
   if (p<155) and (p>=140) then cm:=40  ;
   if (p<140) and (p>=130) then cm:=39.3;
   if (p<130) and (p>=120) then cm:=38.5;
   if (p<120) and (p>=100) then cm:=37.5;
   if (p<100) and (p>=80)  then cm:=36.5;
   if  p<80 then cm:=34.5;
   exit;
prossegue3:
   if  p>=265 then cm:=41.5;
   if (p<265) and (p>=200) then cm:=41.7;
   if (p<200) and (p>=175) then cm:=41.5;
   if (p<175) and (p>=165) then cm:=41  ;
   if (p<160) and (p>=140) then cm:=40.5;
   if (p<140) and (p>=135) then cm:=40  ;
   if (p<130) and (p>=120) then cm:=39.5;
   if (p<120) and (p>=110) then cm:=39  ;
   if (p<110) and (p>=100) then cm:=38.2;
   if (p<100) and (p>=80)  then cm:=37  ;
   if  p<80 then cm:=35.5;
   exit;
prossegue4:
   if (p<300) and (p>=280) then cm:=41.7;
   if (p<280) and (p>=240) then cm:=42  ;
   if (p<240) and (p>=190) then cm:=42.3;
   if (p<190) and (p>=175) then cm:=42  ;
   if (p<175) and (p>=160) then cm:=41.5;
   if (p<160) and (p>=140) then cm:=41  ;
   if (p<140) and (p>=125) then cm:=40.5;
   if (p<125) and (p>=115) then cm:=40  ;
   if (p<115) and (p>=100) then cm:=39  ;
   if (p<100) and (p>=80)  then cm:=38  ;
   if  p<80 then cm:=36;
   exit;
prossegue5:
   if  p>=248 then cm:=42.2;
   if (p<248) and (p>=235) then cm:=42.4;
   if (p<235) and (p>=200) then cm:=42.6;
   if (p<200) and (p>=165) then cm:=42  ;
   if (p<165) and (p>=155) then cm:=41.7;
   if (p<155) and (p>=140) then cm:=41.4;
   if (p<140) and (p>=130) then cm:=41  ;
   if (p<130) and (p>=110) then cm:=40  ;
   if (p<110) and (p>=100) then cm:=39  ;
   if  p<100 then cm:=38;
   exit;
prossegue6:
   if  p>=140 then cm:=42;
   if (p<140) and (p>=120) then cm:=41.5;
   if (p<120) and (p>=110) then cm:=41  ;
   if (p<110) and (p>=100) then cm:=40.5;
   if (p<100) and (p>=80)  then cm:=40  ;
   if  p<80 then cm:=39;
end;
{-------------------------------------------------------------------------}
Procedure CaixaS130;
begin
   if y > 16 then y:=16;
   if y < 1 then  y:=1;
   if y = 16 then begin u:=1    ; gb[i]:='8H'; end;
   if y = 15 then begin u:=1.18 ; gb[i]:='8L'; end;
   if y = 14 then begin u:=1.41 ; gb[i]:='7H'; end;
   if y = 13 then begin u:=1.65 ; gb[i]:='7L'; end;
   if y = 12 then begin u:=1.96 ; gb[i]:='6H'; end;
   if y = 11 then begin u:=2.31 ; gb[i]:='6L'; end;
   if y = 10 then begin u:=2.86 ; gb[i]:='5H'; end;
   if y = 9  then begin u:=3.36 ; gb[i]:='5L'; end;
   if y = 8  then begin u:=4.07 ; gb[i]:='4H'; end;
   if y = 7  then begin u:=4.79 ; gb[i]:='4L'; end;
   if y = 6  then begin u:=5.73 ; gb[i]:='3H'; end;
   if y = 5  then begin u:=6.73 ; gb[i]:='3L'; end;
   if y = 4  then begin u:=8    ; gb[i]:='2H'; end;
   if y = 3  then begin u:=9.4  ; gb[i]:='2L'; end;
   if y = 2  then begin u:=11.64; gb[i]:='1H'; end;
   if y = 1  then begin u:=13.68; gb[i]:='1L'; end;
end;
{-------------------------------------------------------------------------}
Procedure Linha;
var ch : char;
    i  : byte;
begin
   ch:='Ä';
   for i:=1 to 79 do write(ch);
   writeln;
end;
{-------------------------------------------------------------------------}
Procedure DadosDoVeiculo;
var sn : char;
begin
   repeat
      clrscr;
      gotoxy(TI,1);
      writeln(TS);
      Linha;
      gotoxy(20,4);
      writeln('Entrada de Dados');
      gotoxy(21,6);
      write('Ve¡culo ..........: '); readln(VS); writeln; writeln;
      write('Motor ( Entre c/ TD101G, THD100ED, etc. )...........: '); readln(MTS); writeln;
      write('          Area frontal (mý) ....: '); readln(a); writeln;
      write('          Raio do pneu  (m) ....: '); readln(r); writeln;
      write('          Relac. diferencial  ..: '); readln(b); writeln;
      write('          Coef. de forma .......: '); readln(cfm); writeln;
      write('          Rend. Mecƒnico .......: '); readln(rdm); writeln;
      gotoxy(10,24);
      write('Quer corrigir alguma coisa? ');
      readln(sn);
   until (sn<>'s') or (sn<>'S');
end;
{-------------------------------------------------------------------------}
Procedure DadosDoPercurso;
var sn : char;
    i  : integer;
begin
   repeat
      clrscr;
      gotoxy(TI,1);
      writeln(TS);
      Linha;
      gotoxy(20,4);
      writeln('Dados do Percurso de Opera‡„o');
      gotoxy(1,6);
      write('          Estrada ...: '); readln(ESS); writeln;
      write('          Quantos trechos ..................: '); readln(nt); writeln;
      write('          Defina velocidade m xima (km/h) ..: '); readln(vn); writeln;
      write('          P. B. T. C. (ton) ................: '); readln(pbt); writeln;
      for i:=1 to nt do
      begin
         gotoxy(1,14+i); write(' Trecho # ',I);
         gotoxy(18,14+i); write('Dist. (km) : '); readln(d[i]);
         gotoxy(40,14+i); write('Rampa (%)  : '); readln(s[i]);
         gotoxy(59,14+i); write('Parada? (P): '); readln(pps[i]);
         t[i]:=pbt;
         vm[i]:=vn;
      end;
      writeln;
      writeln;
      write('          Resist. rolamento (kg/ton)  ......: '); readln(rq); writeln;
      for i:=1 to nt do rr[i]:=rq;
      gotoxy(10,24);
      write('Quer corrigir alguma coisa? ');
      readln(sn);
   until (sn<>'s') or (sn<>'S');
end;
{-------------------------------------------------------------------------}
Procedure TorqueIndiceCM;
begin
   if (mts='td101f') or (mts='TD101F') then motor101f;
end;
{-------------------------------------------------------------------------}
Procedure CalculoForcas;
begin
   f1:=rr[i]*t[i];
   f2:=cfm*a*vq/210;
   f3:=10*s[i]*t[i];
   fm:=rdm*m*b*u/r;
   f:=f1+f2+f3;
   fa:=fm-f;
end;
{-------------------------------------------------------------------------}
Procedure DefineCaixa;
begin
   if ((tms='6S130') or (tms='6S160')) or ((tms='6s130') or (tms='6s160')) then CaixaS130;
end;
{-------------------------------------------------------------------------}
Procedure Abertura;
begin
   TS:='Engenharia de Vendas   -   Estimativa do Servi‡o de Transporte ';
   TI:=(80-length(TS)) div 2;
   TS1:='Eng. Juarez Fioravanti';
   TS2:='Data: ';
   getdate(yy,mm,dd,ds);
   textcolor(0);
   textbackground(7);
   clrscr;
   gotoxy(TI,10);
   writeln(TS);
   gotoxy(40,16);
   writeln('Programado por');
   gotoxy(40,18);
   writeln(TS1);
   gotoxy(40,20);
   writeln('Dezembro de 1985');
   gotoxy(10,24);
   writeln(TS2,dd,'/',mm,'/',yy);
   textcolor(7);
   textbackground(0);
   repeat until keypressed;
end;
{-------------------------------------------------------------------------}

Procedure MenuPrincipal;
var z : char;
begin
   repeat
   clrscr;
   gotoxy(TI,1);
   writeln(TS);
   Linha;
   gotoxy(33,5);   writeln('Menu Principal');
   gotoxy(25,7);   writeln('1.  Dados do ve¡culo');
   gotoxy(25,8);   writeln('2.  Dados do percurso');
   gotoxy(25,9);   writeln('3.  Processar dados');
   gotoxy(25,10);  writeln('4.  Listar resultados');
   gotoxy(25,11);  writeln('5.  Imprimir resultados');
   gotoxy(25,12);  writeln('6.  Arquivo de percurso');
   gotoxy(25,13);  writeln('7.  Corrigir/alterar dados gerais');
   gotoxy(25,14);  writeln('8.  Fim do processamento');
   gotoxy(1,23);
   textcolor(0);
   textbackground(7);
   writeln('Mem¢ria livre: ',memavail,' bytes');
   textcolor(7);
   textbackground(0);
   repeat
      gotoxy(45,23);
      write('Qual a sua escolha? ');
      z:=readkey;
   until ((z>='0') and (z<='8'));
   case (ord(z)-48) of
      1 : DadosDoVeiculo;
      2 : DadosDoPercurso;
{      3 : TorqueIndiceCM;
      4 : Resultados;
      5 : ImprimirResultados;
      6 : GravarCarregarPercurso;
      7 : CorrigirPercurso;}
      8 : Exit;
   end;
   until false;
end;
{-------------------------------------------------------------------------}

begin
   Abertura;
   MenuPrincipal;
end.