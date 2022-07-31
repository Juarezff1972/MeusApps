// bird.cpp : Define o ponto de entrada para o aplicativo.
//

#include "framework.h"
#include "bird.h"

#define MAX_LOADSTRING 100

// Variáveis Globais:
HINSTANCE hInst;                                // instância atual
WCHAR szTitle[MAX_LOADSTRING];                  // O texto da barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // o nome da classe da janela principal

// Declarações de encaminhamento de funções incluídas nesse módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Coloque o código aqui.

	// Inicializar cadeias de caracteres globais
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_BIRD, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Realize a inicialização do aplicativo:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BIRD));

	MSG msg;

	// Loop de mensagem principal:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNÇÃO: MyRegisterClass()
//
//  FINALIDADE: Registra a classe de janela.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BIRD));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_BIRD);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNÇÃO: InitInstance(HINSTANCE, int)
//
//   FINALIDADE: Salva o identificador de instância e cria a janela principal
//
//   COMENTÁRIOS:
//
//        Nesta função, o identificador de instâncias é salvo em uma variável global e
//        crie e exiba a janela do programa principal.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Armazenar o identificador de instância em nossa variável global

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNÇÃO: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  FINALIDADE: Processa as mensagens para a janela principal.
//
//  WM_COMMAND  - processar o menu do aplicativo
//  WM_PAINT    - Pintar a janela principal
//  WM_DESTROY  - postar uma mensagem de saída e retornar
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Analise as seleções do menu:
			switch (wmId)
			{
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		case WM_PAINT:
		{
			RECT rc;
			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);
			SetBkColor(hdc, RGB(0, 0, 0));
			GetClientRect(hWnd, &rc);
			ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, 0, 0, 0);
			SetGraphicsMode(hdc, GM_COMPATIBLE);
			SelectObject(hdc, GetStockObject(DC_PEN));

			graphics_setup();
			sx = 0;
			sy = 24;
			coords();
			sx1 = sx;
			sy1 = sy;
			sx = 638;
			sy = 455;
			coords();
			sx2 = sx;
			sy2 = sy;
			setfillstyle(SOLID_FILL, clr[14]);
			forecolor = setcolor(clr[14]);
			//rectangle(sx1, sy1, sx2, sy2);
			setviewport(sx1 + 1, sy1 + 1, sx2 - 1, sy2 - 1, 1);
			//cor = 1;
			frame = 0;
			draw_bird();
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Manipulador de mensagem para a caixa 'sobre'.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}

void setarea(long x1, long y1, long x2, long y2)
{
	minx = x1;
	miny = y1;
	maxx = x2;
	maxy = y2;
}

void coords()
{
	sx = sx * (x_res / 1600);
	sy = sy * (y_res / 1200);
}

void moveto(int x, int y)
{
	MoveToEx(hdc, x, y, NULL);
}


void lineto(int x, int y)
{
	LineTo(hdc, x, y);
}

void graphics_setup()
{
	x_res = 1600;
	y_res = 1200;
	maxx = 838;
	minx = 1;
	maxy = 678;
	miny = 1;
	screen_x = x_res - 1;
	screen_y = y_res - 1;
	mode_flag = 4;
	//clr[0] = 1;
	for (t1 = 0; t1 < 16; t1++)
	{
		clr[t1] = t1;
	}
}

COLORREF setcolor(unsigned char cor)
{
	COLORREF color;
	color = RGB(255, 255, 255);
	switch (cor)
	{

		case 0:
			color = RGB(0, 0, 0);
			break;
		case 1:
			color = RGB(0, 0, 127);
			break;
		case 2:
			color = RGB(0, 127, 0);
			break;
		case 3:
			color = RGB(0, 127, 127);
			break;
		case 4:
			color = RGB(127, 0, 0);
			break;
		case 5:
			color = RGB(127, 0, 127);
			break;
		case 6:
			color = RGB(165, 42, 42);
			break;
		case 7:
			color = RGB(127, 127, 127);
			break;
		case 8:
			color = RGB(64, 64, 64);
			break;
		case 9:
			color = RGB(0, 0, 255);
			break;
		case 10:
			color = RGB(0, 255, 0);
			break;
		case 11:
			color = RGB(0, 255, 255);
			break;
		case 12:
			color = RGB(255, 0, 0);
			break;
		case 13:
			color = RGB(255, 0, 255);
			break;
		case 14:
			color = RGB(255, 255, 0);
			break;
		case 15:
			color = RGB(255, 255, 255);
			break;
		default:
			break;
	}
	SetDCBrushColor(hdc, color);
	SetDCPenColor(hdc, color);
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	/*
	 BLACK        0
	 BLUE         1
	 GREEN        2
	 CYAN         3
	 RED          4
	 MAGENTA      5
	 BROWN        6
	 LIGHTGRAY    7
	 DARKGRAY     8
	 LIGHTBLUE    9
	 LIGHTGREEN   10
	 LIGHTCYAN    11
	 LIGHTRED     12
	 LIGHTMAGENTA 13
	 YELLOW       14
	 WHITE        15
   */
	return color;
}


void rectangle(int x1, int y1, int x2, int y2)
{
	moveto(x1, y1);
	lineto(x2, y1);
	lineto(x2, y2);
	lineto(x1, y2);
	lineto(x1, y1);
}

void floodfill(int x, int y, COLORREF cor)
{
	BOOL r;
	SetBkColor(hdc, cor);
	r = ExtFloodFill(hdc, x, y, cor, FLOODFILLBORDER);
}

void bar(int x1, int y1, int x2, int y2)
{
	rectangle(x1, y1, x2, y2);
	floodfill((x1 + x2) / 2, (y1 + y2) / 2, fillColor);
}

/*void setpalette(int colornum, int color)
{
	clr[colornum] = color;
}*/


void setviewport(int left, int top, int right, int bottom, int clip)
{
	SetViewportOrgEx(hdc, left, top, NULL);
	SetViewportExtEx(hdc, right, bottom, NULL);
}

void setfillstyle(int style, int cor)
{
	fillColor = setcolor(cor);
}

void draw_bird(void)
{
	sx = 0;
	sy = 30;
	coords();
	sx1 = sx;
	sy1 = sy;
	sx = 385;
	sy = 458;
	coords();
	sx2 = sx;
	sy2 = sy;
	setfillstyle(SOLID_FILL, clr[7]);
	forecolor = setcolor(clr[7]);
	bar(sx1, sy1, sx2, sy2);
	forecolor = setcolor(clr[0]);
	pl = 0;
	sx = array1[pl][0];
	sy = array1[pl][1];
	pl++;
	coords();
	moveto(sx, sy);

	for (t1 = 1; t1 <= 55; t1++)
	{
		sx = array1[pl][0];
		sy = array1[pl][1];
		pl++;
		coords();
		lineto(sx, sy);
	}

	if (frame == 0)
	{
		pl = 0;
		sx = array2[pl][0];
		sy = array2[pl][1];
		pl++;
		coords();
		moveto(sx, sy);
		for (t1 = 1; t1 <= 26; t1++)
		{
			sx = array2[pl][0];
			sy = array2[pl][1];
			pl++;
			coords();
			lineto(sx, sy);
		}
		sx = array2[pl][0];
		sy = array2[pl][1];
		coords();
		setfillstyle(SOLID_FILL, clr[0]);
		floodfill(sx, sy, fillColor);
		forecolor = setcolor(clr[0]);
		pl = 0;
		sx = array3[pl][0];
		sy = array3[pl][1];
		pl++;
		coords();
		moveto(sx, sy);
		for (t1 = 1; t1 <= 15; t1++)
		{
			sx = array3[pl][0];
			sy = array3[pl][1];
			pl++;
			coords();
			lineto(sx, sy);
		}
		sx = array3[pl][0];
		sy = array3[pl][1];
		coords();
		setfillstyle(SOLID_FILL, clr[0]);
		floodfill(sx, sy, fillColor);
		forecolor = setcolor(clr[0]);
	}

	if (frame == 1)
	{
		pl = 0;
		sx = array6[pl][0];
		sy = array6[pl][1];
		pl++;
		coords();
		moveto(sx, sy);
		for (t1 = 1; t1 <= 20; t1++)
		{
			sx = array6[pl][0];
			sy = array6[pl][1];
			pl++;
			coords();
			lineto(sx, sy);
		}
		sx = array6[pl][0];
		sy = array6[pl][1];
		coords();
		setfillstyle(SOLID_FILL, clr[0]);
		floodfill(sx, sy, fillColor);
		forecolor = setcolor(clr[0]);
		pl = 0;
		sx = array7[pl][0];
		sy = array7[pl][1];
		pl++;
		coords();
		moveto(sx, sy);
		for (t1 = 1; t1 <= 16; t1++)
		{
			sx = array7[pl][0];
			sy = array7[pl][1];
			pl++;
			coords();
			lineto(sx, sy);
		}
		sx = array7[pl][0];
		sy = array7[pl][1];
		coords();
		setfillstyle(SOLID_FILL, clr[0]);
		floodfill(sx, sy, fillColor);
		forecolor = setcolor(clr[0]);
	}

	if (frame == 2)
	{
		pl = 0;
		sx = array8[pl][0];
		sy = array8[pl][1];
		pl++;
		coords();
		moveto(sx, sy);
		for (t1 = 1; t1 <= 15; t1++)
		{
			sx = array8[pl][0];
			sy = array8[pl][1];
			pl++;
			coords();
			lineto(sx, sy);
		}
		sx = array8[pl][0];
		sy = array8[pl][1];
		coords();
		setfillstyle(SOLID_FILL, clr[0]);
		floodfill(sx, sy, fillColor);
		forecolor = setcolor(clr[0]);
		pl = 0;
		sx = array9[pl][0];
		sy = array9[pl][1];
		pl++;
		coords();
		moveto(sx, sy);
		for (t1 = 1; t1 <= 15; t1++)
		{
			sx = array9[pl][0];
			sy = array9[pl][1];
			pl++;
			coords();
			lineto(sx, sy);
		}
		sx = array9[pl][0];
		sy = array9[pl][1];
		coords();
		setfillstyle(SOLID_FILL, clr[0]);
		floodfill(sx, sy, fillColor);
		forecolor = setcolor(clr[0]);
	}

	if (frame == 3)
	{
		pl = 0;
		sx = array10[pl][0];
		sy = array10[pl][1];
		pl++;
		coords();
		moveto(sx, sy);
		for (t1 = 1; t1 <= 15; t1++)
		{
			sx = array10[pl][0];
			sy = array10[pl][1];
			pl++;
			coords();
			lineto(sx, sy);
		}
		sx = array10[pl][0];
		sy = array10[pl][1];
		coords();
		setfillstyle(SOLID_FILL, clr[0]);
		floodfill(sx, sy, fillColor);
		forecolor = setcolor(clr[0]);
		pl = 0;
		sx = array11[pl][0];
		sy = array11[pl][1];
		pl++;
		coords();
		moveto(sx, sy);
		for (t1 = 1; t1 <= 12; t1++)
		{
			sx = array11[pl][0];
			sy = array11[pl][1];
			pl++;
			coords();
			lineto(sx, sy);
		}
		sx = array11[pl][0];
		sy = array11[pl][1];
		coords();
		setfillstyle(SOLID_FILL, clr[0]);
		floodfill(sx, sy, fillColor);
		forecolor = setcolor(clr[0]);
	}

	forecolor = setcolor(clr[0]);
	setfillstyle(SOLID_FILL, clr[0]);
	floodfill(sx, sy, fillColor);

	forecolor = setcolor(clr[1]);
	pl = 0;
	sx = array4[pl][0];
	sy = array4[pl][1];
	pl++;
	coords();
	moveto(sx, sy);

	for (t1 = 1; t1 <= 9; t1++)
	{
		sx = array4[pl][0];
		sy = array4[pl][1];
		pl++;
		coords();
		lineto(sx, sy);
	}
	sx = array4[pl][0];
	sy = array4[pl][1];
	coords();
	setfillstyle(SOLID_FILL, clr[1]);
	floodfill(sx, sy, fillColor);
	setfillstyle(SOLID_FILL, clr[4]);
	forecolor = setcolor(clr[4]);
	pl = 0;
	sx = array5[pl][0];
	sy = array5[pl][1];
	pl++;
	coords();
	moveto(sx, sy);
	for (t1 = 1; t1 <= 33; t1++)
	{
		sx = array5[pl][0];
		sy = array5[pl][1];
		pl++;
		coords();
		lineto(sx, sy);
	}
	sx = array5[pl][0];
	sy = array5[pl][1];
	coords();
	floodfill(sx, sy, fillColor);
	forecolor = setcolor(clr[7]);
	return;
}
