#include <math.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <process.h>
////////////////////////////////////////////////////////
float mat[10][11], L[10][11], x[10], x1[11];
int i, j, k;
unsigned n, m;
float c[11];
////////////////////////////////////////////////////////
void Inicializa (void)
{
    printf (" O programa resolve sistemas de ate 10 linhas por 10 vari veis\n");
    printf ("\n N£mero de linhas=");
    scanf (" %u", &m);
    printf ("\n%u\n", m);

    if ( (m < 1) || (m > 10))
        exit (1);

    printf ("\n N£mero de vari veis=");
    scanf (" %u", &n);
    printf ("\n%u\n", n);

    if ( (n < 1) || (n > 10))
        exit (2);

    n++;

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
        {
            printf ("\n Entre com o elemento %d da linha %d: ", j, i);
            scanf (" %f", &mat[i][j]);
        }
}
////////////////////////////////////////////////////////////////////
void Resolve (void)
{
    int a = 0;

    for (k = 0; k < m - 1; k++)
    {
        i = k + 1;

        while (i != m)
        {
            c[a] = (mat[i][k] / mat[k][k]);

            for (j = 0; j < m; j++)
                mat[i][j] = (mat[i][j] - (c[a] * mat[k][j]));

            i++;
            a++;
        }
    }
}
//////////////////////////////////////////////////////////////////////////
void Inferior (void)
{
    int a = 0;

    for (i = 0; i < m; i++)
        for (j = 0; j < m; j++)
        {
            if (i == j)
                L[i][j] = 1;

            if (i < j)
                L[i][j] = 0;

            if (i > j)
            {
                L[i][j] = c[a];
                a++;
            }
        }

    for (i = 0; i < m; i++)
        L[i][m] = mat[i][m];
}
/////////////////////////////////////////////////////////////////////////
void Sub_Avante (void)
{
    float sum1[11];
    int i, j;

    for (i = 0; i < 11; i++)
        sum1[i] = 0;

    for (j = 0; j < 11; j++)
        x1[j] = 0;

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < i; j++)
            sum1[i] = sum1[i] + (L[i][j] * x1[j]);

        x1[i] = (L[i][m] - sum1[i]);
    }
}
////////////////////////////////////////////////////////////////////////
void Retro_Sub (void)
{
    float sum[11];
    int i, j;

    for (i = 0; i < m; i++)
        mat[i][m] = x1[i];

    for (i = 0; i < 11; i++)
        sum[i] = 0;

    for (j = 0; j < 11; j++)
        x[j] = 0;

    for (i = m; i > 0; i--)
    {
        for (j = i; j < n; j++)
            sum[i - 1] = sum[i - 1] + (mat[i - 1][j] * x[j]);

        x[i - 1] = (mat[i - 1][m] - sum[i - 1]) / mat[i - 1][i - 1];
    }
}
///////////////////////////////////////////////////////////////////////
int main()
{
    Inicializa();
    Resolve();
    Inferior();
    Sub_Avante();
    Retro_Sub();

    for (i = 0; i < m; i++)
    {
        if (fabs (x[i]) < 0.00001)
            x[i] = 0;

        printf ("\n x[%d]=%.3f ", i, x[i]);
    }
    return 0;
}
