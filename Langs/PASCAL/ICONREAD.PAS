uses dos,crt,graph;
var f    : file of byte;
    regs : registers;
    n    : array[1..500] of byte;
    aux  : byte;
    i    : word;
    x    : byte;
    gd,gm: integer;
    a,d,c: word;
    r,g,b:integer;
    ch   : char;
    strg : string;
    S    : searchrec;
    X1,Y1:word;
begin
   gd:=detect;
   directvideo:=false;
   initgraph(gd,gm,'');
   X1:=0;
   Y1:=0;
   ch:=#0;
   chdir('c:\langs\i');
   findfirst('*.ico',anyfile,s);
   repeat
   assign(f,s.name);
   reset(f);
   if doserror <> 0 then
   begin
      ch:=readkey;
      closegraph;
      chdir('c:\langs');
      exit;
   end;
   for i:=0 to $8e do
   read(f,aux);
   for i:=1 to 495 do read(f,n[i]);
   close(f);
   setpalette(0,0);   setpalette(1,4);
   setpalette(2,2);   setpalette(3,20);
   setpalette(4,1);   setpalette(5,5);
   setpalette(6,3);   setpalette(7,56);
   setpalette(8,7);   setpalette(9,60);
   setpalette(10,58); setpalette(11,62);
   setpalette(12,57); setpalette(13,61);
   setpalette(14,59); setpalette(15,63);
   c:=X1;
   a:=32+c;
   d:=Y1;
   for i:=495 downto 1 do
   begin
      aux:=hi(n[i]*16);
      x:=trunc(lo(n[i]*16)/16);
      putpixel(a,d,x);
      putpixel(a-1,d,aux);
      dec(a,2);
      if a=c then a:=32+c;
      if a=32+c then inc(d);
   end;
   inc(X1,32);
   if x1>=(getmaxx-34) then
   begin
      x1:=0;
      inc(y1,32);
   end;
   if y1>=(getmaxy-32) then
   begin
      ch:=readkey;
      setbkcolor(0);
      for i:=0 to getmaxy do
      begin
         setcolor(0);
         line(0,i,getmaxx,i);
      end;
      x1:=0;
      y1:=0;
   end;
   findnext(s);
   until ch=#27;
   closegraph;
   chdir('c:\langs');
end.