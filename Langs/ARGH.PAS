var a : ^byte;
begin
   getmem(a,65535);
   fillchar(a^,65000,68);
   freemem(a,65535);
end.