uses dos;
var senha : array[1..3] of string[20];
	 coded : array[1..3] of string[20];
	 i     : word;
	 linha : string;
	 ent,sai: file of char;
	 x      : char;
{---------------------------------------------------------------}
Procedure checksum;
var x,i : word;
	 arq : file of byte;
	 y   : byte;
begin
	x:=0;
	assign(arq,'senha.dat');
	reset(arq);
	for i:=1 to 524 do
	begin
		read(arq,y);
		x:=x+y;
	end;
	if x<>48948 then
	begin
		writeln;
		writeln('SENHA.DAT foi adulterado! N„o posso prosseguir!');
		writeln;
		halt(1);
	end;
end;
{---------------------------------------------------------------}
begin
	checksum;
	Writeln;
	write('Entre com a nova senha:');			readln(senha[1]);
	write('Entre com a senha alternativa:');	readln(senha[2]);
	senha[3]:='Ababadar';
	for i:=1 to 20-length(senha[1]) do senha[1]:=senha[1]+' ';
	for i:=1 to 20-length(senha[2]) do senha[2]:=senha[2]+' ';
	for i:=1 to 20-length(senha[3]) do senha[3]:=senha[3]+' ';
	writeln;
	coded[1]:='';	coded[2]:='';	coded[3]:='';
	for i:=1 to 20 do coded[1]:=coded[1]+chr(ord(senha[1,i])+1);
	for i:=1 to 20 do coded[2]:=coded[2]+chr(ord(senha[2,i])+1);
	for i:=1 to 20 do coded[3]:=coded[3]+chr(ord(senha[3,i])+1);
	writeln('Senha alterada.');
	writeln;
	assign(ent,'senha.dat');
	assign(sai,'senha.sys');
	reset(ent);
	rewrite(sai);
	for i:=0 to 524 do
	begin
		if (i<128) or (i>188) then
		begin
			read(ent,x);
			write(sai,x);
		end;
		if (i>127) and (i<148) then
		begin
			read(ent,x);
			write(sai,coded[1,i-127]);
		end;
		if (i>147) and (i<168) then
		begin
			read(ent,x);
			write(sai,coded[2,i-147]);
		end;
		if (i>167) and (i<188) then
		begin
			read(ent,x);
			write(sai,coded[3,i-167]);
		end;
	end;
	close(ent);
	close(sai);
end.
