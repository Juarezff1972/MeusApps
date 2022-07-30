#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

#define MAX_X 80
#define MAX_Y 20

#define OBSTACULO '#'
#define FRUTA '@'
#define BICHO 219

typedef struct
{
    int x;
    int y;
} COBRA;

char get_char_at_xy (int x, int y)
{
    CHAR_INFO ci;
    COORD xy = { 0, 0 };
    COORD zt = {1, 1};
    SMALL_RECT rect = { x, y, x, y };
    return ReadConsoleOutput (GetStdHandle (STD_OUTPUT_HANDLE), &ci, zt, xy, &rect) ? ci.Char.AsciiChar : '\0';
}

void gotoxy (int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition (GetStdHandle (STD_OUTPUT_HANDLE), pos);
}

int main()
{
    int i;
    int tamCobra = 15;
    char dx;
    char dy;
    int x;
    int y;
    char c;
    unsigned char alvo;
    int pontos = 0;
    COBRA serpente[MAX_X * MAX_Y];

    for (i = 0; i < MAX_X * MAX_Y; i++)
    {
        serpente[i].x = 0;
        serpente[i].y = 0;
    }

    cout << "Hello world!" << endl;

    for (i = 0; i < 12; i++)
    {
        cout << get_char_at_xy (i, 0) << endl;
    }

    dx = 0;
    dy = 0;
    x = MAX_X / 2;
    y = MAX_Y / 2;

    for (i = 0; i < tamCobra; i++)
    {
        serpente[i].x = x;
        serpente[i].y = y;
    }

    while (1)
    {
        if (kbhit())
        {
            c = getch();

            switch (toupper (c))
            {
                case 27:
                    return pontos;
                    break;

                case 'W':
                    if (dy == 0)
                    {
                        dy = -1;
                        dx = 0;
                    }

                    break;

                case 'S':
                    if (dy == 0)
                    {
                        dy = 1;
                        dx = 0;
                    }

                    break;

                case 'A':
                    if (dx == 0)
                    {
                        dx = -1;
                        dy = 0;
                    }

                    break;

                case 'D':
                    if (dx == 0)
                    {
                        dx = 1;
                        dy = 0;
                    }

                    break;
            }
        }

        for (i = tamCobra; i > 0; i--)
        {
            serpente[i].x = serpente[i - 1].x;
            serpente[i].y = serpente[i - 1].y;
        }

        x = x + dx;
        y = y + dy;

        if (x > MAX_X)
        {
            x = 0;
        }

        if (x < 0)
        {
            x = MAX_X;
        }

        if (y > MAX_Y)
        {
            y = 0;
        }

        if (y < 0)
        {
            y = MAX_Y;
        }

        alvo = get_char_at_xy (x, y);

        if (alvo != 32)
        {
            gotoxy (0, 0);
            printf ("-->> %c <<--", alvo);
        }

        switch (alvo)
        {
            case OBSTACULO:
            case BICHO:
                gotoxy (0, MAX_Y + 1);
                printf ("Fim de jogo... Você fez %i pontos.", pontos);
                return pontos;
                break;

            case FRUTA:
                pontos++;
                break;
        }

        serpente[0].x = x;
        serpente[0].y = y;
        gotoxy (serpente[0].x, serpente[0].y);
        printf ("%c", BICHO);
        gotoxy (serpente[tamCobra - 1].x, serpente[tamCobra - 1].y);
        printf (" ");
        usleep (100000);
    }

    return pontos;
}
