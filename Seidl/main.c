#include <stdio.h>
#include <process.h>
#include <math.h>
#include <conio.h>
////////////////////////////////////////////////////////////////////////////
#define Linha 30
#define Coluna 31
////////////////////////////////////////////////////////////////////////////
void Inicializa (void);
void Crit_Linhas (void);
void Crit_Sassenfeld (void);
void No_Crit (void);
void Resolve (void);
void Parada();
////////////////////////////////////////////////////////////////////////////
float mat[Linha][Coluna], vet[Coluna], vet1[Coluna], aux, aux1, prec;
int i, j, k, l = 1;
unsigned n, m, cript;
////////////////////////////////////////////////////////////////////////////
void Inicializa (void)
{
    printf (" O programa resolve sistemas de ate 30 linhas por 30 vari veis\n");
    printf ("\n Qual o crit‚rio? (1 - Linhas, 2 - Sassenfeld, 3 - Nenhum)");
    scanf (" %u", &cript);
    printf ("\n N£mero de linhas=");
    scanf (" %u", &m);
    printf ("\n%u\n", m);

    if ( (m < 1) || (m > Linha))
        exit (1);

    printf ("\n N£mero de vari veis=");
    scanf (" %u", &n);
    printf ("\n%u\n", n);

    if ( (n < 1) || (n > (Coluna - 1)))
        exit (2);

    n = n + 1;

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
        {
            printf ("\n Entre com o elemento %d da linha %d: ", j, i);
            scanf (" %f", &mat[i][j]);
        }

    printf (" \nEntre com a aproxima‡„o desejada:");
    scanf (" %f", &prec);

    for (i = 0; i < m; i++)
        vet[i] = prec;

    printf ("N§ maximo de iteracoes:");
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
void No_Crit (void)
{
    int i;

    for (i = 0; i < m; i++)
        Resolve();
}
////////////////////////////////////////////////////////////////////////////
void Crit_Sassenfeld (void)
{
    float beta[Linha], sum = 0, maxbeta = 0;
    int i, j;

    for (i = 0; i <= Linha; i++)
        beta[i] = 1;

    i = 0;
    j = 0;

    for (i = 0; i <= (m - 1); i++)
    {
        for (j = 0; j <= (n - 2); j++)
        {
            if (i != j)
                sum = sum + (mat[i][j] * beta[j]);
        }

        beta[i] = sum / mat[i][i];
        sum = 0;

        if (beta[i] > maxbeta)
            maxbeta = beta[i];
    }

    if (fabs (maxbeta) >= 1)
    {
        printf ("\n N„o converge!");
        return;
    }

    for (i = 0; i < m; i++)
        Resolve();
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
            sum = sum + (mat[i][j] * vet[j]);

        vet1[i] = (mat[i][m] - (sum - (vet[i] * mat[i][i]))) / mat[i][i];
        printf ("\n x[%d]:%f", i, vet1[i]);
    }

    Parada();
    l = l + 1;

    if ( (fabs (aux) > prec) && (l < k))
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

    if (cript == 1)
        Crit_Linhas();

    if (cript == 2)
        Crit_Sassenfeld();

    if (cript == 3)
        No_Crit();

    return 0;
}
