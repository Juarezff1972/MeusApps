#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <process.h>
////////////////////////////////////////////////////////
float MAT[10][11],x[11];
int i,j,k;
unsigned n,m;
float c;
////////////////////////////////////////////////////////
void INICIALIZA(void)
{
 printf(" O programa resolve sistemas de ate 10 linhas por 10 vari veis\n");
 printf("\n N£mero de linhas=");
 scanf(" %u",&m);
 printf("\n%u\n",m);
 if ((m<1)||(m>10)) exit(1);
 printf("\n N£mero de vari veis=");
 scanf(" %u",&n);
 printf("\n%u\n",n);
 if ((n<1)||(n>10)) exit(2);
 n=n+1;
 for(i=0;i<m;i++)
  for(j=0;j<n;j++)
  {
   printf("\n Entre com o elemento %d da linha %d: ",j,i);
   scanf(" %f",&MAT[i][j]);
  }
}
////////////////////////////////////////////////////////
void RESOLVE(void)
{
 for(k=0;k<m-1;k++)
  {
   i=k+1;
   while(i!=m)
    {
     c=(MAT[i][k]/MAT[k][k]);
     for(j=0;j<n;j++)  MAT[i][j]=(MAT[i][j]-(c*MAT[k][j]));
     i=i+1;
    }
  }
}
////////////////////////////////////////////////////////
void resposta(void)
{
 float sum[11];
 int i,j;
 for(i=0;i<10;i++) sum[i]=0;
 for(j=0;j<11;j++) x[j]=0;
 for(i=m;i>0;i--)
 {
  for(j=n;j>0;j--)
  {
   sum[i-1]=sum[i-1]+(MAT[i-1][j-1]*x[j-1]);
  }
  x[i-1]=(MAT[i-1][m]-sum[i-1])/MAT[i-1][i-1];
 }
}
void main()
{
 int i;
 clrscr();
 INICIALIZA();
 RESOLVE();
 resposta();
 for(i=0;i<m;i++)
 {
    if (fabs(x[i])<0.001) x[i]=0;
    printf("\n x[%d]=%.3f ",i,x[i]);
 }
}