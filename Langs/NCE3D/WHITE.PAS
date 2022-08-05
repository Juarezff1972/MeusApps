Program WhiteNoiseBM;

Uses Fract,Graph,Nce3d;

Var N : Integer;

Procedure WhiteNoise;
Var
   x : Array[0..N] of Real;
   i : Integer;
   y:Real;

Begin
   InicVideo3d ( SvgaHi256 );
   x[0] := 0;
   y:=0;
   InitGauss ( N );
   For i := 1 to N-1 Do
      Begin
         x[i] := x[i-1]+Gauss/(200-1);
         PutPixel3d ( x[i], y, 0, AMARELO );
         If i < N/2 then
              y:=y+0.1
         Else
              y:=y-0.1;
      End;
End;

Begin
   Write ( 'Entre com o numero de valores de X : ');
   ReadLn (N);
   WhiteNoise;
   ReadLn;
   Termina3d
End.


