{-------------------------------------------------------------}
{                                                             }
{    NCE3D - Rotinas de Manuseio Tridimensional               }
{                                                             }
{            Unit NCE3D                                       }
{                                                             }
{            Arquivo include ZBUF3D.PAS                       }
{            Modulo de Gerenciamento do Z-Buffer e            }
{            Pintura e Recorte de Poligonos 3D                }
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

{ $ define DEBUGZBUFFER }

{-------------------------------------------------------------}
{                multiplicacao gerando longint                }
{-------------------------------------------------------------}

function longmul (a,b: integer): longint;
begin
    longmul := trunc (1.0 * a * b);
end;

{-------------------------------------------------------------}
{    Calcula a cor da palette mais proxima a cor RGB dada     }
{-------------------------------------------------------------}

function CalcCorAproximada3d (r,g,b: real): word;
var
    d, menorDist: longint;
    i: integer;
    corR, corG, corB: byte;

begin
    if (ModoDither = VGA256) or (ModoDither = SVGA256) or
       (ModoDither = SVGAHI256) then
       begin
           corR:= round (r * 63);
           corG:= round (g * 63);
           corB:= round (b * 63);
           menorDist:= 64*64*64;
           for i := 0 to 255 do
               begin
                   with TabPal[i] do
                       d:= sqr(corR-r) + sqr(corG-g) + sqr(corB-b);

                   if d < menorDist then
                       begin
                           menorDist:= d;
                           CalcCorAproximada3d:= i;
                       end;
               end;
       end
    else
        CalcCorAproximada3d:= 0;
end;

{-------------------------------------------------------------}
{                  Inicializacao do Zbuffer                   }
{-------------------------------------------------------------}

{$ifdef DEBUGZBUFFER}
procedure DebugZbuf;
var
    y: integer;
begin
    for y:= 0 to getmaxy do
        if zbuf[y] = EMDISCO then
            putpixel (0,y,0)
        else
            if zbuf[y] = NIL then
                putpixel (0,y,255)
            else
                putpixel (0,y,140);
end;
{$endif}

procedure InicZBuffer3d ( MaxLinMemoria:integer; fundo:word; Path:string );

   const
     { InicZBuffer3d precisa deixar no minimo 30k disponiveis. }
     MINMEMODISP = 30000;

   var
     xt, yt,
     NLinMemoria: integer;
     yminimo,
     pos,
     MemDisp,
     MemTotal: longint;


   procedure CriaArquivoOvl;
       begin
           if ovlNovo then
               begin
                  assign (arqzovl, Path+'nce3dovl.$$$');
                  {$I-}
                  rewrite (arqzovl, 1);
                  {$I+}
                  if IoResult <> 0 then
                     begin
                         Termina3d;
                         clrscr;
                         writeln ( '*** Nao consegui abrir arquivo para ',
                                   'o ZBuffer. ***' );
                         writeln ( '*** Path invalido na InicZBuffer3d ['+
                                   Path + 'nce3dovl.$$$]. ***' );
                         writeln ( '*** Programa Cancelado. ***' );
                         halt;
                     end;

                  ovlNovo := false;
                  ovlSorteado := 0;
               end;
       end;


   begin

     yminimo:= (getmaxy+1) div 20;  { ideal: (getmaxy+1) div 10 }
     if MaxLinMemoria < yminimo
         then MaxLinMemoria:= yminimo;

     MemTotal:= MINMEMODISP + yminimo*NBYTESZBUF*(getmaxx+1);
     MemDisp:= MemAvail;
     if ( not JaAlocouZbuf ) and
        ( MemDisp < MemTotal ) then
             begin
                 Termina3d;
                 clrscr;
                 writeln ( '*** Nao ha memoria suficiente para ',
                           'o ZBuffer. ***' );
                 writeln ( '    Memoria Disponivel = ', MemDisp, ' bytes.' );
                 writeln ( '    Memoria Necessaria = ', MemTotal, ' bytes.' );
                 writeln ( '*** Programa Cancelado. ***' );
                 halt;
             end;

     NLinMemoria:= 0;
     for yt:= 0 to getmaxy do
         begin
           if (zbuf[yt] = NIL) or (zbuf[yt] = EMDISCO) then
                getmem ( zbuf[yt], NBYTESZBUF*(getmaxx+1) );
                                              { reinicializacao }
           if (memavail < MINMEMODISP) or
              (NLinMemoria > MaxLinMemoria) then
               begin
                   CriaArquivoOvl;
                   freemem (zbuf[yt], NBYTESZBUF*(getmaxx+1));
                   zbuf[yt]:= NIL;
               end
           else
               begin
                   fillchar ( zbuf[yt]^[0], NBYTESZBUF*(getmaxx+1), $FF );
                   NLinMemoria := NLinMemoria + 1;
               end;
         end;

     { Inicializa a tela com a cor de fundo. }
     CorFundo:= fundo;
     setfillstyle (solidfill, fundo);
     bar (0,0, getmaxx, getmaxy);

     jaAlocouZbuf:= TRUE;

   end;

{-------------------------------------------------------------}
{       Libera areas de memoria alocadas para o ZBuffer       }
{-------------------------------------------------------------}

procedure LiberaZBuffer3d;
   var
      nlin, yt: integer;
   begin
      nlin:= 0;
      OvlNovo:= true;
      if jaAlocouZbuf then
          begin
             jaAlocouZbuf:= FALSE;
             for yt:= 0 to getmaxy do
                if (zbuf[yt] <> EMDISCO) and (zbuf[yt] <> NIL)  then
                  begin
                    freemem ( zbuf[yt], NBYTESZBUF*(getmaxx+1) );
                    zbuf[yt]:= NIL;
                    nlin:= nlin + 1;
                  end;

             {$I-} close(arqzovl);  {$I+}
             if ioresult <> 0 then;
             {$I-} erase(arqzovl);  {$I+}
             if ioresult <> 0 then;
          end;
   end;


{-------------------------------------------------------------}
{      Seleciona o nome do arquivo de imagem e                }
{      aloca bufffers de E/S.                                 }
{      Obs:                                                   }
{      - O arquivo so sera aberto quando uma rotina de        }
{        pintura for chamada pela primeira vez.               }
{      - Se o arquivo ja estiver aberto, fecha primeiro.      }
{      - AbreArqImagem3d('') equivale a FechaArqImagem3d.     }
{-------------------------------------------------------------}

procedure AbreArqImagem3d ( nomeArq: string );
begin
    if NomeArq = '' then
        FechaArqImagem3d
    else
        begin
            if ArqImagemAberto then
                FechaArqImagem3d;
            ArqImagemAberto:= TRUE;
            NomeArqImagem := nomeArq + '.rgb';
            InicBufWrite;
            new (corGrav);
        end;
end;

{-------------------------------------------------------------}
{               Fecha arquivo de imagem                       }
{-------------------------------------------------------------}

procedure FechaArqImagem3d;
begin
    if ArqImagemAberto then
        begin
            GuardaCor24bits (-1,-1,0,0,0);
            LibBufWrite(arqRGB);
            close (arqRGB);
            dispose (corGrav);
            ArqImagemAberto:= FALSE;
            NomeArqImagem:= '';
        end;
end;

{-------------------------------------------------------------}
{            seleciona ou nao randomico na imagem             }
{-------------------------------------------------------------}

procedure TiraMachBand3d ( op: boolean );
begin
    trataMachBand := op;
end;

{-------------------------------------------------------------}
{         rotina de selecao de modo de preenchimento          }
{-------------------------------------------------------------}

procedure SelecModoPintura3d ( modo:integer );
   begin
       ModoPintura :=  modo;
   end;



{-------------------------------------------------------------}
{                                                             }
{              Recorte de Poligonos em 3D                     }
{                                                             }
{-------------------------------------------------------------}

{-------------------------------------------------------------}
{          retorna se um ponto esta visivel ou nao            }
{-------------------------------------------------------------}

function visivel( p:PONTO; plano: integer ) : boolean;
   begin

      case plano of

	0: { xmin }
            visivel:= ( (D/Sx)*p.x >= -p.z );

	1: { xmax }
            visivel:= ( (D/Sx)*p.x <= p.z );

	2: { ymin }
            visivel:= ( (D/Sy)*p.y >= -p.z );

	3: { ymax }
            visivel:= ( (D/Sy)*p.y <= p.z );

	4: { zmin }
            visivel:= ( D <= p.z );

	5: { zmax }
            visivel:= ( F >= p.z );

      end;

   end;


{-------------------------------------------------------------}
{          retorna o ponto de interseccao de uma linha        }
{          com o plano de clipping                            }
{-------------------------------------------------------------}

procedure Intersecao( p1,p2:PONTO; plano:integer; var ptoint:PONTO );

   var
     dx,dy,dz, dsx, dsy, t : real;

   begin
     dx:= p2.x - p1.x;
     dy:= p2.y - p1.y;
     dz:= p2.z - p1.z;
     dsx:= D / Sx;
     dsy:= D / Sy;

     case plano of

       0: t:= (-dsx*p1.x - p1.z) / ( dz + dsx*dx );

       1: t:= ( dsx*p1.x - p1.z) / ( dz - dsx*dx );

       2: t:= (-dsy*p1.y - p1.z) / ( dz + dsy*dy );

       3: t:= ( dsy*p1.y - p1.z) / ( dz - dsy*dy );

       4: t:= ( D - p1.z ) / dz;

       5: t:= ( F - p1.z ) / dz;

     end;

     ptoint.x   := (1-t) * p1.x   + t * p2.x;
     ptoint.y   := (1-t) * p1.y   + t * p2.y;
     ptoint.z   := (1-t) * p1.z   + t * p2.z;
     ptoint.cor := (1-t) * p1.cor + t * p2.cor;

   end;


{-------------------------------------------------------------}
{         determina se a aresta cruza o plano de visao        }
{-------------------------------------------------------------}

function CruzaPlano( p1, p2: PONTO; plano: integer ) : boolean;

   var
     v1, v2 : boolean;

   begin
     v1 := visivel ( p1, plano );
     v2 := visivel ( p2, plano );
     CruzaPlano := ( v1 and not v2 ) or ( not v1 and v2 );
   end;


{-------------------------------------------------------------}
{             executa o clipping em um poligono               }
{             pelo metodo Sutherland-Hodgman                  }
{-------------------------------------------------------------}

procedure RecortaPoligono (var nv:integer; var vert);

  var
    p1, p2, inter : PONTO;
    i, j,
    nov: integer;
    Q: PPOLIG_AUX;

  begin
    new(Q);

    for i:= 0 to 5 do   { Para cada plano de recorte... }
       begin
          POLIG(vert)[nv+1] := POLIG(vert)[1];
          nov := 0;

          for j:= 1 to nv do
             begin
                p1 := POLIG(vert)[j];
                p2 := POLIG(vert)[j+1];
                if visivel(p1,i) then
                   begin
                     nov:= nov + 1;
                     Q^[nov] := p1;
                   end;

                if cruzaplano(p1,p2,i) then
                   begin
                     intersecao(p1,p2,i,inter);
                     nov:= nov + 1;
                     Q^[nov] := inter;
                   end;
	     end;

	   for j:= 1 to nov do
	       POLIG(vert)[j] := Q^[j];
	   nv := nov;
       end;

    if nv <= 2 then
	nv := 0;

    dispose (Q);
  end;

{-------------------------------------------------------------}
{             pinta um ponto na tela usando dither            }
{             de acordo com o modo grafico utilizado          }
{-------------------------------------------------------------}

procedure PintaComDither (xt, yt:integer; cor: real);
   const
       tabconvcga: array [0..4] of byte = (0,2,1,3,3);
   var
      corbasica, corint, dit: integer;

   begin
     case ModoDither of
          VGA256,
          SVGA256,
          SVGAHI256:
               VGADither (xt,yt, trunc(corRpoli*cor),
                                 trunc(corGpoli*cor),
                                 trunc(corBpoli*cor));

          VGADEFAULT,
          SVGADEFAULT:
                 if TrataMachBand then
                     putpixel (xt, yt, trunc(cor*63 + random))
                 else
                     putpixel (xt, yt, trunc(cor*63));

          HERCULES,
          CGAHI: begin
                   if TrataMachBand then
                       corint := trunc (cor * 16 + random)
                   else
                       corint := trunc (cor * 16);

                   if corint > tabdither[((yt and 3) shl 2) or (xt and 3)] then
                      putpixel (xt, yt, 1)
                   else
                      putpixel (xt, yt, 0);
                 end;

          EGAHi,
          VGAHi,
          CGAC0:
                 begin
                   corint := trunc (cor * (4-1) * 16 * 128);
                   if TrataMachBand then
                      corint:= corint + random(128);
                   corbasica := corint shr (4+7);
                   corint := corint - corbasica shl (4+7);
                   dit:= tabdither[((yt and 3) shl 2) or (xt and 3)] shl 7;
                   if corint > dit then
                      putpixel (xt, yt, tabconvcga [corbasica+1])
                   else
                      putpixel (xt, yt, tabconvcga [corbasica]);
                 end;
      end;
   end;

{-------------------------------------------------------------}
{                   faz projecao perspectiva                  }
{-------------------------------------------------------------}

procedure ConvParaTela ( var xt, yt, zt: real; vert:PONTO );

    begin
       xt:= vert.x / vert.z * D / Sx * Vsx + Vcx;
       yt:= vert.y / vert.z * D / Sy * Vsy + Vcy;
       zt := ( 1/D - 1/vert.z ) / ( 1/D - 1/F );
    end;


{-------------------------------------------------------------}
{        rotina de tratamento de overlay do zbuffer           }
{-------------------------------------------------------------}

function trazZOvl ( y:integer ): PLZBUF;
   var
       posic: longint;

   begin

{ busca candidato a remover da memoria }
     repeat
         ovlSorteado := (ovlSorteado+1) mod (getmaxy+1);
     until (zbuf[ovlSorteado] <> EMDISCO) and (zbuf[ovlSorteado] <> NIL);

{ grava ele em disco }
     posic := longmul (NBYTESZBUF*(getmaxx+1), OvlSorteado);
     seek ( arqzovl, posic );
     blockwrite ( arqzovl, zbuf[OvlSorteado]^, NBYTESZBUF*(getmaxx+1) );

{ traz o zbuf do disco, se ele existir }
     if zbuf[y] = EMDISCO then
         begin
             posic:= longmul (NBYTESZBUF*(getmaxx+1), y);
             seek ( arqzovl, posic );
             blockread ( arqzovl, zbuf[OvlSorteado]^,
                         NBYTESZBUF*(getmaxx+1) );
         end
     else
         fillchar ( zbuf[ovlSorteado]^[0], NBYTESZBUF*(getmaxx+1), $FF );

{ acerto os ponteiros }
     zbuf[y] := zbuf[ovlSorteado];
     zbuf[OvlSorteado]:= EMDISCO;

     trazZOvl := zbuf[y];
   end;


{-------------------------------------------------------------}
{             rotina de preenchimento de poligonos            }
{-------------------------------------------------------------}

procedure PintaPolig3d ( nvert:integer; var vert );

type
    PLISTA = ^LISTA;

    LISTA = record
                xl, dxl, zl, dzl, corl, dcorl: real;
                dyl: integer;
                proxl: PLISTA;
            end;

var
    vertRecort: PPOLIG_AUX; { vertices transformados e recortados }
    nvertRecort: integer;   { numero de vertices transformados e recortados }

    bucket: array [0..YMAX] of PLISTA;
    nptslinha: integer;
    plAtiva: PLISTA;

    v, v1,
    i, y: integer;

{-------------------------------------------------------------}

    procedure inicBucket;
    var y: integer;
    begin
        for y := 0 to TamY do
            bucket[y] := NIL;
    end;

{-------------------------------------------------------------}

    procedure insbucket (x1, y1, z1, cor1, x2, y2, z2, cor2: real);
    var
        menory, maiory: integer;
        t, dy, xinter, yinter, zinter, corinter: real;
        p: PLISTA;

    begin
        if y1 = y2 then exit;    { programacao defensiva }

        new (p);
        with p^ do
           begin
               dy := y2 - y1;
               if dy > 0 then
                    begin
                         menory := trunc (y1 + 1);
                         maiory := trunc (y2);
                         if maiory < menory then exit;

                         t := (menory-y1) / dy;
                         xl := x1 + t * (x2-x1);
                         zl := z1 + t * (z2-z1);
                         corl := cor1 + t * (cor2-cor1);
                    end
               else
                    begin
                         menory := trunc (y2 + 1);
                         maiory := trunc (y1);
                         if maiory < menory then exit;

                         t := (y2-menory) / dy;
                         xl := x2 + t * (x1-x2);
                         zl := z2 + t * (z1-z2);
                         corl := cor2 + t * (cor1-cor2);
                    end;

               dyl := maiory - menory;
               dxl := (x2-x1) / dy;
               dzl := (z2-z1) / dy;
               dcorl := (cor2-cor1) / dy;

               proxl := bucket[menory];
               bucket[menory] := p;
           end;
    end;

{-------------------------------------------------------------}

    procedure OrdenaLista (y: integer;
                           var nptslinha: integer);
    var
       p, q, salva1, salva2, menor: PLISTA;
       temp: LISTA;

    label achei;

    begin
        p := plAtiva;
        nptslinha:= 0;

        while p <> NIL do
            begin
                q:= p^.proxl;
                menor := p;      { toma um elemento da lista }

                while q <> NIL do
                   begin
                     if q^.xl < menor^.xl then
                       menor := q;
                     q := q^.proxl;
                   end;

                salva1 := p^.proxl;
                salva2 := menor^.proxl;
                temp := p^;
                p^ := menor^;
                menor^ := temp;
                p^.proxl := salva1;
                menor^.proxl := salva2;

                nptslinha:= nptslinha + 1;

                p:= p^.proxl;
            end;
    end;

{-------------------------------------------------------------}

    procedure une (x1, x2: real; y: integer; z1, z2, cor1, cor2: real);
    var i: integer;
        dz, dcor: real;
        difx: real;
        xm: real;
        zz1: word;
        zb: PLZBUF;
        z21, cor21: real;

    begin
        xm := round(x1) + 0.5;
        difx := 1.0 / (x2-x1+1);
        if x2 <> x1 then
            begin
                z1 := z1 - (x1-xm) * (z2-z1) * difx;
                    if z1>1.0 then z1:= 1.0;
                    if z1<0.0 then z1:= 0.0;
                cor1 := cor1 - (x1-xm) * (cor2-cor1) * difx;
                    if cor1>1.0 then cor1:= 1.0;
                    if cor1<0.0 then cor1:= 0.0;
                dcor := (cor2-cor1) * difx;
                dz :=  (z2-z1) * difx;
            end;

        zb := zbuf[y];
        if (zb = EMDISCO) or (zb = NIL) then
            zb := trazZOvl (y);
{$ifdef DEBUGZBUFFER}
DebugZbuf;
{$endif}

        for i := trunc(xm) to trunc(x2) do
            begin
                zz1 := trunc (z1*65535.0);

                if zb^[i] > zz1 then
                    begin
                      PintaComDither ( i, y, cor1 );
                      if ArqImagemAberto then
                          GuardaCor24bits ( i, y,
                                        round (cor1 * corRPoli),
                                        round (cor1 * corGPoli),
                                        round (cor1 * corBPoli) );
                      zb^[i] := zz1;
                    end;

                cor1 := cor1 + dcor;
                z1 := z1 + dz;
            end;
    end;

{-------------------------------------------------------------}

    procedure daSaida (y: integer; nptslinha: integer);

    var i: integer;
        p: PLISTA;
        x1, x2, z1, z2, cor1, cor2: real;
    begin
        i := 1;
        p := plAtiva;
        while i < nptslinha do
           begin
              with p^ do
                  begin
                      x1   := xl;
                      z1   := zl;
                      cor1 := corl;
                  end;

              p := p^.proxl;

              with p^ do
                  begin
                      x2   := xl;
                      z2   := zl;
                      cor2 := corl;
                  end;

              p := p^.proxl;

              i := i + 2;

              une (x1, x2, y, z1, z2, cor1, cor2);
           end;
    end;

{-------------------------------------------------------------}

    procedure TrataSingularidades (var y: real);
    begin
        if abs (trunc (y)-y) < 0.005 then
            y := y + 0.01;
    end;

{-------------------------------------------------------------}

    procedure removeZerados;
    var
        p, pant: PLISTA;
    begin
        p := plAtiva;
        pant := NIL;
        while p <> NIL do
            begin
                if p^.dyl = 0 then
                    begin
                        if pant <> NIL then
                            begin
                                pant^.proxl := p^.proxl;
                                p := pant^.proxl;
                            end
                        else
                            begin
                                {primeiro da lista}
                                p := p^.proxl;
                                plAtiva := p;
                            end;
                    end
                else
                    begin
                        with p^ do
                            begin
                                xl := xl + dxl;
                                zl := zl + dzl;
                                corl := corl + dcorl;
                                dyl := dyl - 1;
                            end;

                        pant := p;
                        p := p^.proxl;
                    end;
            end;
    end;

{-------------------------------------------------------------}

    procedure atualizaLista (y: integer);
    var
        p: PLISTA;

    begin
        p := bucket [y];
        if p <> NIL then
            begin
                while p^.proxl <> NIL do
                    p := p^.proxl;
                p^.proxl := plAtiva;
                plAtiva := bucket[y];
            end;
    end;

{-------------------------------------------------------------}

{ Corpo da procedure PintaPolig3d }

label FIM;

var
    x0, y0, z0, x1, y1, z1: real;
    pheap: pointer;

begin
    { Retorna se o numero de vertices for invalido. }
    if (nvert < 1) or (nvert > MAX_VERT_POLIG) then
        exit;

    { Se PINTAARAMADO so' desenha arestas }
    if ModoPintura = PINTAARAMADO then
       begin
          with POLIG(vert)[nvert] do
              Move3d ( x,y,z );
          for v:= 1 to nvert do
             with POLIG(vert)[v] do
                 Risca3d ( x,y,z );
          exit;
       end;

    { Aborta se o Z-Buffer nao foi alocado. }
    if not jaAlocouZbuf then
        begin
            Termina3d;
            clrscr;
            writeln ( '*** O Z-Buffer nao foi inicializado. ***' );
            writeln ( '*** Programa Cancelado. ***' );
            halt;
        end;

    { Aplica transformacoes nos vertices... }
    new (vertRecort);
    for v:= 1 to nvert do
       begin
          AplicaTransf3d ( POLIG(vert)[v].x,
                           POLIG(vert)[v].y,
                           POLIG(vert)[v].z,
                           vertRecort^[v].x,
                           vertRecort^[v].y,
                           vertRecort^[v].z );
          vertRecort^[v].cor := POLIG(vert)[v].cor;
       end;

    { Recorte... }
    nvertRecort:= nvert;
    RecortaPoligono ( nvertRecort, vertRecort^ );

    if nvertRecort < 1 then     { O poligono foi totalmente recortado }
        goto FIM;

    { Criacao do Bucket... }
    Mark (pheap);
    inicBucket;
    for v := 1 to nvertRecort do
        begin
            v1 := v+1;
            if v1 > nvertRecort then v1 := 1;

            ConvParaTela (x0, y0, z0, vertRecort^[v]);
            ConvParaTela (x1, y1, z1, vertRecort^[v1]);

            if y0<>y1 then
              begin
                TrataSingularidades (y0);
                TrataSingularidades (y1);
                insbucket (x0, y0, z0, vertRecort^[v].cor,
                           x1, y1, z1, vertRecort^[v1].cor);
              end;
        end;

    { Pintura... }
    plAtiva := NIL;
    for y := 0 to TamY do
         begin
            removeZerados;
            atualizaLista (y);
            ordenaLista (y, nptslinha);
            if nptslinha <> 0 then
                daSaida (y, nptslinha);
        end;
    Release(pheap);

FIM:
    dispose(vertRecort);
end;

{-------------------------------------------------------------}
{             pinta um ponto utilizando ZBuffer               }
{-------------------------------------------------------------}

procedure PintaPonto3d ( pto:PONTO );

var
   xt, yt, zt : real;
   zt1: word;
   ptom: PONTO;
   zb: PLZBUF;
   i: integer;

begin
    AplicaTransf3d (pto.x,pto.y,pto.z, ptom.x,ptom.y,ptom.z);

    for i:= 0 to 5 do   { Para cada plano de recorte... }
       if not visivel(ptom,i) then
          exit;

    ConvParaTela ( xt, yt, zt, ptom );
    zt1 := trunc (zt*65535.0);

    zb := zbuf [trunc(yt)];
    if (zb = EMDISCO) or (zb = NIL) then
       zb := trazZOvl (trunc(yt));

    if zb^[trunc(xt)] > zt1 then
        begin
          PintaComDither (trunc(xt), trunc(yt), pto.cor);
          if ArqImagemAberto then
              GuardaCor24bits (trunc(xt), trunc(yt),
                               round (pto.cor * corRPoli),
                               round (pto.cor * corGPoli),
                               round (pto.cor * corBPoli) );
          zb^[trunc(xt)] := zt1;
        end;
end;
