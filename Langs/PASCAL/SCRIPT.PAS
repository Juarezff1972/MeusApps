uses dos,crt;
var x : text;
    i : byte;
begin
   assign(x,'d:\color.scr');
   rewrite(x);
   for i:= 1 to 255 do
   begin
      writeln(x,'Color ',i);
      writeln(x,'line ',i,',1 ',i,',255'+chr(13));
      writeln('Color ',i);
      writeln('line ',i,',1 ',i,',255');
   end;
   writeln(x,'zoom e');
   close(x);
end.