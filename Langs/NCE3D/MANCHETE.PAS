Program Manchete;

{$N+ $G}

Uses Nce3d, Graph, UTNce3d;


Begin
   InicVideo3D ( SVGAHI256 );
   InicTransf3D;
   InicZBuffer3D ( GetMaxY+1, 0, 'D:' );
   SelecLuz3D ( 0, 100, 100);
   SelecCorPolig3D ( DOURADO );
   Observador3D ( 0,0,150, 0,0,0 );
   InicVertDegrade (1,0,0,Azul);
   InicVertDegrade (2,0,GetMaxY,Vermelho);
   InicVertDegrade (3,GetMaxX,GetMaxY,Vermelho);
   InicVertDegrade (4,GetMaxX,0,Azul);
   PintaPoligDegrade (4);
   Esfera3D ( 4, 4 );
   Translada3D ( 10,10,0 );  { Bolas de tras em cima }
   Esfera3D ( 4, 4);
   Escala3D ( -1, 1, 1);
   Esfera3D ( 4, 4);
   InicTransf3D;
   Translada3D ( 16,-10, 0);  { Bolas da frente em baixo }
   Esfera3D ( 4, 4);
   Escala3D ( -1, 1, 1);
   Esfera3D ( 4, 4);
   InicTransf3D;
   RodaX3D (-3.14159/2);
   RodaZ3D (-3.14159/4);
   Cilindro3D ( 8, 1.25, Sqrt(Sqr(10)+Sqr(10)) );
   Escala3D ( -1, 1, -1);
   Cilindro3D ( 8, 1.25, Sqrt(Sqr(10)+Sqr(10)) );
   InicTransf3D;
   RodaX3D (3.14159/2);
   RodaZ3D (3.14159/10);
   Translada3D ( 10,10,0 );
   Cilindro3D ( 8, 1.25, Sqrt(Sqr(20)+Sqr(6)) );
   Escala3D ( -1, 1, -1);
   Cilindro3D ( 8, 1.25, Sqrt(Sqr(20)+Sqr(6)) );
   GravaPcx('Die',0,0,GetmaxX,GetMaxY);
   ReadLn;
   Termina3d;

End.
