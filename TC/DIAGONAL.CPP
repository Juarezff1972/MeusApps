#include <stdio.h>
#include <conio.h>
#include <process.h>
/////////////////////////////////////////////////////////////////////////
typedef float DIAG[6];
DIAG D;
int x,y,b;
float a;
/////////////////////////////////////////////////////////////////////////
float Consulta(int i,int j)
{
	if((1<=i)&&(i<=7)&&(1<=j)&&(j<=5))
	{
		if(i==j)
		{
			return (D[i]);
		}
		else return (0.0);
	}
	else
	return (100000);
}
/////////////////////////////////////////////////////////////////////////
void Atribui(int i,int j,float c)
{
	if((1<=i)&&(i<=7)&&(1<=j)&&(j<=5))
	{
		if(i==j)
		{
			D[i]=c;
		}
	}
}
/////////////////////////////////////////////////////////////////////////
void main()
{
	char zz,ii;
	FILE *fp;
	char *fname="DIAG.MAT";
	cprintf("\n\r\n\rLendo matriz diagonal...\n\r");
	if (!(fp=fopen(fname,"rt")))
	{
		cprintf("ERRO: O arquivo nÆo pode ser aberto.\n\r");
	};
	for(ii=1;ii<6;ii++) fscanf(fp,"%f",&D[ii]);
	fclose(fp);
	for(zz=1;zz!=0;zz=1)
	{
      cprintf("Ú                                  ¿\n\r");
      cprintf("³ %.2f   0.00   0.00   0.00   0.00 ³\n\r",D[1]);
      cprintf("³ 0.00   %.2f   0.00   0.00   0.00 ³\n\r",D[2]);
      cprintf("³ 0.00   0.00   %.2f   0.00   0.00 ³\n\r",D[3]);
      cprintf("³ 0.00   0.00   0.00   %.2f   0.00 ³\n\r",D[4]);
      cprintf("³ 0.00   0.00   0.00   0.00   %.2f ³\n\r",D[5]);
      cprintf("³ 0.00   0.00   0.00   0.00   0.00 ³\n\r",D[6]);
      cprintf("³ 0.00   0.00   0.00   0.00   0.00 ³\n\r",D[7]);
      cprintf("À                                  Ù\n\r");
		cprintf("\n\r Digite 0 para consulta, 1 para atribuir e 2 para sair:\n\r");
		cscanf("%d",&b);
		if(b==2)
		{
			cprintf("\n\r\n\rSalvando matriz diagonal...\n\r");
			if (!(fp=fopen(fname,"wt")))
			{
				cprintf("ERRO: O arquivo nÆo pode ser aberto.\n\r");
				exit(1);
			};
			for(ii=1;ii<6;ii++) fprintf(fp,"%.2f,\n\r",D[ii]);
			fclose(fp);
			exit(0);
		};
		cprintf("\n\r   X = ");
		cscanf("%d",&x);
		cprintf("\n\r   Y = ");
		cscanf("%d",&y);

		if(b==1)
		{
         a=20;
         while(!((a>=0)&&(a<10)))
         {
			   cprintf("\n\r   Valor = ");
			   cscanf("%f",&a);
         };
			Atribui(x,y,a);
		};
		if(b==0)
		{
			a=Consulta(x,y);
			if(a==100000)
			{
				cprintf("\n O n£mero nÆo pertence … matriz\r");
			}
			else cprintf("\n D[x,y]=%.1f\n\r",a);
		};
	}
}
