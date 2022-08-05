{-------------------------------------------------------------}
{                                                             }
{    EDMALHA   Editor de Malhas para o NCE3D                  }
{                                                             }
{                                                             }
{    Versao 0.2 ( Junho de 1993 )                             }
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
{    Autor: Mauricio Nunes da Costa Bomfim                    }
{                                                             }
{-------------------------------------------------------------}

program EditorDeMalhas;
uses dos, crt, graph, nce3d;

const
    maxM = 10;
    maxN = 10;
    du   = 0.1;
    dv   = 0.1;
    ESC  = #$1B;
    CR   = #$0D;

type
    TIPOEVENTO = ( EscolheuPto, Render, RodaX, RodaY, RodaZ,
                   SelecBezier, SelecSpline, SelecHermitiana,
                   ApagaSuperf, Fim );
    TIPOSUPERFICIE = ( Bezier, Spline, Hermitiana, Nenhuma );

var
    { Pontos de controle }
    ptos: array [0..maxM*maxN] of COORDPTO;
    m,n: integer;

    { Pontos da superficie calculada }
    sup: array [0..2200] of PONTO;
    iu, iv: integer;

    narqEditado: string;
    Evento: TIPOEVENTO;
    SuperfSelecionada: TIPOSUPERFICIE;
    alterouMalha: boolean;
    GraphDriver, GraphMode: Integer;

{-------------------------------------------------------------}
{                  Inicializa malha M X N                     }
{-------------------------------------------------------------}

procedure InicMalha;
var
    i,j: integer;
    x0,z0, dx,dz: integer;
begin
    for i:= 0 to m-1 do
        for j:= 0 to n-1 do
            with ptos[i*n+j] do
                begin
                    dx:= trunc (30 / (m-1));
                    dz:= trunc (30 / (n-1));
                    x0:= trunc (-dx * ((m-1) / 2));
                    z0:= trunc (-dz * ((n-1) / 2));
                    x:= - x0 - i * dx;
                    y:= 0;
                    z:=  z0 + j * dz;
                end;
end;

{-------------------------------------------------------------}
{            Le um arquivo de pontos de controle              }
{-------------------------------------------------------------}

procedure LeArqPtos;
var
    arq: text;
    i,j: integer;
begin
    clrscr;
    writeln ('EDMALHA - Editor de Malhas para o NCE3D');
    writeln ('-------   ------ -- ------ ---- - -----');
    writeln;
    repeat
        write ('Arquivo a editar (.CTR): '); readln (narqEditado);
    until narqEditado <> '';

    assign (arq,narqEditado+'.CTR');
    {$I-} reset (arq); {$I+}
    if ioresult <> 0 then
        begin
            repeat
                write ('Entre com as dimensoes da malha: ');
                readln (m,n);
                if (not (n in [2..maxN])) or (not (m in [2..maxM])) then
                    writeln ('Tamanho maximo da malha e'' ',
                             maxM, ' X ', maxN);
            until (n in [2..maxN]) and (m in [2..maxM]);

            InicMalha;
            alterouMalha:= TRUE;
            exit;
        end;

    readln (arq,m,n);
    if (n > maxN) or (m > maxM) then
        begin
            writeln ('Tamanho maximo da malha e'' ', maxM, ' X ', maxN);
            halt;
        end;

    for i:= 0 to m-1 do
        for j:= 0 to n-1 do
            with ptos[i*n+j] do
                readln (arq, x,y,z);
    close (arq);
    alterouMalha:= FALSE;
end;

{-------------------------------------------------------------}
{            Grava um arquivo de pontos de controle           }
{-------------------------------------------------------------}

procedure GravaArqPtos;
var
    arq: text;
    i,j: integer;
    resp: string;
    arqOk: boolean;
    DirInfo  : SearchRec;

begin
    if not alterouMalha then
        exit;

    alterouMalha:= FALSE;
    assign (arq,narqEditado+'.CTR');
    rewrite (arq);
    writeln (arq, m,' ',n);
    for i:= 0 to m-1 do
        for j:= 0 to n-1 do
            with ptos[i*n+j] do
                writeln (arq, x:8:2,y:8:2,z:8:2);
    close (arq);
end;

{-------------------------------------------------------------}
{    Desenha um ponto no espaco localizando sua posicao       }
{    atraves de retas pontilhadas.                            }
{-------------------------------------------------------------}

procedure DesenhaCota (x,y,z:real);
var
    corAnt: word;
begin
    corAnt:= GetColor;
    setcolor (yellow);
    xis3d ( x,y,z );
    SetLineStyle (DottedLn,0,NormWidth);
    reta3d ( x,0,z, x,0,0 );
    reta3d ( x,0,z, 0,0,z );
    reta3d ( x,0,z, x,y,z );
    SetLineStyle (SolidLn,0,NormWidth);
    SetColor (corAnt);
end;

{-------------------------------------------------------------}
{                  Redesenha toda a malha                     }
{-------------------------------------------------------------}

procedure DesenhaMalha;
var
    i,j: integer;
begin
    setcolor (yellow);
    for j:= 0 to n-1 do
        for i:= 0 to m-1 do
            begin
                with ptos[i*n+j] do
                    begin
                        if y = 0 then
                            xis3d (x,y,z)
                        else
                            DesenhaCota (x,y,z);
                        if i = 0 then
                            Move3d (x,y,z)
                        else
                            Risca3d (x,y,z);
                    end;
            end;
end;

{-------------------------------------------------------------}
{       Verifica se o ponto (x,y,z) pertence a malha          }
{-------------------------------------------------------------}

function Pertence ( x,y,z: real; var ii,jj: integer) : boolean;
var
   i,j: integer;
begin
   Pertence := FALSE;
   for i:= 0 to m-1 do
       for j:= 0 to n-1 do
           if (ptos[i*n+j].x = x) and
              (ptos[i*n+j].y = y) and
              (ptos[i*n+j].z = z) then
               begin
                   Pertence:= TRUE;
                   ii:= i; jj:= j;
                   exit;
               end;
end;

{-------------------------------------------------------------}
{       Desenha a superficie renderizada ou em wireframe      }
{-------------------------------------------------------------}

procedure CalcSuperficie (modo:integer);
var
   c: char;
begin
   if modo = pintasolido then
       begin
           if (GraphDriver <> CGA) then
               begin
                   Termina3d;
                   InicVideo3d (svga256);
               end;
           InicZBuffer3d (getmaxy+1,0,'c:');
       end;
   selecModoPintura3d (modo);
   case SuperfSelecionada of
       Bezier:
           CalcSupBezier3d  (ptos, m,n, du,dv, sup,iu,iv);
       Spline:
           CalcSupBSpline3d  (ptos, m,n, du,dv, sup,iu,iv);
       Hermitiana:
           CalcSupHermit3d  (ptos, m,n, 0.2, 0.2, sup,iu,iv);
   end;
   DesenhaMalha;
   SetColor (white);
   DesenhaSuperf3d  (sup, iu,iv);

   if modo = pintasolido then
       begin
           c:= readkey;
           if (GraphDriver <> CGA) then
               begin
                   Termina3d;
                   InicVideo3d ( egaDefault );
               end;
       end;
end;

{-------------------------------------------------------------}
{       Redesenha retas de controle que unem um ponto a       }
{       seus vizinhos.                                        }
{-------------------------------------------------------------}

procedure UnePonto (i,j: integer);
    var
        corAnt: word;
    begin
        setwritemode (XORPUT);
        corAnt:= getcolor;
        setcolor (White);
        with ptos[i*n+j] do
            if y<>0 then
                DesenhaCota (x,y,z)
            else
                xis3d (x,y,z);
        if i > 0 then
            Reta3d ( ptos[(i-1)*n+j].x,ptos[(i-1)*n+j].y,ptos[(i-1)*n+j].z,
                     ptos[i*n+j].x,ptos[i*n+j].y,ptos[i*n+j].z );
        if i < m-1 then
            Reta3d ( ptos[i*n+j].x,ptos[i*n+j].y,ptos[i*n+j].z,
                     ptos[(i+1)*n+j].x,ptos[(i+1)*n+j].y,ptos[(i+1)*n+j].z );
        setcolor (corAnt);
        setwritemode (NORMALPUT);
    end;

{-------------------------------------------------------------}
{        Permite alteracao da localizacao de um ponto         }
{-------------------------------------------------------------}

procedure MovePonto (var xp,yp,zp: real; i,j: integer);
var
    c: char;
    begin
        UnePonto (i,j);
        repeat
            setcolor (LightBlue);
            c:= MoveCursor3d(xp,yp,zp);
            setcolor (White);
        until c = CR;
        with ptos[i*n+j] do
            if (xp <> x) or (yp <> y) or (zp <> z) then
                begin
                    alterouMalha:= TRUE;
                    x:= xp;
                    y:= yp;
                    z:= zp;
                end;
        UnePonto (i,j);
    end;

{-------------------------------------------------------------}
{                     Classifica eventos                      }
{-------------------------------------------------------------}

function LeuInterrupcao: boolean;
   begin
       if keypressed then
           if ( readkey = CR ) then
                LeuInterrupcao:= TRUE
           else
                begin
                  repeat until keypressed;
                  LeuInterrupcao:= ( readkey = CR );
                end
       else
           LeuInterrupcao:= FALSE;
   end;

{-------------------------------------------------------------}

procedure anima;
var
   pg: integer;
begin
   SelecModoPintura3d (pintaaramado);
   case SuperfSelecionada of
       Bezier:
           CalcSupBezier3d  (ptos, m,n, du,dv, sup,iu,iv);
       Spline:
           CalcSupBSpline3d  (ptos, m,n, du,dv, sup,iu,iv);
       Hermitiana:
           CalcSupHermit3d  (ptos, m,n, 0.2, 0.2, sup,iu,iv);
   end;

   repeat
        pg := (pg+1) and 1;
        SetActivePage(pg);
        ClearDevice;
        setcolor (Lightblue);
        reta3d (-20,0,0, 20,0,0);
        reta3d (0,-20,0, 0,20,0);
        reta3d (0,0,-20, 0,0,20);
        DesenhaMalha;
        setcolor (white);
        DesenhaSuperf3d  (sup, iu,iv);
        SetVisualPage(pg);
        case evento of
            RodaX: RodaX3d (pi/36);
            RodaY: RodaY3d (pi/36);
            RodaZ: RodaZ3d (pi/36);
        end;
   until LeuInterrupcao;
   InicTransf3d;
end;

{-------------------------------------------------------------}
{    Verifica se a malha editada esta contida no plano XZ     }
{-------------------------------------------------------------}

function SuperfPlana: boolean;
var
   i,j: integer;
begin
   SuperfPlana:= TRUE;
   for i:= 0 to m-1 do
       for j:= 0 to n-1 do
           if (ptos[i*n+j].y <> 0) then
               begin
                   SuperfPlana:= FALSE;
                   exit;
               end;
end;

{-------------------------------------------------------------}
{                     Classifica eventos                      }
{-------------------------------------------------------------}

procedure PegaEvento (var x,y,z: real; var i,j:integer);
var
    c: char;
begin
    while TRUE do
        begin
            setcolor (LightBlue);
            c:= MoveCursor3d (x,y,z);
            setcolor (White);
            case upcase(c) of
                'R': begin
                         Evento:= Render;
                         exit;
                     end;
                'B': begin
                         Evento:= SelecBezier;
                         exit;
                     end;
                'S': begin
                         Evento:= SelecSpline;
                         exit;
                     end;
                'H': begin
                         Evento:= SelecHermitiana;
                         exit;
                     end;
                'A': begin
                         Evento:= ApagaSuperf;
                         exit;
                     end;
                'X': begin
                         Evento:= RodaX;
                         exit;
                     end;
                'Y': begin
                         Evento:= RodaY;
                         exit;
                     end;
                'Z': begin
                         Evento:= RodaZ;
                         exit;
                     end;
                ESC: begin
                         Evento:= Fim;
                         exit;
                     end;
                CR:  if Pertence (x,y,z,i,j) then
                         begin
                             Evento:= EscolheuPto;
                             exit;
                         end
                     else
                         begin
                             sound(800); delay(80); nosound;
                         end;
            end;
        end;
end;

{-------------------------------------------------------------}
label
    FIMPROG;

var
    x,y,z: real;
    i,j: integer;
    c: char;

begin
    directvideo:= false;
    x:= 0; y:= 0; z:= 0;
    SuperfSelecionada:= Bezier;
    LeArqPtos;
    DetectGraph(GraphDriver, GraphMode);
    if (GraphDriver = CGA) then
        InicVideo3d ( cgahi )
    else
        InicVideo3d ( egaDefault );
    janela3d (40,400,20.0,15.0,0,0,getmaxx+1,getmaxy+1);
    Observador3d ( 40,40,-80, 0,0,0 );
    selecluz3d(-20,20,-20);
    selecluzambiente3d(0.1);
    SelecCorPolig3d (vermelho);


    while TRUE do
        begin
            ClearDevice;
            DesenhaMalha;
            if (SuperfSelecionada <> Nenhuma) and
               (not SuperfPlana) then
                CalcSuperficie (pintaaramado);
            SetColor (white);
            SetTextStyle ( SansSeriffont, Horizdir, 1 );
            OutTextXY ( 340, 20, 'Editor de Malhas para o NCE3D');
            SetTextStyle ( Defaultfont, Horizdir, 1 );
            OutTextXy ( 2, getmaxy-20, '<Enter> Edita ponto    ' +
                                       '<X><Y><Z> Anima     ' +
                                       '<R> Renderiza    ' +
                                       '<Esc> Termina');
            OutTextXy ( 2, getmaxy-10, '<S>     B-Spline       ' +
                                       '<B>       Bezier    ' +
                                       '<H> Hermitiana   ' +
                                       '<A>   Apaga Superf');

            PegaEvento (x,y,z, i,j);
            case Evento of
                SelecBezier: begin
                                 SuperfSelecionada:= Bezier;
                             end;
                SelecSpline: begin
                                 SuperfSelecionada:= Spline;
                             end;
                SelecHermitiana: begin
                                     SuperfSelecionada:= Hermitiana;
                                 end;
                ApagaSuperf: begin
                                 SuperfSelecionada:= Nenhuma;
                             end;
                Render: begin
                            CalcSuperficie (pintasolido);
                        end;
                RodaX,
                RodaY,
                RodaZ: begin
                           anima;
                       end;
                EscolheuPto: begin
                                 MovePonto (x,y,z, i,j);
                             end;
                Fim: goto FIMPROG;
            end;
        end;

FIMPROG:
    GravaArqPtos;
    Termina3d;
end.