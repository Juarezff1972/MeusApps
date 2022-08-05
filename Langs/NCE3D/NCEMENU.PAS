{-------------------------------------------------------------}
{                                                             }
{    Modulo de popup menu                                     }
{                                                             }
{    Autor: Luis Carlos Serpa                                 }
{           Jose' Antonio Borges                              }
{                                                             }
{    Em maio/93                                               }
{                                                             }
{-------------------------------------------------------------}

unit ncemenu;

interface

uses graph, crt;

var passocur: integer;

function MoveCursor (var xcursor, ycursor: integer): char;
function MenuBarra (X1, Y1: integer; Opcoes: string) : integer;
function LeTexto (x, y : integer; Mensagem : string) : string;

implementation

{------------------------------------------------------------}
{                rotinas para mover o cursor                 }
{------------------------------------------------------------}

function MoveCursor (var xcursor, ycursor: integer): char;
const
    TECCIMA  = #72;
    TECBAIXO = #80;
    TECESQ   = #75;
    TECDIR   = #77;
    ENTER    = #13;

const
    tabxcur: array [1..12] of integer = (-3,-2,-1, 1, 2, 3,
                                          0, 0, 0, 0, 0, 0);
    tabycur: array [1..12] of integer = ( 0, 0, 0, 0, 0, 0,
                                         -3,-2,-1, 1, 2, 3);

var c: char;
    acabou: boolean;
    salva: array [1..12] of integer;

    procedure poecursor;
    var i, coordx, coordy: integer;
    begin
        for i := 1 to 12 do
            begin
                coordx := xcursor + tabxcur[i];
                coordy := ycursor + tabycur[i];
                salva [i] := getPixel (coordx, coordy);
                putPixel (coordx, coordy, maxcolors);
            end;
    end;

    procedure tiracursor;
    var i, coordx, coordy: integer;
    begin
        for i := 1 to 12 do
            begin
                coordx := xcursor + tabxcur[i];
                coordy := ycursor + tabycur[i];
                PutPixel (coordx, coordy, salva[i]);
            end;
    end;

begin
    acabou := false;

    repeat
        if xcursor > GetMaxx then  xcursor := GetMaxx;
        if ycursor > GetMaxy then  ycursor := GetMaxy;
        if xcursor < 0 then  xcursor := 0;
        if ycursor < 0 then  ycursor := 0;

        poecursor;
        c := readkey;
        tiracursor;
        if (c = #0) and keypressed then { teclas especiais }
            begin
                c := readkey;

                case c of
                    TECCIMA:   ycursor := ycursor - passocur;
                    TECBAIXO:  ycursor := ycursor + passocur;
                    TECESQ:    xcursor := xcursor - passocur;
                    TECDIR:    xcursor := xcursor + passocur;
               end;
           end
       else
           case c of
                '-':  passocur := 1;
                '+':  passocur := 10;
           else
               acabou := true
           end;

    until acabou;

    movecursor := c;
end;

{-------------------------------------------------------------}
{                 Menu de Barra                               }
{-------------------------------------------------------------}

function MenuBarra (X1, Y1 : integer; Opcoes : string) : integer;
var
   salvaCor: integer;
   VetOpcoes : array [1..23] of string;
   NumOpcoes,
   Altura,
   Largura,
   AltBox,
   LargBox,
   i,j,k : integer;
   Tam,
   TamBox   : word;
   Pt    : pointer;
   PtBox : pointer;
   Tecla : char;

begin
   salvaCor := getcolor;
   NumOpcoes := 0;

   Opcoes := Opcoes + '/';
   while Opcoes <> '' do
      begin
      NumOpcoes := NumOpcoes + 1;
      VetOpcoes[NumOpcoes] := Copy(Opcoes, 1, Pos('/',Opcoes)-1);
      Delete (Opcoes,1, Pos('/', Opcoes));
      end;

    { desenha o box }

   Altura := 12;
   Largura := 0;
   AltBox := Altura * NumOpcoes + 12;
   for i := 1 to NumOpcoes do
      if TextWidth(VetOpcoes[i]) > Largura then
         Largura := TextWidth(VetOpcoes[i]);
   LargBox := Largura + 10;

   if X1+LargBox+11 > GetMaxX then
       X1 := GetMaxX-LargBox - 11;
   if Y1+AltBox > GetMaxY then
       Y1 := GetMaxY-AltBox;

   TamBox := ImageSize (X1, Y1, X1+LargBox+11, Y1+AltBox);
   GetMem (PtBox, TamBox);
   GetImage (X1, Y1, X1+LargBox+11, Y1+AltBox, PtBox^);

   SetLineStyle (SolidLn, 0, NormWidth);
   SetFillStyle(SolidFill,0);
   Bar (X1, Y1, X1+LargBox+11, Y1+AltBox);
   Rectangle (X1, Y1, X1+LargBox+11, Y1+AltBox);

   SetTextStyle (DefaultFont, HorizDir, 1);

   for i := 1 to NumOpcoes do
      OutTextXY (X1+10, (Y1+12*(i-1))+10, VetOpcoes[i]);

   i := 1;
   Tam := ImageSize (X1, Y1, X1+(Largura)+2, Y1+11);
   GetMem (Pt, Tam);

   GetImage (X1+10-4,           (Y1+12*(i-1))+10-2,
             X1+10+Largura+6,   (Y1+12*(i-1))+21-3, Pt^);

   PutImage (X1+10-4, (Y1+12*(i-1))+10-2, Pt^, NotPut);

   Tecla := #255;

   while (Tecla <> #13) and (Tecla <> #27) do
      begin
      if keypressed then
         begin
         Tecla := ReadKey;
         if Tecla = #0 then
            begin
            Tecla := ReadKey;
            case Tecla of
               #72 : begin
                     GetImage (X1+10-4,           (Y1+12*(i-1))+10-2,
                               X1+10+Largura+6,   (Y1+12*(i-1))+21-3, Pt^);
                     PutImage (X1+10-4, (Y1+12*(i-1))+10-2, Pt^, NotPut);
                     i := i - 1;
                     if i < 1 then
                        i := NumOpcoes;
                     GetImage (X1+10-4,           (Y1+12*(i-1))+10-2,
                               X1+10+Largura+6,   (Y1+12*(i-1))+21-3, Pt^);
                     PutImage (X1+10-4, (Y1+12*(i-1))+10-2, Pt^, NotPut);
                     end;
               #80 : begin
                     GetImage (X1+10-4,           (Y1+12*(i-1))+10-2,
                               X1+10+Largura+6,   (Y1+12*(i-1))+21-3, Pt^);
                     PutImage (X1+10-4, (Y1+12*(i-1))+10-2, Pt^, NotPut);
                     i := i + 1;
                     if i > NumOpcoes then
                        i := 1;
                     GetImage (X1+10-4,           (Y1+12*(i-1))+10-2,
                               X1+10+Largura+6,   (Y1+12*(i-1))+21-3, Pt^);
                     PutImage (X1+10-4, (Y1+12*(i-1))+10-2, Pt^, NotPut);
                     end;
               end;
            end;
         end;
      end;

   if Tecla = #27 then
      i := 0;

   MenuBarra := i;

   PutImage (X1, Y1, PtBox^, NormalPut);

   FreeMem (PtBox, TamBox);
   FreeMem (Pt, Tam);

   setcolor (salvaCor);
end;

{-------------------------------------------------------------------------}
{                Solicita string ao usuario                               }
{-------------------------------------------------------------------------}

function LeTexto (x, y : integer; Mensagem : string) : string;

var
   pt : ^byte;
   Tam : word;
   resp : string[40];
   x2, y2 : integer;


begin

   x := x div 8 * 8; y := y div 8 * 8;

   x2 := x + length(mensagem) * 8 + 24;
   if x2-x < 104 then
      x2 := x + 104;
   y2 := y + 40;

   if x2 > GetMaxX then
      begin
      x2 := GetMaxX;
      x := x2 - (length(mensagem) * 8 + 24);
      end;
   if y2 > GetMaxY then
      begin
      y2 := GetMaxY;
      y := y2 - 40;
      end;

   Tam := ImageSize(x,y,x2,y2);
   GetMem(pt,Tam);

   GetImage(x,y,x2,y2,pt^);

   setfillstyle(EmptyFill,GetBkColor);
   bar(x,y,x2,y2);

   Rectangle(x,y,x2,y2);

   OutTextXY(x+8,y+8,mensagem);

   directvideo := false;
   GotoXY((x+16)div 8, (y+32)div 8); write('>'); readln(resp);

   LeTexto := resp;

   PutImage(x,y,pt^,NormalPut);
   FreeMem(pt,Tam);

end;


begin
    passocur := 10;
end.

