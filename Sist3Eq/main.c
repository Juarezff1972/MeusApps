#include <stdio.h>
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////
const n = 3;
const true = 1;
const false = 0;
float a[4][4];
unsigned char x, y;
unsigned vai, rsp;
/////////////////////////////////////////////////////////////////////////
void LerMatriz (unsigned char x, unsigned char y)
{
    char *c, *l;

    switch (x)
    {
        case 1 :
            l = "PRIMEIRO coeficiente";
            break;

        case 2 :
            l = "SEGUNDO coeficiente";
            break;

        case 3 :
            l = "TERCEIRO coeficiente";
            break;

        case 4 :
            l = "TERMO INDEPENDENTE";
            break;
    }

    switch (y)
    {
        case 1 :
            c = "PRIMEIRA";
            break;

        case 2 :
            c = "SEGUNDA";
            break;

        case 3 :
            c = "TERCEIRA";
            break;
    }

    printf ("Entre com o %s da %s linha:", l, c);
    scanf ("%f", &a[y][x]);
    printf ("\n");
}
/////////////////////////////////////////////////////////////////////////
void Det (void)
{
    float x, z;
    unsigned char t, i, s, g;
    char *c;

    for (t = 1; t <= n; t++)
    {
        if (a[t][1] == 0 && a[t][2] == 0 && a[t][3] == 0 && t != n)
        {
            i = 0;

            do
            {
                i++;

                while (a[3][1] != 0 && a[3][2] != 0 && a[3][3] != 0)
                {
                    for (s = 1; s <= n + 1; s++)
                    {
                        x = a[t][s];
                        a[t][s] = a[4 - i][s];
                        a[4 - i][s] = x;
                    }
                }
            }
            while (a[3][1] == 0 && a[3][2] == 0 && a[3][3] == 0);
        }
    }

    rsp = true;

    for (t = 1; t <= n; t++)
    {
        vai = false;

        for (i = 1; i <= n; i++)
        {
            if (a[t][i] != 0)
            {
                if (!vai)
                    g = i;

                vai = true;
            }
        }

        if (vai)
        {
            while (a[t][t] == 0 && a[t][t - 1] != 0)
            {
                for (s = 1; s <= n + 1; s++)
                {
                    x = a[t][s];
                    a[t][s] = a[g][s];
                    a[g][s] = x;
                }
            }

            while ( (a[t][t] == 0) && (a[t][t - 1] != 0))
                if (a[t][t] == 0)
                    z = 1 / a[t][t + 1];
                else
                    z = 1 / a[t][t];

            for (s = 1; s <= n + 1; s++)
                a[t][s] = z * a[t][s];

            for (i = 1; i <= n; i++)
            {
                if (i != t)
                {
                    z = -a[i][t];

                    for (s = 1; s <= n + 1; s++)
                        a[i][s] = a[i][s] + z * a[t][s];
                }
            }
        }

        if (!vai)
        {
            printf ("\n");
            c = "O sistema ‚ poss¡vel e indeterminado.";

            for (i = 1; i <= 3; i++)
                if ( (a[i][1] == 0) && (a[i][2] == 0) && (a[i][3] == 0) && (a[i][4] != 0))
                    c = "O sistema ‚ imposs¡vel.";

            if ( (a[3][1] == 0) && (a[3][2] == 0) && (a[3][3] == 0) && (a[3][4] == 0))
            {
                for (i = 1; i <= 2; i++)
                {
                    if ( (a[i][1] == 1.0) && (a[i][2] == 0) && (a[i][3] == 0))
                        printf ("X[1] = %.5f", a[i][4]);

                    if ( (a[i][1] == 0) && (a[i][2] == 1.0) && (a[i][3] == 0))
                        printf ("X[2] = %.5f", a[i][4]);

                    if ( (a[i][1] == 0) && (a[i][2] == 0) && (a[i][3] == 1.0))
                        printf ("X[3] = %.5f", a[i][4]);
                }

                c = " ";
            }

            printf ("%s\n", c);
            rsp = false;
        }
    }
}
/////////////////////////////////////////////////////////////////////////
void resposta (void)
{
    unsigned char i;
    printf ("\n");

    for (i = 1; i <= n; i++)
        printf ("X[%d] = %.5f", i, a[i][n + 1]);
}
/////////////////////////////////////////////////////////////////////////
int main()
{
    rsp = true;
    printf ("\n");

    for (x = 1; x <= n; x++)
    {
        for (y = 1; y <= n + 1; y++)
            LerMatriz (y, x);

        printf ("\n");
    }

    if (rsp)
        Det();

    if (rsp)
        resposta();

    return 0;
}
