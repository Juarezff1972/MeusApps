/*    Programa para verificacao de tokens da linguagem C

      Alunos: (equipe Coyote)  Adriana,  Anderson, Jos‚ Geraldo, Juarez
*/

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/////////////////////////////////////////////////////////////////////////////
#define SIMBOLO    0
#define STRING     1
#define CHARACT    2
#define COMENTARIO 3
#define DIRETIVA   4
#define IDENTIF    5
#define INTEIRO    6
#define REAL       7
#define HEXA       8
#define ERRO       9
/////////////////////////////////////////////////////////////////////////////
FILE *fp;
char special=0;
char *buf;
char last=10;
/////////////////////////////////////////////////////////////////////////////
void TipoToken(char code)
{
   if (code==SIMBOLO) printf("\nSimbolo: ");
   if (code!=last)
   {
      if (code==CHARACT) printf("\nCaracter: ");
      if (code==COMENTARIO) printf("\nComentario: ");
      if (code==DIRETIVA) printf("\nDiretiva: ");
      if (code==STRING) printf("\nString: ");
      if (code==IDENTIF) printf("\nIdentificador: ");
      if (code==INTEIRO) printf("\nNumero inteiro: ");
      if (code==REAL) printf("\nNumero real: ");
      if (code==HEXA) printf("\nNumero hexadecimal: ");
      if (code==ERRO) printf("\nErro: ");
   }
   textcolor(15-code);
   last=code;
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
   int i=0,aux,aux2,aux3,y=0;
   unsigned char c;
   unsigned char token[127],auxtkn[127];
   auxtkn[0]=0;
   c=getc(fp);
   while((c=='_')||(isalpha(c)!=0)||(isdigit(c)!=0)||(c=='.')||(c=='-'))
   {
      token[i]=c;
      c=getc(fp);
      i++;
   }
   token[i]=0;
   aux=i;
   aux2=0;
   if (token[0]=='-')
   {
      TipoToken(SIMBOLO);Imprime(token[0]);
      for(i=aux-1;i>=1;i--) ungetc(token[i],fp);
      return;
   }
   if ((isdigit(token[0])!=0)||(token[0]=='.'))
   {
      if ((token[0]=='0')&&((token[1]=='x')||(token[1]=='X')))
      {
          auxtkn[0]='0'; auxtkn[1]='x';
          i=2;
          while(isxdigit(token[i]))
          {
             auxtkn[i]=token[i];
             i++;
          }
          auxtkn[i]=0;
          aux2=i;
          TipoToken(HEXA);for (i=0;i<=aux2-1;i++) Imprime(auxtkn[i]);
          for(i=aux2;i<=aux;i++) ungetc(token[i],fp);
          ungetc(c,fp); return;
      }
      y=1;
      i=0;
      aux3=0;
      while(1)
      {
         if (isdigit(token[i])!=0) i++; else break;
         aux3=i;
      }
      if (toupper(token[i])=='E')
      {
         if (i>0)
         {
            i++;
            aux3=i;
            if (token[i]=='-')
            {
               i++;
               aux3=i;
               if (isdigit(token[i])!=0)
               {
                  y=2;
                  while(1)
                  {
                     i++;
                     aux3=i;
                     y=2;
                     if (isdigit(token[i])==0) break;
                  }
               }
            } else
               if (isdigit(token[i])!=0)
               {
                  y=2;
                  while(1)
                  {
                     i++;
                     aux3=i;
                     y=2;
                     if (isdigit(token[i])==0) break;
                  }
               }
         } else {TipoToken(ERRO);for (i=0;i<=aux-1;i++) Imprime(token[i]);}
      } else
         if (token[i]=='.')
         {
            i++;
            aux3=i;
            y=2;
            while(1)
            {
               if (isdigit(token[i])==0) break; else i++;
               aux3=i;
            }
            if (toupper(token[i])=='E')
            {
               i++;
               aux3=i;
               if (token[i]=='-')
               {
                  i++;
                  aux3=i;
                  if (isdigit(token[i])!=0)
                  {
                     y=2;
                     while(1)
                     {
                        i++;
                        aux3=i;
                        y=2;
                        if (isdigit(token[i])==0) break;
                     }
                  }
               } else
                  if (isdigit(token[i])!=0)
                  {
                     y=2;
                     while(1)
                     {
                        i++;
                        aux3=i;
                        y=2;
                        if (isdigit(token[i])==0) break;
                     }
                  }else {TipoToken(ERRO);for (i=0;i<=aux-1;i++) Imprime(token[i]);}
            }
         }
      if (y==1)
      {
         TipoToken(INTEIRO);
         for(i=0;i<=aux3-1;i++)
         Imprime(token[i]);
      }
      if (y==2)
      {
         TipoToken(REAL);
         for(i=0;i<=aux3-1;i++)
         Imprime(token[i]);
      }
      if(aux3<aux)
      {
         ungetc(c,fp);
         for(i=aux-1;i>aux3-1;i--)
          ungetc(token[i],fp);
         return;
      }
   } else
   if ((token[0]=='_')||(isalpha(token[0])!=0))
   {
      aux2=0;
      TipoToken(IDENTIF);
      while((token[aux2]!='-')&&(aux2<aux))
      {
         Imprime(token[aux2]);
         aux2++;
      }
      if (aux2<aux)
      {
         ungetc(c,fp);
         for(i=aux-1;i>aux2-1;i--) ungetc(token[i],fp);
         return;
      }
   } else
   { TipoToken(IDENTIF); for (i=0;i<=aux-1;i++) Imprime(token[i]);}
   ungetc(c,fp);
}
/////////////////////////////////////////////////////////////////////////////
void LerLinha(void)
{
   unsigned char c=0,first=0,ant=0,bug=0;
   while(!0)
   {
      c=getc(fp);
      if (c==9) {c=32;/*Imprime(32);Imprime(32);*/}
      if (feof(fp)) break;
      if ((c>32)&&(first==0)) first=c;
      if(((c=='(')||(c==')')||(c==',')||(c==':')||
          (c==';')||(c=='?')||(c=='[')||
          (c==']')||(c=='^')||(c=='{')||(c=='}')||(c=='~'))&&
          (special==0)) {TipoToken(SIMBOLO);Imprime(c);c=0;}
      if ((c=='\\')&&((special==1)||(special==2)))
      {
         ant=getc(fp);
         Imprime(c);Imprime(ant);
         c=0;
      }
      if ((c=='!')&&(special==0))
      {
         ant=getc(fp);
         if (ant=='=')
         {
            TipoToken(SIMBOLO);
            Imprime(c);Imprime(ant);
         }else { TipoToken(SIMBOLO);Imprime(c); ungetc(ant,fp); }
         c=0;
      }
      if ((c=='^')&&(special==0))
      {
         ant=getc(fp);
         if (ant=='=')
         {
            TipoToken(SIMBOLO);
            Imprime(c);Imprime(ant);
         }else { TipoToken(SIMBOLO);Imprime(c); ungetc(ant,fp); }
         c=0;
      }
      if ((c=='|')&&(special==0))
      {
         ant=getc(fp);
         if (ant=='=')
         {
            TipoToken(SIMBOLO);
            Imprime(c);Imprime(ant);
         }
         if (ant=='|')
         {
            TipoToken(SIMBOLO);
            Imprime(c);Imprime(ant);
         }else { TipoToken(SIMBOLO);Imprime(c); ungetc(ant,fp); }
         c=0;
      }
      if ((c=='&')&&(special==0))
      {
         ant=getc(fp);
         if (ant=='=')
         {
            TipoToken(SIMBOLO);
            Imprime(c);Imprime(ant);
         }
         if (ant=='&')
         {
            TipoToken(SIMBOLO);
            Imprime(c);Imprime(ant);
         }else { TipoToken(SIMBOLO);Imprime(c); ungetc(ant,fp); }
         c=0;
      }
      if ((c=='=')&&(special==0))
      {
         ant=getc(fp);
         if (ant=='=')
         {
            TipoToken(SIMBOLO);
            Imprime(c);Imprime(ant);
         }else { TipoToken(SIMBOLO);Imprime(c); ungetc(ant,fp); }
         c=0;
      }
      if ((c=='>')&&(special==0))
      {
         ant=getc(fp);
         if (ant=='>')
         {
            bug=getc(fp);
            if (bug=='=')
            {
               TipoToken(SIMBOLO);
               Imprime(c);Imprime(ant);Imprime(bug);
            } else { TipoToken(SIMBOLO); Imprime(c);Imprime(ant); ungetc(bug,fp);}
         }
         if (ant=='=')
         {
            TipoToken(SIMBOLO);
            Imprime(c);Imprime(ant);
         }else { TipoToken(SIMBOLO);Imprime(c); ungetc(ant,fp); }
         c=0;
      }
      if ((c=='<')&&(special==0))
      {
         ant=getc(fp);
         if (ant=='<')
         {
            bug=getc(fp);
            if (bug=='=')
            {
               TipoToken(SIMBOLO);
               Imprime(c);Imprime(ant);Imprime(bug);
            } else { TipoToken(SIMBOLO); Imprime(c);Imprime(ant); ungetc(bug,fp);}
         }
         if (ant=='=')
         {
            TipoToken(SIMBOLO);
            Imprime(c);Imprime(ant);
         }else { TipoToken(SIMBOLO);Imprime(c); ungetc(ant,fp); }
         c=0;
      }
      if ((c=='%')&&(special==0))
      {
         ant=getc(fp);
         if (ant=='=')
         {
            TipoToken(SIMBOLO);
            Imprime(c);Imprime(ant);
         }else { TipoToken(SIMBOLO);Imprime(c); ungetc(ant,fp); }
         c=0;
      }
      if ((c=='+')&&(special==0))
      {
         ant=getc(fp);
         if (ant=='=')
         {
            TipoToken(SIMBOLO);
            Imprime(c);Imprime(ant);
         }
         if (ant=='+')
         {
            TipoToken(SIMBOLO);
            Imprime(c);Imprime(ant);
         }
         else { TipoToken(SIMBOLO);Imprime(c); ungetc(ant,fp); }
         c=0;
      }
      if ((c=='-')&&(special==0))
      {
         ant=getc(fp);
         if (ant=='=')
         {
            TipoToken(SIMBOLO);
            Imprime(c);Imprime(ant);
         }
         if (ant=='>')
         {
            TipoToken(SIMBOLO);
            Imprime(c);Imprime(ant);
         }
         if (ant=='-')
         {
            TipoToken(SIMBOLO);
            Imprime(c);Imprime(ant);
         }
         else { TipoToken(SIMBOLO);Imprime(c);ungetc(ant,fp); }
         c=0;
      }
      if ((c=='/')&&(!((special>0)&&(special<4))))
      {
         ant=getc(fp);
         if ((ant =='=')&&(special==0))
         {
             TipoToken(SIMBOLO);
             Imprime(c);Imprime(ant);
             c=0;
         }
         if (ant=='/')
         {
            TipoToken(COMENTARIO);
            special=3;
            Imprime(c);Imprime(ant);
            c=0;
         } else
         if (ant=='*')
         {
            TipoToken(COMENTARIO);
            if (special==5) special=6; else special=4;
            Imprime(c);Imprime(ant);
            c=0;
         }else{TipoToken(SIMBOLO);Imprime(c);ungetc(ant,fp);}
         c=0;
      }
      if ((c=='*')&&(!((special>0)&&(special<4))))
      {
         ant=getc(fp);
         if ((ant =='=')&&(special==0))
         {
             TipoToken(SIMBOLO);
             Imprime(c);Imprime(ant);
             c=0;
         }
         if ((ant=='/')&&(special==6))
         {
             special=5;
             TipoToken(COMENTARIO);
             Imprime(c);Imprime(ant);
             c=0;
         }
         if ((ant=='/')&&(special==4))
         {
             special=0;
             TipoToken(COMENTARIO);
             Imprime(c);Imprime(ant);
             c=0;
         } else { TipoToken(SIMBOLO); Imprime(c);ungetc(ant,fp); }
         c=0;
      }
      if ((c==34)&&(special==0)) special=1; else
      if ((c==34)&&(special==1)) {special=0;TipoToken(STRING);}
      if ((c==39)&&(special==0)) special=2; else
      if ((c==39)&&(special==2)) {special=0;TipoToken(CHARACT);}
      if (special==1) TipoToken(STRING);
      if (special==2) TipoToken(CHARACT);
      if (special==3) TipoToken(COMENTARIO);
      if (special==4) TipoToken(COMENTARIO);
      if (special==5) TipoToken(DIRETIVA);
      if (special==6) TipoToken(DIRETIVA);
      if (first=='#') { TipoToken(DIRETIVA); special=5; }
      if (c==10)
      {
         Imprime(13);
         Imprime(10);
         if ((special==3)||(special==5)) special=0;
         break;
      }
      if (((c=='-')||(c=='_')||(isalpha(c)!=0)||(isdigit(c)!=0)
         ||(c=='.'))&&(special==0))
      {
         ungetc(c,fp);
         VerificaRes();
         TipoToken(100);
      } else if (c>=32) Imprime(c);
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
