Program fastclock;

Uses Crt,ATI,Dos;

Label re;

Var ys,rh,rm,rs,mr,h2,m2,sr     : Real;
    h1,m1,s1,hd,md,sd,hr,s2     : Real;
    xh,xm,xs,yh,ym              : Real;
    x,y,h,m,s,z                 : Word;
    x1,x2,x3,y1,y2,y3           : Real;
    gm,gd                       : word;

Begin
   SetGraphicsMode(640,480);
   directvideo:=false;
   x:=MaxXRes div 2; y:=MaxYRes div 2; rh:=90; rm:=110; rs:=80;
   Circle(x,y,120,15);
   textcolor(15);   gotoxy(1,1);   writeln('Relogio');
   textcolor(14);   gotoxy(1,2);  writeln('Copyright(C) 1994 by JFF');
   textcolor(10);   gotoxy(1,3);  writeln('Unauthorized duplication permited.');
re:gettime(h,m,s,z);
   begin
      h1:=hd;      hd:=(360 / 12) * (h + (md / 360));
      m1:=md;      md:=(360 / 60) * (m + (sd / 360));
      s1:=sd;      sd:=(360 / 60) * s;
      h2:=(h1 + 90) * (Pi / 180);      hr:=(hd + 90) * (Pi / 180);
      m2:=(m1 + 90) * (Pi / 180);      mr:=(md + 90) * (Pi / 180);
      s2:=(s1 + 90) * (Pi / 180);      sr:=(sd + 90) * (Pi / 180);
      xh:=((rh * Cos(hr)));               yh:=( - (rh * Sin(hr)) );
      xm:=((rm * Cos(mr)));               ym:=( - (rm * Sin(mr)) );
      xs:=((rs * Cos(sr)));               ys:=( - (rs * Sin(sr)) );
      x1:=((rh * Cos(h2)));               y1:=( - (rh * Sin(h2)) );
      x2:=((rm * Cos(m2)));               y2:=( - (rm * Sin(m2)) );
      x3:=((rs * Cos(s2)));               y3:=( - (rs * Sin(s2)) );
      If (h1<>hd) then draw(x,y,MaxXRes - Trunc(x+x1),Trunc(y+y1),0);
      If (m1<>md) then draw(x,y,MaxXRes - Trunc(x+x2),Trunc(y+y2),0);
      If (s1<>sd) then draw(x,y,MaxXRes - Trunc(x+x3),Trunc(y+y3),0);
      draw(x,y,MaxXRes - Trunc(x+xh),Trunc(y+yh),13);
      draw(x,y,MaxXRes - Trunc(x+xm),Trunc(y+ym),12);
      draw(x,y,MaxXRes - Trunc(x+xs),Trunc(y+ys),11);
   end;
   if keypressed then begin setmode(3); exit; end;
   Goto re;
   setmode(3);
end.