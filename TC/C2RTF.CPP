#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dos.h>
/////////////////////////////////////////////////////////////////////////////
FILE *fp,*wp;
char special=0;
char *buf;
/////////////////////////////////////////////////////////////////////////////
void MandaCodigo(char code)
{
   textcolor(code);
   if (code==BLACK) textbackground(WHITE); else textbackground(BLACK);
   putc('\\',wp);putc('c',wp);putc('f',wp);
   if (code==0)   putc('1',wp);
   if (code==9)   putc('2',wp);
   if (code==11)  putc('3',wp);
   if (code==10)  putc('4',wp);
   if (code==13)  putc('5',wp);
   if (code==12)  putc('6',wp);
   if (code==14)  putc('7',wp);
   if (code==15)  putc('8',wp);
   if (code==1)   putc('9',wp);
   if (code==3)  {putc('1',wp);putc('0',wp);}
   if (code==2)  {putc('1',wp);putc('1',wp);}
   if (code==5)  {putc('1',wp);putc('2',wp);}
   if (code==4)  {putc('1',wp);putc('3',wp);}
   if (code==6)  {putc('1',wp);putc('4',wp);}
   if (code==8)  {putc('1',wp);putc('5',wp);}
   if (code==7)  {putc('1',wp);putc('6',wp);}
   putc(' ',wp);
}
/////////////////////////////////////////////////////////////////////////////
void Imprime(unsigned char c)
{
   putch(c);
   if (c=='\\') putc(c,wp);
   if ((c=='{') || (c=='}')) putc('\\',wp);
   putc(c,wp);
}
/////////////////////////////////////////////////////////////////////////////
void VerificaRes(void)
{
   int i=0,frita;
   unsigned char c;
   unsigned char *batata;
   c=getc(fp);
   while((c=='_')||(isalpha(c)!=0)||(isdigit(c)!=0)||(c=='.'))
   {
      batata[i]=c;
      c=getc(fp);
      i++;
   }
   batata[i]=0;
   frita=i;
   MandaCodigo(LIGHTGREEN);
   if (strcmp(batata,"asm")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"inline")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"auto")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"break")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"case")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"cdecl")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"char")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"class")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"struct")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"const")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"continue")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"default")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"delete")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"do")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"double")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"else")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"enum")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"extern")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"far")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"float")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"for")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"friend")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"goto")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"huge")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"if")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"int")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"interrupt")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"near")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"short")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"long")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"new")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"operator")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"pascal")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"private")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"protected")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"public")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"register")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"return")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"signed")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"sizeof")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"static")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"switch")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"template")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"this")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"typedef")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"union")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"unsigned")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"virtual")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"void")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"volatile")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"while")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"_cs")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"_ds")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"_es")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"_ss")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"_export")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"_fastcall")==0) MandaCodigo(BLACK);
   if (strcmp(batata,"_loadds")==0) MandaCodigo(BLACK);
   for (i=0;i<=frita-1;i++)
   {
      if ((isdigit(batata[0])!=0)||
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
          (special==0)) MandaCodigo(RED); else MandaCodigo(LIGHTGREEN);

      if (c=='/')
      {
         ant=getc(fp);
         if (ant=='/')
         {
            MandaCodigo(BLUE);
            special=3;
            Imprime(c);Imprime(ant);
            c=0;
         } else
	 if (ant=='*')
         {
            MandaCodigo(BLUE);
            special=4;
            Imprime(c);Imprime(ant);
            c=0;
         } else { MandaCodigo(RED); ungetc(ant,fp); }
      }
      if (c=='*')
      {
         ant=getc(fp);
         if ((ant=='/')&&(special==4))
         {
             special=0;
             MandaCodigo(BLUE);
             Imprime(c);Imprime(ant);
             c=0;
         } else { MandaCodigo(RED); ungetc(ant,fp); }
      }
      if ((c==34)&&(special==0)) special=1; else
      if ((c==34)&&(special==1)) {special=0;MandaCodigo(LIGHTRED);}
      if ((c==39)&&(special==0)) special=2; else
      if ((c==39)&&(special==2)) {special=0;MandaCodigo(CYAN);}

      if (special==1) MandaCodigo(LIGHTRED);
      if (special==2) MandaCodigo(CYAN);
      if (special==3) MandaCodigo(BLUE);
      if (special==4) MandaCodigo(BLUE);
      if (special==5) MandaCodigo(GREEN);

      if (first=='#') { MandaCodigo(GREEN); special=5; }
      if (c==10)
      {
         Imprime(13);
         Imprime(10);
         putc('\\',wp);putc('p',wp);putc('a',wp);putc('r',wp);putc(' ',wp);
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
      printf("USO: C2RTF nome-de-arquivo-entrada nome-de-arquivo-saida\n");
      exit(2);
   }
   if (!(fp=fopen(argv[1],"rt")))
	{
		printf("O arquivo n„o pode ser aberto\n");
		exit(1);
	};
   if (setvbuf(fp,buf,_IOFBF,16383)!=0)
   {
      printf("Buffer fp falhou");
      exit(1);
   }
   if (!(wp=fopen(argv[2],"wt")))
   {
      printf("Arquivo de saida n„o pode ser aberto\n");
      exit(1);
   }
   if (setvbuf(wp,NULL,_IONBF,16383)!=0)
   {
      printf("Buffer wp falhou");
      exit(1);
   }
   fprintf(wp,"{\\rtf1\\ansi \\deff11\\deflang1033{\\fonttbl{\\f11\\fmodern\\fcharset0\\fprq1 Courier New;} }{\\colortbl;\\red0\\green0\\blue0;\\red0\\green0\\blue255;\n");
   fprintf(wp,"\\red0\\green255\\blue255;\\red0\\green255\\blue0;\\red255\\green0\\blue255;\\red255\\green0\\blue0;\\red255\\green255\\blue0;\\red255\\green255\\blue255;\\red0\\green0\\blue128;\\red0\\green128\\blue128;\\red0\\green128\\blue0;\\red128\\green0\\blue128;\\red128\\green0\\blue0;\n");
   fprintf(wp,"\\red128\\green128\\blue0;\\red128\\green128\\blue128;\\red192\\green192\\blue192;} \\margl1440\\margr1440\\margt1417\\margb1417 \\widowctrl\\ftnbj\\aenddoc\\hyphhotz425\\hyphcaps0 \\fet0\\sectd \\pgnrestart\\linex0\\headery1440\\footery1440\\colsx709\n");
   fprintf(wp,"\\pard\\plain \\qj\\tx0\\tx720\\tx1440\\tx2160\\tx2880\\tx3600\\tx4320\\tx5040\\tx5760\\tx6480\\tx7200\\tx7920\\tx8640\\tx9360\\tx10080\n");
   fprintf(wp,"{\\f11\\fs24\n");
   while (!feof(fp)) LerLinha();
   fclose(fp);
   fprintf(wp,"\n");
   fprintf(wp,"\\par }\\pard \\tx0\\tx720\\tx1440\\tx2160\\tx2880\\tx3600\\tx4320\\tx5040\\tx5760\\tx6480\\tx7200\\tx7920\\tx8640\\tx9360\\tx10080 \\pard\n");
   fprintf(wp,"\\par }\n");
   fclose(wp);
}
