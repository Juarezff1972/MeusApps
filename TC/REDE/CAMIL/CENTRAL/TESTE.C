#include <nxt.h>
#include "netgloba.h"
#include "central.h"
#include <mem.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "databank.h"

struct request_queue *r_queue,req_queue;

void main(void)
{
  FILE *datb;
  int i,j;
  char *num1,*num2,ref[10],quant[6];
  char prod_buffer[310];

  r_queue=&req_queue;
  if((datb=fopen("databank.dat","r+b"))==NULL)
    if((datb=fopen("databank.dat","w+b"))==NULL)
      return;

  r_queue->operacao=AT;
  strcpy((char *)r_queue->item_prod,"Roupas ");
  strcpy((char *)r_queue->nome_prod,"Calcas azuis. ");
  r_queue->qtde_prod=1;
  strcpy((char *) r_queue->info_prod,"Lindas calcas jeans. ");

  num2=itoa(r_queue->qtde_prod,num2,10);
  strcpy(quant,num2);
  if (r_queue->operacao==AT) {
    setmem(prod_buffer,310,0);
    for (i=0,j=10;i<20;i++,j++) {
      prod_buffer[j]=r_queue->item_prod[i];
      if(r_queue->item_prod[i]==0)
	break;
    }
    for (i=0,j=30;i<20;i++,j++) {
      prod_buffer[j]=r_queue->nome_prod[i];
      if(r_queue->nome_prod[i]==0)
	break;
    }
    for (i=0,j=50;i<5;i++,j++) {
      prod_buffer[j]=quant[i];
      if(quant[i]==0)
	break;
    }
    for (i=0,j=55;i<255;i++,j++) {
      prod_buffer[j]=r_queue->info_prod[i];
      if(r_queue->info_prod[i]==0)
	break;
    }
    for (i=0;i<150;i++) {
      r_queue->ref_prod=1000+i;
      num1=ultoa(r_queue->ref_prod,num1,10);
      strcpy(ref,num1);
      for (j=0;j<10;j++) {
	prod_buffer[j]=ref[j];
	if(ref[j]==0)
	  break;
      }
      DataBank(datb,prod_buffer,310,Atu);
      if((strcmp(prod_buffer,"000000000"))==0) {
	clrscr();
	cprintf("Nao achou");
      }
      if((strcmp(prod_buffer,"000000000"))!=0) {
	clrscr();
	cprintf("achou");
      }
    }
    fclose(datb);
  }
}