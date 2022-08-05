Program cubo;
{$N+ $G}

 Uses Graph, Nce3D,crt;

 Var c: char;x,y,z:real;i:integer;

Procedure triang;
begin
    move3d ( 0,0,0);
    Risca3d (0,15,0);
    risca3d (15,0,0);
    risca3d (0,0,0);
end;


Begin
   InicVideo3D ( VGADefault);
   triang;
   x:=0;y:=0;z:=0;
   c:=' ';
   While c <> #13 Do
      Begin
         Case c of
              'x','X':Rodax3D ( 20*pi/180);
              'y','Y':RodaY3D ( 20*pi/180);
              'z','Z':RodaZ3D ( 20*pi/180);
         End;
{              Rodax3D ( 20*pi/180);
              RodaY3D ( 20*pi/180);
              RodaZ3D ( 20*pi/180);
}         c:=readkey;
         SetActivePage ( i and 1 );
{         clearDevice;}
         triang;
         i:=i+1;
         If i > 36 then i:=0;
         SetVisualPage ( i and 1 );
   End;

   Readln;
   termina3d;
end.

