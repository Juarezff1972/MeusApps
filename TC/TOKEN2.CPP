/*    Programa para verificacao de tokens da linguagem C
*/

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/////////////////////////////////////////////////////////////////////////////
FILE *fp,*out;
char special=0,lastcode=0;
char *buf;
/////////////////////////////////////////////////////////////////////////////
void MandaCodigo(char code)
{
   textcolor(code);  // APAGAR APOS PRONTO!
   if (code==WHITE) printf("\nPalavra Reservada: ");
   if (code==MAGENTA) printf("\nN£mero: ");
   if (code==YELLOW) printf("\nS¡mbolo: ");
   if (code==GREEN) printf("\nCaracter: ");
   if (code==LIGHTRED) printf("\nString: ");
   if (code==LIGHTCYAN) printf("\nIdentificador: ");
   if (code==DARKGRAY) printf("\nComent rio: ");
}
/////////////////////////////////////////////////////////////////////////////
void Imprime(unsigned char c)
{
   putch(c);
   if (wherey()>24) { getch(); clrscr();}
}
/////////////////////////////////////////////////////////////////////////////
void VerificaRes(void)
{
   int i=0,frita,res=0;
   unsigned char c;
   unsigned char batata[127];
   c=getc(fp);
   while((c=='_')||(islower(c)!=0)||(isupper(c)!=0)||(isdigit(c)!=0)||(c=='.'))
   {
      batata[i]=tolower(c);
      c=getc(fp);
      i++;
   }
   batata[i]=0;
   frita=i;
   if (strcmp(batata,"and")==0) res=1;
   if (strcmp(batata,"asm")==0) res=1;
   if (strcmp(batata,"array")==0) res=1;
   if (strcmp(batata,"begin")==0) res=1;
   if (strcmp(batata,"case")==0) res=1;
   if (strcmp(batata,"const")==0) res=1;
   if (strcmp(batata,"constructor")==0) res=1;
   if (strcmp(batata,"destructor")==0) res=1;
   if (strcmp(batata,"div")==0) res=1;
   if (strcmp(batata,"do")==0) res=1;
   if (strcmp(batata,"downto")==0) res=1;
   if (strcmp(batata,"else")==0) res=1;
   if (strcmp(batata,"end")==0) res=1;
   if (strcmp(batata,"end.")==0) res=1;
   if (strcmp(batata,"exports")==0) res=1;
   if (strcmp(batata,"file")==0) res=1;
   if (strcmp(batata,"for")==0) res=1;
   if (strcmp(batata,"function")==0) res=1;
   if (strcmp(batata,"goto")==0) res=1;
   if (strcmp(batata,"if")==0) res=1;
   if (strcmp(batata,"implementation")==0) res=1;
   if (strcmp(batata,"in")==0) res=1;
   if (strcmp(batata,"inherited")==0) res=1;
   if (strcmp(batata,"inline")==0) res=1;
   if (strcmp(batata,"interface")==0) res=1;
   if (strcmp(batata,"label")==0) res=1;
   if (strcmp(batata,"library")==0) res=1;
   if (strcmp(batata,"mod")==0) res=1;
   if (strcmp(batata,"nil")==0) res=1;
   if (strcmp(batata,"not")==0) res=1;
   if (strcmp(batata,"object")==0) res=1;
   if (strcmp(batata,"of")==0) res=1;
   if (strcmp(batata,"or")==0) res=1;
   if (strcmp(batata,"packed")==0) res=1;
   if (strcmp(batata,"procedure")==0) res=1;
   if (strcmp(batata,"program")==0) res=1;
   if (strcmp(batata,"record")==0) res=1;
   if (strcmp(batata,"repeat")==0) res=1;
   if (strcmp(batata,"set")==0) res=1;
   if (strcmp(batata,"shl")==0) res=1;
   if (strcmp(batata,"shr")==0) res=1;
   if (strcmp(batata,"string")==0) res=1;
   if (strcmp(batata,"then")==0) res=1;
   if (strcmp(batata,"to")==0) res=1;
   if (strcmp(batata,"type")==0) res=1;
   if (strcmp(batata,"unit")==0) res=1;
   if (strcmp(batata,"until")==0) res=1;
   if (strcmp(batata,"uses")==0) res=1;
   if (strcmp(batata,"var")==0) res=1;
   if (strcmp(batata,"while")==0) res=1;
   if (strcmp(batata,"with")==0) res=1;
   if (strcmp(batata,"xor")==0) res=1;
   if (strcmp(batata,"absolute")==0) res=1;
   if (strcmp(batata,"assembler")==0) res=1;
   if (strcmp(batata,"export")==0) res=1;
   if (strcmp(batata,"external")==0) res=1;
   if (strcmp(batata,"far")==0) res=1;
   if (strcmp(batata,"forward")==0) res=1;
   if (strcmp(batata,"interrupt")==0) res=1;
   if (strcmp(batata,"near")==0) res=1;
   if (strcmp(batata,"private")==0) res=1;
   if (strcmp(batata,"public")==0) res=1;
   if (strcmp(batata,"virtual")==0) res=1;
   if (isdigit(batata[0])!=0) MandaCodigo(MAGENTA); else
   if (res==0) MandaCodigo(LIGHTCYAN); else MandaCodigo(WHITE);
   for (i=0;i<=frita-1;i++) Imprime(batata[i]);
   ungetc(c,fp);
}
/////////////////////////////////////////////////////////////////////////////
void LerLinha(void)
{
   unsigned char c=0,first=0,ant=0;
   while(1)
   {
      c=getc(fp);
      if (c==9) c=32;
      if (feof(fp)) break;
      if ((c>32)&&(first==0)) first=c;
      if(((c=='*')||(c=='@')||(c=='$')||(c==')')||
          (c=='+')||(c==',')||(c=='-')||(c=='.')||(c=='/')||
          (c==';')||(c=='=')||(c=='[')||
          (c==']')||(c=='^'))&&
          (special==0)) MandaCodigo(YELLOW);

      if (c==':')
      {
         ant=getc(fp);
         if (ant=='=') {MandaCodigo(YELLOW);Imprime(c);Imprime(ant);c=0;}
         else {MandaCodigo(YELLOW);Imprime(c);ungetc(ant,fp);c=0;}
      }
      if (c=='<')
      {
         ant=getc(fp);
         if (ant=='=') {MandaCodigo(YELLOW);Imprime(c);Imprime(ant);c=0;}
         else if (ant=='>') {MandaCodigo(YELLOW);Imprime(c);Imprime(ant);c=0;}
         else {MandaCodigo(YELLOW);Imprime(c);ungetc(ant,fp);c=0;}
      }
      if (c=='>')
      {
         ant=getc(fp);
         if (ant=='=') {MandaCodigo(YELLOW);Imprime(c);Imprime(ant);c=0;}
         else {MandaCodigo(YELLOW);Imprime(c);ungetc(ant,fp);c=0;}
      }

      if (c=='(')
      {
         ant=getc(fp);
         if (ant=='*')
         {
            MandaCodigo(DARKGRAY);
            special=4;
            Imprime(c);Imprime(ant);
            c=0;
         } else {MandaCodigo(YELLOW);Imprime(c);ungetc(ant,fp);c=0;}
      }
      if (c=='*')
      {
         ant=getc(fp);
         if ((ant==')')&&(special==4))
         {
             special=0;
             MandaCodigo(DARKGRAY);
             Imprime(c);Imprime(ant);
             c=0;
         } else { MandaCodigo(YELLOW); ungetc(ant,fp); }
      }
      if (c=='{')
      {
         MandaCodigo(DARKGRAY);
         special=3;
         Imprime(c);
         c=0;
      }
      if ((c=='}')&&(special==3))
      {
         MandaCodigo(DARKGRAY);
         special=0;
         Imprime(c);
         c=0;
      }
      if ((c==34)&&(special==0)) special=1; else
      if ((c==34)&&(special==1)) {special=0;MandaCodigo(LIGHTRED);}
      if ((c==39)&&(special==0)) {special=2;MandaCodigo(GREEN);}else
      if ((c==39)&&(special==2)) {special=0;MandaCodigo(GREEN);}

/*      if (special==1) MandaCodigo(LIGHTRED);
      if (special==2) MandaCodigo(GREEN);
      if (special==3) MandaCodigo(DARKGRAY);
      if (special==4) MandaCodigo(DARKGRAY);
      if (special==5) MandaCodigo(BLUE);*/

      if (c==10)
      {
         Imprime(13);
         Imprime(10);
         if ((special==3)||(special==5)) special=0;
         break;
      }
      if (((c=='_')||(islower(c)!=0)||(isupper(c)!=0)||(isdigit(c)!=0)||(c=='.'))&&(special==0)) {ungetc(c,fp); VerificaRes();} else
      if (c>32) Imprime(c);
   }
}
//////////////////////////////////////////////////////////////////////////////
void main(int argc,char *argv[])
{
   clrscr();
   if (argc==0)
   {
      printf("USO: TOKEN nome-de-arquivo\n");
      exit(2);
   }
   if (!(fp=fopen(argv[1],"rt")))
	{
		printf("O arquivo n„o pode ser aberto\n");
		exit(1);
	};
   while (!feof(fp))
   LerLinha();
   fclose(fp);
}