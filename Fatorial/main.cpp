#include <iostream>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <process.h>
#include <errno.h>
#include <conio.h>
#include <windows.h>
#include <direct.h>
#include <io.h>

using namespace std;

long int n, i;
double m, f, g;
char c;
bool fim;

double pot (double x, double y)
{
    if (y > 0)
    {
        return exp (y * log (x));
    }
    else
    {
        return 1;
    }
}
///////////////////////////////////////////////////////////////////////
void gotoxy (int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition (GetStdHandle (STD_OUTPUT_HANDLE), pos);
}
///////////////////////////////////////////////////////////////////////
void clrscr()
{
    system ("cls");
}
///////////////////////////////////////////////////////////////////////



int main()
{
    fim = false;

    do
    {
        clrscr();
        m = 1;
        gotoxy (1, 12);
        printf ("0! = 1");
        gotoxy (1, 1);
        printf ("Entre com o numero: ");
        cin >>  g;

        if (g > 268609166)
        {
            g = 268609166;
        }

        n = trunc (g);

        if (n > 268609166)
        {
            n = 268609166;
        }

        if (n < 0)
        {
            n = -n;
        }

        gotoxy (1, 12);
        printf ("%i! =                                 ", n);

        if (n < 34)
        {
            for (i = 1; i <= n; i++)
            {
                m = m * i;
            }
        }

        if (n >= 34)
        {
            f = ( (n * log (n)) + (log (2 * M_PI * n)) / 2 - n) / (log (10));
            m = pot (10, (f - trunc (f)));
        }

        gotoxy (1, 12);

        if (n < 34)
        {
            printf ("%i! = %f", n, m);
        }

        if (n >= 34)
        {
            if (trunc (f) > 999)
            {
                printf ("%i! = %f E+%i", n, m, trunc (f));
            }
            else
            {
                if (trunc (f) > 99)
                {
                    printf ("%i! = %f E+0%i", n, m, trunc (f));
                }
                else
                {
                    if (trunc (f) > 9)
                    {
                        printf ("%i! = %f E+00%f", n, m, trunc (f));
                    }
                }
            }
        }

        do
        {
            gotoxy (1, 24);
            printf ("Quer continuar? (S/N) ");
            c = getch();
            c = toupper (c);
            //c = c.;

            if (c == 'S')
            {
                fim = false;
            }

            if (c == 'N')
            {
                fim = true;
            }
        }
        while ( (c != 'S') && (c != 'N'));
    }
    while (!fim);

    cout << "Hello world!" << endl;
    return 0;
}
