#include <stdio.h>
#include <conio.h>
#include <process.h>
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
   int ind;
   float val;
} dupla;
typedef dupla matab[31];
matab Atrib,Compct,Deslc;
//////////////////////////////////////////////////////////////////////////////
int lugar(matab w,int j)
{
   int k,corr;
   for(k=1;k<=10;k++)
   {
      corr=w[k].ind;
      if (corr==j) return k;
      if ((corr>0)||(corr>30)) break;
   }
   return 0;
}
//////////////////////////////////////////////////////////////////////////////
void desloca(matab x,int j,float g)
{
   matab y;
   int k,knovo;
   unsigned char aux;
   k=1;
   while ((unsigned)x[k].ind<(unsigned)j)
   {
      y[k]=x[k];
      k++;
   }
   for (knovo=k;knovo<=10;knovo++)
   {
      if ((x[knovo].ind<0)||(x[knovo].ind>30)) break;
      y[knovo+1]=x[knovo];
   }
   y[k].ind=j;
   y[k].val=g;
   for (aux=1;aux<=30;aux++)
   {
      Deslc[aux].ind=y[aux].ind;
      Deslc[aux].val=y[aux].val;
   }
}
//////////////////////////////////////////////////////////////////////////////
int compacta(matab x, int j)
{
   matab v;
   int k;
   unsigned char aux;
   if (j<1)
   {
      for (aux=1;aux<=30;aux++)
      {
         Compct[aux].ind=x[aux].ind;
         Compct[aux].val=x[aux].val;
      }
      return 0;
   };
   for (k=1;k<=(j-1);k++) v[k]=x[k];
   for (k=1;k<=9;k++)
   {
      v[k]=x[k+1];
      v[k].ind=-1;
      v[k].val=0.0;
      for (aux=1;aux<=30;aux++)
      {
         Compct[aux].ind=v[aux].ind;
         Compct[aux].val=v[aux].val;
      }
      return 0;
   }
   return 0;
}
//////////////////////////////////////////////////////////////////////////////
int AtribMatrEsp(matab v,int i,float r)
{
   matab u;
   int pos;
   unsigned char aux;
   if ((i<0)||(i>30)) {cprintf("\n\rERRO NO INDICE!\n\r");return 10000;}
   pos=lugar(v,i);
   if (pos!=0)
   {
      if (r!=0.0)
      {
         for (aux=1;aux<=30;aux++)
         {
            u[aux].ind=v[aux].ind;
            u[aux].val=v[aux].val;
         }
         u[pos].val=r;
         for (aux=1;aux<=30;aux++)
         {
            Atrib[aux].ind=u[aux].ind;
            Atrib[aux].val=u[aux].val;
         }
         return 0;
      }
      else
      {
         compacta(v,pos);
         for (aux=1;aux<=30;aux++)
         {
            u[aux].ind=Compct[aux].ind;
            u[aux].val=Compct[aux].val;
            Atrib[aux].ind=u[aux].ind;
            Atrib[aux].val=u[aux].val;
         }
         return 0;
      }
   }
   else
   {
      if (r==0.0) {*Atrib=*v; return 0;}
      else
      {
         if ((v[10].ind>=0)&&(v[10].ind<=30))
         {
            cprintf("\n\rSEM ESPAÄO!\n\r");
            return 10000;
         }
         else
         {
            desloca(v,i,r);
            for (aux=1;aux<=30;aux++)
            {
               u[aux].ind=Deslc[aux].ind;
               u[aux].val=Deslc[aux].val;
               Atrib[aux].ind=u[aux].ind;
               Atrib[aux].val=u[aux].val;
            }
            return 0;
         }
      }
   }
}
//////////////////////////////////////////////////////////////////////////////
float LeMatrEsp(matab v, int i)
{
   int pos;
   if ((i<0)||(i>30)) {cprintf("\n\rERRO NO INDICE!\n\r");return 9999.9999;}
   pos=lugar(v,i);
   if (pos!=0) return v[pos].val; else return 0.0;
}
//////////////////////////////////////////////////////////////////////////////
void main(void)
{
   unsigned char aux;
   matab Rough;
   int i,index;
   float value;
   unsigned short int escl;
   FILE *fp;
   char *fname="MATESP.MAT";
   cprintf("\r\n\r\nLendo matriz diagonal...\r\n");
	if (!(fp=fopen(fname,"rt")))
	{
		cprintf("ERRO: O arquivo n∆o pode ser aberto.\r\n");
	};
	for(i=0;i<31;i++)
   {
      fscanf(fp,"%f",&Rough[i].val);
      fscanf(fp,"%d",&Rough[i].ind);
   }
	fclose(fp);
   escl='3';
   while (escl!='2')
   {
      cprintf("\n\r\n\r\n\r");
      cprintf("Entre com 0 para atribuir, 1 para ler e 2 para sair:\n\r");
      cscanf("%d",&escl);
      if (escl==2)
      {
         cprintf("\r\n\r\nGravando matriz diagonal...\r\n");
      	if (!(fp=fopen(fname,"wt")))
        	{
      		cprintf("ERRO: O arquivo n∆o pode ser aberto.\r\n");
        	};
        	for(i=0;i<31;i++)
         {
            fprintf(fp,"%.2f\n\r",Rough[i].val);
            fprintf(fp,"%d\n\r",Rough[i].ind);
         }
      	fclose(fp);
         exit(0);
      }
      cprintf("\n\rEntre com o °ndice:");
      cscanf("%d",&index);
      if (escl==0)
      {
         cprintf("\n\rEntre com o valor:");
         cscanf("%f",&value);
         if (AtribMatrEsp(Rough,index,value)!=10000)
         for (aux=1;aux<=30;aux++)
         {
            Rough[aux].ind=Atrib[aux].ind;
            Rough[aux].val=Atrib[aux].val;
         }
      }
      if (escl==1)
      {
         cprintf("\n\rValor: %.2f\n\r",LeMatrEsp(Rough,index));
      }
   }
}
