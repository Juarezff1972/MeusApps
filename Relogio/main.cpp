#include <iostream>
#include <graphics.h>
#include <time.h>
#include <math.h>


using namespace std;

int graphdriver;
int graphmode;


int main()
{
    const float Pi = 3.1415926535897932384654;
    float ys, rh, rm, rs, mr, h2, m2, sr;
    float h1, m1, s1, hd, md, sd, hr, s2;
    float xh, xm, xs, yh, ym;
    int x, y, h, m, s, z;
    float x1, x2, x3, y1, y2, y3;
    char c;
    time_t tempo;
    struct tm * timeinfo;
    int xmax;
    int ymax;
    graphdriver = DETECT;
    initgraph (&graphdriver, &graphmode, "");
    initwindow (1280, 960, "Gráficos");
    xmax = getmaxx();
    ymax = getmaxy();
    setlinestyle (SOLID_LINE, 0, NORM_WIDTH);
    rectangle (0, 0, xmax, ymax);
    int txtTam = 1;
    settextstyle (TRIPLEX_FONT, HORIZ_DIR, txtTam);
    //outtextxy (55, 40, "Hello world!");
    cout << "Hello world!" << endl;
    x = xmax / 2;
    y = ymax / 2;
    rh = 90;
    rm = 110;
    rs = 80;
    setcolor (15);
    circle (x, y, 120);
    setcolor (15);
    outtextxy (1, 1, "Relogio");
    setcolor (14);
    outtextxy (1, 15, "Copyright(C) 1994 by JFF");
    setcolor (10);
    outtextxy (1, 30, "Unauthorized duplication permited.");

    while (c != 27)
    {
        time (&tempo);

        timeinfo = localtime (&tempo);
        //cout << timeinfo << '\n';
        h = timeinfo->tm_hour;
        m = timeinfo->tm_min;
        s = timeinfo->tm_sec;
        h1 = hd;
        hd = (360 / 12) * (h + (md / 360));
        m1 = md;
        md = (360 / 60) * (m + (sd / 360));
        s1 = sd;
        sd = (360 / 60) * s;
        h2 = (h1 + 90) * (Pi / 180);
        hr = (hd + 90) * (Pi / 180);
        m2 = (m1 + 90) * (Pi / 180);
        mr = (md + 90) * (Pi / 180);
        s2 = (s1 + 90) * (Pi / 180);
        sr = (sd + 90) * (Pi / 180);
        xh = ( (rh * cos (hr)));
        yh = (- (rh * sin (hr)));
        xm = ( (rm * cos (mr)));
        ym = (- (rm * sin (mr)));
        xs = ( (rs * cos (sr)));
        ys = (- (rs * sin (sr)));
        x1 = ( (rh * cos (h2)));
        y1 = (- (rh * sin (h2)));
        x2 = ( (rm * cos (m2)));
        y2 = (- (rm * sin (m2)));
        x3 = ( (rs * cos (s2)));
        y3 = (- (rs * sin (s2)));
        setcolor (0);

        if (h1 != hd)
        {
            line (x, y, xmax - trunc (x + x1), trunc (y + y1));
        }

        if (m1 != md)
        {
            line (x, y, xmax - trunc (x + x2), trunc (y + y2));
        }
        if (s1 != sd)
        {
            line (x, y, xmax - trunc (x + x3), trunc (y + y3));
        }
        setcolor (13);
        line (x, y, xmax - trunc (x + xh), trunc (y + yh));
        setcolor (12);
        line (x, y, xmax - trunc (x + xm), trunc (y + ym));
        setcolor (11);
        line (x, y, xmax - trunc (x + xs), trunc (y + ys));

        if (kbhit() != 0)
        {
            c = getch();
        }
    }

    return 0;
}
