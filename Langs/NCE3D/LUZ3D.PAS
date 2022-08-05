{-------------------------------------------------------------}
{                                                             }
{    NCE3D - Rotinas de Manuseio Tridimensional               }
{                                                             }
{            Unit NCE3D                                       }
{                                                             }
{            Arquivo include LUZ3D.PAS                        }
{            Modulo de Calculo de Iluminacao e Coloracao      }
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
{              Mauricio Bomfim                                }
{                                                             }
{-------------------------------------------------------------}


{-------------------------------------------------------------}
{          seleciona cor basica dos proximos poligonos        }
{-------------------------------------------------------------}

procedure SelecCorPolig3d ( cor:integer );
   begin
       if (cor >= 0) and (cor < NCORESRGB) then
           with CoresRGB [cor] do
               begin
                   corRpoli:= r;
                   corGpoli:= g;
                   corBpoli:= b;
               end;
   end;

{-------------------------------------------------------------}
{         seleciona cor basica dos proximos poligonos         }
{-------------------------------------------------------------}

procedure SelecRGBPolig3d (corR, corG, corB: real);
   begin
       if corR > 1.0 then corR:= 1.0;
       if corR < 0.0 then corR:= 0;
       corRpoli := trunc (corR * 255);

       if corG > 1.0 then corG:= 1.0;
       if corG < 0.0 then corG:= 0;
       corGpoli := trunc (corG * 255);

       if corB > 1.0 then corB:= 1.0;
       if corB < 0.0 then corB:= 0;
       corBpoli := trunc (corB * 255);
   end;

{-------------------------------------------------------------}
{                     normaliza um vetor                      }
{-------------------------------------------------------------}

procedure Normaliza ( var nx,ny,nz:real );
    var
       norma:real;
    begin
       norma:= sqrt ( nx*nx + ny*ny + nz*nz );
       if norma<>0 then
          begin
            nx:= nx/norma;
            ny:= ny/norma;
            nz:= nz/norma;
          end;
    end;

{-------------------------------------------------------------}
{          seleciona intensidade de luz ambiente              }
{-------------------------------------------------------------}

procedure SelecLuzAmbiente3d ( luz:real );
   begin
     if luz > 1.0 then luz := 1.0;
     if luz < 0.0 then luz := 0.0;
     luzAmbiente := luz;
   end;

{-------------------------------------------------------------}
{            determina posicao do ponto de luz                }
{-------------------------------------------------------------}

procedure SelecLuz3d ( lx, ly, lz: real );
   begin
     luzx := lx;
     luzy := ly;
     luzz := lz;
   end;

{-------------------------------------------------------------}
{         calcula normal ao poligono dados os vertices        }
{-------------------------------------------------------------}

procedure CalcNormPlano3d ( nv:integer; var v; var nx,ny,nz:real );
    var
        a,b,c,
        xi,yi,zi,
        xj,yj,zj: real;
        i: integer;

    begin
        a:= 0.0; b:= 0.0; c:= 0.0;
        for i:= 1 to nv do
            begin
	        xi := POLIG(v)[i].x;
	        yi := POLIG(v)[i].y;
	        zi := POLIG(v)[i].z;
                if i = nv then
                    begin
	                xj := POLIG(v)[1].x;
	                yj := POLIG(v)[1].y;
	                zj := POLIG(v)[1].z;
                    end
                else
                    begin
	                xj := POLIG(v)[i+1].x;
	                yj := POLIG(v)[i+1].y;
	                zj := POLIG(v)[i+1].z;
                    end;

	        a:= a + (yi-yj)*(zi+zj);
	        b:= b + (zi-zj)*(xi+xj);
	        c:= c + (xi-xj)*(yi+yj);
            end;

        {  d := -( a*xi + b*yi + c*zi );  }

        nx:= a; ny:= b; nz:= c;
        Normaliza ( nx,ny,nz );
    end;

{-------------------------------------------------------------}
{         calcula cor de acordo com a lei de Lambert          }
{-------------------------------------------------------------}

procedure CalcCor3d (nx, ny, nz:real; var cor:real);
   var
     x0, y0, z0,
     lx, ly, lz,
     ntx, nty, ntz: real;

   begin
     { Transformo a origem }
     AplicaTransf3d ( 0,0,0, x0,y0,z0 );

     { Transformo vetor normal }
     AplicaTransf3d ( nx,ny,nz, ntx,nty,ntz );
     ntx:= ntx-x0; nty:= nty-y0; ntz:= ntz-z0;
     Normaliza ( ntx,nty,ntz );

     { Transforma posicao da luz }
     AplicaTransfOlho ( luzx,luzy,luzz, lx,ly,lz );
     lx:= lx-x0; ly:= ly-y0; lz:= lz-z0;
     Normaliza (lx,ly,lz);

     { Calculo o produto escalar }
     cor := ntx*lx + nty*ly + ntz*lz;
     if cor < 0 then
         cor := luzAmbiente
     else
         begin
            cor := luzambiente + cor;
            if cor > 1.0 then cor := 1.0;
         end;
   end;

