Program Sist3Eq;
type matriz = array[1..4,1..4] of real;
const n = 3;
var a   : matriz;
	 x,y : byte;
	 vai : boolean;
	 rsp : boolean;
{-------------------------------------------------------}
Procedure LerMatriz(y,x : byte);
var c,l : string;
begin
	case x of
		1 : l:='PRIMEIRO coeficiente ';
		2 : l:='SEGUNDO coeficiente ';
		3 : l:='TERCEIRO coeficiente ';
		4 : l:='TERMO INDEPENDENTE ';
	end;
	case y of
		1 : c:=' PRIMEIRA';
		2 : c:=' SEGUNDA';
		3 : c:=' TERCEIRA';
	end;
	write('Entre com o '+l+'da'+c+' linha:');
	readln(a[y,x]);
end;
{-------------------------------------------------------}
Procedure Det;
var x,z     : real;
	 t,i,s,g : byte;
	 c       : string;
begin
	for t:=1 to n do
	begin
		if (a[t,1]=0) and (a[t,2]=0) and (a[t,3]=0) and (t<>n) then
		begin
			i:=0;
			repeat
				inc(i);
				while (a[3,1]<>0)and(a[3,2]<>0)and(a[3,3]<>0) do
				for s:=1 to n+1 do
				begin
					x:=a[t,s];
					a[t,s]:=a[4-i,s];
					a[4-i,s]:=x;
				end;
			until (a[3,1]=0)and(a[3,2]=0)and(a[3,3]=0);
		end;
	end;
	rsp:=true;
	for t:=1 to n do
	begin
		vai:=false;
		for i:=1 to n do if a[t,i]<>0 then
		begin
			if not(vai) then g:=i;
			vai:=true;
		end;
		if vai then
		begin
			while (a[t,t]=0)and(a[t,t-1]<>0) do
			for s:=1 to n+1 do
			begin
				x:=a[t,s];
				a[t,s]:=a[g,s];
				a[g,s]:=x;
			end;
			if a[t,t]=0 then z:=1/a[t,t+1] else	z:=1/a[t,t];
			for s:=1 to n+1 do a[t,s]:=z*a[t,s];
			for i:=1 to n do
			begin
				if i<>t then
				begin
					z:=-a[i,t];
					for s:=1 to n+1 do a[i,s]:=a[i,s]+z*a[t,s];
				end;
			end;
		end;
		if not(vai) then
		begin
			writeln;
			c:='O sistema ‚ poss¡vel e indeterminado.';
			for i:=1 to 3 do
				if (a[i,1]=0) and (a[i,2]=0)
				and (a[i,3]=0) and (a[i,4]<>0) then c:='O sistema ‚ imposs¡vel.';
			if (a[3,1]=0) and (a[3,2]=0)
			and (a[3,3]=0) and (a[3,4]=0) then
			begin
				for i:=1 to 2 do
				if (a[i,1]=1) and (a[i,2]=0) and (a[i,3]=0) then
					writeln('X[1] = ',a[i,4]:1:5);
				for i:=1 to 2 do
				if (a[i,1]=0) and (a[i,2]=1) and (a[i,3]=0) then
					writeln('X[2] = ',a[i,4]:1:5);
				for i:=1 to 2 do
				if (a[i,1]=0) and (a[i,2]=0) and (a[i,3]=1) then
					writeln('X[3] = ',a[i,4]:1:5);
				c:='';
			end;
			writeln(c);
			rsp:=false;
		end;
	end;
end;
{-------------------------------------------------------}
Procedure Resposta;
var i : byte;
begin
	writeln;
	for i:=1 to n do
	begin
		writeln('X[',i,'] = ',a[i,n+1]:1:5);
	end;
end;
{-------------------------------------------------------}
begin
	rsp:=true;
	writeln;
	for x:=1 to n do
	begin
		for y:=1 to n+1 do LerMatriz(x,y);
		writeln;
	end;
	if rsp then Det;
	if rsp then Resposta;
end.