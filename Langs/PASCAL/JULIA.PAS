Program JuliaIIM;

{$N+ $G}

Uses Graph, Nce3D;

   Var
      x,y,r,wx,wy : Real;
      theta,cx,cy : Real;
      i:Integer;

Begin
   Randomize;
   Write ( 'Entre com cx, cy :');
   ReadLn (cx,cy);
   Write ( 'Entre com x, y :' );
   ReadLn (x,y);
   InicVideo3D ( SVGAHI256 );
   For i := 1 to 6000 Do
      Begin
         wx:= x-cx;
         wy:= y-cy;
         If wx > 0 then theta := ArcTan ( wy/wx );
         If wx < 0 then theta := 3.14159 + ArcTan ( wy/wx );
         If wx = 0 then theta := 1.57079;
         theta:=theta/2;
         r:=Sqrt(Sqr(wx)+Sqr(wy));
         If Random (1) < 0.5 then
            r:=Sqrt(r)-0.2
         Else
            r:=-Sqrt(r)+0.2;
         x:= r * cos(theta);
         y:= r * sin(theta);
         SelecCorTraco3D ( AMARELO );
         Risca3D ( x, y, 0 );
   end;
   ReadLn;
   Termina3D
End.
{PutPixel3D ( x,y,z,cor );}