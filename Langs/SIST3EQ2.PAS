Program Sist3Eq;
var a          : array[1..4,1..4] of real;
	 d,dx,dy,dz : real;
	 x,y,z      : real;
{----------------------------------------------------------------------}
Procedure LerMatriz;
var q,w : byte;
begin
	writeln;
	for q:=1 to 3 do
	for w:=1 to 4 do
	begin
		write('Entre com o valor ',w,' da linha ',q,':');
		readln(a[q,w]);
	end;
end;
{----------------------------------------------------------------------}
Procedure CalculaDets;
begin
	d:=a[1,1]*a[2,2]*a[3,3]+a[2,1]*a[3,2]*a[1,3]+a[3,1]*a[1,2]*a[2,3]-
		a[3,1]*a[2,2]*a[1,3]-a[3,2]*a[2,3]*a[1,1]-a[3,3]*a[2,1]*a[1,2];
	dx:=a[1,4]*a[2,2]*a[3,3]+a[2,4]*a[3,2]*a[1,3]+a[3,4]*a[1,2]*a[2,3]-
		 a[3,4]*a[2,2]*a[1,3]-a[3,2]*a[2,3]*a[1,4]-a[3,3]*a[2,4]*a[1,2];
	dy:=a[1,1]*a[2,4]*a[3,3]+a[2,1]*a[3,4]*a[1,3]+a[3,1]*a[1,4]*a[2,3]-
		 a[3,1]*a[2,4]*a[1,3]-a[3,4]*a[2,3]*a[1,1]-a[3,3]*a[2,1]*a[1,4];
	dz:=a[1,1]*a[2,2]*a[3,4]+a[2,1]*a[3,2]*a[1,4]+a[3,1]*a[1,2]*a[2,4]-
		 a[3,1]*a[2,2]*a[1,4]-a[3,2]*a[2,4]*a[1,1]-a[3,4]*a[2,1]*a[1,2];
end;
{----------------------------------------------------------------------}
begin
	LerMatriz;
	CalculaDets;
	if d=0 then writeln('Sistema Imposs¡vel ou Poss¡vel Indeterminado.');
	if (d<>0) then
	begin
		x:=dx/d;
		y:=dy/d;
		z:=dz/d;
		writeln;
		writeln('X = ',x:1:3);
		writeln('Y = ',y:1:3);
		writelN('Z = ',z:1:3);
	end;
end.
