Program fastclock;

Uses Graph,Crt;

Label re;

Var ys,rh,rm,rs,mr,h2,m2,sr     : Real;
    h1,m1,s1,hd,md,sd,hr,s2     : Real;
    xh,xm,xs,yh,ym              : Real;
    x,y,h,m,s                   : Integer;
    x1,x2,x3,y1,y2,y3           : Real;
    gm,gd                       : Integer;

Begin
   directvideo:=false;
   gd:=CGA; gm:=CGAc1;
   InitGraph(gd,gm,'');
   ClearDevice;
   x:=160; y:=100; rh:=45; rm:=50; rs:=50;
   Circle(x,y,60);
   SetTextStyle(DefaultFont,HorizDir,1);
   OutTextXY(0,0,'Relogio de hora falsa');
   OutTextXY(0,180,'Copyright(C) 1990 by JFF');
   OutTextXY(0,190,'Unauthorized duplication permited.');
re:For h:=1 To 12 Do Begin
       For m:=1 To 60 Do Begin
           For s:=1 To 60 Do Begin
               h1:=hd;
               hd:=(360 / 12) * (h + (md / 360));
               m1:=md;
               md:=(360 / 60) * (m + (sd / 360));
               s1:=sd;sd:=(360 / 60) * s;
               h2:=(h1 + 90) * (Pi / 180);
               hr:=(hd + 90) * (Pi / 180);
               m2:=(m1 + 90) * (Pi / 180);
               mr:=(md + 90) * (Pi / 180);
               s2:=(s1 + 90) * (Pi / 180);
               sr:=(sd + 90) * (Pi / 180);
               xh:=((rh * Cos(hr)));
               yh:=( - (rh * Sin(hr)) / 1.1);
               xm:=((rm * Cos(mr)));
               ym:=( - (rm * Sin(mr)) / 1.1);
               xs:=((rs * Cos(sr)));
               ys:=( - (rs * Sin(sr)) / 1.1);
               x1:=((rh * Cos(h2)));
               y1:=( - (rh * Sin(h2)) / 1.1);
               x2:=((rm * Cos(m2)));
               y2:=( - (rm * Sin(m2)) / 1.1);
               x3:=((rs * Cos(s2)));
               y3:=( - (rs * Sin(s2)) / 1.1);
               SetColor(0);
               If (h1<>hd) then line(x,y,320 - Trunc(x+x1),Trunc(y+y1));
               If (m1<>md) then line(x,y,320 - Trunc(x+x2),Trunc(y+y2));
               If (s1<>sd) then line(x,y,320 - Trunc(x+x3),Trunc(y+y3));
               SetColor(2);
               line(x,y,320 - Trunc(x+xh),Trunc(y+yh));
               line(x,y,320 - Trunc(x+xm),Trunc(y+ym));
               line(x,y,320 - Trunc(x+xs),Trunc(y+ys));
               end;
           end;
   end;
   Goto re;
end.