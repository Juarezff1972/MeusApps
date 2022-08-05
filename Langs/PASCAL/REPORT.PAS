uses dos;
var arq : text;
	nome : string;
	s    : searchrec;
	dd,mm,aa,hh,mn,ss,cc,sm : word;
begin
	nome:=paramstr(1);
	getdate(aa,mm,dd,sm);
	gettime(hh,mn,ss,cc);
	findfirst(nome,anyfile,s);
	assign(arq,nome);
	if doserror<>0 then
	begin
		rewrite(arq);
		writeln(arq,'Arquivo criado em: ',dd,'/',mm,'/',aa,'.');
		close(arq);
	end;
	append(arq);
	write(arq,'Data: ',dd,'/',mm,'/',aa,' ');write(arq,'         ');
	writeln(arq,'Hora: ',hh,':',mn,':',ss,' ');
	close(arq);
end.