Program Cubo;

Uses Graph, nce3d,crt;

Var i,j:Integer;c:char;

Procedure Cubo2d;
Begin
   Reta3d ( 0,0,0, 15,0,0 );
   Reta3d ( 15,0,0, 15,15,0 );
   Reta3d ( 15,15,0, 0,15,0 );
   Reta3d ( 0,15,0, 0,0,0 );
   Reta3d ( 0,0,15, 15,0,15 );
   Reta3d ( 15,0,15, 15,15,15 );
   Reta3d ( 15,15,15, 0,15,15 );
   Reta3d ( 0,15,15, 0,0,15 );
   Reta3d ( 0,0,0, 0,0,15 );
   Reta3d ( 15,0,0, 15,0,15 );
   Reta3d ( 15,15,0, 15,15,15 );
   Reta3d ( 0,15,0, 0,15,15 );
end;

Begin
   InicVideo3D ( 0 );
   Observador3d ( 50,50,100, 0,0,0 );
   SelecLuz3d ( 0,0,100 );
   i:=0;j:=0;
   While c <> #13 Do
      Begin
         Observador3d ( 50, 50-j, 100, 0,0 ,0 );
         SetVisualPage (i and 1);
         cleardevice;
         Cubo2d;
         SetActivePage (i and 1);
         j:=j+1;
         i:=i+10;
         if j > 300 then j:=j-1;
      End;
   ReadLn;
end.
