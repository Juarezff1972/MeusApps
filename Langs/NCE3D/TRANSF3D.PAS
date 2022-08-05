{-------------------------------------------------------------}
{                                                             }
{    NCE3D - Rotinas de Manuseio Tridimensional               }
{                                                             }
{            Unit NCE3D                                       }
{                                                             }
{            Arquivo include TRANSF3D.PAS                     }
{            Modulo de Transformacoes Tridimensionais         }
{                                                             }
{                                                             }
{    Versao 2.7 ( Agosto de 1993 )                            }
{                                                             }
{                                                             }
{    Desenvolvido pelo:                                       }
{        Grupo de Computacao Grafica da DAU                   }
{        NCE/UFRJ                                             }
{        Caixa Postal 2324 - Rio de Janeiro - RJ              }
{        CEP 20001-970                                        }
{        Tel. (021) 598-3117                                  }
{        Fax. (021) 270-8554                                  }
{        E-mail: mauricio@nce.ufrj.br                         }
{                                                             }
{                                                             }
{    Autores:  Jose' Antonio Borges                           }
{              Mauricio Nunes da Costa Bomfim                 }
{                                                             }
{                                                             }
{-------------------------------------------------------------}


{-------------------------------------------------------------}
{              multiplica a matriz atual por outra            }
{              ( MatAtual := MatAtual * m )                   }
{-------------------------------------------------------------}

procedure MultMat3d (m: MATTRANSF);
var l, c: integer;
    aux: MATTRANSF;
begin
    if ModoTransf = NORMAL then
         for l := 1 to 4 do
             for c := 1 to 4 do
                  aux [l,c] := MatAtual [l,1] * m[1,c]  +
                               MatAtual [l,2] * m[2,c]  +
                               MatAtual [l,3] * m[3,c]  +
                               MatAtual [l,4] * m[4,c]
    else
         for l := 1 to 4 do
             for c := 1 to 4 do
                  aux [l,c] := m[l,1] * MatAtual [1,c]  +
                               m[l,2] * MatAtual [2,c]  +
                               m[l,3] * MatAtual [3,c]  +
                               m[l,4] * MatAtual [4,c];
    matatual := aux;
end;


{-------------------------------------------------------------}
{              Inicializa matriz de transformacoes            }
{-------------------------------------------------------------}

procedure InicTransf3d;
begin
   MatAtual:= MatIdent;
end;

{-------------------------------------------------------------}
{             Seleciona um modo de transformacoes             }
{-------------------------------------------------------------}

procedure SelecModoTransf3d ( modo:integer );
begin
   if (modo=NORMAL) or (modo=HIERARQUICO) then
        ModoTransf:= Modo;
end;


{-------------------------------------------------------------}
{               aplica a transf. do olho a um ponto           }
{-------------------------------------------------------------}

procedure AplicaTransfOlho (x,y,z: real;  var xm,ym,zm: real);
begin
    xm := x * MatOlho [1,1] +
          y * MatOlho [2,1] +
          z * MatOlho [3,1] +
              MatOlho [4,1];
    ym := x * MatOlho [1,2] +
          y * MatOlho [2,2] +
          z * MatOlho [3,2] +
              MatOlho [4,2];
    zm := x * MatOlho [1,3] +
          y * MatOlho [2,3] +
          z * MatOlho [3,3] +
              MatOlho [4,3];
end;


{-------------------------------------------------------------}
{               aplica a transf. atual a um ponto             }
{-------------------------------------------------------------}

procedure AplicaTransfObjeto3d (x,y,z: real;  var xm,ym,zm: real);
begin
    xm := x * matatual [1,1] +
          y * matatual [2,1] +
          z * matatual [3,1] +
              matatual [4,1];
    ym := x * matatual [1,2] +
          y * matatual [2,2] +
          z * matatual [3,2] +
              matatual [4,2];
    zm := x * matatual [1,3] +
          y * matatual [2,3] +
          z * matatual [3,3] +
              matatual [4,3];
end;

{-------------------------------------------------------------}
{               aplica a transf. Olho + Objeto                }
{-------------------------------------------------------------}

procedure AplicaTransf3d (x,y,z: real;  var xm,ym,zm: real);
var
    x1, y1, z1: real;
begin
    AplicaTransfObjeto3d ( x,y,z, x1,y1,z1 );
    AplicaTransfOlho   ( x1,y1,z1, xm,ym,zm );
end;

{-------------------------------------------------------------}
{              Empilha matriz de transformacoes               }
{-------------------------------------------------------------}

procedure SalvaTransf3d;
var
   p: PPILHA;

begin
   new(p);
   p^.m:= MatAtual;
   p^.ptr:= PilhaMat;
   PilhaMat:= p;
end;


{-------------------------------------------------------------}
{              Desempilha matriz de transformacoes            }
{-------------------------------------------------------------}

procedure RecuperaTransf3d;
var
   p: PPILHA;

begin
   if PilhaMat = NIL then
       begin
         Termina3d;
         writeln ('*** Pilha de Matrizes de Transformacoes vazia ***');
         writeln ('*** Programa Cancelado ***');
         halt;
       end
   else
       begin
         p:= PilhaMat;
         MatAtual := p^.m;
         PilhaMat := p^.ptr;
         dispose (p);
       end
end;


{-------------------------------------------------------------}
{               aplica translacao 'a matriz atual             }
{-------------------------------------------------------------}

procedure Translada3d (tx, ty, tz: real);
var
   mattr: MATTRANSF;
begin
    mattr := matIdent;
    mattr[4,1] := tx;
    mattr[4,2] := ty;
    mattr[4,3] := tz;
    MultMat3d (mattr);
end;

{-------------------------------------------------------------}
{                aplica rotacoes 'a matriz atual              }
{-------------------------------------------------------------}

procedure RodaX3d (ang: real);
var
    matrot: MATTRANSF;
begin
    matrot := matIdent;
    matrot[2,2] := cos(ang);
    matrot[2,3] := sin(ang);
    matrot[3,2] := -sin(ang);
    matrot[3,3] := cos(ang);
    MultMat3d (matrot);
end;

{-------------------------------------------------------------}

procedure RodaY3d (ang: real);
var
    matrot: MATTRANSF;
begin
    matrot := matIdent;
    matrot[1,1] := cos(ang);
    matrot[1,3] := -sin(ang);
    matrot[3,1] := sin(ang);
    matrot[3,3] := cos(ang);
    MultMat3d (matrot);
end;

{-------------------------------------------------------------}

procedure RodaZ3d (ang: real);
var
    matrot: MATTRANSF;
begin
    matrot := matIdent;
    matrot[1,1] := cos(ang);
    matrot[1,2] := sin(ang);
    matrot[2,1] := -sin(ang);
    matrot[2,2] := cos(ang);
    MultMat3d (matrot);
end;

{-------------------------------------------------------------}
{               aplica escalas 'a matriz atual                }
{-------------------------------------------------------------}

procedure Escala3d (sx, sy, sz: real);
var
    matescala: MATTRANSF;
begin
     matescala := matIdent;
     matescala[1,1] := sx;
     matescala[2,2] := sy;
     matescala[3,3] := sz;
     MultMat3d (matescala);
end;

{-------------------------------------------------------------}
{      define matriz de transformacao para o observador       }
{-------------------------------------------------------------}

procedure Observador3d ( x1,y1,z1, x2,y2,z2: real );

var
   a,b,c, v1,v2: real;
   m: MATTRANSF;
   modo: integer;

begin

    SalvaTransf3d;                { Salva a MatAtual }
    modo:= ModoTransf;            { Salva modo de transformacoes }
    SelecModoTransf3d ( NORMAL ); { Transf. do olho sempre usam modo NORMAL }

    InicTransf3d;

    a:= x1-x2;
    b:= y1-y2;
    c:= z1-z2;

    v1:= sqrt( a*a + c*c );
    v2:= sqrt( v1*v1 + b*b );

    translada3d ( -x1, -y1, -z1 );

    if v1<>0 then
       begin        { Rotacao em relacao ao eixo Y }
         m := MatIdent;
         m[1,1] := -c/v1;         {  cos(ang) }
         m[3,3] := m[1,1];
         m[1,3] := -a/v1;         { -sen(ang) }
         m[3,1] := -m[1,3];
         MultMat3d (m);
       end;

    if v2<>0 then
       begin        { Rotacao em relacao ao eixo X }
	 m := MatIdent;
         m[2,2] := v1/v2;         { cos(ang) }
         m[3,3] := m[2,2];
         m[2,3] := -b/v2;         { sen(ang) }
         m[3,2] := -m[2,3];
    	 MultMat3d (m);
       end;

    MatOlho:= MatAtual;
    SelecModoTransf3d (modo);     { Recupera o modo de transf. corrente }
    RecuperaTransf3d;             { Recupera a MatAtual }

end;

