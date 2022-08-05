Program Funcao;

Uses CRT,ATI;

var

 A             : integer;
 B,C,D,E,F,G   : real;    { Constants defined }
 n1,n2         : real;
 H,BB          : integer; { Loop Variables }
 AA {,BB}      : integer; { Dependent on H }
 CC,D1,DD      : real;    { Dependent on H and BB }
 X,Y           : real;
 {X,}X1,{Y,}Y1 : longint; { Plotting variables }
 {  Y,Y1       : integer;}{ Dependent on DD and H }
{-------------------------------------------------------------------}
Procedure Fundos;
begin
   x:=bb+(h/b)+e;
   y:=dd-(h/b)+f;
   x1:=trunc(0.85*x);
   y1:=trunc(0.9 *(g-y));
	if ((y1<0) or (y1>190)) then exit;
	plot(2*x1,trunc(y1*maxy/200),trunc(2*d1)+60);
	draw(2*x1,1+trunc(y1*maxy/200),2*x1,30+trunc(y1*maxy/200),black);
{	plot(2*x1,30+trunc(y1*maxy/200),trunc(2*d1)+60);}
end;
{-------------------------------------------------------------------------}
Procedure Criar;
{*************************************************************************}
Function Funcao : real;
begin
{	Funcao:=5*exp(-cc)*sin(2*3.14159266*cc);}
{	Funcao:=cos(2*cc)+cos(5*cc);}
{	Funcao:=cos(2*cc)+sin(5*cc);}
{	Funcao:=sin(2*cc)+cos(5*cc);}
	Funcao:=sin(2*cc)+sin(5*cc);
end;
{*************************************************************************}
begin
   {cleardevice;}
   h:=-a;
   repeat
      aa:=trunc(0.5+sqrt(a*a-h*h));
		for bb:=-aa to aa do
		begin
			cc:=sqrt(bb*bb+h*h)*d;
			d1:=Funcao;
			dd:=d1*c;
			Fundos;
		end;
		h:=h+trunc(b);
   until (h=a);
end;
{-------------------------------------------------------------------------}
begin
   clrscr;
	writeln;
   writeln;
   writeln('Para expandir ou comprimir o desenho verticalmente, entre um numero');
   write  ('entre -40 e 40 (20 e'' tipico).'); readln(n1);
   writeln;
   writeln;
   writeln('Para mover o desenho para cima ou para baixo na tela, entre um numero');
	write  ('entre -50 e 150 (90 e'' tipico).'); readln(n2);
	directvideo:=false;
   a:=144;
   b:=2.25;
   c:=n1;
   d:=0.0327;
   e:=160;
   f:=n2;
	g:=199;
   SetGraphicsMode(800,600);
	Criar;
	readln;
   SetMode(3);
end.


