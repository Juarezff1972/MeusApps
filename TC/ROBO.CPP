#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
//////////////////////////////////////////////////////////////////////
#define MAX 200
#define SQUARE 0
#define TRIANGLE 1
#define PROGSIZE 1000
//////////////////////////////////////////////////////////////////////
struct oldpoints {
        int x,y;
} oldp[MAX];
struct loc_point {
        int x,y;
        char name[30];
} point[MAX];

int pos=0;
char prog[PROGSIZE];
char *p_pos=0;
char token[80];
char cur_x,cur_y;
//////////////////////////////////////////////////////////////////////
void gotoxy(int x,int y)
{
   union REGS regs;
   regs.h.ah=2;
   regs.h.dh=y;
   regs.h.dl=x;
   regs.h.bh=0;
   int86(16,&regs,&regs);
}
//////////////////////////////////////////////////////////////////////
void serror(char *s)
{
   gotoxy(0,24);
   printf("%s \n",s);
}
//////////////////////////////////////////////////////////////////////
int check_point(int a,int b)
{
   union REGS regs;
   gotoxy(a,b);
   regs.h.ah=8;
   regs.h.bh=0;
   int86(16,&regs,&regs);
   if(regs.h.al=='*') return 1;
   return 0;
}
//////////////////////////////////////////////////////////////////////
int outrange(int x,int y)
{
   if(x<0 || x>79) return 1;
   if(y<0 || y>24) return 1;
   return 0;
}
//////////////////////////////////////////////////////////////////////
void get_token(void)
{
   char *p;
   p=token;
   while(*p_pos==' ') p_pos++;
   if(*p_pos=='\n')
   {
      *p++='\n';
      *p='\0';
      p_pos++;
      return;
   }
   if(*p_pos=='\0')
   {
      *p++='\0';
      return;
   }
   while(*p_pos!=' ' && *p_pos!='\n')
   {
      *p=*p_pos++;
      p++;
   }
   *p='\0';
}
//////////////////////////////////////////////////////////////////////
void make_triangle(int x,int y)
{
   int t;
   for(t=0;t<5;t++)
   {
      gotoxy(x-t,y+t);
      printf("*");
      gotoxy(x+t,y+t);
      printf("*");
   }
   gotoxy(x-5,y+5);
   printf("***********");
}
//////////////////////////////////////////////////////////////////////
void make_right_triangle(int x,int y)
{
   int t;
   for(t=0;t<10;t++)
   {
      gotoxy(x,y+t);
      printf("*");
   }
   for(t=0;t<9;t++)
   {
      gotoxy(x-t+9,y+t);
      printf("*");
   }
   gotoxy(x,y);
   printf("*********");
}
//////////////////////////////////////////////////////////////////////
void make_square(int x,int y)
{
   int t;
   for (t=0;t<5;t++)
   {
      gotoxy(x,y+t);      printf("*");
      gotoxy(x+10,y+t);   printf("*");
   }
   gotoxy(x,y);     printf("***********");
   gotoxy(x,y+5);   printf("***********");
}
//////////////////////////////////////////////////////////////////////
void position(void)
{
   int y;
   clrscr();
   for(y=0;y<=22;y++)
   {
      gotoxy(65,y);
      printf("*");
   }
   gotoxy(65,9);    printf("**********");
   gotoxy(65,15);   printf("**********");
   for(y=9;y<=15;y++)
   {
      gotoxy(25,y);
      printf("*");
   }
   gotoxy(20,9);    printf("*****");
   gotoxy(20,15);   printf("*****");


   make_triangle(10,4);
   make_square(30,10);
   make_right_triangle(50,11);
}
//////////////////////////////////////////////////////////////////////
char move(int x,int y)
{
   if(outrange(cur_x,cur_y)) return 1;
   if(check_point(x,y)) return 0;
   gotoxy(cur_x,cur_y);
   putchar('.');
   gotoxy(x,y);
   putchar('#');
   delay(100);
   cur_x=x;cur_y=y;
   return 1;
}
//////////////////////////////////////////////////////////////////////
int find_path_right(int x,int y)
{
   if(check_point(x+1,y))
   {
      move(cur_x-1,cur_y);
      return 1;
   }
   else
   {
      move(cur_x+1,cur_y);
      return -1;
   }
}
//////////////////////////////////////////////////////////////////////
int find_path_down(int x,int y)
{
   if(check_point(x,y+1))
   {
      move(cur_x,cur_y-1);
      return 1;
   }
   else
   {
      move(cur_x,cur_y+1);
      return -1;
   }
}
//////////////////////////////////////////////////////////////////////
int find(int x,int y)
{
   register int t;
   for(t=0;t<pos;t++)
      if(oldp[t].x==x && oldp[t].y==y) return 1;
   return 0;
}
//////////////////////////////////////////////////////////////////////
int find_direction(int x,int y,int *incx, int *incy)
{
   register int a,b;
   for(a=-1;a<2;a++)
      for(b=-1;b<2;b++)
         if((check_point(x+a,y+b)) && (!find(x+a,y+b)))
         {
            *incx=a;
            *incy=b;
            return 1;
         }
   return 0;
}
//////////////////////////////////////////////////////////////////////
void assert_oldp(int x,int y)
{
   if(pos==MAX)
   {
      printf("Banco de dados cheio!\n");
      return;
   }
   if(find(x,y)) return;
   oldp[pos].x=x;
   oldp[pos].y=y;
   pos++;
}
//////////////////////////////////////////////////////////////////////
int follow(int x,int y)
{
   int incx,incy,startx,starty,count;
   startx=x;
   starty=y;
   count=0;
   assert_oldp(x,y);
   if(!find_direction(x,y,&incx,&incy)) return 0;
   do
   {
      while(check_point(x+incx,y+incy))
      {
         x=incx+x;
         y=incy+y;
         assert_oldp(x,y);
      }
      if((x==startx) && (y==starty)) return count;
      count++;
      if(!find_direction(x,y,&incx,&incy)) return 0;
   } while(1);
}
//////////////////////////////////////////////////////////////////////
int istriangle(int x,int y)
{
   if(follow(x,y)==2) return 1;
   return 0;
}
//////////////////////////////////////////////////////////////////////
int issquare(int x,int y)
{
   if(follow(x,y)==3) return 1;
   return 0;
}
//////////////////////////////////////////////////////////////////////
int find_point(int startx,int starty,int *x,int *y)
{
   int a,b;
   a=startx;
   b=starty;
   do
   {
      do
      {
         if(check_point(a,b))
         {
            *x=a;
            *y=b;
            return 1;
         }
         a++;
      } while(a<79);
      a=0;
      b++;
   } while(b<24);
   return 0;
}
//////////////////////////////////////////////////////////////////////
void clear_db(void)
{
   register int t;
   for(t=0;t<MAX;t++) oldp[t].x=oldp[t].y=0;
}
//////////////////////////////////////////////////////////////////////
int recognize(int *x1,int *y1,char obj)
{
   int x,y,t;
   t=0;
   x=cur_x;
   y=cur_y;
   if(obj==TRIANGLE)
      while(find_point(x,y,&x,&y))
      {
         if(istriangle(x,y))
         {
            gotoxy(t*40,0);
            t++;
            *x1=x;
            *y1=y;
            return 1;
         }
         x++;
      }
      else
      {
         clear_db();
         while(find_point(x,y,&x,&y))
         {
            if(issquare(x,y))
            {
               gotoxy(0,1);
               *x1=x;
               *y1=y;
               return 1;
            }
            x++;
         }
      }
   return 0;
}
//////////////////////////////////////////////////////////////////////
void findt(void)
{
   int x,y;
   if(recognize(&x,&y,TRIANGLE)) move(x-1,y);
}
//////////////////////////////////////////////////////////////////////
void finds(void)
{
   int x,y;
   if(recognize(&x,&y,SQUARE)) move(x-1,y);
}
//////////////////////////////////////////////////////////////////////
void do_move(void)
{
   get_token();
   if(!strcmp(token,"up")) move(cur_x,cur_y-1);
   else if(!strcmp(token,"down")) move(cur_x,cur_y+1);
        else if(!strcmp(token,"left")) move(cur_x-1,cur_y);
             else if(!strcmp(token,"right")) move(cur_x+1,cur_y);
}
//////////////////////////////////////////////////////////////////////
void ifsense(void)
{
   char xinc,yinc;
   xinc=yinc=0;
   get_token();
   if(!strcmp(token,"up")) yinc=1;
   else if(!strcmp(token,"down")) yinc=-1;
        else if(!strcmp(token,"left")) xinc=-1;
             else if(!strcmp(token,"right")) xinc=1;
   if(!check_point(cur_x+xinc,cur_y+yinc))
   {
      while(*p_pos!='\n' && *p_pos!='\0') p_pos++;
      return;
   }
   get_token();
   if(strcmp(token,"then"))
   {
      serror("-then- esperado\n");
   }
}
//////////////////////////////////////////////////////////////////////
void do_goto(void)
{
   char *p,label[80];
   get_token();
   strcpy(label,token);
   p=p_pos;
   p_pos=prog;
   get_token();
   while(*token)
   {
      if(!strcmp(token,label)) return;
      get_token();
   }
   p_pos=p;
}
//////////////////////////////////////////////////////////////////////
void moveto(int x,int y)
{
   signed char incx,incy;
   unsigned char mx,my;
   x=x-cur_x;
   y=y-cur_y;
   do
   {
      while(y)
      {
         if(y<0) incy=-1; else incy=1;
         if(move(cur_x,cur_y+incy)) y-=incy;
            else
            {
               x+=find_path_right(cur_x,cur_y);
            }
      }
      while(x)
      {
         if(x<0) incx=-1; else incx=1;
         if(move(cur_x+incx,cur_y)) x-=incx;
            else
            {
               y+=find_path_down(cur_x,cur_y);
            }
      }
   } while(x||y);
}
//////////////////////////////////////////////////////////////////////
void do_movep()
{
   register int t;
   get_token();
   for(t=0;t<MAX;t++)
   {
      if(!strcmp(point[t].name,token))
      {
         moveto(point[t].x,point[t].y);
         return;
      }
   }
   serror("Ponto n„o encontrado.");
}
//////////////////////////////////////////////////////////////////////
void moveto_setup(void)
{
   int x,y;
   get_token();
   x=atoi(token);
   get_token();
   y=atoi(token);
   if(outrange(x,y))
   {
      serror("Argumentos de 'moveto' est„o fora da faixa\n");
   }
   else moveto(x,y);
}
//////////////////////////////////////////////////////////////////////
void run(void)
{
   get_token();
   while(*token)
   {
      if(!strcmp(token,"moveto")) moveto_setup();
      if(!strcmp(token,"findt")) findt();
      if(!strcmp(token,"finds")) finds();
      if(!strcmp(token,"move")) do_move();
      if(!strcmp(token,"ifsense")) ifsense();
      if(!strcmp(token,"goto")) do_goto();
      if(!strcmp(token,"movep")) do_movep();
      get_token();
   }
}
//////////////////////////////////////////////////////////////////////
int get_point(void)
{
   register int t;
   for (t=1;t<MAX;t++) if(!*(point[t].name)) return t;
   return 0;
}
//////////////////////////////////////////////////////////////////////
void teach(void)
{
   int x,y,i;
   char name[30],num[10];
   strcpy(name,"point");
   position();
   gotoxy(0,0);
   x=cur_x, y=cur_y;
   for(;;)
   {
      switch(tolower(getch()))
      {
         case '8': if (move(x,y-1)) y--; break;
         case '2': if (move(x,y+1)) y++; break;
         case '4': if (move(x-1,y)) x--; break;
         case '6': if (move(x+1,y)) x++; break;
         case '0': if (i=get_point())
                   {
                      itoa(i,num,10);
                      strcpy(point[i].name,name);
                      strcat(point[i].name,num);
                      gotoxy(0,24);
                      printf("%s armazenado",point[i].name);
                      printf("                          ");
                      point[i].x=x;
                      point[i].y=y;
                   } break;
         case '1': cur_x=cur_y=0; return;
      }
   }
}
//////////////////////////////////////////////////////////////////////
void edit(void)
{
   char s[100];
   printf("\n");
   *prog='\0';
   do
   {
      printf(": ");
      gets(s);
      if (*s)
      {
         strcat(s,"\n");
         strcat(prog,s);
      }
   } while(*s);
}
//////////////////////////////////////////////////////////////////////
void setup(void)
{
   pos=0;
   p_pos=prog;
   cur_x=cur_y=0;
}
//////////////////////////////////////////////////////////////////////
void main(void)
{
   char ch;
   register int t;
   for (t=0;t<MAX;t++) *(point[t].name)='\0';
   for (;;)
   {
      gotoxy(0,24);
      printf("Escolha a op‡„o: (E)ditar, (R)odar, e(N)sinar, (S)air: ");
      setup();
      switch(tolower(getch()))
      {
         case 'e':edit();break;
         case 'r':position();run();break;
         case 'n':teach();clrscr();break;
         case 's':exit(0);
      }
   }
}
