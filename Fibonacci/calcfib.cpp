//cálculo da sequencia de fibonacci.
#include "calcfib.h"

unsigned long calcfib::getValor()
{
    return Valor;
}

void calcfib::setValor(unsigned long v)
{
    Valor=v;
}

unsigned long calcfib::calcula(unsigned long v)
{
    unsigned long f1,f2;

    if ((v==0) || (v==1)) return v;
    f1 = calcula(v - 1);
    f2 = calcula(v - 2);
    return  f1 + f2;
}

calcfib::calcfib()
{
}


