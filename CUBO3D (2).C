/*************************************************************************/
/* Referencia aos "header files"                                         */

#include <dos.h>
#include <stdio.h>
#include <graphics.h>
#include <math.h>
#include <stdlib.h>
/*************************************************************************/
float x=0.0, y=0.0, z=0.0;                        /* Coordenadas Globais */
float sx=0.0, sy=0.0;        /* Saida para as formulas de perspectiva 3D */
float xa=0.0, ya=0.0, za=0.0;     /* Valores temporarios nas formulas 3D */
float sxa=0.0, sya=0.0, sxb=0.0, syb=0.0;  /* Extremidades das linhas 2D */
float sxs=0.0, sys=0.0;       /* Armazenamento temporario para linhas 2D */
float temp_swap=0.0;                    /* Usada para troca de variaveis */
float d=1200.0;                          /* Fator de perspectiva angular */
double r1=0;                                     /* Angulo X em radianos */
double r2=0;                                     /* Angulo Y em radianos */
double r3=0;                                     /* Angulo Z em radianos */
double sr1=0.0, sr2=0.0, sr3=0.0;             /* Fatores de rotacao Seno */
double cr1=0.0, cr2=0.0, cr3=0.0;          /* Fatores de rotacao Cosseno */
float mx=0.0, my=0.0, mz=-350.0;              /* Posicao de visualizacao */
int maxx=638, minx=1, maxy=198, miny=1;  /* Limitacao da area de desenho */
float screen_x=639, screen_y=199;                   /* Dimensoes da tela */
float c=0.0;                             /* Usada na rotina de limitacao */
float rx=0.0, ry=0.0; /* Valores de escala usados na rotina de mapeamento */
int t1=0, t2=0;                                    /* Contadores de loop */
int p1=0;                                            /* Indice de matriz */
/*************************************************************************/
/*  Banco de dados das coordenadas  cartesianas globais xyz para o cubo  */

int array1[][3]=
{
    30, -30, 30,    30, -30, 30,    30, 30, 30,     30, -30, 30,    30, -30, -30,
    30, 30, -30,    30, -30, -30,  -30, -30, -30,   -30, 30, -30,  -30, -30, -30,
    -30, -30, 30,  -30, 30, 30,    -30, -30, 30,    30, -30, 30,    30, 30, 30,
    30, 30, -30,   -30, 30, -30,   -30, 30, 30,     30, 30, 30,     30, 30, -30,
    -30, 30, -30,   30, -30, -30,   30, 30, -30,   -30, -30, -30,  -30, -30, 30,
    30, -30, -30,   30, 30, 30,     30, 30, -30,   -30, 30, -30,   -30, 30, 30
};
/*************************************************************************/
/*            Declaracao das variaveis auxiliares                        */

int c0=0, c1=1, c2=2, c3=3, c4=4, c5=5, c6=6, c7=7, c8=8, c9=9, c10=10,
    c11=11, c12=12, c13=13, c14=14, c15=15;
float sx1, sy1, sx2, sy2;
float x_res, y_res;
int cor;
char ch;
float rotacaoX=0.0, rotacaoY=0.0, vira=0.0, moveX=0.0, moveY=0.0, moveZ=0.0;
const pi=3.1415926535897932384654;
/*************************************************************************/
/* Declaracao das subrotinas globais                                     */

void quit_pgm(void);
void calc_3d(void);
void rotation(void);
void windows(void);
void viewport(void);
void graphics_setup(void);
void coords(void);
char get_key(void);
/*************************************************************************/
/* Le tecla pressionada                                                  */

char get_key(void)
{
    union REGS regs;
    regs.h.ah=0;
    int86(0x16, &regs, &regs);
    return regs.h.al;
}
/*************************************************************************/
/* Bloco principal                                                       */

void main(void)
{
    graphics_setup();
    sx=0;
    sy=24;
    coords();
    sx1=sx;
    sy1=sy;
    sx=638;
    sy=455;
    coords();
    sx2=sx;
    sy2=sy;
    setcolor(c15);
    rectangle(sx1, sy1, sx2, sy2);
    setviewport(sx1+1, sy1+1, sx2-1, sy2-1, 1);
    cor=1;
    do
    {
        cor=1;
        if((ch=='j')||(ch=='k')||(ch=='m')||(ch=='i')||(ch=='[')||(ch==']')||(ch=='e')||(ch=='z')|| (ch=='w')||(ch=='x')||(ch=='a')||(ch=='d'))
            cor=0;
        do
        {
            p1=0;
            rotation();
            for(t2=1; t2<=6; t2++)
            {
                x=array1[p1][0];
                y=array1[p1][1];
                z=array1[p1][2];
                calc_3d();
                windows();
                sxa=sx;
                sya=sy;
                for(t1=1; t1<=4; t1++)
                {
                    p1++;
                    setcolor(c14*cor);
                    if(p1==1)
                        setcolor(c0);
                    x=array1[p1][0];
                    y=array1[p1][1];
                    z=array1[p1][2];
                    calc_3d();
                    windows();
                    sxs=sx;
                    sys=sy;
                    sxb=sx;
                    syb=sy;
                    viewport();
                    moveto(sxa, sya);
                    lineto(sxb, syb);
                    sxa=sxs;
                    sya=sys;
                };
                p1++;
            };
            rotacaoX=0;
            rotacaoY=0;
            vira=0;
            moveX=0;
            moveY=0;
            moveZ=0;
            cor++;
            if(ch=='j')
                rotacaoX=-0.05;
            if(ch=='k')
                rotacaoX=+0.05;
            if(ch=='m')
                rotacaoY=-0.05;
            if(ch=='i')
                rotacaoY=+0.05;
            if(ch==']')
                vira=-0.05;
            if(ch=='[')
                vira=+0.05;
            if(ch=='a')
                moveX=+1;
            if(ch=='d')
                moveX=-1;
            if(ch=='w')
                moveY=+1;
            if(ch=='x')
                moveY=-1;
            if(ch=='e')
                moveZ=+1;
            if(ch=='z')
                moveZ=-1;
            ch=0;
            if(r1>(2*pi))
                r1=r1-(2*pi);
            if(r2>(2*pi))
                r2=r2-(2*pi);
            if(r3>(2*pi))
                r3=r3-(2*pi);
            r1=r1+rotacaoX;
            r2=r2+vira;
            r3=r3+rotacaoY;
            mx=mx+moveX;
            my=my+moveY;
            mz=mz+moveZ;
        }
        while(cor!=2);
        if(cor>1)
            cor=1;
        ch=get_key();
    }
    while(ch!=27);
    quit_pgm();
}
/*************************************************************************/
/* Calcula rotacao do cubo                                               */

void rotation(void)
{
    sr1=sin(r1);
    sr2=sin(r2);
    sr3=sin(r3);
    cr1=cos(r1);
    cr2=cos(r2);
    cr3=cos(r3);
}
/*************************************************************************/
/* Transforma eixos xyz cartesianos para xy da tela                      */

void calc_3d(void)
{
    x=-x;
    xa=cr1*x-sr1*z;
    za=sr1*x+cr1*z;
    x=cr2*xa+sr2*y;
    ya=cr2*y-sr2*xa;
    z=cr3*za-sr3*ya;
    y=sr3*za+cr3*ya;
    x=x+mx;
    y=y+my;
    z=z+mz;
    sx=d*x/z;
    sy=d*y/z;
    return;
}
/*************************************************************************/
/* Determina resolucao do cubo para a tela                               */

void windows(void)
{
    sx=sx+399;
    sy=sy+299;
    rx=screen_x/799;
    ry=screen_y/599;
    sx=sx*rx;
    sy=sy*ry;
    return;
}
/*************************************************************************/
/* Determina limites para o desenho 2D                                   */

void viewport(void)
{
    if(sxa>sxb)
    {
        temp_swap=sxa;
        sxa=sxb;
        sxb=temp_swap;
        temp_swap=sya;
        sya=syb;
        syb=temp_swap;
    }
    if(sxa<minx)
        if(sxb<minx)
            return;
    if(sxa>maxx)
        if(sxb>maxx)
            return;
    if(sya<miny)
        if(syb<miny)
            return;
    if(sya>maxy)
        if(syb>maxx)
            return;
    if((sxa<minx)&&(sxb!=sxa))
    {
        {
            c=(syb-sya)/(sxb-sxa)*(sxb-minx);
            sxa=minx;
            sya=syb-c;
        };
        if(sya<miny)
            if(syb<miny)
                return;
        if(sya>maxy)
            if(syb>maxy)
                return;
    };
    if((sxb>maxx)&&(sxb!=sxa))
    {
        {
            c=(syb-sya)/(sxb-sxa)*(maxx-sxa);
            sxb=maxx;
            syb=sya-c;
        };
        if(sya<miny)
            if(syb<miny)
                return;
        if(sya>maxy)
            if(syb>maxy)
                return;
    };
    if(sya>syb)
    {
        temp_swap=sya;
        sya=syb;
        syb=temp_swap;
        temp_swap=sxa;
        sxa=sxb;
        sxb=temp_swap;
    };
    if((sya<miny)&&(syb!=sya))
    {
        c=(sxb-sxa)/(syb-sya)*(syb-miny);
        sxa=sxb-c;
        sya=miny;
    };
    if((syb>maxy)&&(syb!=sya))
    {
        c=(sxb-sxa)/(syb-sya)*(maxy-sya);
        sxb=sxa-c;
        syb=maxy;
    };
    return;
}
/*************************************************************************/
/* Termino do programa                                                   */

void quit_pgm(void)
{
    cleardevice();
    restorecrtmode();
    exit(0);
}
/*************************************************************************/
/* Ajusta modo grafico                                                   */

void graphics_setup(void)
{
    int ga, gm;
    detectgraph(&ga, &gm);
    ga=VGA;
    gm=VGAHI;
    initgraph(&ga, &gm, "");
    x_res=640;
    y_res=480;
    maxx=838;
    minx=1;
    maxy=678;
    miny=1;
    screen_x=639;
    screen_y=479;
    setpalette(0, 8);
    setcolor(c12);
    outtextxy(0, 472, "CUBO 3D - (C)1993 By ABC Software Ltd.");
    setcolor(c10);
    outtextxy(230, 0, "Trabalho de Algoritmos");
    return;
};
/*************************************************************************/
/* Transforma coordenadas 2D virtuais para fisicas                       */

void coords(void)
{
    sx=sx*(x_res/640);
    sy=sy*(y_res/480);
    return;
}
/*************************************************************************/
/* Teclas para a movimentacao do cubo:                                   */
/*                                                                       */
/*     i  - Rotacao em torno do eixo X sentido positivo                  */
/*     m  - Rotacao em torno do eixo X sentido negativo                  */
/*     j  - Rotacao em torno do eixo Z sentido positivo                  */
/*     k  - Rotacao em torno do eixo Z sentido negativo                  */
/*     [  - Rotacao em torno do eixo Y sentido positivo                  */
/*     ]  - Rotacao em torno do eixo Y sentido negativo                  */
/*     w  - Translacao pelo eixo Y sentido positivo                      */
/*     x  - Translacao pelo eixo Y sentido negativo                      */
/*     a  - Translacao pelo eixo X sentido positivo                      */
/*     d  - Translacao pelo eixo X sentido negativo                      */
/*     e  - Translacao pelo eixo Z sentido positivo                      */
/*     z  - Translacao pelo eixo Z sentido negativo                      */
/*************************************************************************/
