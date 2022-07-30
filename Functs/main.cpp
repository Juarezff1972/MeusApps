#include <iostream>
#include <graphics.h>
#include <math.h>
#include <malloc.h>


using namespace std;

//int graphdriver;
//int graphmode;

int a;
float b, c, d, e, f, g;
float n1, n2;
int h, bb;
int aa;
float cc, d1, dd;
float x, y;
long int x_1, y_1;
int maxx;
int maxy;
char frame;
int mx = 0;

void *buf[14];
int cnt;
unsigned tam;

void fundos()
{
    int corpt;
    x = bb + (h / b) + e;
    y = dd - (h / b) + f;
    x_1 = trunc (0.85 * x) + 50;
    y_1 = trunc (0.9 * (g - y));

    if ( (y_1 < 0) or (y_1 > 190))
    {
        return;
    }

    //if (mx < ( ( (30 + dd) / 70) * 255))
    //{
    mx = ( ( (30 + dd) / 70) * 255);
    //cout << mx << "\n"; //max 35
    //}
    //cout << dd << "\n";
    corpt = COLOR (255 - mx, mx, mx); //abs(d1) / 2
    //setcolor (15);
    putpixel (2 * x_1, trunc (y_1 * maxy / 200), corpt);
    setcolor (COLOR (0, 64 + (abs (d1) / 2) * 64, 0)); //10
    line (2 * x_1, 1 + trunc (y_1 * maxy / 200), 2 * x_1, 30 + trunc (y_1 * maxy / 200));
    putpixel (2 * x_1, 30 + trunc (y_1 * maxy / 200), corpt);
}

float funcao()
{
    //return 5 * exp (-cc) * sin (2 * 3.14159266 * cc);
    //return cos (2 * cc) + cos (5 * cc);
    //return cos (2 * cc) + sin (5 * cc);
    //return sin (2 * cc) + cos (5 * cc);
    return sin (2 * cc) + sin (5 * cc);
}

void criar()
{
    h = -a;

    do
    {
        aa = trunc (0.5 + sqrt (a * a - h * h));

        for (bb = -aa; bb < aa; bb++)
        {
            cc = sqrt (bb * bb + h * h) * d;
            d1 = funcao();
            dd = d1 * c;
            fundos();
        }

        h = h + trunc (b);
    }
    while (h != a);
}

int main()
{
    //graphdriver = DETECT;
    //initgraph (&graphdriver, &graphmode, "");
    initwindow (800, 600, "Functs");
    maxx = getmaxx();
    maxy = getmaxy();
    setlinestyle (SOLID_LINE, 0, NORM_WIDTH);
    rectangle (0, 0, maxx, maxy);
    int txtTam = 10;
    settextstyle (TRIPLEX_FONT, HORIZ_DIR, txtTam);
    //outtextxy (55, 40, "Hello world!");
    /*
       writeln('Para expandir ou comprimir o desenho verticalmente, entre um numero');
    write  ('entre -40 e 40 (20 e'' tipico).'); readln(n1);
    writeln;
    writeln;
    writeln('Para mover o desenho para cima ou para baixo na tela, entre um numero');
    write  ('entre -50 e 150 (90 e'' tipico).'); readln(n2);
    */
    n1 = 20;
    n2 = 90;
    a = 144;
    b = 2.25;
    //c = n1;
    d = 0.0327;
    e = 160;
    f = n2;
    g = 199;
    c = 0;

    for (cnt = 1; cnt <= 12; cnt++)
    {
        cleardevice();
        criar();
        c = c + (n1 / 12);
        tam = imagesize (0, 0, maxx, maxy);

        if ( (buf[cnt] = malloc (tam)) == NULL)
        {
            closegraph();
            printf ("Erro: sem memória.\n");
            exit (1);
        }

        getimage (0, 0, maxx, maxy, buf[cnt]);
    }

    cleardevice();

    while (!kbhit())
    {
        for (cnt = 1; cnt <= 11; cnt++)
        {
            putimage (0, 0, buf[cnt], COPY_PUT);
            delay (100);
        }

        for (cnt = 12; cnt >= 2; cnt--)
        {
            putimage (0, 0, buf[cnt], COPY_PUT);
            delay (100);
        }
    }

    for (cnt = 1; cnt <= 12; cnt++)
    {
        free (buf[cnt]);
    }

    //cout << "Hello world!" << endl;
    //getch();
    return 0;
}
