Uses crt,printer;

var x,y,r,ry,xx,yy,f,d1 : real;
    d                   : integer;

label 200,350,490;

begin
clrscr;
X:=80;Y:=70;
RY:=70;F:=1.1;
writeln(lst, 'MA',trunc(2500*(2*X)/640),',',trunc(1800*Y/200),';');
d1:=0;
while(d1 < 1) do begin
d1:=d1+0.05;
for D := 0 to 359 do begin
r:=d*pi/180;
XX:=RY*cos(R);
YY:=RY*sin(R);
YY:=-YY/F;
writeln(lst, 'DA',trunc(2500*(2*(X+XX)+4*(70-RY))/640),',',trunc(1800*(Y+YY+((2*(70-RY))/3))/200),';');
RY:=RY-(exp(-D1*PI/180)*sin(2*PI*(D1*PI/180)));
if RY <= 1 then goto 200;
end;
end;
200: writeln('Troque a pena...');
readln;
X:=80;Y:=70;
RY:=70;F:=1.1;
writeln(lst, 'MA',trunc(2500*(2*(70+X))/640),',',trunc(1800*Y/200),';');
d1:=0;
while(d1 < 1) do begin
d1:=d1+0.05;
for D := 0 to 359 do begin
r:=d*pi/180;
XX:=RY*cos(R);
YY:=RY*sin(R);
YY:=-YY/F;
writeln(lst, 'DA',trunc(2500*(2*(70+X)+4*(70-RY)+10)/640),',',trunc(1800*(Y+YY+((2*(70-RY))/3))/200),';');
RY:=RY-(exp(-D1*PI/180)*sin(2*PI*(D1*PI/180)));
if RY <= 1 then goto 350;
end;
end;
350: X:=80;Y:=70;
RY:=70;F:=1.1;
writeln(lst, 'MA',trunc(2500*(2*X)/640),',',trunc(1800*(5+(70+Y))/200),';');
d1:=0;
while(d1 < 1) do begin
d1:=d1+0.05;
for D := 0 to 359 do begin
r:=d*pi/180;
XX:=RY*cos(R);
YY:=RY*sin(R);
YY:=-YY/F;
writeln(lst, 'DA',trunc(2500*(2*(X+XX)+4*(70-RY))/640),',',trunc(1800*(5+(70+Y)+((2*(70-RY))/3))/200),';');
RY:=RY-(exp(-D1*PI/180)*sin(2*PI*(D1*PI/180)));
if RY <= 1 then goto 490;
end;
end;
490:X:=80;Y:=70;
RY:=70;F:=1.1;
writeln(lst, 'MA',trunc(2500*(2*X)/640),',',trunc(1800*Y/200),';');
d1:=0;
while(d1 < 1) do begin
d1:=d1+0.05;
for D := 0 to 359 do begin
r:=d*pi/180;
XX:=RY*cos(R);
YY:=RY*sin(R);
YY:=-YY/F;
writeln(lst, 'DA',trunc(2500*(2*(X+XX))/640),',',trunc(1800*(Y+YY)/200),';');
RY:=RY-(exp(-D1*PI/180)*sin(2*PI*(D1*PI/180)));
if RY <= 1 then exit;
end;
end;
writeln(lst,'PU;');
end.