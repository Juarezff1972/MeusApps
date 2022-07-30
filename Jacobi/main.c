#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <math.h>
#include <conio.h>
////////////////////////////////////////////////////////////////////////////
#define Linha 10
#define Coluna 11
////////////////////////////////////////////////////////////////////////////
void Inicializa (void);
void Crit_Linhas (void);
void Resolve (void);
void Parada();
////////////////////////////////////////////////////////////////////////////
float mat[Linha][Coluna], vet[Coluna], vet1[Coluna], aux, aux1;
int i, j, k, l = 1;
unsigned n, m;
////////////////////////////////////////////////////////////////////////////
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

    n = n + 1;

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
        {
            printf ("\n Entre com o elemento %d da linha %d: ", j, i);
            scanf (" %f", &mat[i][j]);
        }

    printf (" \nEntre com as aproxima‡”es desejadas:");

    for (i = 0; i < m; i++)
    {
        printf ("\n x[%d]:", i);
        scanf (" %f", &vet[i]);
    }

    printf ("N§ maximo de iteracoes");
    scanf (" %d", &k);
}
/////////////////////////////////////////////////////////////////////////////
void Crit_Linhas (void)
{
    float sum = 0, alfa[Coluna];

    for (i = 0; i < Coluna; i++)
        alfa[i] = 0;

    for (i = 0; i < m; i++)
    {
        sum = 0;

        for (j = 0; j < m; j++)
        {
            if (i != j)
                sum = sum + mat[i][j];
        }

        alfa[i] = sum / mat[i][i];
    }

    for (i = 0; i < m; i++)
    {
        if (fabs (alfa[i]) >= 1)
        {
            printf ("\n Nada se Pode Afirmar!");
            return;
        }

        Resolve();
    }
}
////////////////////////////////////////////////////////////////////////////
void Resolve (void)
{
    float sum = 0;

    for (i = 0; i < Coluna; i++)
        vet1[i] = 0;

    for (i = 0; i < m; i++)
    {
        sum = 0;

        for (j = 0; j < m; j++)
            sum = sum + vet[j];

        vet1[i] = (mat[i][m] - (sum - vet[i])) / m;
        printf ("\n x[%d]:%f", i, vet1[i]);
    }

    Parada();
    l = l + 1;

    if ( (fabs (aux) > 0.001) && (l < k))
    {
        for (i = 0; i < Coluna; i++)
            vet[i] = vet1[i];

        Resolve();
    }
}
////////////////////////////////////////////////////////////////////////////
void Parada (void)
{
    int a;
    aux = 0;

    for (a = 0; a < m; a++)
    {
        if (aux < vet1[a])
        {
            aux = vet1[a];
            aux1 = a;
        }
    }

    aux = aux - vet[(int)aux1];
}
////////////////////////////////////////////////////////////////////////////
int main()
{
    Inicializa();
    Crit_Linhas();
    return 0;
}
