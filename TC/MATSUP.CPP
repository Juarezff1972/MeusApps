#include <stdio.h>
#include <conio.h>
#include <process.h>
//////////////////////////////////////////////////////////////////////
typedef float DIAG2[46];
DIAG2 D2;
int x,y,b;
float a;
//////////////////////////////////////////////////////////////////////
float Constri(int j,int i)
{
 if((1<=i)&&(i<=10)&&(1<=j)&&(j<=10))
 {
  if(j<i)
  {
	return(0.0);
  }
  else
  return(D2[(i+(j*(j-1))/2)]);
 }
 else return (100000);
}
//////////////////////////////////////////////////////////////////////
void Atrib(int j,int i,float c)
{
 if((1<=i)&&(i<=10)&&(1<=j)&&(j<=10))
 {
  if(j<i)
  {
  }
  else D2[(i+(j*(j-1))/2)]=c;
 };
}
//////////////////////////////////////////////////////////////////////
void main()
{
	char zz,ii;
	FILE *fp;
	char *fname="MATSUP.MAT";
	cprintf("\r\n\r\nLendo matriz diagonal...\r\n");
	if (!(fp=fopen(fname,"rt")))
	{
		cprintf("ERRO: O arquivo nÆo pode ser aberto.\r\n");
	};
	for(ii=1;ii<46;ii++) fscanf(fp,"%f",&D2[ii]);
	fclose(fp);
	for(zz=1;zz!=0;zz=1)
	{
      cprintf("Ú                                                              ¿\n\r");
      cprintf("³ %.2f   %.2f   %.2f   %.2f   %.2f   %.2f   %.2f   %.2f   %.2f ³\n\r",D2[1],D2[2],D2[4],D2[7],D2[11],D2[16],D2[22],D2[29],D2[37]);
      cprintf("³ 0.00   %.2f   %.2f   %.2f   %.2f   %.2f   %.2f   %.2f   %.2f ³\n\r",D2[3],D2[5],D2[8],D2[12],D2[17],D2[23],D2[30],D2[38]);
      cprintf("³ 0.00   0.00   %.2f   %.2f   %.2f   %.2f   %.2f   %.2f   %.2f ³\n\r",D2[6],D2[9],D2[13],D2[18],D2[24],D2[31],D2[39]);
      cprintf("³ 0.00   0.00   0.00   %.2f   %.2f   %.2f   %.2f   %.2f   %.2f ³\n\r",D2[10],D2[14],D2[19],D2[25],D2[32],D2[40]);
      cprintf("³ 0.00   0.00   0.00   0.00   %.2f   %.2f   %.2f   %.2f   %.2f ³\n\r",D2[15],D2[20],D2[26],D2[33],D2[41]);
      cprintf("³ 0.00   0.00   0.00   0.00   0.00   %.2f   %.2f   %.2f   %.2f ³\n\r",D2[21],D2[27],D2[34],D2[42]);
      cprintf("³ 0.00   0.00   0.00   0.00   0.00   0.00   %.2f   %.2f   %.2f ³\n\r",D2[28],D2[35],D2[43]);
      cprintf("³ 0.00   0.00   0.00   0.00   0.00   0.00   0.00   %.2f   %.2f ³\n\r",D2[36],D2[44]);
      cprintf("³ 0.00   0.00   0.00   0.00   0.00   0.00   0.00   0.00   %.2f ³\n\r",D2[45]);
      cprintf("À                                                              Ù\n\r");
		cprintf("\r\n Digite 0 para fazer a consulta, 1 para atribuir valores e 2 para sair:\r\n");
		cscanf(" %d",&b);
		if(b==2)
		{
			cprintf("\r\n\r\nSalvando matriz diagonal...\r\n");
			if (!(fp=fopen(fname,"wt")))
			{
				cprintf("ERRO: O arquivo nÆo pode ser aberto.\r\n");
				exit(1);
			};
			for(ii=1;ii<46;ii++) fprintf(fp,"%.2f\n\r",D2[ii]);
			fclose(fp);
			exit(0);
		};
		cprintf("\r\n X = ");
		cscanf(" %d",&x);
		cprintf("\r\n Y = ");
		cscanf(" %d",&y);
		if(b==1)
		{
         a=20;
         while(!((a>=0)&&(a<10)))
         {
			   cprintf("\n\r   Valor = ");
			   cscanf("%f",&a);
         };
			Atrib(x,y,a);
			b=3;
		};
		if(b==0)
		{
			a=Constri(x,y);
			if(a==100000)
			{
				cprintf("\r\n O n£mero nÆo pertence … matriz.\r\n");
			}
			else cprintf("\r\n D[x,y] = %f\r\n",a);
		};
	}
}

