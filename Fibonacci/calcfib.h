#ifndef _ARTIF_FIB_H
#define _ARTIF_FIB_H

//cálculo da sequencia de fibonacci.
class calcfib
{
private:
    unsigned long Valor = 0;


public:
    unsigned long getValor() ;

    void setValor(unsigned long v);

    unsigned long calcula(unsigned long v);

    calcfib();


};
#endif
