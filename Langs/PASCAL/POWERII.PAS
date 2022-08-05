Program PowerV2;

Uses Graph,CRT;

var

 A             : integer;
 B,C,D,E,F,G   : real;    { Constants defined }
 n1,n2         : real;
 H,BB          : integer; { Loop Variables }
 AA {,BB}      : integer; { Dependent on H }
 CC,D1,DD      : real;    { Dependent on H and BB }
 X,Y           : real;
 {X,}X1,{Y,}Y1 : integer; { Plotting variables }
 {  Y,Y1       : integer;}{ Dependent on DD and H }
 gd,gm         : integer;
 _dest,_orig   : word;
 frame         : byte;
{-------------------------------------------------------------------}
Procedure MovePagina;
begin
   inline($b9/$80/$3e/               { mov cx,3E80  }
          $33/$ff/                   { xor di,di    }
          $8b/$16/>_dest/            { mov dx,_DEST }
          $8e/$c2/                   { mov es,dx    }
          $33/$f6/                   { xor si,si    }
          $8b/$16/>_orig/            { mov dx,_ORIG }
          $1e/                       { push ds      }
          $8e/$da/                   { mov ds,dx    }
          $d1/$e9/                   { shr cx,1     }
          $f3/$a5/                   { rep movsw    }
          $1f);                      { pop ds       }

          { Pagina 0 : $b800 }  { Pagina 8 : $a400 }
          { Pagina 1 : $8800 }  { Pagina 9 : $a800 }
          { Pagina 2 : $8c00 }  { Pagina 10: $ac00 }
          { Pagina 3 : $9000 }  { Pagina 11: $b000 }
          { Pagina 4 : $9400 }  { Pagina 12: $b400 }
          { Pagina 5 : $9800 }
          { Pagina 6 : $9c00 }
          { Pagina 7 : $a000 }
end;
{-------------------------------------------------------------------------}
Procedure Fundos;
begin
   x:=bb+(h/b)+e;
   y:=dd-(h/b)+f;
   x1:=trunc(0.85*x);
   y1:=trunc(0.9 *(g-y));
   if ((y1<0) or (y1>190)) then exit;
   putpixel(2*x1,y1,15);
   setcolor(0);
   line(2*x1,y1+1,2*x1,190);
   setcolor(15);
end;
{-------------------------------------------------------------------------}
Procedure Criar;
begin
   cleardevice;
   h:=-a;
   repeat
      aa:=trunc(0.5+sqrt(a*a-h*h));
      for bb:=-aa to aa do
      begin
         cc:=sqrt(bb*bb+h*h)*d;
{*************************************************************************}
{         d1:=5*exp(-cc)*sin(2*3.14159266*cc); }
          d1:=cos(2*cc)+cos(5*cc);
{*************************************************************************}
         dd:=d1*c;
         Fundos;
      end;
      h:=h+trunc(b);
   until (h=a);
   _orig:=$b800;
   _dest:=$8800+($400*(frame-1));
   MovePagina;
end;
{-------------------------------------------------------------------------}
Procedure Animate;
begin
   for frame:=1 to 5 do
   begin
      _dest:=$b800;
      _orig:=$8800+($400*(frame-1));
      MovePagina;
      delay(50);
   end;
   for frame:=6 downto 2 do
   begin
      _dest:=$b800;
      _orig:=$8800+($400*(frame-1));
      MovePagina;
      delay(50);
   end;
end;
{-------------------------------------------------------------------------}
begin
   clrscr;
   frame:=1;
   writeln;
   writeln;
   writeln('Para expandir ou comprimir o desenho verticalmente, entre um numero');
   write  ('entre -40 e 40 (20 e'' tipico).'); readln(n1);
   writeln;
   writeln;
   writeln('Para mover o desenho para cima ou para baixo na tela, entre um numero');
   write  ('entre -50 e 150 (90 e'' tipico).'); readln(n2);
   a:=144;
   b:=2.25;
   c:=n1;
   d:=0.0327;
   e:=160;
   f:=n2;
   g:=199;
   gd:=detect;
   initgraph(gd,gm,'');
   c:=0;
   repeat
      Criar;
      inc(frame);
      c:=c+(n1/6);
   until(c=n1);
   repeat
      Animate;
   until keypressed;
end.


