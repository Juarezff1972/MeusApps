program Mandelbrot_Set;

uses graph,crt;

type color = array[0..7] of integer;

var i,y,x,gd,gm       : integer;
    a,b,c,d,p,t,r,j,q : real;
    maior,igual       : integer;
    ko                : color;
    xx,yy,pp          : word;
    linha,coluna      : word;
label aa,ab,ac;
begin
   for i:=0 to 7 do ko[i]:=i;
   gd:=VGA;
   gm:=VGAlo;
   initgraph(gd,gm,'');
   cleardevice;
   for y:=0 to 96 do
      for x:=0 to 279 do
         begin
            a:=0;
            b:=0;
            c:=(x-140)/90-0.5;
            d:=(y-96)/96;
            i:=-1;
            repeat
                  i:=i+1;
                  q:=a*a-b*b+c;
                  b:=2*a*b+d;
                  a:=q;
                  maior:=0;
                  igual:=0;
                  if (i>3) then maior:=1;
                  if (i=7) then igual:=1;
                  p:=i+maior-igual*4;
                  putpixel(2*x,y,ko[trunc(p)]);
                  putpixel(2*x,191-y,ko[trunc(p)]);
                  t:=a*a;
                  r:=b*b;
                  j:=r+t;
                  maior:=0;
                  if (j>4) then maior:=1;
                  i:=i+(maior)*7;
            until i >= 7;
         end;
   repeat
   asm
      push ax
      push cx
      push bx
      push dx
      push es
      pushf
      mov cx,32768
      xor bx,bx
      mov dx,$b800
      mov es,dx
   aa:mov ah,es:[bx]
      sub ah,85
      mov es:[bx],ah
      inc bx
      loop aa
      popf
      pop es
      pop dx
      pop bx
      pop cx
      pop ax
   end;
   until keypressed;
   closegraph;
end.
