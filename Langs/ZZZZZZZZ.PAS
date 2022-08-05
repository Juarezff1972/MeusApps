uses crt;
var i,a : byte;
begin
	for i:=0 to 255 do
	begin
		textattr:=i;
		write('°±²Û');
	end;
	writeln;
	for i:=0 to 255 do
	begin
		gotoxy(1,1);textattr:=lightcyan;write('Valor: ',i,'   ');
		asm
			push dx
			push ax
			mov dx,$3cf
			mov al,i
			out dx,al
			mov a,al
			pop ax
			pop dx
		end;
		gotoxy(1,2);write('No.: ',a,'   ');
		delay(500);
	end;
	asm
		push dx
		push ax
		mov dx,$3cf
		mov al,16
		out dx,al
		pop ax
		pop dx
	end;
end.