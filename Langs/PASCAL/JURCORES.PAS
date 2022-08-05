Program Configura_Cores;
uses dos,crt,cpjtools,drivers;
var c : char;
	 i : byte;
begin
	LerTabelaASCII;  { <----- A ser apagada! }

	initevents;
	hidemouse;
	cursor(true,6400);
	cores(black,white);
	sombra(2,2,12,20,8);
	retangulo(2,2,12,20);
	for i:=0 to 15 do
	begin
		gotoxy(5,3+i);cores(0,i);write('ллл');
	end;
	gotoxy(4,3);
	asm
		mov ah,9;mov al,$10;xor bh,bh;mov bl,7;mov cx,1;int $10
	end;
	gotoxy(8,3);
	asm
		mov ah,9;mov al,$11;xor bh,bh;mov bl,7;mov cx,1;int $10
	end;
	c:=#2;
	repeat
		if keypressed then c:=readkey;
		relogio;
	until (c=#27);

	cursor(false,6400);
	doneevents;
end.