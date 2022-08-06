// Prototypes e variaveis globais de databank.c

#ifndef _DATABANK_H_
#define _DATABANK_H_

#define Atu 0
#define Req 1
#define Del 2

void DataBank(FILE *datb,char *buffer,unsigned long size,int operacao);

#endif