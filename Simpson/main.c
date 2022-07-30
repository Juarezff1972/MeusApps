#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float a, b, h, z, x = 0;
unsigned n = 0, i;
///////////////////////////////////////////////////////////////////////
void Simpson (void);
float Funcao (void);
///////////////////////////////////////////////////////////////////////
void Simpson (void)
{
    h = ( (b - a) / (n - 1));
    z = h / 3.0;

    for (i = 0; i < n; i++)
    {
        if ( (i == 0) || (i == (n - 1)))
            x = x + Funcao();
        else
            if (i % 2 == 0)
                x = x + (2.0 * Funcao());
            else
                x = x + (4.0 * Funcao());
    }

    x = z * x;
    printf ("\n\nSimpson:%f", x);
}
////////////////////////////////////////////////////////////////////////
void Trapezio (void)
{
    h = ( (b - a) / (n - 1));
    z = h / 2.0;

    for (i = 0; i < n; i++)
    {
        if ( (i == 0) || (i == (n - 1)))
            x = x + Funcao();
        else
            x = x + (2.0 * Funcao());
    }

    x = z * x;
    printf ("\n\nTrap‚zio:%f", x);
}
////////////////////////////////////////////////////////////////////////
float Funcao (void)
{
    float y, w, g;
    g = (a + (i * h));
    w = (-g * g) / 2.0;
    y = (1.0 / sqrt (2.0 * M_PI)) * (exp (w));
    return y;
}
/*          Ú   ¿
            ³-gý³
            ³ÄÄÄ³
            ³ 2 ³
            À   Ù
           e
 f(g)  =  ÄÄÄÄÄÄÄÄ
            û2ã

*/
////////////////////////////////////////////////////////////////////////
int main()
{
    printf ("\nEntre com o inicio:");
    scanf ("%f", &a);
    printf ("\nEntre com o final:");
    scanf ("%f", &b);
    printf ("\nEntre com o n:");
    scanf ("%d", &n);
    Simpson();
    Trapezio();
    return 0;
}
