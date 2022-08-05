Program Bird;

uses Graph,Crt;

type
   frame1 = array[1..56,1..2]  of integer;
   frame2 = array[1..28,1..2]  of integer;
   frame3 = array[1..17,1..2]  of integer;
   frame4 = array[1..11,1..2]  of integer;
   frame5 = array[1..35,1..2]  of integer;
   frame6 = array[1..22,1..2]  of integer;
   frame7 = array[1..18,1..2]  of integer;
   frame8 = array[1..17,1..2]  of integer;
   frame9 = array[1..17,1..2]  of integer;
   frame10= array[1..17,1..2]  of integer;
   frame11= array[1..14,1..2]  of integer;

const
   array1:frame1=((205,219),(208,218),(212,214),(218,212),(221,208),(229,197),
                  (232,192),(241,187),(250,183),(258,183),(265,185),(273,189),
                  (282,198),(377,180),(376,182),(310,200),(297,207),(289,211),
                  (286,217),(282,221),(278,230),(269,238),(256,248),(249,253),
                  (240,261),(235,269),(231,278),(228,280),(214,294),(199,304),
                  (188,310),(181,317),(177,324),(176,321),(169,328),(168,325),
                  (162,330),(162,326),(135,340),(126,345),(120,345),(111,346),
                  (106,349),( 58,390),( 49,399),( 42,405),( 42,402),( 51,391),
                  ( 74,364),( 87,353),( 99,339),(107,324),(114,309),(127,289),
                  (138,274),(146,259));

   array2:frame2=((146,259),(143,246),(136,234),(128,221),(110,199),( 98,184),
                  ( 88,169),( 79,144),( 76,136),( 74,118),( 73, 98),( 69, 72),
                  ( 74, 69),( 80, 71),( 86, 74),(107,101),(119,112),(124,118),
                  (143,139),(154,154),(161,166),(166,171),(181,188),(187,195),
                  (193,200),(197,209),(205,219),(179,259));

   array3:frame3=((119,112),(118, 99),(118, 87),(121, 59),(121, 52),(126, 49),
                  (134, 47),(139, 62),(146, 73),(154, 88),(174,129),(187,170),
                  (194,184),(199,197),(207,209),(212,214),(139, 99));

   array4:frame4=((248,199),(250,196),(258,195),(261,197),(263,200),(261,204),
                  (258,207),(252,205),(250,203),(248,199),(257,200));

   array5:frame5=((212,214),(218,212),(224,212),(222,217),(239,217),(237,219),
                  (245,222),(259,219),(258,222),(269,220),(268,224),(278,220),
                  (278,224),(282,221),(278,230),(269,238),(256,248),(249,253),
                  (240,261),(240,258),(234,261),(234,257),(232,258),(232,253),
                  (226,252),(227,249),(219,248),(222,244),(219,242),(220,240),
                  (211,238),(214,230),(211,228),(212,214),(239,239));

   array6:frame6=((146,259),(140,250),(129,245),(109,234),( 92,219),( 82,209),
                  ( 77,193),( 73,179),( 74,159),( 78,146),( 84,136),( 96,138),
                  (119,147),(129,157),(140,165),(152,178),(159,184),(175,199),
                  (189,210),(194,213),(205,219),( 85,179));

   array7:frame7=((152,178),(151,156),(151,137),(154,121),(156,113),(164, 98),
                  (166, 89),(170, 84),(178, 93),(180,100),(185,107),(188,127),
                  (191,148),(193,169),(197,179),(202,199),(212,214),(177,119));

   array8:frame8=((146,259),(119,258),(104,253),( 98,249),( 93,241),( 90,237),
                  ( 90,228),( 94,204),( 97,198),(106,193),(124,190),(137,189),
                  (151,195),(163,199),(175,208),(205,219),(109,242));

   array9:frame9=((163,199),(163,184),(169,171),(169,152),(173,144),(178,138),
                  (187,127),(194,119),(199,118),(202,125),(203,134),(204,162),
                  (207,179),(209,195),(210,207),(212,214),(199,139));

 array10:frame10=((146,259),(137,270),(124,277),(118,277),(114,276),(111,269),
                  (112,260),(114,258),(125,246),(127,237),(139,228),(163,219),
                  (174,218),(179,218),(191,219),(205,219),(119,269));

 array11:frame11=((174,218),(178,203),(185,185),(192,177),(199,169),(211,160),
                  (224,157),(227,161),(227,169),(226,177),(224,188),(217,205),
                  (212,214),(219,179));


var
   frame,t1,t2,c0,c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15 : integer;
   sx,sy,sx1,sy1,sx2,sy2,Xres,Yres,pl,tX,tY,Ax,Ay,Bx,By : integer;
   Scr : Word;
   One,Two,Three,Four : Pointer;

{-------------------------------------------------------------}
Procedure SetTextPosition(x,y : integer);
begin
   tX:=trunc(x*(Xres/80))-8;
   tY:=trunc(y*(Yres/25))-8;
end;
{-------------------------------------------------------------}
Procedure Animation;
begin
   repeat
      PutImage(Ax,Ay,Two^,NormalPut);      Delay(1);
      PutImage(Ax,Ay,Three^,NormalPut);    Delay(1);
      PutImage(Ax,Ay,Four^,NormalPut);     Delay(1);
      PutImage(Ax,Ay,Three^,NormalPut);    Delay(1);
      PutImage(Ax,Ay,Two^,NormalPut);      Delay(1);
      PutImage(Ax,Ay,One^,NormalPut);      Delay(1);
   until keypressed;
   FreeMem(One,Scr);
   FreeMem(Two,Scr);
   FreeMem(Three,Scr);
   FreeMem(Four,Scr);
end;
{-------------------------------------------------------------}
Procedure Labels;
begin
   SetColor(c7);
   SetTextPosition(1,25);
   OutTextXY(tX,tY,'Highest Graphics Mode');
   SetTextPosition(21,1);
   OutTextXY(tX,tY,'USING PASCAL FOR HIGH SPEED FRAME ANIMATION');
   SetTextPosition(51,25);
   OutTextXY(tX,tY,'Press any key to quit.');
   SetColor(c14);
   SetTextPosition(53,5);
   OutTextXY(tX,tY,'Ruby-Throated Hummingbird');
   SetColor(c7);
   SetTextPosition(55,7);
   OutTextXY(tX,tY,'(Archilochus colubirs)');
   SetTextPosition(51,11);
   OutTextXY(tX,tY,'Range: Central & eastern U.S.');
   SetTextPosition(51,13);   OutTextXY(tX,tY,'Breeds from southern');
   SetTextPosition(51,14);   OutTextXY(tX,tY,'Canada to the Gulf Coast.');
   SetTextPosition(51,16);   OutTextXY(tX,tY,'Depicted here in its');
   SetTextPosition(51,17);   OutTextXY(tX,tY,'characteristic hovering');
   SetTextPosition(51,18);   OutTextXY(tX,tY,'flight pattern.');
end;
{-------------------------------------------------------------}
Procedure Coords;
begin
   sx:=trunc(sx*(Xres/640));
   sy:=trunc(sy*(Yres/480));
end;
{-------------------------------------------------------------}
Procedure DrawBird;
begin
   SetColor(c7);
   sx:=0;   sy:=30;  coords;  sx1:=sx;  sy1:=sy;
   sx:=385; sy:=458; coords;  sx2:=sx;  sy2:=sy;
   Ax:=sx1;  Bx:=sx2;
   Ay:=sy1;  By:=sy2;
   SetFillStyle(SolidFill,c7);
   Bar(sx1,sy1,sx2,sy2);

   SetColor(c0);
   SetFillStyle(SolidFill,c0);

   pl:=1;
   sx:=array1[pl,1];  sy:=array1[pl,2];
   pl:=pl+1;
   coords;
   moveto(sx,sy);
   for t1:=1 to 55 do
   begin
      sx:=array1[pl,1];   sy:=array1[pl,2];
      pl:=pl+1;
      coords;
      lineto(sx,sy);
   end;

   if frame=0 then
   begin
      pl:=1;
      sx:=array2[pl,1];   sy:=array2[pl,2];
      pl:=pl+1;
      coords;
      moveto(sx,sy);
      for t1:=1 to 26 do
      begin
         sx:=array2[pl,1];
         sy:=array2[pl,2];
         pl:=pl+1;
         coords;
         lineto(sx,sy);
      end;
      sx:=array2[pl,1];   sy:=array2[pl,2];
      coords;
      FloodFill(sx,sy,c0);
      pl:=1;
      sx:=array3[pl,1];   sy:=array3[pl,1];
      pl:=pl+1;
      coords;
      moveto(sx,sy);
      for t1:=1 to 15 do
      begin
         sx:=array3[pl,1];
         sy:=array3[pl,2];
         pl:=pl+1;
         coords;
         lineto(sx,sy);
      end;
      sx:=array3[pl,1];   sy:=array3[pl,2];
      coords;
      floodfill(sx,sy,c0);
   end;

   if frame=1 then
   begin
      pl:=1;
      sx:=array6[pl,1];   sy:=array6[pl,2];
      pl:=pl+1;
      coords;
      moveto(sx,sy);
      for t1:=1 to 20 do
      begin
         sx:=array6[pl,1];
         sy:=array6[pl,2];
         pl:=pl+1;
         coords;
         lineto(sx,sy);
      end;
      sx:=array6[pl,1];   sy:=array6[pl,2];
      coords;
      FloodFill(sx,sy,c0);
      pl:=1;
      sx:=array7[pl,1];   sy:=array7[pl,2];
      pl:=pl+1;
      coords;
      moveto(sx,sy);
      for t1:=1 to 16 do
      begin
         sx:=array7[pl,1];
         sy:=array7[pl,2];
         pl:=pl+1;
         coords;
         lineto(sx,sy);
      end;
      sx:=array7[pl,1];   sy:=array7[pl,2];
      coords;
      floodfill(sx,sy,c0);
   end;
   if frame=2 then
   begin
      pl:=1;
      sx:=array8[pl,1];   sy:=array8[pl,2];
      pl:=pl+1;
      coords;
      moveto(sx,sy);
      for t1:=1 to 15 do
      begin
         sx:=array8[pl,1];
         sy:=array8[pl,2];
         pl:=pl+1;
         coords;
         lineto(sx,sy);
      end;
      sx:=array8[pl,1];   sy:=array8[pl,2];
      coords;
      FloodFill(sx,sy,c0);
      pl:=1;
      sx:=array9[pl,1];   sy:=array9[pl,2];
      pl:=pl+1;
      coords;
      moveto(sx,sy);
      for t1:=1 to 15 do
      begin
         sx:=array9[pl,1];
         sy:=array9[pl,2];
         pl:=pl+1;
         coords;
         lineto(sx,sy);
      end;
      sx:=array9[pl,1];   sy:=array9[pl,2];
      coords;
      floodfill(sx,sy,c0);
   end;
   if frame=3 then
   begin
      pl:=1;
      sx:=array10[pl,1];   sy:=array10[pl,2];
      pl:=pl+1;
      coords;
      moveto(sx,sy);
      for t1:=1 to 15 do
      begin
         sx:=array10[pl,1];
         sy:=array10[pl,2];
         pl:=pl+1;
         coords;
         lineto(sx,sy);
      end;
      sx:=array10[pl,1];   sy:=array10[pl,2];
      coords;
      FloodFill(sx,sy,c0);
      pl:=1;
      sx:=array11[pl,1];   sy:=array11[pl,2];
      pl:=pl+1;
      coords;
      moveto(sx,sy);
      for t1:=1 to 12 do
      begin
         sx:=array11[pl,1];
         sy:=array11[pl,2];
         pl:=pl+1;
         coords;
         lineto(sx,sy);
      end;
      sx:=array11[pl,1];   sy:=array11[pl,2];
      coords;
      floodfill(sx,sy,c0);
   end;
   setcolor(c7);
   pl:=1;
   sx:=array4[pl,1];   sy:=array4[pl,2];
   pl:=pl+1;
   coords;
   moveto(sx,sy);
   for t1:=1 to 9 do
   begin
      sx:=array4[pl,1];
      sy:=array4[pl,2];
      pl:=pl+1;
      coords;
      lineto(sx,sy);
   end;
   sx:=array4[pl,1];
   sy:=array4[pl,2];
   coords;
   putpixel(sx,sy,c7);
   setcolor(c4);
   pl:=1;
   sx:=array5[pl,1];   sy:=array5[pl,2];
   pl:=pl+1;
   coords;
   moveto(sx,sy);
   for t1:=1 to 9 do
   begin
      sx:=array5[pl,1];
      sy:=array5[pl,2];
      pl:=pl+1;
      coords;
      lineto(sx,sy);
   end;
   sx:=array5[pl,1];
   sy:=array5[pl,2];
   coords;
   floodfill(sx,sy,c4);
   setcolor(c7);
end;
{-------------------------------------------------------------}
Procedure GraphicsSetup;
var gd,gm : integer;
begin
   gd:=cga;
   gm:=cgahi;
   initgraph(gd,gm,'');
   ClearDevice;
   xres:=getmaxx+1;
   yres:=getmaxy+1;
   c0:=0;  c1:=1;  c2:=2;  c3:=1;  c4:=0;  c5:=1; c6:=1; c7:=1; c9:=1;
   c10:=1; c11:=1; c12:=1; c13:=1; c14:=1; c15:=1;
end;
{-------------------------------------------------------------}
begin
   c0:=0;   c1:=1;   c2:=2;   c3:=3;   c4:=4;   c5:=5;   c6:=6;
   c7:=7;   c8:=8;   c9:=9;  c10:=10; c11:=11; c12:=12; c13:=c13;
                    c14:=14;          c15:=15;
   frame:=0;
   t1:=1;


   GraphicsSetup;
   SetColor(c7);

   ClearDevice;
   frame:=0;
   Labels;
   DrawBird;
   Scr:=ImageSize(Ax,Ay,Bx,By);
   GetMem(One,Scr);
   GetImage(Ax,Ay,Bx,By,One^);

   frame:=1;
   DrawBird;
   GetMem(Two,Scr);
   GetImage(Ax,Ay,Bx,By,Two^);

   frame:=2;
   DrawBird;
   GetMem(Three,Scr);
   GetImage(Ax,Ay,Bx,By,Three^);

   frame:=3;
   DrawBird;
   GetMem(Four,Scr);
   GetImage(Ax,Ay,Bx,By,Four^);

   PutImage(Ax,Ay,One^,NormalPut);
   Delay(3000);
   Animation;
   CloseGraph;
end.
