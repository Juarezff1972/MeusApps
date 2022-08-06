#include <owl.h>
#include <windobj.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <malloc.h>
#include <string.h>
#include <inputdia.h>

#include "vars.h"
#include "bin.h"
#include "linha.h"
#include "coords.h"
#include "curva.h"
#include "figura.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TDrawTool)
_CLASSDEF(TCanvas)

////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TState {
    PTDrawTool DrawTool;
         HBITMAP   BitMap;
         int       PenSize;
         COLORREF  PenColor;
         COLORREF  BrushColor;
};
typedef TState* PTState;

////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL primpto=TRUE;             // primeiro ponto da polilinha/curva
float px1,py1,anginic,angfin;  // variáveis auxiliares
poli plaux;                    // variável auxiliar
unsigned char plcount=0;       // Número de vértices da polilinha
int curtool;                   // ferramenta atual
char txto[80]="Texto";         // variável de texto

////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TDrawTool
{
protected:
         HCURSOR Cursor;
         HICON Icon;
         PTState State;
         HWND Window;
         HPEN Pen;
         HBRUSH Brush;
         HDC DC;
         HPEN OrigPen;
         HBRUSH OrigBrush;
public:
         TDrawTool( LPSTR IconName);
         void MouseDown( HWND AWindow, int X, int Y, PTState AState);
         void MouseMove( int X, int Y);
         void MouseUp();
         virtual void DrawBegin( int, int) {};       // Abstract draw methods will be implemented
         virtual void DrawTo( int, int) {};          // in derrived classes
         virtual void DrawEnd() {};
         HCURSOR GetCursor() { return Cursor; };     // Classes other than derrived may want
         HICON GetIcon() { return Icon; };           // access to these member fields
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TPenTool)

class TPenTool : public TDrawTool
{
protected:
    HPEN BrushPen;
    HPEN OrigBrushPen;
public:
    TPenTool( LPSTR IconName ) : TDrawTool( IconName ) {};
    virtual void DrawBegin( int X, int Y);
    virtual void DrawTo( int X, int Y);
    virtual void DrawEnd();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TFillTool)

class TFillTool : public TDrawTool
{
public:
    TFillTool( LPSTR IconName ) : TDrawTool( IconName ) {};
    virtual void DrawBegin( int X, int Y);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TCleanTool)

class TCleanTool : public TDrawTool
{
public:
    TCleanTool( LPSTR IconName ) : TDrawTool( IconName ) {};
    virtual void DrawBegin( int X, int Y);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TTextTool)

class TTextTool : public TDrawTool
{
public:
    TTextTool( LPSTR IconName ) : TDrawTool( IconName ) {};
    virtual void DrawBegin( int X, int Y);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TBoxTool)

class TBoxTool : public TDrawTool
{
protected:
         BOOL Filled;
         int X1, Y1, X2, Y2;
public:
         TBoxTool( LPSTR IconName, BOOL AFilled ):TDrawTool( IconName ) { Filled = AFilled; };
         virtual void DrawBegin( int X, int Y);
         virtual void DrawTo( int X, int Y);
         virtual void DrawEnd();
         virtual void DrawObject() {};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TRectTool)

class TRectTool : public TBoxTool
{
public:  TRectTool( LPSTR IconName, BOOL AFilled )
          : TBoxTool( IconName, AFilled ) {};
         virtual void DrawObject();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TLineTool)

class TLineTool : public TBoxTool
{
public:  TLineTool( LPSTR IconName, BOOL AFilled )
          : TBoxTool( IconName, AFilled ) {};
         virtual void DrawObject();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TPLineTool)

class TPLineTool : public TBoxTool
{
public:  TPLineTool( LPSTR IconName, BOOL AFilled )
          : TBoxTool( IconName, AFilled ) {};
         virtual void DrawObject();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TBezierTool)

class TBezierTool : public TDrawTool
{
public:  TBezierTool( LPSTR IconName ) : TDrawTool( IconName ) {};
         virtual void DrawBegin(int X,int Y);
};   

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TSplineTool)

class TSplineTool : public TDrawTool
{
public:  TSplineTool( LPSTR IconName ) : TDrawTool( IconName ) {};
         virtual void DrawBegin(int X,int Y);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TEllipseTool)

class TEllipseTool : public TBoxTool
{
public:  TEllipseTool( LPSTR IconName, BOOL AFilled ): TBoxTool( IconName, AFilled ) {};
         virtual void DrawObject();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TCircleTool)

class TCircleTool : public TBoxTool
{
public:  TCircleTool( LPSTR IconName, BOOL AFilled ): TBoxTool( IconName, AFilled ) {};
         virtual void DrawObject();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TPolygonTool)

class TPolygonTool : public TBoxTool
{
public:  TPolygonTool( LPSTR IconName, BOOL AFilled ): TBoxTool( IconName, AFilled ) {};
         virtual void DrawObject();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TArcCircTool)

class TArcCircTool : public TBoxTool
{
public:  TArcCircTool( LPSTR IconName, BOOL AFilled ): TBoxTool( IconName, AFilled ) {};
         virtual void DrawObject();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TArcElipTool)

class TArcElipTool : public TBoxTool
{
public:  TArcElipTool( LPSTR IconName, BOOL AFilled ): TBoxTool( IconName, AFilled ) {};
         virtual void DrawObject();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TPalette)

class TPalette : public TWindow
{
private:
         PTState State;
public:
    TPalette( PTWindowsObject AParent, PTState AState );
         virtual void Paint( HDC PaintDC, PAINTSTRUCT& PaintInfo );
         void SelectColor( TMessage& Message, COLORREF& Color );
         virtual void WMLButtonDown( TMessage& Message ) = [ WM_FIRST + WM_LBUTTONDOWN ];
         virtual void WMRButtonDown( TMessage& Message ) = [ WM_FIRST + WM_RBUTTONDOWN ];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TToolBar)

class TToolBar : public TWindow
{
private:
         PTState State;
         PTDrawTool Tools[ ToolCount ];
public:
         TToolBar( PTWindowsObject AParent, PTState AState );
         virtual ~TToolBar();
         virtual void Paint( HDC PaintDC, PAINTSTRUCT& PaintInfo );
         virtual void WMLButtonDown( TMessage& Message ) = [ WM_FIRST + WM_LBUTTONDOWN ];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TLineBar)

class TLineBar : public TWindow
{
public:
         TLineBar( PTWindowsObject AParent, PTState AState );
         virtual void Paint( HDC PaintDC, PAINTSTRUCT& PaintInfo );
         virtual void WMLButtonDown( TMessage& Message ) = [ WM_FIRST + WM_LBUTTONDOWN ];
private:
         PTState State;
         void Notch( HDC PaintDC, int X, int W, int Y, int DY );
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TCanvas has already been _CLASSDEF'd

class TCanvas : public TWindow
{
private:
         PTState State;
         BOOL Drawing;
public:
         TCanvas( PTWindowsObject AParent, PTState AState );
         virtual ~TCanvas();
         virtual LPSTR GetClassName() { return "TCanvas"; };
         virtual void Paint( HDC PaintDC, PAINTSTRUCT& PaintInfo );
         virtual void WMLButtonDown( TMessage& Message ) = [ WM_FIRST + WM_LBUTTONDOWN ];
         virtual void WMLButtonUp( TMessage& Message ) = [ WM_FIRST + WM_LBUTTONUP ];
         virtual void WMMouseMove( TMessage& Message ) = [ WM_FIRST + WM_MOUSEMOVE ];
         virtual void WMSetCursor( TMessage& Message ) = [ WM_FIRST + WM_SETCURSOR ];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CLASSDEF(TPaintWin)

class TPaintWin : public TWindow
{
private:
         TState State;
         PTPalette Palette;
         PTToolBar ToolBar;
         PTLineBar LineBar;
         PTCanvas Canvas;
public:
         TPaintWin( PTWindowsObject AParent, LPSTR ATitle);
         virtual LPSTR GetClassName() { return "TPaintWin"; };
         virtual void GetWindowClass( WNDCLASS& WndClass );
         virtual void WMSize( TMessage& Message ) = [ WM_FIRST + WM_SIZE ];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TPaintApp : public TApplication
{
public:
   TPaintApp( LPSTR name, HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmd,int nCmdShow)
           : TApplication(name, hInstance,hPrevInstance, lpCmd, nCmdShow) {};
   virtual void InitMainWindow( void );
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TDrawTool ------------------------

TDrawTool::TDrawTool( LPSTR IconName )
{
  char Temp[30];

  _fstrcpy(Temp, IconName);
  strncat(Temp, "Tool", sizeof(Temp)-1);
  Icon = LoadIcon(GetApplicationObject()->hInstance, Temp);

  _fstrcpy(Temp, IconName);
  strncat(Temp, "Cursor", sizeof(Temp)-1);
  Cursor = LoadCursor(GetApplicationObject()->hInstance, Temp);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TDrawTool::MouseDown( HWND AWindow, int X, int Y, PTState AState )
{
  Window = AWindow;
  State = AState;
  SetCapture(Window);
  Pen = CreatePen(PS_SOLID, State->PenSize, State->PenColor);
  Brush = CreateSolidBrush(State->BrushColor);
  DC = GetDC(Window);
  OrigPen = (HPEN)SelectObject(DC, Pen);
  OrigBrush = (HBRUSH)SelectObject(DC, Brush);
  if (curtool==7)
  {
     if (primpto)
     {                // aqui adiciona-se mais uma figura do tipo polilinha/curva nas estruturas
        pc.x=X;
        pc.y=Y;
        primpto=FALSE;
        plaux[0].x=X;
		  plaux[0].y=Y;

        numpolicurv++;
		  EstPolilinhaCurva[numpolicurv].cor=State->PenColor;
		  EstPolilinhaCurva[numpolicurv].tipo=1;
		  EstPolilinhaCurva[numpolicurv].sub=0;
	  }
	  plcount++; // passa para o próximo vértice do vetor
	  EstPolilinhaCurva[numpolicurv].vertices=plaux;
  }
  DrawBegin(X,Y);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TDrawTool::MouseMove( int X, int Y )
{
  DrawTo(X, Y);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TDrawTool::MouseUp()
{
  DrawEnd();
  pc.x=px1;pc.y=py1;
  ReleaseCapture();
  SelectObject(DC, OrigPen);
  SelectObject(DC, OrigBrush);
  DeleteObject(Brush);
  DeleteObject(Pen);
  ReleaseDC(Window, DC);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TPenTool ------------------------

void TPenTool::DrawBegin( int X, int Y ) // inicia desenho à mão livre
{
  BrushPen = CreatePen(PS_SOLID, State->PenSize, State->PenColor);
  OrigBrushPen = (HPEN)SelectObject(DC, BrushPen);
  MoveTo(DC, X, Y);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TPenTool::DrawTo( int X, int Y ) // faz desenho à mão livre, armazena no "layer" bitmap
{
  LineTo(DC, X, Y);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TPenTool::DrawEnd() // termina desenho à mão livre
{
  SelectObject(DC, OrigBrushPen);
  DeleteObject(BrushPen);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TFillTool ------------------------

void TFillTool::DrawBegin( int X, int Y ) // executa preenchimento e armazena no "layer" bitmap
{
  DWORD SurfaceColor;
  SurfaceColor = GetPixel(DC, X, Y);
  ExtFloodFill(DC, X, Y, SurfaceColor, FLOODFILLSURFACE);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TCleanTool ------------------------

void TCleanTool::DrawBegin( int X, int Y ) // limpa o "layer" bitmap
{
   limpatela(DC,State->BrushColor);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TTextTool ------------------------

void TTextTool::DrawBegin( int X, int Y ) // armazena mais uma figura do tipo texto
{
   texto(DC,X,Y,txto,State->PenColor);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TBoxTool ------------------------

void TBoxTool::DrawBegin( int X, int Y )
{
  X1 = X;
  Y1 = Y;
  X2 = X;
  Y2 = Y;
  SelectObject(DC, GetStockObject(BLACK_PEN));
  SelectObject(DC, GetStockObject(NULL_BRUSH));
  SetROP2(DC, R2_NOT);
  DrawObject();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TBoxTool::DrawTo( int X, int Y )
{
  DrawObject();
  X2 = X;
  Y2 = Y;
  DrawObject();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TBoxTool::DrawEnd()
{
  DrawObject();
  SetROP2(DC, R2_COPYPEN);
  SelectObject(DC, Pen);
  if (!Filled) DrawObject();
  SelectObject(DC, Brush);
  if (Filled) DrawObject();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TRectTool ------------------------

void TRectTool::DrawObject() // armazena mais uma figura do tipo retângulo
{
  retangulo(DC,X1,Y1,X2,Y2,State->PenColor);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TLineTool ------------------------

void TLineTool::DrawObject() // armazena mais uma figura do tipo linha
{
  linha(DC,X1,Y1,X2,Y2,State->PenColor);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TPLineTool ------------------------

void TPLineTool::DrawObject() // armazena os vértices da polilinha/curva
{
  px1=X2;py1=Y2;
  plaux[plcount].x=X2; plaux[plcount].y=Y2;
  polilinha(DC,plcount,plaux,0,State->PenColor);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TBezierTool ------------------------

void TBezierTool::DrawBegin(int X,int Y) // define que a polilinha é uma curva bézier
{
  bezier(DC,plcount+1,plaux,State->PenColor);
  EstPolilinhaCurva[numpolicurv].sub=1;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TSplineTool ------------------------

void TSplineTool::DrawBegin(int X,int Y) // define que a polilinha é uma curva spline                           
{
	spline(DC,plcount+1,plaux,State->PenColor);
	EstPolilinhaCurva[numpolicurv].sub=2;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TEllipseTool ------------------------

void TEllipseTool::DrawObject() // armazena mais uma figura do tipo elipse
{
  elipse(DC,(X1+X2)/2,(Y1+Y2)/2,fabs(X2-X1)/2,fabs(Y2-Y1)/2,State->PenColor);
  numcircelip++;
  EstCirculoElipse[numcircelip].tipo=0;
  EstCirculoElipse[numcircelip].xc=(X1+X2)/2;
  EstCirculoElipse[numcircelip].yc=(Y1+Y2)/2;
  EstCirculoElipse[numcircelip].raioX=fabs(X1-X2)/2;
  EstCirculoElipse[numcircelip].raioY=fabs(Y1-Y2)/2;
  EstCirculoElipse[numcircelip].cor=State->PenColor;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TEllipseTool ------------------------

void TArcElipTool::DrawObject()  // armazena mais uma figura do tipo arco de elipse
{
  arcoelip(DC,(X1+X2)/2,(Y1+Y2)/2,fabs(X2-X1)/2,fabs(Y2-Y1)/2,anginic,angfin,State->PenColor);
  numarccircelip++;
  EstArcoSetor[numarccircelip].tipo=0;
  EstArcoSetor[numarccircelip].subtipo=1;
  EstArcoSetor[numarccircelip].xc=(X1+X2)/2;
  EstArcoSetor[numarccircelip].yc=(Y1+Y2)/2;
  EstArcoSetor[numarccircelip].raioX=fabs(X1-X2)/2;
  EstArcoSetor[numarccircelip].raioY=fabs(Y1-Y2)/2;
  EstArcoSetor[numarccircelip].anginic=anginic;
  EstArcoSetor[numarccircelip].angfim=angfin;
  EstArcoSetor[numarccircelip].cor=State->PenColor;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TCircleTool ------------------------

void TCircleTool::DrawObject() // armazena mais uma figura do tipo círculo
{
  float xaux,yaux;
  xaux=fabs(X2-X1);
  yaux=fabs(Y2-Y1);
  circulo(DC,X1,Y1,sqrt(xaux*xaux+yaux*yaux),State->PenColor);
  numcircelip++;
  EstCirculoElipse[numcircelip].tipo=1;
  EstCirculoElipse[numcircelip].xc=X1;
  EstCirculoElipse[numcircelip].yc=Y1;
  EstCirculoElipse[numcircelip].raioX=sqrt(xaux*xaux+yaux*yaux);
  EstCirculoElipse[numcircelip].raioY=0;
  EstCirculoElipse[numcircelip].cor=State->PenColor;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TArcCircTool ------------------------

void TArcCircTool::DrawObject() // armazena mais uma figura do tipo arco de círculo
{
  float xaux,yaux;
  xaux=fabs(X2-X1);
  yaux=fabs(Y2-Y1);
  arco(DC,X1,Y1,sqrt(xaux*xaux+yaux*yaux),anginic,angfin,State->PenColor);
  numarccircelip++;
  EstArcoSetor[numarccircelip].tipo=1;
  EstArcoSetor[numarccircelip].subtipo=1;
  EstArcoSetor[numarccircelip].xc=X1;
  EstArcoSetor[numarccircelip].yc=Y1;
  EstArcoSetor[numarccircelip].raioX=sqrt(xaux*xaux+yaux*yaux);
  EstArcoSetor[numarccircelip].raioY=0;
  EstArcoSetor[numarccircelip].anginic=anginic;
  EstArcoSetor[numarccircelip].angfim=angfin;
  EstArcoSetor[numarccircelip].cor=State->PenColor;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TPolygonTool ------------------------

void TPolygonTool::DrawObject() // armazena mais uma figura do tipo polilinha, gerada por
{                               // polígono regular
  float xaux,yaux;
  xaux=fabs(X2-X1);
  yaux=fabs(Y2-Y1);
  poligono_regular(DC,X1,Y1,AuxNumLados,sqrt(xaux*xaux+yaux*yaux),State->PenColor);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TPalette ------------------------

TPalette::TPalette( PTWindowsObject AParent, PTState AState) : TWindow(AParent, NULL)
{
  Attr.Style = WS_CHILD | WS_VISIBLE;
  State = AState;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TPalette::Paint( HDC PaintDC, PAINTSTRUCT& )
{
  int X, Y, S;
  HPEN OldPen;
  HBRUSH OldBrush;
  RECT R;

  GetClientRect(HWindow, &R);
  S = R.bottom / 17;
  for( Y = 0; Y < 16; Y++ )
    for( X = 0; X < 3; X++ )
    {
      OldBrush = (HBRUSH)SelectObject(PaintDC, CreateSolidBrush(Colors[X][Y]));
      Rectangle(PaintDC, X * S, Y * S, (X + 1) * S + 1, (Y + 1) * S + 1);
      DeleteObject(SelectObject(PaintDC, OldBrush));
    };

  SelectObject(PaintDC, GetStockObject(NULL_BRUSH));
  Rectangle(PaintDC, 0, S * 16, R.right, R.bottom);

  OldPen = (HPEN)SelectObject(PaintDC, CreatePen(PS_SOLID, 5, State->PenColor));
  OldBrush = (HBRUSH)SelectObject(PaintDC, CreateSolidBrush(State->BrushColor));
  Rectangle(PaintDC, 3, S * 16 + 3, R.right - 3, R.bottom - 3);
  DeleteObject(SelectObject(PaintDC, OldBrush));
  DeleteObject(SelectObject(PaintDC, OldPen));
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TPalette::SelectColor( TMessage& Message, COLORREF& Color )
{
  int X, Y, S;
  RECT R;

  GetClientRect(HWindow, &R);
  S = R.bottom / 17;
  X = Message.LP.Lo / S;
  Y = Message.LP.Hi / S;
  if ((X < 3) && (Y < 16))
  {
    Color = Colors[X][Y];
    InvalidateRect(HWindow, NULL, FALSE);
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TPalette::WMRButtonDown( TMessage& Message ) // Botão direito
{
  SelectColor(Message, State->BrushColor);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TPalette::WMLButtonDown( TMessage& Message ) // Botão esquerdo
{
  SelectColor(Message, State->PenColor);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TToolBar ------------------------

TToolBar::TToolBar( PTWindowsObject AParent, PTState AState) : TWindow(AParent, NULL)
{                                      // Aqui colocar-se-ão chamadas para transformações, cortes,
  Attr.Style = WS_CHILD | WS_VISIBLE;  // salvar e ler do disco, apagar e "undo". 
  State = AState;
  Tools[ 0] = new TPenTool("Pen");
  Tools[ 1] = new TFillTool("Fill");
  Tools[ 2] = new TRectTool("Rect", FALSE);
  Tools[ 3] = new TLineTool("Line", FALSE);
  Tools[ 4] = new TEllipseTool("Ellipse", FALSE);
  Tools[ 5] = new TCircleTool("Circle", FALSE);
  Tools[ 6] = new TPolygonTool("Polygon", FALSE);
  Tools[ 7] = new TPLineTool("Pline", FALSE);
  Tools[ 8] = new TBezierTool("Bezier");
  Tools[ 9] = new TSplineTool("Spline");
  Tools[10] = new TArcCircTool("ArcCircle", FALSE);
  Tools[11] = new TArcElipTool("ArcEllipse", FALSE);
  Tools[12] = new TCleanTool("Clean");
  Tools[13] = new TTextTool("Text"); 
  State->DrawTool = Tools[0];
  curtool=0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

TToolBar::~TToolBar()
{
  for( int i = 0; i < ToolCount; i++) delete Tools[i];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TToolBar::Paint( HDC PaintDC, PAINTSTRUCT& )
{
  int I;
  RECT R;

  for ( I = 0; I < ToolCount; I++)
  {
    DrawIcon(PaintDC, I * 31, 0, Tools[I]->GetIcon());
    if (Tools[I] == State->DrawTool)
    {
      R.top = 1;
      R.left = I * 31 + 1;
      R.bottom = R.top + 30;
      R.right = R.left + 30;
      InvertRect(PaintDC, &R);
    }
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TToolBar::WMLButtonDown( TMessage& Message ) // aqui executam-se os inícios dos comandos, 
{                                                 // e/ou selecionam-se as figuras para
  int I;                                          // uso em cortes/transformações
  char NumLados[ 64 ] = "3";
  char Angulo[5]="0";

  I = Message.LP.Lo / 31;
  curtool=I;
  if ( I < ToolCount )
  {
    State->DrawTool = Tools[I];
    InvalidateRect(HWindow, NULL, FALSE);
    if (I==6)
    {
       if (GetApplication()->ExecDialog(new TInputDialog(this,"Polígono","&Número de lados:",
                                                         NumLados,sizeof(NumLados)))!= IDCANCEL)
       {
          AuxNumLados=atoi(NumLados);
          if (AuxNumLados<3) AuxNumLados=3;
          if (AuxNumLados>254) AuxNumLados=255;
       } else AuxNumLados=3;
    }
    if (I==7)
    {
       primpto=TRUE;
       plcount=0;
    }
    if ((I==10)||(I==11))
    {
       if (GetApplication()->ExecDialog(new TInputDialog(this,"Arco","Ângulo &Inicial:",
                                                         Angulo,sizeof(Angulo)))!= IDCANCEL)
       {
          anginic=atof(Angulo);
          if (anginic< -360.0) anginic=-360.0;
          if (anginic>360.0) anginic=360.0;
       } else anginic=0.0;
       if (GetApplication()->ExecDialog(new TInputDialog(this,"Arco","Ângulo &Final:",
                                                         Angulo,sizeof(Angulo)))!= IDCANCEL)
       {
          angfin=atof(Angulo);
          if (angfin< -360.0) angfin=-360.0;
          if (angfin>360.0) angfin=360.0;
       } else angfin=360.0;
    }
    if (I==13)
    {
       if (GetApplication()->ExecDialog(new TInputDialog(this,"Texto","&Entre com o texto:",
                                                         txto,sizeof(txto)))!= IDCANCEL)
       {
       } else strcpy(txto,"Texto");
    }
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TLineBar ------------------------

TLineBar::TLineBar( PTWindowsObject AParent, PTState AState) : TWindow(AParent, NULL)
{
  Attr.Style = WS_BORDER | WS_CHILD | WS_VISIBLE;
  State = AState;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TLineBar::Notch( HDC PaintDC, int X, int W, int Y, int DY )
{
  for( int L = 3; L >= 0; L-- )
  {
    MoveTo(PaintDC, X + W / 2 - L, Y);
    LineTo(PaintDC, X + W / 2 + L + 1, Y);
    Y += DY;
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TLineBar::Paint( HDC PaintDC, PAINTSTRUCT& )
{
  int I, X, W;
  RECT R;

  X = 4;
  for ( I = 0; I < LineCount; I++)
  {
    W = LineWidth[I];
    SetRect(&R, X, 5, X + W, 25);
    FillRect(PaintDC, &R, (HBRUSH)GetStockObject(BLACK_BRUSH));
    if (W == State->PenSize)
    {
      Notch(PaintDC, X, W, 0, 1);
      Notch(PaintDC, X, W, 29, -1);
    };
    X += W + 4;
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TLineBar::WMLButtonDown( TMessage& Message )
{
  int I, X, W;

  X = 2;
  for( I = 0; I < LineCount; I++)
  {
    W = LineWidth[I];
    if ((Message.LP.Lo >= X) && (Message.LP.Lo < (X + W + 4)))
    {
      State->PenSize = W;
      InvalidateRect(HWindow, NULL, TRUE);
      return;
    }
    X += W + 4;
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TCanvas ------------------------

TCanvas::TCanvas( PTWindowsObject AParent, PTState AState) : TWindow(AParent, NULL)
{
  HDC DC, MemDC;

  Attr.Style = WS_BORDER | WS_CHILD | WS_VISIBLE;
  State = AState;
  Drawing = FALSE;
  DC = GetDC(0);
  State->BitMap = CreateCompatibleBitmap(DC, BitMapWidth, BitMapHeight);
  MemDC = CreateCompatibleDC(DC);
  SelectObject(MemDC, State->BitMap);
  PatBlt(MemDC, 0, 0, BitMapWidth, BitMapHeight, WHITENESS);
  DeleteDC(MemDC);
  ReleaseDC(0, DC);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

TCanvas::~TCanvas()
{
  DeleteObject(State->BitMap);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TCanvas::Paint( HDC PaintDC, PAINTSTRUCT& )
{
  HDC MemDC;
  RECT R;

  MemDC = CreateCompatibleDC(PaintDC);
  SelectObject(MemDC, State->BitMap);
  GetClientRect(HWindow, &R);
  BitBlt(PaintDC, 0, 0, R.right, R.bottom, MemDC, 0, 0, SRCCOPY);
  DeleteDC(MemDC);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TCanvas::WMLButtonDown( TMessage& Message )
{
  if (!Drawing)
  {
    Drawing = TRUE;
    State->DrawTool->MouseDown(HWindow, Message.LP.Lo, Message.LP.Hi, State);
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TCanvas::WMMouseMove( TMessage& Message )
{
  if (Drawing) State->DrawTool->MouseMove(Message.LP.Lo, Message.LP.Hi);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TCanvas::WMLButtonUp( TMessage& )
{
  HDC DC, MemDC;
  RECT R;

  if (Drawing)
  {
    State->DrawTool->MouseUp();
    Drawing = FALSE;
    DC = GetDC(HWindow);
    MemDC = CreateCompatibleDC(DC);
    SelectObject(MemDC, State->BitMap);
    GetClientRect(HWindow, &R);
    BitBlt(MemDC, 0, 0, R.right, R.bottom, DC, 0, 0, SRCCOPY);
    DeleteDC(MemDC);
    ReleaseDC(HWindow, DC);
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TCanvas::WMSetCursor( TMessage& )
{
  SetCursor(State->DrawTool->GetCursor());
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TPaintWin ------------------------

TPaintWin::TPaintWin( PTWindowsObject AParent, LPSTR ATitle) :
              TWindow(AParent, ATitle)
{
  State.DrawTool = NULL;
  State.BitMap = 0;
  State.PenSize = 3;
  State.PenColor = 0x00FFFFFFl;
  State.BrushColor = 0l;
  Palette = new TPalette(this, &State);
  ToolBar = new TToolBar(this, &State);
  LineBar = new TLineBar(this, &State);
  Canvas =  new TCanvas(this, &State);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TPaintWin::GetWindowClass( WNDCLASS& WndClass )
{
  TWindow::GetWindowClass(WndClass);
  WndClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE + 1;
  WndClass.hIcon = LoadIcon(GetApplication()->hInstance, "PaintIcon");
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TPaintWin::WMSize( TMessage& )
{
  int S;
  RECT R;

  GetClientRect(HWindow, &R);
  S = ((R.bottom - 8) / 17) * 3 + 1;
  MoveWindow(Palette->HWindow, 4, 4, S, R.bottom - 8, TRUE);
  MoveWindow(ToolBar->HWindow, S + 8, 4, ToolCount * 31 + 1, 32, TRUE);
  MoveWindow(LineBar->HWindow, S + ToolCount * 31 + 13, 4, LineBarWidth, 32, TRUE);
  MoveWindow(Canvas->HWindow, S + 8, 40, R.right - S - 12, R.bottom - 44, TRUE);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------- TPaintApp ------------------------

void TPaintApp::InitMainWindow()
{
   MainWindow = new TPaintWin(NULL, "Coyote Paint");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// -------------Main Program--------------------

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
         LPSTR lpCmd, int nCmdShow)
{
   TPaintApp App( "CoyPaint", hInstance, hPrevInstance,lpCmd, nCmdShow);

   inicializa();
   setcoord(XMIN,YMIN,XMAX,YMAX);

   App.Run();
   return ( App.Status );
}
