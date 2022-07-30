#include <math.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <process.h>
////////////////////////////////////////////////////////
float mat[30][31], x[31];
int i, j, k;
unsigned n, m;
float c;
////////////////////////////////////////////////////////
void Inicializa (void)
{
    printf (" O programa resolve sistemas de ate 30 linhas por 30 vari veis\n");
    printf ("\n N£mero de linhas=");
    scanf (" %u", &m);
    printf ("\n%u\n", m);

    if ( (m < 1) || (m > 30))
        exit (1);

    printf ("\n N£mero de vari veis=");
    scanf (" %u", &n);
    printf ("\n%u\n", n);

    if ( (n < 1) || (n > 30))
        exit (2);

    n = n + 1;

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
        {
            printf ("\n Entre com o elemento %d da linha %d: ", j, i);
            scanf (" %f", &mat[i][j]);
        }
}
////////////////////////////////////////////////////////
void Pivoteamento (void)
{
    int a;
    float aux[31];

    for (a = 0; a < 31; a++)
        aux[a] = 0;

    for (k = 0; k < m - 1; k++)
    {
        for (i = k; i < m; i++)
            for (a = i; a < m; a++)
            {
                if (fabs (mat[i][k]) < fabs (mat[a + 1][k]))
                {
                    for (j = 0; j < n; j++)
                    {
                        aux[j] = mat[i][j];
                        mat[i][j] = mat[a + 1][j];
                        mat[a + 1][j] = aux[j];
                    }
                }
            }
    }
}
////////////////////////////////////////////////////////
void Resolve (void)
{
    for (k = 0; k < m - 1; k++)
    {
        i = k + 1;

        while (i != m)
        {
            c = (mat[i][k] / mat[k][k]);

            for (j = 0; j < n; j++)
                mat[i][j] = (mat[i][j] - (c * mat[k][j]));

            i = i + 1;
        }
    }
}
////////////////////////////////////////////////////////
void Resposta (void)
{
    float sum[31];
    int i, j;

    for (i = 0; i < 31; i++)
        sum[i] = 0;

    for (j = 0; j < 31; j++)
        x[j] = 0;

    for (i = m; i > 0; i--)
    {
        for (j = n; j > 0; j--)
            sum[i - 1] = sum[i - 1] + (mat[i - 1][j - 1] * x[j - 1]);

        if (mat[i - 1][i - 1] == 0)
        {
            printf ("\nO sistema n„o ‚ determinado.");
            return;
        }
        else
            x[i - 1] = (mat[i - 1][m] - sum[i - 1]) / mat[i - 1][i - 1];
    }
}
/////////////////////////////////////////////////////////
int main()
{
    int i;
    //clrscr();
    Inicializa();
    Pivoteamento();
    Resolve();
    Resposta();

    for (i = 0; i < m; i++)
    {
        if (abs (x[i]) < 0.001)
            x[i] = 0;

        printf ("\n x[%d]=%.3f ", i, x[i]);
    }

   //getch();
    return 0;
}
