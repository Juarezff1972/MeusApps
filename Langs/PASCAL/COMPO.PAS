Unit Compo;

INTERFACE

Uses Graph, Crt, Figures;

Type

{  Mostrador = object (Point)
    MostRaio:Integer;
    AngIni  :Real;
    Interv  :Real;
}

  Aro = object(Point)
    CircIn:Circle;
    CircEx:Circle;
    TintaX,TintaY:Integer;
    Constructor Init(InitX,InitY,Raio:Integer);
    Procedure Show; virtual;
    Procedure Hide; virtual;
  end;

  MarcaRadial = object (Raio)
    DXR1,DYR1:Real;
    DXR2,DYR2:Real;
    Constructor Init(InitX,InitY,InitRaio:Integer;InitFat:Real);
    Procedure Show; virtual;
    Procedure Hide; virtual;
    Procedure Pos(AngPos:Real);
    Procedure Sai(AngPos:Real);
  end;



IMPLEMENTATION

{--------------------------------------------------------}
{ Declara‡”es de M‚todos para Aro.                      }
{--------------------------------------------------------}

Constructor Aro.Init(InitX,InitY,Raio:Integer);
Begin
  Point.Init(InitX,InitY);
  CircIn.Init(InitX,InitY,Raio);
  CircEx.Init(InitX,InitY,Round(1.05*Raio));
  TintaX:=InitX+Round(1.03*Raio);
  TintaY:=InitY;
End;

Procedure Aro.Show;
Begin
  CircIn.Show;
  CircEx.Show;
  SetColor(GetMaxColor);
  FloodFill(TintaX,TintaY,GetMaxColor);
End;

Procedure Aro.Hide;
Begin
  CircIn.Hide;
  CircEx.Hide;
  SetColor(GetBkColor);
  FloodFill(TintaX,TintaY,GetBkColor);
End;

{--------------------------------------------------------}
{ Declara‡”es de M‚todos para MarcaRadial.               }
{--------------------------------------------------------}

Constructor MarcaRadial.Init(InitX,InitY,InitRaio:Integer;InitFat:Real);
Begin
  Raio.Init(InitX,InitY,0,InitRaio);
  DXR1:=1.0*InitRaio*(1-InitFat);
  DYR1:=0.5*InitRaio*(1-InitFat);
  DXR2:=1.0*InitRaio;
  DYR2:=0.5*InitRaio;
End;

Procedure MarcaRadial.Show;
Var
  DX1,DY1:Integer;
  DX2,DY2:Integer;
Begin
  Visible:=True;
  DX1:=Round(DXR1*Cos(Angulo-Pi/2));
  DY1:=Round(DYR1*Sin(Angulo-Pi/2));
  DX2:=Round(DXR2*Cos(Angulo-Pi/2));
  DY2:=Round(DYR2*Sin(Angulo-Pi/2));
  Graph.Line(X+DX1,Y+DY1,X+DX2,Y+DY2);
End;

Procedure MarcaRadial.Hide;
Var
  TempColor : Word;
  DX1,DY1:Integer;
  DX2,DY2:Integer;
Begin
  TempColor := Graph.GetColor;
  Graph.SetColor(GetBkColor);
  Visible := False;
  DX1:=Round(DXR1*Cos(Angulo-Pi/2));
  DY1:=Round(DYR1*Sin(Angulo-Pi/2));
  DX2:=Round(DXR2*Cos(Angulo-Pi/2));
  DY2:=Round(DYR2*Sin(Angulo-Pi/2));
  Graph.Line(X+DX1,Y+DY1,X+DX2,Y+DY2);
  SetColor(TempColor);
End;

Procedure MarcaRadial.Pos(AngPos:Real);
Begin
  Angulo:=AngPos;
  Show;
End;

Procedure MarcaRadial.Sai(AngPos:Real);
Begin
  Angulo:=AngPos;
  Hide;
End;

End.