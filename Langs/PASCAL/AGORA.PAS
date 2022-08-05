program clocking;

uses crt,dos;

var
    h,m,s,c,d,n,y,w : word;

begin

getdate(y,n,d,w);
gettime(h,m,s,c);
writeln('Agora s„o ',h:1,':',m:1,':',s:2,'.');
writeln;
write('Hoje ‚ ',d:1,' de ');
if n=1 then write('janeiro');
if n=2 then write('fevereiro');
if n=3 then write('mar‡o');
if n=4 then write('abril');
if n=5 then write('maio');
if n=6 then write('junho');
if n=7 then write('julho');
if n=8 then write('agosto');
if n=9 then write('setembro');
if n=10 then write('outubro');
if n=11 then write('novembro');
if n=12 then write('dezembro');
write(' de ',y:1,', um');
if w=7 then writeln(' domingo.');
if w=1 then writeln('a segunda.');
if w=2 then writeln('a ter‡a.');
if w=3 then writeln('a quarta.');
if w=4 then writeln('a quinta.');
if w=5 then writeln('a sexta.');
if w=6 then writeln(' s bado.');
if w=0 then writeln(' domingo.');

end.