#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <ctype.h>
#include <process.h>
#include <graphics.h>
#include <alloc.h>

//char ecuaciones[]="((((4*5)+2)*3)-7)";
//char ecuaciones[]="((1*(2+3))/(5-(3*4)))";
//char ecuaciones[]="a*b*c+(4+5/(2*3)+8-(3+4*5)-(5/d+3)+((2*g)+5))";
//char ecuaciones[]="2*3+4*5";
//char ecuaciones[]="a+b*c-d/e";
//char ecuaciones[]="4*5+2-5*3";
char *ecuaciones;
char *index;
char token[80],left[80],right[80];
char size,i,aux,val;
char *tempstr;
char openpars;
char LastPut;
///////////////////////////////////////////////////////////////////
char Min(char *ind)
{
   char aux,g,pos;
   pos=0;
   aux=99;
   for (g=0;g<=strlen(ind)-1;g++)
   {
      if ((ind[g]<aux)&&(ind[g]!=-1))
      {
         aux=ind[g];
         pos=g;
      }
   }
   return pos;
}
///////////////////////////////////////////////////////////////////
void substr(char *dest,char *str1,char tam,char inic)
{
   char q;
   for (q=0;q<=tam;q++) dest[q]=str1[q+inic];
   dest[q]=0;
}
///////////////////////////////////////////////////////////////////
void EmOrdem(char addr)
{
    if ((token[addr]!=-1)&&(addr!=-1))
    {
       EmOrdem(left[addr]);
       putch(ecuaciones[token[addr]]);
       EmOrdem(right[addr]);
    }
}
///////////////////////////////////////////////////////////////////
void PreOrdem(char addr)
{
    if ((token[addr]!=-1)&&(addr!=-1))
    {
       putch(ecuaciones[token[addr]]);
       PreOrdem(left[addr]);
       PreOrdem(right[addr]);
    }
}
///////////////////////////////////////////////////////////////////
void PosOrdem(char addr)
{
    if ((token[addr]!=-1)&&(addr!=-1))
    {
       PosOrdem(left[addr]);
       PosOrdem(right[addr]);
       putch(ecuaciones[token[addr]]);
    }
}
///////////////////////////////////////////////////////////////////
void Tree(char *substrg,char stpoint,char dir)
{
   char pos,*substrg1,*substrg2;
   char i,aux;
   pos=Min(substrg);
   aux=strlen(substrg);
   substrg1 = (char *) malloc(pos-1);
   substrg2 = (char *) malloc(aux-pos+1);
   for (i=0;i<=pos-1;i++) substrg1[i]=-1;
   for (i=0;i<=aux-pos+1;i++) substrg2[i]=-1;
   substr(substrg1,substrg,pos-1,0);
   substr(substrg2,substrg,aux-pos+1,pos+1);
   if ((strlen(substrg1)<1) || (strlen(substrg2)<1))
   {
      if (strlen(substrg1)<1) left[val]=-1;
      if (strlen(substrg2)<1) right[val]=-1;
      token[val]=stpoint+pos;
      return;
   }
   token[val]=stpoint+pos;
   val++;
   left[val-1]=val;
   Tree(substrg1,stpoint,0);
   val++;
   if(strlen(substrg2) !=0 ) LastPut++;
   right[LastPut]=val;
   Tree(substrg2,stpoint+pos+1,1);
}
///////////////////////////////////////////////////////////////////
void Inic(void)
{
   char y;
   openpars=0;
   val=0;
   LastPut=0;
   index = (char *) malloc(strlen(ecuaciones));
   for (i=0;i<=80;i++)
   {
      index[i]=-1;
      token[i]=-1;
      left[i]=-1;
      right[i]=-1;
   }
   size=strlen(ecuaciones);
   i=0;
   aux=0;
   for (y=0;y<=size-1;y++)
   {
      if (ecuaciones[y]=='(') i=i+10;
      if (ecuaciones[y]==')') i=i-10;
      if ((ecuaciones[y]=='*')||(ecuaciones[y]=='/')) index[y]=i+2;
      if ((ecuaciones[y]=='+')||(ecuaciones[y]=='-')) index[y]=i+1;
      if (isalnum(toupper(ecuaciones[y]))!=0) index[y]=i+3;
   }
   index[size]=0;
}
///////////////////////////////////////////////////////////////////
void SpaceCleaner(void)
{
   char e,k,v;
   char *aux;
   k=0;
   for (e=0;e<=strlen(ecuaciones);e++)
   {
      if (ecuaciones[e]!=32)
      {
         aux[k]=ecuaciones[e];
         k++;
      }
   }
   for (v=0;v<=k;v++)
   {
      ecuaciones[v]=aux[v];
   }
   ecuaciones[v]=0;
}
///////////////////////////////////////////////////////////////////
char FindError(void)
{
   char g,error=0;
   char op=0,cp=0;
   for (g=0;g<=strlen(ecuaciones);g++)
   {
      if (ecuaciones[g]==40) op++;
      if (ecuaciones[g]==41) cp++;
      if (cp>op)
      {
         error=g;
         textcolor(RED);
         cprintf("\r\n\r\nERRO!! Fecha parˆntese extra! \r\n\r\n");
         cprintf(ecuaciones);
         textcolor(RED+BLINK);
         gotoxy(error+1,wherey()); putch(ecuaciones[error]);
         break;
      }
   }
   if (error==0)
   {
      op=0;cp=0;
      for (g=strlen(ecuaciones);g>=0;g--)
      {
         if (ecuaciones[g]==40) op++;
         if (ecuaciones[g]==41) cp++;
         if (cp<op)
         {
            error=g;
            textcolor(RED);
            cprintf("\r\n\r\nERRO!! Abre parˆntese extra! \r\n\r\n");
            cprintf(ecuaciones);
            textcolor(RED+BLINK);
            gotoxy(error+1,wherey()); putch(ecuaciones[error]);
            break;
         }
      }
   }
   if (error==0)
   {
      for(g=0;g<strlen(ecuaciones);g++)
      {
        if ((strchr("+-/*(",ecuaciones[g])!=0)
           && (strchr("+-/*)",ecuaciones[g+1])!=0))
        {
           error=g;
           textcolor(RED);
           cprintf("\r\n\r\nERRO!! Operador ou parˆntese inv lido! \r\n\r\n");
           cprintf(ecuaciones);
           textcolor(RED+BLINK);
           gotoxy(error+1,wherey()); putch(ecuaciones[error]);
           break;
        }
      }
   }
   if (error==0)
   {
      for(g=0;g<strlen(ecuaciones);g++)
      {
        if ((strchr("+-/*",ecuaciones[g])!=0)
           && ((ecuaciones[g+1]==0)||(g==0)))
        {
           error=g;
           textcolor(RED);
           cprintf("\r\n\r\nERRO!! Operador deve ter dois operandos! \r\n\r\n");
           cprintf(ecuaciones);
           textcolor(RED+BLINK);
           gotoxy(error+1,wherey()); putch(ecuaciones[error]);
           if (error==0) error=-1;
           break;
        }
      }
   }
   if (error==0)
   {
      for(g=0;g<strlen(ecuaciones);g++)
      {
        if ((isalnum(toupper(ecuaciones[g]))!=0)
           && (isalnum(toupper(ecuaciones[g+1]))!=0))
        {
           error=g;
           textcolor(RED);
           cprintf("\r\n\r\nERRO!! Operando deve ter apenas um caracter! \r\n\r\n");
           cprintf(ecuaciones);
           textcolor(RED+BLINK);
           gotoxy(error+1,wherey()); putch(ecuaciones[error]);
           if (error==0) error=-1;
           break;
        }
      }
   }
   textcolor(WHITE);
   gotoxy(1,wherey()+2);
   return error;
}
///////////////////////////////////////////////////////////////////
void GetData(void)
{
   char MeepMeep=0;
   ecuaciones = (char *) malloc(80);
   do
   {
      printf("\nEntre com a equa‡„o: ");
      gets(ecuaciones);
      SpaceCleaner();
      if (ecuaciones=="0") exit(0);
      MeepMeep=FindError();
   } while (MeepMeep!=0);
}
///////////////////////////////////////////////////////////////////
int LerGrade(void)
{
   FILE *in;
   unsigned char *aux1;
   int i;
   int x,y;
   int aux2,aux3;
   long maxx,maxy;
   if (!(in = fopen("arvbin5.bmp", "rb")) )
   {
      fprintf(stderr, "N„o posso abrir arquivo.\n");
      return 1;
   }
   for(i=1;i<=18;i++) getc(in);
   for(i=3;i>=0;i--) aux1[i]=getc(in);
   maxx=(aux1[2]*256+aux1[3])+(aux1[0]*256+aux1[1])*65536;
   for(i=3;i>=0;i--) aux1[i]=getc(in);
   maxy=(aux1[2]*256+aux1[3])+(aux1[0]*256+aux1[1])*65536;
   for(i=1;i<=28;i++) getc(in);
   for(i=0;i<=15;i++)
   {
      aux1[1]=getc(in);
      aux1[2]=getc(in);
      aux1[3]=getc(in);
      if (i<=7) setrgbpalette(i,aux1[3]/4,aux1[2]/4,aux1[1]/4);
      if (i==8) setrgbpalette(20,aux1[3]/4,aux1[2]/4,aux1[1]/4);
      if (i>=9) setrgbpalette(48+i,aux1[3]/4,aux1[2]/4,aux1[1]/4);
      getc(in);
   }

   for(y=maxy-1;y>=0;y--)
   {
      for(x=0;x<=maxx-1;x=x+2)
      {
         aux2=getc(in);
         aux3=(aux2/16);
         putpixel(x,y,aux3);
         aux3=aux2-aux3*16;
         putpixel(x+1,y,aux3);
      }
   }
   fclose(in);
   if (kbhit()) getc(stdin);
   setcolor(15);
   while (!kbhit());
   getch();
   return 0;
}
///////////////////////////////////////////////////////////////////
void Inicializa(void)
{
   int gdriver, gmode;
   gdriver = DETECT;
   initgraph(&gdriver, &gmode, "");
   setgraphmode(VGAHI);
}
///////////////////////////////////////////////////////////////////
void main(void)
{
/*   Inicializa();
   LerGrade();
   closegraph();*/
   clrscr();
   GetData();
   Inic();
   Tree(index,0,-1);
   textcolor(15);
   putch(13);putch(10);
   cprintf("\nFormato Pr‚-ordem: ");
   PreOrdem(0);
   putch(13);putch(10);
   cprintf("\nFormato P¢s-ordem: ");
   PosOrdem(0);
   putch(13);putch(10);
   cprintf("\nFormato Central: %s ");
   EmOrdem(0);
   printf("\n\n\n");
}


