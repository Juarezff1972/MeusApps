uses dos;
var t,s : searchrec;
    FromF, ToF: file;
    NumRead, NumWritten: Word;
    Buf: array[1..2048] of Char;
begin
   writeln;
   writeln('Install - By Coyote');
   writeln;
   findfirst('*.*',AnyFile-Directory,s);
   while doserror=0 do
   begin
      findfirst('c:\coyote',Directory,t);
      if doserror<>0 then mkdir('c:\coyote');
      Write('Copiando '+s.name+'.');
      Assign(FromF, s.name); { Open input file }
      Reset(FromF, 1);  { Record size = 1 }
      Assign(ToF, 'c:\coyote\'+s.name); { Open output file }
      Rewrite(ToF, 1);  { Record size = 1 }
      repeat
         BlockRead(FromF, Buf, SizeOf(Buf), NumRead);
         BlockWrite(ToF, Buf, NumRead, NumWritten);
         Write('.');
      until (NumRead = 0) or (NumWritten <> NumRead);
      writeln;
      Close(FromF);
      Close(ToF);
      findnext(s);
   end;
   chdir('c:\coyote');
end.
