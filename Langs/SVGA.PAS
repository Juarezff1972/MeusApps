uses dos,crt;
type RGB = record
                Red,Grn,Blu : byte;
           end;
     PalRegs = array [0..255] of RGB;

var regs            : registers;
    Xoff,Yoff       : word;
    PreCalcY1       : array[0..768] of word;
    PreCalcY2       : array[0..768] of longint;
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

const LowRes=1;
      MediumRes=2;
      HighRes=3;
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
      if (Res=MediumRes) then PreCalcY2[j]:=l*k else
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
Procedure DefinePal(var temp:PalRegs);
var i,x,y: byte;
begin
   for x:=1 to 255 do
   begin
      temp[x].Red:=0;
      temp[x].Grn:=0;
      temp[x].Blu:=0;
   end;
   for x:=1 to 16 do
   begin
      temp[x].Grn:=(x-1)*4;
      temp[x].Blu:=60-((x-1)*4);
   end;
   for x:=17 to 32 do
   begin
      temp[x].Grn:=60-(((x-16)-1)*4);
      temp[x].Blu:=((x-16)-1)*4;
   end;
   for x:=33 to 48 do
   begin
      temp[x].Red:=((x-32)-1)*4;
      temp[x].Blu:=60-(((x-32)-1)*4);
   end;
   for x:=49 to 64 do
   begin
      temp[x].Grn:=((x-48)-1)*4;
      temp[x].Blu:=((x-48)-1)*4;
      temp[x].Red:=60-(((x-48)-1)*4);
   end;
   for x:=65 to 80 do
   begin
      temp[x].Red:=((x-64)-1)*4;
      temp[x].Grn:=60-(((x-64)-1)*4);
      temp[x].Blu:=60-(((x-64)-1)*4);
   end;
   for x:=81 to 96 do
   begin
      temp[x].Red:=60;
      temp[x].Blu:=((x-80)-1)*4;
   end;
   for x:=97 to 112 do
   begin
      temp[x].Red:=60;
      temp[x].Blu:=60;
      temp[x].Grn:=((x-96)-1)*4;
   end;
   for x:=113 to 128 do
   begin
      temp[x].Red:=60;
      temp[x].Blu:=60-(((x-112)-1)*4);
      temp[x].Grn:=60;
   end;
   for x:=129 to 144 do
   begin
      temp[x].Red:=60-(((x-128)-1)*4);
      temp[x].Grn:=60;
   end;
   for x:=145 to 160 do
   begin
      temp[x].Grn:=60-(((x-144)-1)*4);
   end;
   for x:=161 to 176 do
   begin
      temp[x].Blu:=((x-160)-1)*4;
   end;
   for x:=177 to 192 do
   begin
      temp[x].Blu:=60;
      temp[x].Grn:=((x-176)-1)*4;
   end;
   for x:=193 to 208 do
   begin
      temp[x].Blu:=60;
      temp[x].Grn:=60;
      temp[x].Red:=((x-192)-1)*4;
   end;
   for x:=209 to 224 do
   begin
      temp[x].Blu:=60;
      temp[x].Grn:=60;
      temp[x].Red:=60;
   end;
   for x:=225 to 240 do
   begin
      temp[x].Blu:=60;
      temp[x].Grn:=60-(((x-224)-1)*4);
      temp[x].Red:=60;
   end;
   for x:=241 to 255 do
   begin
      temp[x].Blu:=60;
      temp[x].Red:=60-(((x-224)-1)*4);
   end;
   SetPal(temp);
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
var L_Offst    : longint;
    Page,Offst : word;
    tmp         : word;
begin
   if ((x<XRes) and (Y<YRes)) then
   case Res of
      HighRes :   begin
                     Offst:=PreCalcY1[y+Yoff]+x+Xoff;
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
                     asm
                        mov ax,$a000
                        mov es,ax
                        mov bx,Offst
                        mov dh,color
                        mov byte ptr es:[bx],dh
                     end;
                  end;
      MediumRes : begin
                     x:=x+Xoff;
                     y:=y+Yoff;
                     L_Offst:=PreCalcY2[y]+x;
                     Page:=L_Offst shr 16;
                     Offst:=L_Offst and 65535;
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
                     asm
                        mov ax,$a000
                        mov es,ax
                        mov bx,Offst
                        mov dh,color
                        mov byte ptr es:[bx],dh
                     end;
                  end;
      LowRes :    begin
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
end;
{-------------------------------------------------------------------}
Function GetPoint(x,y : word) : byte;
var L_Offst    : longint;
    Page,Offst : word;
    tmp        : word;
    aux        : byte;
begin
   if ((x<XRes) and (Y<YRes)) then
   case Res of
      HighRes :   begin
                     Offst:=PreCalcY1[y+Yoff]+x+Xoff;
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
                     asm
                        mov ax,$a000
                        mov es,ax
                        mov bx,Offst
                        mov dh,byte ptr es:[bx]
                        mov aux,dh
                     end;
                  end;
      MediumRes : begin
                     x:=x+Xoff;
                     y:=y+Yoff;
                     L_Offst:=PreCalcY2[y]+x;
                     Page:=L_Offst shr 16;
                     Offst:=L_Offst and 65535;
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
                     asm
                        mov ax,$a000
                        mov es,ax
                        mov bx,Offst
                        mov dh,byte ptr es:[bx]
                        mov aux,dh
                     end;
                  end;
      LowRes :    begin
                     Offst:=PreCalcY1[y+Yoff]+x+Xoff;
                     asm
                        mov ax,$a000
                        mov es,ax
                        mov bx,Offst
                        mov dh,byte ptr es:[bx]
                        mov aux,dh
                     end;
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
Procedure Swapint(var f,s : integer);
var tmp : integer;
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
Procedure Rectangle(x1,y1,x2,y2,cor : word);
begin
   Draw(x1,y1,x2,y1,cor);
   Draw(x1,y2,x2,y2,cor);
   Draw(x1,y1,x1,y2,cor);
   Draw(x2,y1,x2,y2,cor);
end;
{-------------------------------------------------------------------}
Procedure Box(x1,y1,x2,y2: word;cor : byte);
var x,y : word;
begin
   for y:=y1 to y2 do Draw(x1,y,x2,y,cor); {FracLine(x1,y,x2+1,y,cor);}
end;
{-------------------------------------------------------------------}
Procedure InitGraphics(Mode : byte);
begin
   case Mode of
      19  : begin Res:=LowRes;    MaxXRes:=320;  MaxYRes:=200; end;
      98  : begin Res:=MediumRes; MaxXRes:=640;  MaxYRes:=480; end;
      99  : begin Res:=MediumRes; MaxXRes:=800;  MaxYRes:=600; end;
      100 : begin Res:=HighRes;   MaxXRes:=1024; MaxYRes:=768; end;
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
   Asp:=(1024/768)*(YRes/XRes);
   PreCalc;
   SetMode(Mode);
end;
{-------------------------------------------------------------------}
Procedure SetGraphicsMode(x,y : word);
begin
   XRes:=x;
   YRes:=y;
   if (x=320)and(y=200) then  InitGraphics(19) else
   if (x=640)and(y=480) then  InitGraphics(98) else
   if (x=800)and(y=600) then  InitGraphics(99) else
   if (x=1024)and(y=768) then InitGraphics(100) else
   InitGraphics(100);
end;
{-------------------------------------------------------------------}
Procedure ClrScr(color : byte);
var Page : byte;
    tmp  : word;
label A,B,C,FIM1,INI1,FIM2,INI2;
begin
   case Res of
      HighRes :   begin
                        asm
                           mov ax,$4B
                      INI2:cmp ax,$3F
                           je FIM2
                           push dx
                           push ax
                           push cx
                           push bx
                           push es
                           cli
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
                           mov ax,$a000
                           mov es,ax
                           mov cx,$ffff
                           mov dh,color
                         A:mov bx,cx
                           mov byte ptr es:[bx],dh
                           loop A
                           mov byte ptr es:[0],dh
                           pop es
                           pop bx
                           pop cx
                           pop ax
                           pop dx
                           dec ax
                           jmp INI2
                      FIM2:nop
                        end;
                  end;
      MediumRes : begin
                        asm
                           mov ax,$47
                      INI1:cmp ax,$3F
                           je FIM1
                           push dx
                           push ax
                           push cx
                           push bx
                           push es
                           cli
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
                           mov ax,$a000
                           mov es,ax
                           mov cx,$ffff
                           mov dh,color
                           push ds
                           push es
                           pop ds
                         B:mov bx,cx
                           mov byte ptr [bx],dh
                           loop B
                           mov byte ptr [0],dh
                           pop ds
                           pop es
                           pop bx
                           pop cx
                           pop ax
                           pop dx
                           dec ax
                           jmp INI1
                      FIM1:nop
                        end;
                  end;
      LowRes :    begin
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
      end;
end;
{-------------------------------------------------------------------}
begin
   SetGraphicsMode(640,480);
   ClrScr(1);
   repeat until keypressed;


{   for c:=1 to 255 do
      FracBox(0,(MaxYRes div 2)-10,MaxXRes,(MaxYRes div 2)+10,c);}

{   for c:=0 to 255 do
   ClrScr(c);}
   c:=1;
{   cr:=c;
   repeat
      circle(MaxXRes div 2,MaxYRes div 2,trunc(2*cr),trunc(cr));
      cr:=cr+0.5;
   until (cr>255);
   repeat until keypressed;
   while keypressed do rrrr:=readkey;}

{   GetPal(Pal);
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
   while keypressed do rrrr:=readkey;}

{   for xx1:=0 to MaxX do
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
      end;}
{   repeat
      CyclePal(Pal);
      delay(50);
   until keypressed;
   SetPal(OrgPal);}
   while keypressed do rrrr:=readkey;
   SetMode(3);
end.

{ Porta $3cd global; $1ce ATI }

{
                            |   Regitradores    | Resolu- |
                            |    da INT $10     |   ‡„o   |
                            ===============================
 Nome da placa de video     | AX | BX | CX | DX |  x |  y | Porta |
===================================================================
IBM 256-Color VGA/MCGA      |  13|   0|   0|   0| 320| 200| ----- |
VESA Standard interface     |4f02| 101|   0|   0| 640| 480| BIOS  | *
VESA Standard interface     |4f02| 103|   0|   0| 800| 600| BIOS  | *
VESA Standard interface     |4f02| 105|   0|   0|1024| 768| BIOS  | *
ATI VGA Wonder (512K)       |  62|   0|   0|   0| 640| 480| $1ce  |
ATI VGA Wonder (512K)       |  63|   0|   0|   0| 800| 600| $1ce  |
ATI VGA Wonder (1024K)      |  64|   0|   0|   0|1024| 768| $1ce  |
Orchid/STB/GENOA/SIGMA      |  2e|   0|   0|   0| 640| 480| $3c4  |
Orchid/STB/GENOA/SIGMA      |  30|   0|   0|   0| 800| 600| $3c4  |
Orchid/STB/GENOA/SIGMA      |  38|   0|   0|   0|1024| 768| $3c4  |
Pdise/AST/COMPAQ VGA        |  5f|   0|   0|   0| 640| 480| $3ce  |
TRIDENT Chipset             |  5d|   0|   0|   0| 640| 480| $3ce  |
TRIDENT Chipset             |  5e|   0|   0|   0| 800| 600| $3ce  |
Video-7  w/512K             |6f05|  67|   0|   0| 640| 480| $3c4  | *
Video-7  w/512K             |6f05|  69|   0|   0| 800| 600| $3c4  | *
Video-7  w/1024k            |6f05|  6a|   0|   0|1024| 768| $3c4  | *
Everex                      |  70|  30|   0|   0| 640| 480| $3c4  | *
Everex                      |  70|  31|   0|   0| 800| 600| $3c4  | *
Tseng 4000                  |  2e|   0|   0|   0| 640| 480| $3bf  |
Tseng 4000                  |  30|   0|   0|   0| 800| 600| $3bf  |
Tseng 4000                  |  38|   0|   0|   0|1024| 768| $3bf  |
Oak Technology Inc OTI-067  |  53|   0|   0|   0| 640| 480| $3de  |
Oak Technology Inc OTI-067  |  54|   0|   0|   0| 800| 600| $3de  |
}
