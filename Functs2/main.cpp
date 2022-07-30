#include <iostream>
#include <graphics.h>
#include <math.h>
#include <malloc.h>


using namespace std;

int graphdriver;
int graphmode;

float pi = 3.1415926535897932384654;

float a, b, c, d, y, g, h, z, x, t;
unsigned char frame;
unsigned char maxframe = 0;
void *buf[14];
int cnt;
unsigned tam;
int maxx;
int maxy;

void cria()
{
    tam = imagesize (0, 0, maxx, maxy);
    t = -100;
    frame = 1;

    do
    {
        b = -40;
        z = -100;

        do
        {
            a = -40;
            x = -100;

            do
            {
                y = t * exp (-x * x / 1000 - z * z / 1000);
                //y = t * sin (x / 100) * sin (z / 100);
                //y = (t / 2) * (exp (-2 * (x + z) / 1000) * sin (2 * pi * 2 * (x + z) / 1000));
                //y = t * ( (z * x * x * x / 100) + (-x * z * z * z / 100)) / 1000000;
                //y = t * exp (-0.04 * (x * x / 1000 + z * z / 1000)) * cos (0.15 * (x * x / 1000 + z * z / 1000));
                //y = t * ( (x * x / 55000) + (z * z / 55000));
                //y = t * sqrt ( (x * x / 55000) + (z * z / 55000));
                //y = (t / 10) * ( (x * x * x / 100000) + (z * z * z / 100000) - (4 * x / 100000) - (4 * z / 100000) + (x * z / 100000));
                //y = t * ( (x / 1000) + (z / 1000));
                //y = (t / 5) * ( (z * x / 10000) + (x * z / 10000));
                //y = t * ( (x / 1000) - (z / 1000));
                //y = (t / 2) * ( ( (x + z) / 100) * ( (x - z) / 100));
                //y = t * ( (x / 100000) + (2 * x * x / 100000));
                //y = t * ( (2 * z / 1000) - (x / 1000));
                //y = (t / 2) * exp (sqrt ( (x * x / 10000) + (z * z / 10000))) * cos (sqrt ( (x * x / 10000) + (z * z / 10000)));
                //y = (t / 200) * ( (3 * x * x / 1000) + (3 * z * z / 1000) + (8 * x / 1000) + (8 * z / 1000) + 9);
                //y = (t / 16) * (cos (2 * ( (x - z) / 50)) + cos (2 * ( (x - z) / 50)));
                //y = (t / 12) * ( (sin (2 * x / 100) + sin (5 * x / 100)) + (sin (2 * z / 100) + sin (5 * z / 100)));
                //y = (t / 12) * ( (cos (2 * x / 100) + cos (5 * x / 100)) + (cos (2 * z / 100) + cos (5 * z / 100)));
                d = 140 + x + b;
                g = 140 + a + z;
                c = 96 - y - b;
                h = 96 - y - a;
                putpixel (trunc (2 * d) * (maxx / 639), trunc (c) * (maxy / 479), 11);
                putpixel (trunc (2 * g) * (maxx / 639), trunc (h) * (maxy / 479), 12);
                a = a + 0.8;
                x = x + 2;
            }
            while (x < 99.99);

            b = b + 4;
            z = z + 10;
        }
        while (z <= 100);

        t += 15;

        if ( (buf[frame] = malloc (tam)) == NULL)
        {
            closegraph();
            printf ("Erro: sem memória.\n");
            exit (1);
        }

        getimage (0, 0, maxx, maxy, buf[frame]);
        frame++;
        cleardevice();
    }
    while (t <= 100);

    maxframe = frame - 1;
}

int main()
{
    graphdriver = DETECT;
    //initgraph (&graphdriver, &graphmode, "");
    initwindow (1280, 960, "Gráficos");
    maxx = getmaxx();
    maxy = getmaxy();
    setlinestyle (SOLID_LINE, 0, NORM_WIDTH);
    rectangle (0, 0, maxx, maxy);
    int txtTam = 10;
    settextstyle (TRIPLEX_FONT, HORIZ_DIR, txtTam);
    cleardevice();
    cria();

    while (!kbhit())
    {
        for (frame = 2; frame <= maxframe; frame++)
        {
            putimage (0, 0, buf[frame], COPY_PUT);
            delay (100);
        }

        for (frame = (maxframe - 1); frame >= 1; frame--)
        {
            putimage (0, 0, buf[frame], COPY_PUT);
            delay (100);
        }
    }

    for (frame = 1; frame <= maxframe; frame++)
    {
        free (buf[frame]);
    }

    return 0;
}
