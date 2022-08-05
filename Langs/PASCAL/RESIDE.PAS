Program Residente;
{$M 1024,0,60000}
uses dos,crt,memory;
var abcd : procedure;
    efgh : pointer;
    ijkl : pointer;
Procedure Now;
interrupt;
begin
{   if port[$60]=68 then}
   begin
      getmem(ijkl,60000);
      swapvectors;
      exec('C:\MSDOS\MEM.EXE','/C');
      swapvectors;
      freemem(ijkl,60000);
      writeln('DOSERROR: ',doserror);
      writeln;
   end;
   inline($9c);
   abcd;
end;
begin
   getintvec($1b,efgh);
   setintvec($1b,SaveInt1B);
   getintvec($28,@abcd);
   setintvec($28,addr(Now));
{   keep(768);}
   inline($cd/$28);
end.
