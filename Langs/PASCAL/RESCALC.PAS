{$M 8192,0,65530}
uses dos,crt;
var abcd : pointer;
    efgh : pointer;
    comd : string[127];
    parm : string[127];

procedure ativar;
interrupt;
begin
   comd:='c:\msdos\mem';
   parm:='/c | more';
   writeln;
   writeln('Mem¢ria livre: ',MemAvail,' bytes');
   writeln('Mem¢ria m xima: ',MaxAvail,' bytes');
   writeln;
   swapvectors;

{    getintvec($5,efgh);
     setintvec($5,@ativar);

      getintvec($1b,efgh);
       setintvec($1b,SaveInt1B);}

        exec(GetEnv('COMSPEC'),'/C '+comd+' '+parm);

{       getintvec($1b,efgh);
      setintvec($1b,SaveInt1B);

     getintvec($5,efgh);
    setintvec($5,@ativar);}

   swapvectors;
   writeln;
   writeln('Mem¢ria livre: ',MemAvail,' bytes');
   writeln('Mem¢ria m xima: ',MaxAvail,' bytes');
   writeln;
   writeln('Ready...');
   writeln;
end;

begin
   getintvec($5,abcd);
   setintvec($5,@ativar);
   getintvec($1b,efgh);
   setintvec($1b,SaveInt1B);
   writeln;
   writeln('Aperte PrtSc para ativar...');
   writeln;
   keep(768);
end.