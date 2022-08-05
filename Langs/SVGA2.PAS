{$M 2000,0,0}
{$R-,S-,I-,F-,O-,A-,V+,B-}
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
    MouseX,MouseY   : word;
    Curs            : array[1..4,1..9] of byte;
    x1,y1           : word;
    Leftbutton,MidButton,Rightbutton : boolean;

    End_Of_Int     : Byte;
	 OriginalVector : Pointer;
	 OriginalMask   : Byte;
	 IntNumber      : $08..$0f;


const LowRes=1;
      MediumRes=2;
      HighRes=3;
      irq=0;
		Controller =$20;
		SpecificEOI=$60;

{-------------------------------------------------------------------}
Procedure DefinirCursor;
var i,x,y : byte;
    regs  : registers;
    x1,y1 : word;
    a,b   : byte;
begin
	for x:=1 to 4 do for y:=1 to 9 do Curs[x,y]:=0;

	for i:=1 to 7 do Curs[1,i]:=15;
	for i:=7 to 9 do Curs[4,i]:=15;
	for i:=5 to 7 do Curs[3,i]:=15;
	for i:=3 to 5 do Curs[2,i]:=12;
	Curs[2,2]:=15;Curs[2,6]:=15;Curs[3,3]:=15;Curs[3,4]:=12;Curs[4,4]:=15;

end;
{-------------------------------------------------------------------}
Procedure STI;
begin
	inline($fb);
end;
{-------------------------------------------------------------------}
Procedure CLI;
begin
	inline($fa);
end;
{-------------------------------------------------------------------}
{$F+}
Procedure InterruptHandler;
interrupt;
var r   : registers;
	 a,b : byte;
    
begin
   r.ax:=3;
	intr($33,r);
   x1:=r.cx;
   y1:=r.dx;
   if ((Res>=LowRes) and (Res<=HighRes)) then
   begin
{      if not ((x1=MouseX) and (y1=MouseY)) then}
	   for a:=0 to 3 do for b:=0 to 8 do
      begin
			regs.ah:=13;
			regs.cx:=x1+a;
			regs.dx:=y1+b;
			intr($10,regs);
			regs.cx:=x1+a;
			regs.dx:=y1+b;
			regs.ah:=12;
			regs.al:=regs.al xor Curs[a+1,b+1];
			intr($10,regs);
		end;
     	r.ax:=3;
	   intr($33,r);
     	x1:=r.cx;
	   y1:=r.dx;
{      if not ((x1=MouseX) and (y1=MouseY)) then
    	for a:=0 to 3 do for b:=0 to 8 do
      begin
			regs.ah:=13;
			regs.cx:=x1+a;
			regs.dx:=y1+b;
			intr($10,regs);
			regs.cx:=x1+a;
			regs.dx:=y1+b;
			regs.ah:=12;
			regs.al:=regs.al xor Curs[a+1,b+1];
			intr($10,regs);
		end;}
   end;
   r.ax:=3;
   intr($33,r);
  	MouseX:=r.cx;
  	MouseY:=r.dx;
  	Leftbutton:=false;MidButton:=false;Rightbutton:=false;
   if r.bx >=4 then begin MidButton:=true; r.bx:=r.bx-4; end;
  	if r.bx >=2 then begin RightButton:=true; r.bx:=r.bx-2; end;
   if r.bx >=1 then begin LeftButton:=true; r.bx:=r.bx-1; end;
	Port[Controller]:=End_Of_Int;
end;
{$F-}
{-------------------------------------------------------------------}
Procedure ShowMouse;
var EnablePattern : Byte;
begin
	GetIntVec(IntNumber,OriginalVector);
	CLI;
	SetIntVec(IntNumber,@InterruptHandler);
	STI;
	OriginalMask:=Port[Controller+1];
	EnablePattern:=$01;
	EnablePattern:=EnablePattern shl IRQ;
	EnablePattern:=not(EnablePattern);
	Port[Controller+1]:=(OriginalMask and EnablePattern);
end;
{-------------------------------------------------------------------}
Procedure HideMouse;
begin
	Port[Controller+1]:=OriginalMask;
	CLI;
	SetIntVec(IntNumber,OriginalVector);
	STI;
end;
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
    tmp         : word;
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
Procedure FracLine(x1,y1,x2,y2,color : word);
var mx,my : word;
begin
   mx:=(x1+x2) div 2;
   my:=(y1+y2) div 2;
   if ((mx=x1)and(my=y1))or((x1=x2)and(y1=y2)) then exit;
   plot(mx,my,color);
   Fracline(x1,y1,mx,my,color);
   Fracline(mx,my,x2,y2,color);
end;
{-------------------------------------------------------------------}
Procedure FracBox(x1,y1,x2,y2,color : word);
var mx : word;
begin
   mx:=(x1+x2) div 2;
   if (mx=x1)or(x1=x2) then exit;
   FracLine(mx,y1,mx,y2,color);
   FracBox(x1,y1,mx,y2,color);
   FracBox(mx,y1,x2,y2,color);
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
   IntNumber:=IRQ+8;
	End_Of_Int:=SpecificEOI+IRQ;
	ShowMouse;
   DefinirCursor;
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
   HideMouse;
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
