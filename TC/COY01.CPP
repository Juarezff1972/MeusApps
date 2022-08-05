#include <stdio.h>
void main(void)
{
   float x;
   float y;
   float *q;
   char *p;
   char p1,p2,p3,p4;
   x=0.1;
   p=(char *)&x;
   q=(float *)&x;
   p1=(char)*(0+p);
   p2=(char)*(1+p);
   p3=(char)*(2+p);
   p4=(char)*(3+p);
   y=*q;
   printf("Valor 1: %lf\t",y);
   printf("Valor 2: %ld\t",(long)*q);
   printf("bytes: %X,%X,%X,%X\n",p1,p2,p3,p4);
}