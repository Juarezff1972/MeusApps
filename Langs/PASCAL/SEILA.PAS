uses dos,crt;
var i:word;
   a:string;
   f:file;
begin
  for i:=1 to 600 do
  begin
    str(i,a);
    assign(f,'c:\langs\setembro.92\'+a);
    rewrite(f);
    close(f);
  end;

end.