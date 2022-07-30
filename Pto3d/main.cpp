#include <iostream>
#include <graphics.h>
#include <math.h>


using namespace std;

int graphdriver;
int graphmode;

char op;
int    k, col, lin, maxy, maxx;

void ponto (int x, int y, int z)
{
    float col, lin, desvio;
    int   cor;
    desvio = x * 0.707106;
    col = (maxx / 2) - desvio + y;
    lin = (maxy / 2) + desvio - z;

    if ( ( (x % 5) == 0) || ( (y % 5) == 0))
    {
        cor = 5;
    }
    else
    {
        cor = 3;
    }

    putpixel (trunc (col), trunc (lin), cor);
}


int main()
{
    graphdriver = DETECT;
    initgraph (&graphdriver, &graphmode, "");
    initwindow (1280, 960, "Gráficos");
    maxx = getmaxx();
    maxy = getmaxy();
    setlinestyle (SOLID_LINE, 0, NORM_WIDTH);
    rectangle (0, 0, maxx, maxy);
    int txtTam = 10;
    settextstyle (TRIPLEX_FONT, HORIZ_DIR, txtTam);
    line (maxx / 2, maxy / 2, maxx, maxy / 2);
    line (maxx / 2, maxy / 2, maxx / 2, 0);
    line (maxx / 2, maxy / 2, (maxx / 2) - 200, (maxy / 2) + 200);

    for (lin = -100; lin <= 100; lin++)
    {
        for (col = -100; col <= 100; col++)
        {
            ponto (col, lin, round (sqrt (col * col + lin * lin)));
        }
    }

    getch();
    return 0;
}
