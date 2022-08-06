#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <malloc.h>
#include <string.h>

#include "vars.h"
#include "bin.h"
#include "linha.h"
#include "coords.h"
#include "curva.h"
#include "figura.h"

#define MAXPTO_X 50
#define MAXPTO_Y 50
#define MAXLIN 50
#define MAXCOL 50
#define PONTO 15
/////////////////////////////////////////////////////////////
   // Equacao da reta: y=ax+b

struct { unsigned x;unsigned y; } ponto[MAXPTO_X][MAXPTO_Y];
struct { unsigned x1;unsigned y1;unsigned x2;unsigned y2; } lin[MAXLIN];
struct { unsigned x1;unsigned y1;unsigned x2;unsigned y2; } col[MAXCOL];

/////////////////////////////////////////////////////////////
void main(void)
{
   unsigned x1,y1,x2,y2,x,y;
   unsigned i,ii;
   unsigned modo=0x102;

   unsigned ix=0,iy=0; // contadores
   unsigned iix=0,iiy=0;
   unsigned ilin=0,icol=0;
   unsigned aux_x=0,aux_y=0; // auxiliares

   vesa();
   mudamodo(modo);
   randomize;
   setcoord(XMIN,YMIN,XMAX,YMAX);
   for(x1=0;x1<=XMAX;x1=x1+50)
   {
      for(y1=0;y1<=YMAX;y1=y1+50)
      {
         imp_ponto(x1,y1,WHITE);
      }
   }

   for(y=0;y<=MAXPTO_Y;y++)
   {
      for(x=0;x<=MAXPTO_X;x++)
      {
         ponto[x][y].x=0;  ponto[x][y].y=0;
      }
   }
   for(aux_y=0;aux_y<=MAXLIN;aux_y++)
   {
      lin[aux_y].x1=0;
      lin[aux_y].y1=0;
      lin[aux_y].x2=0;
      lin[aux_y].y2=0;
   }
   for(aux_y=0;aux_y<=MAXCOL;aux_y++)
   {
      col[aux_y].x1=0;
      col[aux_y].y1=0;
      col[aux_y].x2=0;
      col[aux_y].y2=0;
   }
////////////////////////////////
/// Inicio do Algoritmo

   for(y=0;y<=YMAX;y++)
   {
      for(x=0;x<=XMAX;x++)
      {
         if (PegaPto(x,y)==PONTO) // encontrou um pixel na imagem
         {
            if (x<aux_x) {ix=0;iy++;}
            ponto[ix][iy].x=x;  ponto[ix][iy].y=y;
            imp_ponto(ponto[ix][iy].x,ponto[ix][iy].y,LIGHTMAGENTA);
            ix++;
            aux_x=x;
         }
      }
   }

   for(iiy=0;iiy<=iy;iiy++)
   {
      for(iix=0;iix<=ix-1;iix++)
      {
         imp_ponto(ponto[iix][iiy].x,ponto[iix][iiy].y,LIGHTBLUE);
      }
   }

   for(iiy=0;iiy<=iy;iiy++)
   {
      for(iix=0;iix<=ix-1;iix++)
      {
         if(ponto[iix][iiy].y==ponto[iix+1][iiy].y)
         {
             lin[ilin].x1=ponto[iix][iiy].x;
             lin[ilin].y1=ponto[iix][iiy].y;
             lin[ilin].x2=ponto[iix+1][iiy].x;
             lin[ilin].y2=ponto[iix+1][iiy].y;
             linha(lin[ilin].x1,lin[ilin].y1,lin[ilin].x2,lin[ilin].y2,YELLOW);
             delay(50);
             ilin++;
         }
      }
   }

   for(iix=0;iix<=ix-1;iix++)
   {
      for(iiy=0;iiy<=iy;iiy++)
      {
         if(ponto[iix][iiy].x==ponto[iix][iiy+1].x)
         {
             col[icol].x1=ponto[iix][iiy].x;
             col[icol].y1=ponto[iix][iiy].y;
             col[icol].x2=ponto[iix][iiy+1].x;
             col[icol].y2=ponto[iix][iiy+1].y;
             linha(col[icol].x1,col[icol].y1,col[icol].x2,col[icol].y2,MAGENTA);
             delay(50);
             icol++;
         }
      }
   }

   getch();
   mudar(0x3);
   printf("Termino Normal");
}
