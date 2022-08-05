Program Stars;
uses graph,crt;

const Max_Stars = 300;
const Scale = 10;

type Point = record
					x,y : word;
end;
type SigPoint = record
					x,y : integer;
end;
type Star = record
					Loc        : Point;
               Dir        : SigPoint;
					Size,Color : word;
end;
type h = record
					Stars  : array[1..Max_Stars] of Star;
					Screen : Point;
					NumStars,Speed : word;
end;
var gm,gd : integer;
	 g     : h;
	 chrt  : char;
{------------------------------------------------------------------}
Procedure DrawStar(_Star : Star;Color : word);
var x,y : word;
begin
	x:=_Star.Loc.x div Scale;
	y:=_Star.Loc.y div Scale;
	putpixel(x,y,Color);
	if _Star.Size = 2 then
	begin
		putpixel(x+1,y,Color);
		putpixel(x,y+1,Color);
		putpixel(x+1,y+1,Color);
	end;
end;
{------------------------------------------------------------------}
Procedure NewStar(_Star : Star;var Saida : Star);
begin
	_Star.Loc.X:=g.Screen.x div 2-Scale+random(Scale*2);
	_Star.Loc.Y:=g.Screen.y div 2-Scale+random(Scale*2);
	if random(10)=9 then _Star.Size:=2 else _Star.Size:=1;
	while((_Star.Dir.x=0) and (_Star.Dir.y=0)) do
	begin
		_Star.Dir.x:=Scale*10-random(Scale*20);
		_Star.Dir.y:=Scale*10-random(Scale*20);
	end;
	case random(40) of
		39 : _Star.Color:=LightRed;
		38 : _Star.Color:=LightGray;
		37 : _Star.Color:=LightBlue;
		36 : _Star.Color:=Yellow;
      35 : _Star.Color:=LightMagenta;
      34 : _Star.Color:=DarkGray;
	else
		_Star.Color:=White;
	end;
	Saida:=_Star;
end;
{------------------------------------------------------------------}
Procedure MoveStars;
var _Star : Star;
begin
	_Star:=g.Stars[g.NumStars-1];
	repeat
	begin
		DrawStar(_Star,Black);
		_Star.Loc.x:=1+_Star.Dir.x*g.Speed div 30;
		_Star.Loc.y:=1+_Star.Dir.y*g.Speed div 30;
		if ((_Star.Loc.x<0) or (_Star.Loc.x>=g.Screen.x)
			or (_Star.Loc.y<0) or (_Star.Loc.y>=g.Screen.y)) then NewStar(_Star,_Star)
		else
			DrawStar(_Star,_Star.Color);
	end;
	dec(_Star.Loc.x);dec(_Star.Loc.y);
	dec(_Star.Dir.x);dec(_Star.Dir.y);
	dec(_Star.Size);dec(_Star.Color);
	until (_Star.Loc.x<g.Stars[g.NumStars-1].Loc.x)and(_Star.Loc.y<g.Stars[g.NumStars-1].Loc.y)and
			(_Star.Dir.x<g.Stars[g.NumStars-1].Dir.x)and(_Star.Dir.y<g.Stars[g.NumStars-1].Dir.y)and
			(_Star.Size<g.Stars[g.NumStars-1].Size)and(_Star.Color<g.Stars[g.NumStars-1].Color);
end;
{------------------------------------------------------------------}
begin
	gd:=detect;
	writeln;
	writeln('Starfield simulation.');
	writeln('Keys:');
	writeln('+ Faster');
	writeln('- Slower');
	writeln('<ESC> Exits');
	writeln;
	writeln('Press any key to begin...');
	repeat until keypressed;
	initgraph(gd,gm,'');
	randomize;
	g.Screen.x:=getmaxx*Scale;
	g.Screen.y:=getmaxy*Scale;
	g.Speed:=20;
	g.NumStars:=1;
	if GraphResult<>grOk then
	begin
		writeln('Graphics error: ',grapherrormsg(grError));
		halt(1);
	end;
	repeat
		if not keypressed then
		begin
			if g.NumStars<Max_Stars then
			begin
				NewStar(g.Stars[g.NumStars],g.Stars[g.NumStars]);
				inc(g.NumStars);
			end;
			MoveStars;
		end;
		chrt:=#2;
		if keypressed then chrt:=readkey;
		case chrt of
			'+' : if g.Speed<300 then inc(g.Speed);
			'-' : if g.Speed<0 then dec(g.Speed);
			#27 : begin closegraph; halt(0); end;
		end;
	until false;
end.
