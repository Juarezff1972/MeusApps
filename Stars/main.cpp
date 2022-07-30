#include <iostream>
#include <process.h>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <dos.h>
#include <math.h>
#include <malloc.h>

#define MAX_STARS 300
#define SCALE 10

// linker options: -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
// library: libbgi.a

using namespace std;


int xmin, ymin, xmax, ymax; //, graphdriver, graphmode;

typedef struct
{
    unsigned int x, y;
} UPOINT;

typedef struct
{
    signed int x, y;
} SIGPOINT;

typedef struct
{
    UPOINT Loc;
    int Dist;
    SIGPOINT Dir;
    int Size;
    int Color;
} STAR;

struct
{
    STAR Stars[MAX_STARS];
    int NumStars;
    UPOINT Screen;
    int Speed;
} g;
/********************************************************************/
int random (int seed)
{
    return rand() % (seed + 1);
}
/********************************************************************/
void DrawStar (STAR *Star, int Color)
{
    int x, y;
    int r, g, b;
    x = Star->Loc.x / SCALE;
    y = Star->Loc.y / SCALE;
    r = RED_VALUE (Color);
    g = GREEN_VALUE (Color);
    b = BLUE_VALUE (Color);
    Color = COLOR (r * Star->Dist / 3000, g * Star->Dist / 3000, b * Star->Dist / 3000);
    putpixel (x, y, Color);

    if (Star->Size == 2)
    {
        putpixel (x + 1, y, Color);
        putpixel (x, y + 1, Color);
        putpixel (x + 1, y + 1, Color);
    }
}
/********************************************************************/
void NewStar (STAR *Star)
{
    Star->Loc.x = (g.Screen.x / 2 - SCALE + random (SCALE * 2));
    Star->Loc.y = (g.Screen.y / 2 - SCALE + random (SCALE * 2));
    Star->Dist = 0;
    Star->Size = random (10) == 9 ? 2 : 1;

    do
    {
        Star->Dir.x = SCALE * 10 - random (SCALE * 20);
        Star->Dir.y = SCALE * 10 - random (SCALE * 20);
    }
    while (Star->Dir.x == 0 && Star->Dir.y == 0);

    switch (random (40))
    {
        case 39:
            Star->Color = converttorgb (LIGHTRED);
            break;

        case 38:
            Star->Color = converttorgb (LIGHTGRAY);
            break;

        case 37:
            Star->Color = converttorgb (LIGHTBLUE);
            break;

        case 36:
            Star->Color = converttorgb (YELLOW);
            break;

        case 35:
            Star->Color = converttorgb (LIGHTMAGENTA);
            break;

        case 34:
            Star->Color = converttorgb (DARKGRAY);
            break;

        default:
            Star->Color = converttorgb (WHITE);
    }
}
/********************************************************************/
void MoveStars (void)
{
    //int i;
    STAR *Star;
    unsigned int origX;
    unsigned int origY;

    for (Star = &g.Stars[g.NumStars - 1]; Star >= g.Stars; Star--)
    {
        DrawStar (Star, BLACK);
        origX = Star->Loc.x;
        origY = Star->Loc.y;
        Star->Loc.x += Star->Dir.x * g.Speed / 30;
        Star->Loc.y += Star->Dir.y * g.Speed / 30;
        origX = Star->Loc.x - origX;
        origY = Star->Loc.y - origY;

        if (Star->Dist < 3000)
        {
            Star->Dist += sqrt (origX * origX + origY * origY);
        }

        if (Star->Dist > 3000)
        {
            Star->Dist = 3000;
        }

        if (Star->Loc.x < 1 || Star->Loc.x >= g.Screen.x || Star->Loc.y < 1 || Star->Loc.y >= g.Screen.y)
        {
            NewStar (Star);
        }
        else
        {
            DrawStar (Star, Star->Color);
        }
    }
}
/********************************************************************/
int main()
{
    //graphdriver = DETECT;
    //graphmode = CGAC1;
    //initgraph (&graphdriver, &graphmode, "");
    initwindow (1680, 1050, "Estrelas");
    xmax = getmaxx();
    ymax = getmaxy();
    puts ("\nStarfield simulation.\n");
    puts ("Keys:");
    puts ("+ Faster");
    puts ("- Slower");
    puts ("<ESC>Exits\n");
    puts ("Press any key to begin...");
    getch();
    srand (0);
    g.Screen.x = getmaxx() * SCALE;
    g.Screen.y = getmaxy() * SCALE;
    g.Speed = 20;

    while (1)
    {
        while (!kbhit())
        {
            if (g.NumStars < MAX_STARS)
            {
                NewStar (&g.Stars[g.NumStars]);
                g.NumStars++;
            }

            MoveStars();
        }

        switch (toupper (getch()))
        {
            case '+':
                if (g.Speed < 300)
                { g.Speed++; }

                break;

            case '-':
                if (g.Speed > 0)
                { g.Speed--; }

                break;

            case 27:
                closegraph();
                exit (0);
        }
    }

    return 0;
}
