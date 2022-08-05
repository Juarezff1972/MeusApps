uses dos,crt;
type RGB = record
                Red,Grn,Blu : byte;
           end;
     PalRegs = array [0..255] of RGB;

var regs            : registers;
    PreCalcY1       : array[0..200] of word;
    Res             : byte;
    XRes,YRes       : integer;
    MaxXRes,MaxYRes : word;
    MaxX,MaxY       : word;
    Asp             : real;
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
begin
   if ((x<XRes) and (y<YRes)) then
   begin
      asm
         mov di,y
         shl di,1
         mov bx,[di+$64]
         add bx,x
         mov ax,$a000
         mov es,ax
         mov dh,color
         mov byte ptr es:[bx],dh
      end;
   end;
end;
{-------------------------------------------------------------------}
Function GetPoint(x,y : word) : byte;
var aux   : byte;
begin
   begin
      asm
         mov di,y
         shl di,1
         mov bx,[di+$64]
         add bx,x
         mov ax,$a000
         mov es,ax
         mov dh,byte ptr es:[bx]
         mov aux,dh
      end;
   end;
   GetPoint:=aux;
end;
{-------------------------------------------------------------------}
Procedure Swap(var f,s : longint);
begin
   inline($1e             { push ds          }
         /$06             { push es          }
         /$c4/$7e/$04     { les  di,[bp+4]   }
         /$c5/$76/$08     { lds  si,[bp+8]   }
         /$26/$66/$8b/$05 { mov  eax,es:[di] }
         /$3e/$66/$8b/$1c { mov  ebx,ds:[si] }
         /$66/$93         { xchg eax,ebx     }
         /$26/$66/$89/$05 { mov  es:[di],eax }
         /$3e/$66/$89/$1c { mov  ds:[si],ebx }
         /$07             { pop  es          }
         /$1f);           { pop  ds          }
end;
{-------------------------------------------------------------------}
Procedure Swapint(var f,s : integer);
begin
   asm
      push ds
      push es
      les  di,[bp+4]
      lds  si,[bp+8]
      mov  ax,es:[di]
      mov  bx,ds:[si]
      xchg ax,bx
      mov  es:[di],ax
      mov  ds:[si],bx
      pop  es
      pop  ds
   end;
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
   for y:=y1 to y2 do Draw(x1,y,x2,y,cor);
end;
{-------------------------------------------------------------------}
Procedure InitGraphics(Mode : byte);
var j : word;
begin
   MaxXRes:=320;
   MaxYRes:=200;
   MaxX:=MaxXRes-1;
   MaxY:=MaxYRes-1;
   if XRes=0 then   XRes:=MaxXRes;
   if YRes=0 then   YRes:=MaxYRes;
   Asp:=(1024/768)*(YRes/XRes);
   for j:=0 to MaxYRes do PreCalcY1[j]:=MaxXRes*j;
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
Procedure ClrScr08(color : byte);
begin
   asm
      mov ax,$a000
      xor di,di
      mov es,ax
      mov cx,$fa00
      mov al,color
      cld
      rep stosb
   end;
end;
{-------------------------------------------------------------------}
Procedure ClrScr16(color : byte);
begin
   asm
      mov ax,$a000
      xor di,di
      mov es,ax
      mov al,color
      mov ah,al
      mov cx,$fa00
      cld
      rep stosw
   end;
end;
{-------------------------------------------------------------------}
Procedure ClrScr32(color : byte);
begin
   inline($b8/$00/$a0                 { mov  ax,A000   }
         /$33/$ff                     { xor  di,di     }
         /$8e/$c0                     { mov  es,ax     }
         /$8a/$46/$04                 { mov  al,[bp+4] }
         /$8a/$e0                     { mov  ah,al     }
         /$50                         { push ax        }
         /$50                         { push ax        }
         /$b9/$00/$fa                 { mov  cx,FA00   }
         /$66/$58                     { pop  eax       }
         /$fc                         { cld            }
         /$f3/$66/$ab);               { rep stosd      }
end;
{-------------------------------------------------------------------}
procedure LHScroll08(x1,y1,x2,y2 : word);
label inicio1,inicio2,final1,final2;
var x3,y3 : word;
begin
    x3:=x2+1;
    y3:=y2+1;
    asm
         push $a000
         pop es
         mov cx,y1
inicio2: cmp cx,y3
         je final2
         mov bx,320
         mov ax,cx
         imul bx
         mov bx,ax
         add bx,x1
         mov dl,byte ptr es:[bx]
         push ax
         pop si
         add si,x2
         push cx
         mov cx,x1
inicio1: cmp cx,x3
         je final1
         mov bx,ax
         add bx,cx
         mov dh,byte ptr es:[bx+1]
         mov byte ptr es:[bx],dh
         inc cx
         jmp inicio1
final1:  pop cx
         inc cx
         mov byte ptr es:[si+1],dl
         jmp inicio2
final2:  nop
    end;
end;
{-------------------------------------------------------------------}
procedure LHScroll16(x1,y1,x2,y2 : word);
label inicio1,inicio2,final1,final2;
var x3,y3 : word;
begin
    x3:=x2+1;
    y3:=y2+1;
    asm
         push $a000
         pop es
         mov cx,y1
inicio2: cmp cx,y3
         jge final2
         mov bx,320
         mov ax,cx
         imul bx
         mov bx,ax
         add bx,x1
         mov dx,word ptr es:[bx]
         push ax
         pop si
         add si,x2
         push cx
         mov cx,x1
inicio1: cmp cx,x3
         jge final1
         mov bx,ax
         add bx,cx
         mov di,word ptr es:[bx+2]
         mov word ptr es:[bx],di
         add cx,2
         jmp inicio1
final1:  pop cx
         add cx,1
         mov word ptr es:[si+1],dx
         jmp inicio2
final2:  nop
    end;
end;
{-------------------------------------------------------------------}
procedure LHScroll32(x1,y1,x2,y2 : word);
var x3,y3 : word;
begin
    x3:=x2+1;
    y3:=y2+1;
    inline($68/$00/$A0               {         push   A000           }
          /$07                       {         pop    es             }
          /$8B/$4E/$08               {         mov    cx,[bp+08]     }
          /$3B/$4E/$FC               { inicio2:cmp    cx,[bp-04]     }
          /$7D/$3B                   {         jnl    final2         }
          /$BB/$40/$01               {         mov    bx,0140        }
          /$8B/$C1                   {         mov    ax,cx          }
          /$F7/$EB                   {         imul   bx             }
          /$8B/$D8                   {         mov    bx,ax          }
          /$03/$5E/$0A               {         add    bx,[bp+0A]     }
          /$26/$66/$8B/$17           {         mov    edx,es:[bx]    }
          /$50                       {         push   ax             }
          /$5E                       {         pop    si             }
          /$03/$76/$06               {         add    si,[bp+06]     }
          /$51                       {         push   cx             }
          /$8B/$4E/$0A               {         mov    cx,[bp+0A]     }
          /$3B/$4E/$FE               { inicio1:cmp    cx,[bp-02]     }
          /$7D/$12                   {         jnl    final1         }
          /$8B/$D8                   {         mov    bx,ax          }
          /$03/$D9                   {         add    bx,cx          }
          /$26/$66/$8B/$7F/$04       {         mov    edi,es:[bx+04] }
          /$26/$66/$89/$3F           {         mov    es:[bx],edi    }
          /$83/$C1/$04               {         add    cx,0004        }
          /$EB/$E9                   {         jmp    inicio1        }
          /$59                       { final1: pop    cx             }
          /$83/$C1/$01               {         add    cx,0001        }
          /$26/$66/$89/$54/$01       {         mov    es:[si+01],edx }
          /$EB/$C0                   {         jmp    inicio2        }
          /$90);                     { final2: nop                   }
end;
{-------------------------------------------------------------------}
procedure BitScroll08;
label salto1,salto2;
begin
    asm
         mov ax,$a000
         mov es,ax
         mov dh,byte ptr es:[$3e80]
         mov bx,$3e80
salto1:  mov ch,byte ptr es:[bx+1]
         mov byte ptr es:[bx],ch
         add bx,1
         cmp bx,$bb80
         je  salto2
         jmp salto1
salto2:  mov byte ptr es:[$bb80],dh
    end;
end;
{-------------------------------------------------------------------}
procedure BitScroll16;
label salto1,salto2;
begin
    asm
         mov ax,$a000
         mov es,ax
         mov dx,word ptr es:[$3e80]
         mov bx,$3e80
salto1:  mov cx,word ptr es:[bx+2]
         mov word ptr es:[bx],cx
         add bx,2
         cmp bx,$bb80
         je  salto2
         jmp salto1
salto2:  mov word ptr es:[$bb80],dx
    end;
end;
{-------------------------------------------------------------------}
procedure BitScroll32;
begin
   inline($b8/$00/$a0/                      {         mov ax,$a000          }
          $8e/$c0/                          {         mov es,ax             }
          $26/$66/$8b/$16/$80/$3e/          {         mov edx,es:[$3e80]    }
          $bb/$80/$3e/                      {         mov bx,$3e80          }
          $26/$66/$8b/$4f/$04/              { salto1: mov ecx,es:[bx+$0004] }
          $26/$66/$89/$0f/                  {         mov es:[bx],ecx       }
          $83/$c3/$04/                      {         add bx,$0004          }
          $81/$fb/$80/$bb/                  {         cmp bx,$bb80          }
          $74/$02/                          {         je  salto2            }
          $eb/$ec/                          {         jmp salto1            }
          $26/$66/$89/$16/$80/$bb);         { salto2: mov es:[$bb80],edx    }
end;
{-------------------------------------------------------------------}
Procedure CircleDemo;
begin
   ClrScr08(0);
   repeat
      circle(MaxXRes div 2,MaxYRes div 2,trunc(2*cr),trunc(cr));
      cr:=cr+0.5;
   until (cr>255);
   repeat until keypressed;
   while keypressed do rrrr:=readkey;
end;
{-------------------------------------------------------------------}
Procedure BoxDemo;
begin
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
end;
{-------------------------------------------------------------------}
Procedure RectangleDemo;
begin
   ClrScr16(0);
   c:=1;
   for yy1:=0 to 16 do
      for xx1:=0 to 16 do
      begin
         Rectangle(trunc(xx1*(MaxXRes/16)),trunc(yy1*(MaxYRes/16)),
         trunc((1+xx1)*(MaxXRes/16))-1,trunc((1+yy1)*(MaxYRes/16))-1,c);
         inc(c);
      end;
   for c:=0 to 255 do
   Rectangle(trunc(c*(MaxXRes/256)),trunc(15*(MaxYRes/16)),MaxXRes-1,MaxYRes-1,c);
   repeat until keypressed;
   while keypressed do rrrr:=readkey;
end;
{-------------------------------------------------------------------}
Procedure GetPointDemo;
begin
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
   while keypressed do rrrr:=readkey;
end;
{-------------------------------------------------------------------}
Procedure PlotDemo;
begin
   ClrScr08(0);
   for c:=0 to 255 do
      for xx1:=0 to MaxX do
      begin
         yy1:=trunc(8*sin(xx1/3)+c);
         Plot(xx1,yy1,c);
      end;
end;
{-------------------------------------------------------------------}
begin
   SetGraphicsMode;
   for c:=0 to 255 do ClrScr32(c);
   GetPal(Pal);
   OrgPal:=Pal;
   DefinePal(Pal);
   c:=1;
   cr:=c;
   CircleDemo;
   RectangleDemo;
   BoxDemo;
   GetPointDemo;
   PlotDemo;
   repeat
      {BitScroll08;
      BitScroll16;
      BitScroll32;}
      if paramstr(1)='8' then   LHScroll08(100,50,220,150);
      if paramstr(1)='16' then LHScroll16(100,50,220,150);
      if paramstr(1)='32' then LHScroll32(100,50,220,150);
      CyclePal(Pal);
   until keypressed;
   while keypressed do rrrr:=readkey;
   SetPal(OrgPal);
   SetMode(3);
end.

