Unit Math;

interface

Function Sign(x:real):real;
Function Tan(x:real):real;
Function Cot(x:real):real;
Function Pot(x,y:real):real;
Function Log(x:real):real;
Function Sec(x:real):real;
Function Csc(x:real):real;
Function Asin(x:real):real;
Function Acos(x:real):real;
Function Asec(x:real):real;
Function Acsc(x:real):real;
Function Acot(x:real):real;
Function Sinh(x:real):real;
Function Cosh(x:real):real;
Function Tanh(x:real):real;
Function Asinh(x:real):real;
Function Acosh(x:real):real;
Function Asech(x:real):real;
Function Acsch(x:real):real;
Function Acoth(x:real):real;
Function Atanh(x:real):real;

implementation

Function Sign;
begin
   if x>0 then Sign:=1;
   if x<0 then Sign:=-1;
   if x=0 then Sign:=0;
end;

Function Tan;
begin
   Tan:=sin(x)/cos(x);
end;

Function Cot;
begin
   Cot:=1/Tan(x);
end;

Function Pot;
begin
   if y>0 then pot:=exp(y*ln(x)) else pot:=1;
end;

Function Log;
begin
   Log:=ln(x)/ln(10);
end;

Function Sec;
begin
   sec:=1/cos(x);
end;

Function Csc;
begin
   csc:=1/sin(x);
end;

Function Asin;
begin
   asin:=arctan(x/sqrt(1-x*x));
end;

Function Acos;
begin
   acos:= (pi/2)-arctan(x/sqr(1-x*x));
end;

Function Asec;
var verd : integer;
begin
   if x<0 then verd:=1 else verd:=0;
   asec:=arctan(sqrt(x*x-1))+(verd)*pi;
end;

Function Acsc;
var verd : integer;
begin
   if x<0 then verd:=1 else verd:=0;
   acsc:=arctan(1/sqrt(x*x-1))+(verd)*pi;
end;

Function Acot;
begin
   acot:=(pi/2)-arctan(x);
end;

Function Sinh;
begin
   sinh:=(exp(x)-exp(-x))/2;
end;

Function Cosh;
begin
   cosh:=(exp(x)+exp(-x))/2;
end;

Function Tanh;
begin
   tanh:=sinh(x)/cosh(x);
end;

Function Asinh;
begin
   asinh:=ln(x+sqrt(x*x+1));
end;

Function Acosh;
begin
   acosh:=ln(x+sqrt(x*x-1));
end;

Function Asech;
begin
   asech:=ln((1+sqrt(1-x*x))/x);
end;

Function Acsch;
begin
   acsch:=ln((1+sign(x)*sqr(1+x*x))/x);
end;

Function Atanh;
begin
   atanh:=ln((1+x)/(1-x))/2;
end;

Function Acoth;
begin
   acoth:=ln((x+1)/(x-1))/2;
end;

begin
end.