{-------------------------------------------------------------}
{                                                             }
{                                                             }
{    Fractal - Rotinas de Manuseio de Fractais                }
{                                                             }
{            Unit FRACT                                       }
{                                                             }
{                                                             }
{    Versao 0.1 ( Outubro de 1993 )                           }
{                                                             }
{                                                             }
{    Desenvolvido por :                                       }
{        Dieferson Luis Alves de Araujo                       }
{        CEP 82600-460                                        }
{        Tel. (041) 256-8020                                  }
{        Fax. (041) 256-5900 das 8:00 as 15:00                }
{        E-mail: dlaa93@inf.ufpr.br                           }
{                                                             }
{                                                             }
{-------------------------------------------------------------}

Unit Fract;

Interface

Const
   Nrand = 4;

Var
    Arand    ,
    GaussAdd ,
    GaussFac : Real;
    delta : Array [0..100] of Real;
{-------------------------------------------------------------}
{                    Procedures Globais                       }
{-------------------------------------------------------------}

Procedure InitGauss ( NumRandomico : Word );
Function Gauss : Real;
Procedure MidPointRecursion ( X : Array of Real; index1, index2 , level, maxLevel : Integer );

{-------------------------------------------------------------}
{                  Definicao de Procedures                    }
{-------------------------------------------------------------}

implementation

Procedure InitGauss;

Begin
   Randomize;
   Arand := Random ( NumRandomico )-1;
   GaussAdd := Sqrt ( 3 * Nrand );
   GaussFac := 2 * GaussAdd/ (Nrand * Arand);
End;

Function Gauss:Real;

Var sum : Real;
    i   : Integer;

Begin
   sum := 0;
   For i:= 1 to Nrand Do
       sum := sum + Random;
   Gauss := (GaussFac * sum) - GaussAdd;
End;

Procedure MidPointRecursion ( X : Array of Real; index1, index2 ,level, maxLevel : Integer );

Var index0,n : Integer;


Begin
   Index0 := (index1+index2) div 2;
   x[index0]:=0.5*(x[index1]+x[Index2])+delta[level]*Gauss;
   If (Level < maxlevel) then
      Begin
         n:=level+1;
         MidPointRecursion ( X, index1, index0, n, maxlevel);
         MidPointRecursion ( X, index0, index2, n, maxlevel);
      end;
End;

Begin
   { Valores Default }
   InitGauss ( 1 );
End.


