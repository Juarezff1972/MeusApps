Program MidPointBM;

{$m 30000,0,655360}


Uses Graph, Nce3d, fract;


Var Delta,x : array [0..100] of real;
    sigma,y : Real;
    i,n,maxlevel:integer;

Begin
   Randomize;
   InicVideo3D ( SVGAHI256 );
   InitGauss(100);
   Maxlevel := 100;
   For i := 1 to Maxlevel do
       delta[i]:= sigma * Random (Round(3.14159*((i+1)/2)));
   n:= Random ( MaxLevel );
   y:=0;
   x[0] := 0;
   x[n] := sigma*Gauss;
   MidPointRecursion (x, 0 , n , 1 , maxlevel );
   For i := 1 to maxlevel do
      Begin
         PutPixel3d ( x[i], y, 0 , 1);
         y:=y+0.1;
      End;
   ReadLn;
   Termina3d
End.


