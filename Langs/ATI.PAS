unit ATI;
interface
uses dos,crt;

var _regs            : registers;
    xoff,yoff       : word;
    PreCalcy1       : array[0..768] of word;
    PreCalcy2       : array[0..768] of longint;
    Res             : byte;
    xRes,yRes       : integer;
    MaxxRes,MaxyRes : word;
    Maxx,Maxy       : word;
    OldPage,OldPage2: word;
    Asp             : real;
    Address         : ^byte;

const LowRes=1;
      MediumRes=2;
      HighRes=3;

Procedure SetMode(Mode : byte);
Procedure Plot(x,y : word;color : byte);
Procedure Circle(x,y,radius : word;color : byte);
Procedure Draw(x1,y1,x2,y2 : word;color:byte);
Procedure InitGraphics(Mode : byte);
Procedure SetGraphicsMode(x,y : word);
Procedure Box(x1,y1,x2,y2,cor : word);

implementation
var    x,y,c           : word;
{-------------------------------------------------------------------}
Procedure SetMode;
begin
   _regs.ah:=0;
   _regs.al:=Mode;
   intr($10,_regs);
end;
{-------------------------------------------------------------------}
Procedure CalcOffsets;
var tmp,tmp2 : word;
begin
   if (xRes<MaxxRes) then
   begin
      tmp:=MaxxRes shr 1;
      tmp2:=xRes shr 1;
      xoff:=tmp-tmp2;
   end else xoff:=0;
   if (yRes<MaxyRes) then
   begin
      tmp:=MaxyRes shr 1;
      tmp2:=yRes shr 1;
      yoff:=tmp-tmp2;
   end else yoff:=0;
end;
{-------------------------------------------------------------------}
Procedure PreCalc;
var j   : word;
    k,l : longint;
begin
   for j:=0 to MaxyRes do
   begin
      k:=j;
      l:=MaxxRes;
      if (Res=MediumRes) then PreCalcy2[j]:=l*k else
                                                   PreCalcy1[j]:=MaxxRes*j;
   end;
end;
{-------------------------------------------------------------------}
Procedure Plot;
var L_Offset    : longint;
    Page,Offset : word;
    tmp         : word;
begin
   if ((x<xRes) and (y<yRes)) then
   case Res of
      HighRes :   begin
                     Offset:=PreCalcy1[y+yoff]+x+xoff;
                     Page:=y shr 6;
                     if (Page <> OldPage) then
                     begin
                        tmp:=Page or 64;
                        asm
                           push dx
                           push ax
                           cli
                           mov ax,tmp
                           mov ah,al
                           mov dx,$1ce
                           mov al,$b2
                           out dx,al
                           inc dl
                           in  al,dx
                           shl ah,1
                           and al,$e1
                           or  ah,al
                           mov al,$b2
                           dec dl
                           out dx,ax
                           sti
                           pop ax
                           pop dx
                        end;
                        OldPage:=Page;
                     end;
                     Address:=ptr($a000,Offset);
                     Address^:=color;
                  end;
      MediumRes : begin
                     x:=x+xoff;
                     y:=y+yoff;
                     L_Offset:=PreCalcy2[y]+x;
                     Page:=L_Offset shr 16;
                     Offset:=L_Offset and 65535;
                     if (Page <> OldPage) then
                     begin
                        tmp:=Page or 64;
                        asm
                           push dx
                           push ax
                           cli
                           mov ax,tmp
                           mov ah,al
                           mov dx,$1ce
                           mov al,$b2
                           out dx,al
                           inc dl
                           in  al,dx
                           shl ah,1
                           and al,$e1
                           or  ah,al
                           mov al,$b2
                           dec dl
                           out dx,ax
                           sti
                           pop ax
                           pop dx
                        end;
                        OldPage:=Page;
                     end;
                     Address:=ptr($a000,Offset);
                     Address^:=color;
                  end;
      LowRes :    begin
                     Offset:=PreCalcy1[y+yoff]+x+xoff;
                     Address:=ptr($a000,Offset);
                     Address^:=color;
                  end;
      end;
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
Procedure Swapint(var f,s : integer);
var tmp : integer;
begin
   tmp:=f;
   f:=s;
   s:=tmp;
end;
{-------------------------------------------------------------------}
Procedure Circle;
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
Procedure Draw;
var lgdelta,shdelta,cycle,lgstep,shstep,dtotal : integer;

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
            cycle:=cycle-lgdelta;
            y1:=y1+shstep;
         end;
         x1:=x1+lgstep;
      end;
      plot(x1,y1,color);
   end else
   begin
      cycle:=shdelta shr 1;
      swapint(lgdelta,shdelta);
      swapint(lgstep,shstep);
      while(y1<>y2) do
      begin
         plot(x1,y1,color);
         cycle:=cycle+shdelta;
         if cycle>lgdelta then
         begin
            cycle:=cycle-lgdelta;
            x1:=x1+shstep;
         end;
         y1:=y1+lgstep;
      end;
      plot(x1,y1,color);
   end;
end;
{-------------------------------------------------------------------}
Procedure InitGraphics;
begin
   case Mode of
      19  : begin Res:=LowRes;    MaxxRes:=320;  MaxyRes:=200; end;
      98  : begin Res:=MediumRes; MaxxRes:=640;  MaxyRes:=480; end;
      99  : begin Res:=MediumRes; MaxxRes:=800;  MaxyRes:=600; end;
      100 : begin Res:=HighRes;   MaxxRes:=1024; MaxyRes:=768; end;
   else
      begin
         Writeln;
         Writeln('Modo ',Mode,' n„o ‚ um modo gr fico v lido.');
         Writeln;
         Halt(1);
      end;
   end;
   Maxx:=MaxxRes-1;
   Maxy:=MaxyRes-1;
   if xRes=0 then   xRes:=MaxxRes;
   if yRes=0 then   yRes:=MaxyRes;
   CalcOffsets;
   Asp:=1; {(1024/768)*(yRes/xRes);}
   PreCalc;
   SetMode(Mode);
end;
{-------------------------------------------------------------------}
Procedure SetGraphicsMode;
begin
   xRes:=x;
   yRes:=y;
   if (x=320)and(y=200) then  InitGraphics(19) else
   if (x=640)and(y=480) then  InitGraphics(98) else
   if (x=800)and(y=600) then  InitGraphics(99) else
   if (x=1024)and(y=768) then InitGraphics(100) else
   InitGraphics(100);
end;
{-------------------------------------------------------------------}
Procedure Box;
var x,y : word;
begin
   for x:=x1 to x2 do
   for y:=y1 to y2 do
   begin
      plot(x,y,cor);
   end;
end;
{-------------------------------------------------------------------}
begin
end.