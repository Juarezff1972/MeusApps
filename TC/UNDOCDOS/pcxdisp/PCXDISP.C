/*
   I quickly put this together so it might not run on all systems.
   If you fix it to run on your system, please let me know and I
   will fix this original version to be more portable. (thanks)
   Also, this technique is extremely slow.  It is used to show
   show a PCX file is used.  Not for speed.
*/

#include <conio.h>
#include <graph.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

typedef struct {
  unsigned char  manufacturer;
  unsigned char  version;
  unsigned char  encoding;
  unsigned char  bitsppxl;
  unsigned short xmin;
  unsigned short ymin;
  unsigned short xmax;
  unsigned short ymax;
  unsigned short hres;
  unsigned short vres;
  unsigned char  egapalette[48];
  unsigned char  reserved;
  unsigned char  clrplns;
  unsigned short bytespline;
  unsigned short palettetype;
  unsigned char  filler[58];
} PCXheader;

unsigned short cpalette[48];
unsigned long palarray[16];
unsigned char buffer[11041];
unsigned char byte, byte1, byte2;
unsigned short i, j, k, b, m;
unsigned short addr, lineend;
unsigned short red, green, blue, hue;

PCXheader thehdr;
FILE *fp;
char *filepos;
char *mempos;

void main(void)
{

  if ((fp = fopen("demo.pcx","rb")) == NULL) exit(-1);
  fread(&thehdr,sizeof(PCXheader),1,fp);
  if (thehdr.manufacturer != 0x0A)  {
    printf("\nThis is not a PCX file!");
    exit(-1);
  }
  fread(&buffer[1],sizeof(buffer),1,fp);
  filepos = &buffer[1];
  fclose(fp);

  for (i = 0;i < 47;i++)
    cpalette[i] = (short) thehdr.egapalette[i+1];

  switch(thehdr.vres) {
    case 350: {
      _setvideomode(_ERESCOLOR);    /* screen 9 */
      for (j = 0;j < 15;j++) {
        red = (short) cpalette[j*3] / 85;
        green = (short) cpalette[(j*3)+1] / 85;
        blue = (short) cpalette[(j*3)+2] / 85;
        red = ((red && 1) * 32) || ((red && 2) * 2);
        green = ((green && 1) * 16) || (green && 2);
        blue = ((blue && 1) * 8) || ((blue && 2) / 2);
        hue = red || green || blue;
        palarray[j] = (long) hue;
      }
    }
    case 480: {
      _setvideomode(_VRES16COLOR);    /* screen 12 */
      for (j = 0;j < 15;j++) {
        red = cpalette[j*3] / 4;
        green = cpalette[(j*3)+1] / 4;
        blue = cpalette[(j*3)+2] / 4;
        palarray[j] = (long) ((65536 * blue) + (256 * green) + red);
      }
    }
    default: {
      printf("\nI don't know how to display this file.");
      exit(-1);
    }
  }

/*   _remapallpalette(&palarray); */

  for (k = thehdr.ymin; k < thehdr.ymax; j++) {
    addr = 80 * k;
    lineend = addr + thehdr.bytespline;
    j = 1;
    do {
      b = j;
      if (j = 3) b = 4;
      if (j = 4) b = 8;
      outp(0x3C4, 2);
      outp(0x3C5, b);
      byte = *filepos++;
      byte1 = byte;
      if ((byte1 && 192) != 192) {
	mempos = 0xA000;
	mempos += addr;
	*mempos = byte1;
        addr++;
        if (addr >= lineend) {
          addr = 80 * k;
          j++;
	}
      } else {
	byte1 &= 63;
	byte = *filepos++;
        byte2 = byte;
        for (m = 1; m < byte1; m++) {
          b = j;
          if (j=3) b = 4;
          if (j=4) b = 8;
          outp(0x3C4, 2);
          outp(0x3C5, b);
	  mempos = 0xA000;
	  mempos += addr;
	  *mempos = byte1;
	  addr++;
          if (addr >= lineend) {
            addr = 80 * k;
            j++;
          }
        }
      }
    } while (j <= 4);
  }
  outp(0x3C4, 2);
  outp(0x3C5, 0x0F);
  getch();
  _setvideomode(_TEXTBW80);
  exit(0);
}
