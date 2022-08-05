{-----------------------------------------------------------}
{                                                           }
{     Movimento de um Automovel                             }
{                                                           }
{     Autor: Mauricio Bomfim                                }
{                                                           }
{     Em agosto de 1992                                     }
{                                                           }
{-----------------------------------------------------------}
program Automovel;
uses crt, graph, nce3d, ncemenu;
var
     opcao   : integer;
     pos    : integer;

{-----------------------------------------------------------}

procedure DesenhaCarroceria;
    begin
       Move3d  (   0,  2, 0 );
       Risca3d (   0,  6, 0 );
       Risca3d ( 1.5,  6, 0 );
       Risca3d ( 3.5, 10, 0 );
       Risca3d (   9, 10, 0 );
       Risca3d (  11,  6, 0 );
       Risca3d (  15,  6, 0 );
       Risca3d (  15,  2, 0 );
       Reta3d  (   5,  2, 0,   10, 2, 0 );
    end;

procedure DesenhaRoda;
   begin
      Move3d ( -1.5, -1.5, 0 );
      Risca3d( -1.5,  1.5, 0 );
      Risca3d(  1.5,  1.5, 0 );
      Risca3d(  1.5, -1.5, 0 );
      Risca3d( -1.5, -1.5, 0 );
   end;

{-----------------------------------------------------------}

procedure RodaTras;
   begin
      SalvaTransf3d;
      Translada3d (2.5,1.5,0);
      RodaZ3d (-pos*pi/8);
      DesenhaRoda;
      RecuperaTransf3d;
   end;

procedure RodaFrente;
   begin
      SalvaTransf3d;
      Translada3d (12.5,1.5,0);
      RodaZ3d (-pos*pi/8);
      DesenhaRoda;
      RecuperaTransf3d;
   end;

{-----------------------------------------------------------}

procedure Carro;
   begin
      SalvaTransf3d;
      Escala3d (0.5,0.5,0.5);
      Translada3d (pos,0,0);
      DesenhaCarroceria;
      RodaTras;
      RodaFrente;
      RecuperaTransf3d;
   end;

{-----------------------------------------------------------}

function LeuInterrupcao: boolean;
   const
       ESC = #27;
   begin
       if keypressed then
           if ( readkey = ESC ) then
                LeuInterrupcao:= TRUE
           else
                begin
                  repeat until keypressed;
                  LeuInterrupcao:= ( readkey = ESC );
                end
       else
           LeuInterrupcao:= FALSE;
   end;

{-----------------------------------------------------------}

procedure Movimenta;
   label 999;
   begin
       InicTransf3d;
       while TRUE do
            for pos:=-45 to 30 do
                 begin
                      Carro;
                      if LeuInterrupcao then
                           goto 999;
                      ClearDevice;
                 end;
   999:
       ClearDevice;
   end;

{-----------------------------------------------------------}

procedure AlteraVisao;
   var
      cod       : integer;
      px, py, pz: real;
   begin
      val ( LeTexto ( 20, 20, 'Pos. do observ. em X' ), px, cod );
      val ( LeTexto ( 20, 20, 'Pos. do observ. em Y' ), py, cod );
      val ( LeTexto ( 20, 20, 'Pos. do observ. em Z' ), pz, cod );
      Observador3d ( px, py, pz, 0, 0, 0 );
   end;

{-----------------------------------------------------------}

begin
   InicVideo3d ( CGAC0 );
   Observador3d ( 0, 0, -100, 0, 0, 0 );
   SelecModoTransf3d (HIERARQUICO);

   repeat
       opcao:= menubarra ( 30,30, '1 - Movimentar o carro/' +
                                  '2 - Posicao do observador/' +
                                  '3 - Fim' );
       if opcao=1 then
           Movimenta
       else if opcao=2 then
           AlteraVisao;
   until opcao = 3;

   Termina3d;
end.