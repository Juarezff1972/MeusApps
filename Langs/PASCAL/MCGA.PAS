uses dos,crt;
type RGB = record
                Red,Grn,Blu : byte;
           end;
     PalRegs = array [0..255] of RGB;

var regs            : registers;
    Xoff,Yoff       : word;
    PreCalcY1       : array[0..768] of word;
    Res             : byte;
    XRes,YRes       : integer;
    MaxXRes,MaxYRes : word;
    MaxX,MaxY       : word;
    OldPage,OldPage2: word;
    Asp             : real;
    Address         : ^byte;

    x,y,c,xx1,yy1   : word;
    rrrr            : char;
    cr              : real;
    Pal,OrgPal      : PalRegs;

{-------------------------------------------------------------------}
Procedure SetMode(Mode : byte);
begin
   regs.ah:=0;
   regs.al:=Mode;
   intr($10,regs);
end;
{-------------------------------------------------------------------}
Procedure CalcOffsets;
var tmp,tmp2 : word;
begin
   if (XRes<MaxXRes) then
   begin
      tmp:=MaxXRes shr 1;
      tmp2:=XRes shr 1;
      Xoff:=tmp-tmp2;
   end else Xoff:=0;
   if (YRes<MaxYRes) then
   begin
      tmp:=MaxYRes shr 1;
      tmp2:=YRes shr 1;
      Yoff:=tmp-tmp2;
   end else Yoff:=0;
end;
{-------------------------------------------------------------------}
Procedure PreCalc;
var j   : word;
    k,l : longint;
begin
   for j:=0 to MaxYRes do
   begin
      k:=j;
      l:=MaxXRes;
      PreCalcY1[j]:=MaxXRes*j;
   end;
end;
{-------------------------------------------------------------------}
Procedure GetPal(var Pal : PalRegs);
var i    : byte;
    regs : registers;
begin
   for i:=0 to 255 do
   begin
      regs.ah:=$10;
      regs.al:=$15;
      regs.bx:=i;
      intr($10,regs);
      Pal[i].Red:=regs.dh;
      Pal[i].Grn:=regs.ch;
      Pal[i].Blu:=regs.cl;
   end;
end;
{-------------------------------------------------------------------}
Procedure SetPal(Pal : PalRegs);
var i    : byte;
    regs : registers;
begin
   regs.ah:=$10;
   regs.al:=$12;
   regs.bx:=0;
   regs.cx:=256;
   regs.dx:=Ofs(Pal[0]);
   regs.es:=Seg(Pal[0]);
   intr($10,regs);
end;
{-------------------------------------------------------------------}
Procedure CyclePal(var Hue : PalRegs);
var tmp : RGB;
    i   : byte;
begin
   tmp:=Hue[1];
   move(Hue[2],Hue[1],762);
   Hue[255]:=tmp;
   SetPal(Hue);
end;
{-------------------------------------------------------------------}
Procedure Plot(x,y : word;color : byte);
var Offst : word;
begin
   if ((x<XRes) and (Y<YRes)) then
   begin
      Offst:=PreCalcY1[y+Yoff]+x+Xoff;
      asm
         mov ax,$a000
         mov es,ax
         mov bx,Offst
         mov dh,color
         mov byte ptr es:[bx],dh
      end;
   end;
end;
{-------------------------------------------------------------------}
Function GetPoint(x,y : word) : byte;
var Offst : word;
    aux   : byte;
begin
   if ((x<XRes) and (Y<YRes)) then
   begin
      Offst:=PreCalcY1[y+Yoff]+x+Xoff;
      asm
         mov ax,$a000
         mov es,ax
         mov bx,Offst
         mov dh,byte ptr es:[bx]
         mov aux,dh
      end;
   end;
   GetPoint:=aux;
end;
{-------------------------------------------------------------------}
Procedure Swap(var f,s : longint);
var tmp : longint;
begin
   tmp:=f;
   f:=s;
   s:=tmp;
end;
{-------------------------------------------------------------------}
Procedure Circle(x,y,radius : word;color : byte);
var a,af,b,bf,target,r2 : longint;
begin
   target:=0;
   a:=radius;
   b:=0;
   r2:=sqr(radius);
   while(a>=b) do
   begin
      b:=round(sqrt(r2-(a*a)));
      swap(target,b);
      while(b<target) do
      begin
         af:=(round(asp*100)*a) div 100;
         bf:=(round(asp*100)*b) div 100;
         plot(x+af,y+b,color);
         plot(x+bf,y+a,color);
         plot(x-af,y+b,color);
         plot(x-bf,y+a,color);
         plot(x-af,y-b,color);
         plot(x-bf,y-a,color);
         plot(x+af,y-b,color);
         plot(x+bf,y-a,color);
         inc(b);
      end;
      dec(a);
   end;
end;
{-------------------------------------------------------------------}
Procedure Draw(x1,y1,x2,y2 : word;color:byte);
var lgdelta,shdelta,cycle,lgstep,shstep,dtotal : longint;
begin
   lgdelta:=x2-x1;
   shdelta:=y2-y1;
   if lgdelta<0 then begin lgdelta:=-lgdelta; lgstep:=-1; end else lgstep:=1;
   if shdelta<0 then begin shdelta:=-shdelta; shstep:=-1; end else shstep:=1;
   if shdelta<lgdelta then
   begin
      cycle:=lgdelta shr 1;
      while(x1<>x2) do
      begin
         plot(x1,y1,color);
         cycle:=cycle+shdelta;
         if cycle>lgdelta then
         begin
            cycle:=-lgdelta;
            y1:=y1+shstep;
         end;
         x1:=x1+lgstep;
      end;
      plot(x1,y1,color);
   end else
   begin
      cycle:=shdelta shr 1;
      swap(lgdelta,shdelta);
      swap(lgstep,shstep);
      while(y1<>y2) do
      begin
         plot(x1,y1,color);
         cycle:=cycle+shdelta;
         if cycle>lgdelta then
         begin
            cycle:=-lgdelta;
            x1:=x1+shstep;
         end;
         y1:=y1+lgstep;
      end;
      plot(x1,y1,color);
   end;
end;
{-------------------------------------------------------------------}
Procedure Rectangle(x1,y1,x2,y2,cor : word);
begin
   Draw(x1,y1,x2,y1,cor);
   Draw(x1,y2,x2,y2,cor);
   Draw(x1,y1,x1,y2,cor);
   Draw(x2,y1,x2,y2,cor);
end;
{-------------------------------------------------------------------}
Procedure Box(x1,y1,x2,y2,cor : word);
var x,y : word;
begin
   for y:=y1 to y2 do Draw(x1,y,x2,y,cor);
end;
{-------------------------------------------------------------------}
Procedure InitGraphics(Mode : byte);
begin
   case Mode of
      19  : begin  MaxXRes:=320;  MaxYRes:=200; end;
   else
      begin
         Writeln;
         Writeln('Modo ',Mode,' n„o ‚ um modo gr fico v lido.');
         Writeln;
         Halt(1);
      end;
   end;
   MaxX:=MaxXRes-1;
   MaxY:=MaxYRes-1;
   if XRes=0 then   XRes:=MaxXRes;
   if YRes=0 then   YRes:=MaxYRes;
   CalcOffsets;
   Asp:=1;
   PreCalc;
   SetMode(Mode);
end;
{-------------------------------------------------------------------}
Procedure SetGraphicsMode;
begin
   XRes:=320;
   YRes:=200;
   InitGraphics(19);
end;
{-------------------------------------------------------------------}
Procedure ClrScr(color : byte);
begin
    asm
       mov ax,$a000
       mov di,0
       mov es,ax
       mov cx,$ffff
       mov al,color
       cld
       rep stosb
    end;
end;
{-------------------------------------------------------------------}
begin
   SetGraphicsMode;

   for c:=0 to 255 do
   ClrScr(c);
   c:=1;
   cr:=c;
   repeat
      circle(MaxXRes div 2,MaxYRes div 2,trunc(2*cr),trunc(cr));
      cr:=cr+0.5;
   until (cr>255);
   repeat until keypressed;
   while keypressed do rrrr:=readkey;

   GetPal(Pal);
   OrgPal:=Pal;

   c:=1;
   for yy1:=0 to 16 do
      for xx1:=0 to 16 do
      begin
         Box(trunc(xx1*(MaxXRes/16)),trunc(yy1*(MaxYRes/16)),
         trunc((1+xx1)*(MaxXRes/16))-1,trunc((1+yy1)*(MaxYRes/16))-1,c);
         inc(c);
      end;
   for c:=0 to 255 do
   Box(trunc(c*(MaxXRes/256)),trunc(15*(MaxYRes/16)),MaxXRes-1,MaxYRes-1,c);
   repeat until keypressed;
   while keypressed do rrrr:=readkey;

   for xx1:=0 to MaxX do
      for yy1:=0 to MaxY do
      begin
         Plot(xx1,yy1,255-GetPoint(xx1,yy1));
      end;
   repeat until keypressed;
   while keypressed do rrrr:=readkey;

   for xx1:=0 to MaxX do
      for yy1:=0 to MaxY do
      begin
         Plot(xx1,yy1,255-GetPoint(xx1,yy1));
      end;
   repeat
      CyclePal(Pal);
      delay(50);
   until keypressed;
   SetPal(OrgPal);
   while keypressed do rrrr:=readkey;
   SetMode(3);
end.
