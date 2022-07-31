/*    Programa para verificacao de tokens da linguagem C

      Alunos: (equipe Coyote)  Aderson Cleber Piffer      No.  1
                               Anderson Ferreira Martins  No.  3
                               Gustavo Henrique Busnardo  No.  9
                               Juarez Fioravanti Filho    No. 13
*/

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/////////////////////////////////////////////////////////////////////////////
FILE *fp;
char special=0;
char *buf;
/////////////////////////////////////////////////////////////////////////////
void MandaCodigo(char code)
{
   textcolor(code);
   if (code==WHITE) {putc(27,stdprn);putc('-',stdprn);putc('1',stdprn);
		     putc(27,stdprn);putc('F',stdprn);
		     putc(27,stdprn);putc('5',stdprn);
		     putc(27,stdprn);putc('r',stdprn);putc('0',stdprn);}
   if (code==MAGENTA) {putc(27,stdprn);putc('-',stdprn);putc('0',stdprn);
		       putc(27,stdprn);putc('F',stdprn);
		       putc(27,stdprn);putc('5',stdprn);
		       putc(27,stdprn);putc('r',stdprn);putc('1',stdprn);}
   if (code==YELLOW) {putc(27,stdprn);putc('-',stdprn);putc('0',stdprn);
		      putc(27,stdprn);putc('E',stdprn);
		      putc(27,stdprn);putc('5',stdprn);
		      putc(27,stdprn);putc('r',stdprn);putc('2',stdprn);}
   if (code==GREEN) {putc(27,stdprn);putc('-',stdprn);putc('0',stdprn);
		     putc(27,stdprn);putc('F',stdprn);
		     putc(27,stdprn);putc('5',stdprn);
		     putc(27,stdprn);putc('r',stdprn);putc('6',stdprn);}
   if (code==BLUE) {putc(27,stdprn);putc('-',stdprn);putc('0',stdprn);
		    putc(27,stdprn);putc('F',stdprn);
		    putc(27,stdprn);putc('5',stdprn);
		    putc(27,stdprn);putc('r',stdprn);putc('2',stdprn);}
   if (code==LIGHTRED) {putc(27,stdprn);putc('-',stdprn);putc('0',stdprn);
			putc(27,stdprn);putc('F',stdprn);
			putc(27,stdprn);putc('5',stdprn);
			putc(27,stdprn);putc('r',stdprn);putc('3',stdprn);}
   if (code==LIGHTGREEN) {putc(27,stdprn);putc('-',stdprn);putc('0',stdprn);
			  putc(27,stdprn);putc('F',stdprn);
			  putc(27,stdprn);putc('5',stdprn);
			  putc(27,stdprn);putc('r',stdprn);putc('5',stdprn);}
   if (code==DARKGRAY) {putc(27,stdprn);putc('-',stdprn);putc('0',stdprn);
			putc(27,stdprn);putc('F',stdprn);
			putc(27,stdprn);putc('4',stdprn);
			putc(27,stdprn);putc('r',stdprn);putc('0',stdprn);}

}
/////////////////////////////////////////////////////////////////////////////
void Imprime(unsigned char c)
{
   putch(c);
   putc(c,stdprn);
//   if (wherey()>24) { getch(); clrscr(); }
}
/////////////////////////////////////////////////////////////////////////////
void VerificaRes(void)
{
   int i=0,frita;
   unsigned char c;
   unsigned char *batata;
   c=getc(fp);
   while((c=='_')||(islower(c)!=0)||(isdigit(c)!=0)||(c=='.'))
   {
      batata[i]=c;
      c=getc(fp);
      i++;
   }
   batata[i]=0;
   frita=i;
   MandaCodigo(LIGHTGREEN);
   if (strcmp(batata,"asm")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"inline")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"auto")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"break")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"case")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"cdecl")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"char")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"class")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"struct")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"const")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"continue")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"default")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"delete")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"do")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"double")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"else")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"enum")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"extern")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"far")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"float")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"for")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"friend")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"goto")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"huge")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"if")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"int")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"interrupt")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"near")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"short")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"long")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"new")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"operator")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"pascal")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"private")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"protected")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"public")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"register")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"return")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"signed")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"sizeof")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"static")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"switch")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"template")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"this")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"typedef")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"union")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"unsigned")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"virtual")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"void")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"volatile")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"while")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"_cs")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"_ds")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"_es")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"_ss")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"_export")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"_fastcall")==0) MandaCodigo(WHITE);
   if (strcmp(batata,"_loadds")==0) MandaCodigo(WHITE);
   for (i=0;i<=frita-1;i++)
   {
      if ((isdigit(batata[i])!=0)||
         ((batata[i]=='.')&&(isdigit(batata[i+1])!=0)))
      MandaCodigo(MAGENTA);
      Imprime(batata[i]);
   }
   ungetc(c,fp);
}
/////////////////////////////////////////////////////////////////////////////
void LerLinha(void)
{
   unsigned char c=0,first=0,ant=0;
   MandaCodigo(LIGHTGREEN);
   while(1)
   {
      c=getc(fp);
      if (c==9) {c=32;Imprime(32);Imprime(32);}
      if (feof(fp)) break;
      if ((c>32)&&(first==0)) first=c;
      if(((c=='!')||(c=='%')||(c=='&')||(c=='(')||(c==')')||(c=='*')||
          (c=='+')||(c==',')||(c=='-')||(c=='.')||(c=='/')||(c==':')||
          (c==';')||(c=='<')||(c=='=')||(c=='>')||(c=='?')||(c=='[')||
          (c==']')||(c=='^')||(c=='{')||(c=='}')||(c=='|')||(c=='~'))&&
          (special==0)) MandaCodigo(YELLOW); else MandaCodigo(LIGHTGREEN);

      if (c=='/')
      {
         ant=getc(fp);
         if (ant=='/')
         {
            MandaCodigo(DARKGRAY);
            special=3;
            Imprime(c);Imprime(ant);
            c=0;
         } else
	 if (ant=='*')
         {
            MandaCodigo(DARKGRAY);
            special=4;
            Imprime(c);Imprime(ant);
            c=0;
         } else { MandaCodigo(YELLOW); ungetc(ant,fp); }
      }
      if (c=='*')
      {
         ant=getc(fp);
         if ((ant=='/')&&(special==4))
         {
             special=0;
             MandaCodigo(DARKGRAY);
             Imprime(c);Imprime(ant);
             c=0;
         } else { MandaCodigo(YELLOW); ungetc(ant,fp); }
      }
      if ((c==34)&&(special==0)) special=1; else
      if ((c==34)&&(special==1)) {special=0;MandaCodigo(LIGHTRED);}
      if ((c==39)&&(special==0)) special=2; else
      if ((c==39)&&(special==2)) {special=0;MandaCodigo(GREEN);}

      if (special==1) MandaCodigo(LIGHTRED);
      if (special==2) MandaCodigo(GREEN);
      if (special==3) MandaCodigo(DARKGRAY);
      if (special==4) MandaCodigo(DARKGRAY);
      if (special==5) MandaCodigo(BLUE);

      if (first=='#') { MandaCodigo(BLUE); special=5; }
      if (c==10)
      {
         Imprime(13);
         Imprime(10);
         if ((special==3)||(special==5)) special=0;
         break;
      }
      if (((c=='_')||(islower(c)!=0)||(isdigit(c)!=0)||(c=='.'))&&(special==0)) {ungetc(c,fp); VerificaRes();} else
      if (c>=32) Imprime(c);
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
   setvbuf(fp,buf,_IOFBF,32767);
   while (!feof(fp))
   LerLinha();
   fclose(fp);
}