#include <iostream>
#include <graphics.h>
#include <math.h>


using namespace std;

int graphdriver;
int graphmode;

int rec;

long long int Fib (long long int n)
{
    long long int f;
    long long int f1;
    long long int f2;
    f = n;

    if (n == 0)
    {
        return 0;
    }
    else
    {
        if (n == 1)
        {
            return 1;
        }
        else
        {
            f1 = Fib (n - 1);
            f2 = Fib (n - 2);
            f = f1 + f2;
        }
    }

    return f;
}

void Fracline (int x1, int y1, int x2, int y2)
{
    int mx, my;
    rec++;
    cout << "Fracline (" << x1 << "," << y1 << "," << x2 << "," << y2 << ") nível " << rec << "\n";
    mx = ( (x1 + x2) / 2.0);
    my = round ( (y1 + y2) / 2.0);

    if ( ( (mx == x1) && (my == y1)) || ( (x1 == x2) && (y1 == y2)))
    {
        rec--;
        return;
    }

    putpixel (mx, my, 11);
    delay (50);
    Fracline (x1, y1, mx, my);
    Fracline (mx, my, x2, y2);
    setcolor (12);
    putpixel (x1, y1, 13);
    putpixel (x2, y2, 13);
    setcolor (13);
    rec--;
}

int main()
{
    int xmax;
    int ymax;
    int i;
    long long int f;
    int x;
    int y;
    int x1;
    int y1;
    graphdriver = DETECT;
    initgraph (&graphdriver, &graphmode, "");
    initwindow (1280, 960, "Fibonacci");
    xmax = getmaxx();
    ymax = getmaxy();
    setlinestyle (SOLID_LINE, 0, NORM_WIDTH);
    rectangle (0, 0, xmax, ymax);
    int txtTam = 10;
    settextstyle (TRIPLEX_FONT, HORIZ_DIR, txtTam);
    outtextxy (55, 40, "Hello world!");
    cout << "Hello world!" << endl;
    x1 = 0;
    y1 = 0;

    for (i = 0; i < 46; i++)
    {
        f = Fib (i);
        cout << i << " - " << f << "[ ";
        y = trunc ( (f + 1) / 1280) + 1;
        x = 1280 - ( (1280 * y) - (f + 1));

        if (x == 0)
        {
            y++;
        }

        while (x > 1280)
        {
            x = x / 2;
        }

        while (y > 960)
        {
            y = y / 2;
        }

        cout << x << "," << y << " ]\n";
        setcolor (12);
        line (x, y, x1, y1);
        setcolor (15);
        putpixel (x, y, 15);
        setcolor (14);
        circle (x, y, 20);
        x1 = x;
        y1 = y;
    }

    rec = 0;
    setlinestyle (SOLID_LINE, 0, THICK_WIDTH);
    Fracline (1, 600, 1270, 30);
    cout << "kbo";
    getch();
    return 0;
}
