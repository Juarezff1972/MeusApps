#include <stdio.h>
#include <dos.h>
////////////////////////////////////////////////////////////
void main(void)
{
   unsigned char buf[256];
   unsigned segm,ofst,tmp;
   char sup,sts;
   segm=FP_SEG(buf);
   ofst=FP_OFF(buf);
   asm{
      push ax
      push cx
      push es
      push di
      mov ax,0x4f01
      mov cx,0x100
      mov es,segm
      mov di,ofst
      int 0x10
      mov sup,al
      mov sts,ah
      pop di
      pop es
      pop cx
      pop ax
   }
   printf("------------------------------------------\n");
   if (sup==0x4f) printf("Funcao suportada\n");
   if (sts==0)    printf("Status: Sucesso\n"); else printf("Status: Falha\n");
   printf("Atributos do modo: %X\n",buf[1]*256+buf[0]);
   printf("Atributos da janela A: %X\n",buf[2]);
   printf("Atributos da janela B: %X\n",buf[3]);
   printf("Granularidade: %u Kb\n",buf[5]*256+buf[4]);
   printf("Tamanho da janela: %u Kb\n",buf[7]*256+buf[6]);
   printf("Segmento da janela A: %X\n",buf[9]*256+buf[8]);
   printf("Segmento da janela B: %X\n",buf[11]*256+buf[10]);
   printf("Bytes por linha de varredura: %u\n",buf[17]*256+buf[16]);
   printf("Largura em pontos: %u\n",buf[19]*256+buf[18]);
   printf("Altura em pontos: %u\n",buf[21]*256+buf[20]);
   printf("Largura dos caracteres em pontos: %u\n",buf[22]);
   printf("Altura dos caracteres em pontos: %u\n",buf[23]);
   printf("Numero de planos: %u\n",buf[24]);
   printf("Numero de bits por ponto: %u\n",buf[25]);
   printf("Numero de bancos: %u\n",buf[26]);
   printf("Tipo do modelo de memoria: %u\n",buf[27]);
}