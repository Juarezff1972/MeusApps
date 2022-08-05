#include <stdio.h>
#include <dos.h>
#include <io.h>
#include <string.h>
#include <graphics.h>
#include <conio.h>
#include <process.h>

struct RGB
{
   unsigned char r;
   unsigned char g;
   unsigned char b;
};

typedef RGB palarray[256];
typedef RGB cmap[16];

struct headerpcx
{
   unsigned char Manufacturer;   // Flag Constante 10 = ZSoft .PCX
   unsigned char Version;        // Informa‡„o de vers„o:
                                 // 0 = Vers„o 2.5
                                 // 2 = Vers„o 2.8 c/ informa‡„o de palheta
                                 // 3 = Vers„o 2.8 s/ informa‡„o de palheta
                                 // 5 = Vers„o 3.0
   unsigned char Encoding;       // 1 = .PCX "run length encoding"
   unsigned char BitsPerPixel;   // Numero de bits/pixel por plano
   int Wxmin,Wymin,Wxmax,Wymax;  // Dimens„o da Figura
                                 // (Xmin, Ymin) - (Xmax - Ymax)
                                 // em pixels, inclusive
   int HRes;                     // Resolu‡„o horizontal do dispositivo
   int VRes;                     // Resolu‡„o vertical do dispositivo
   cmap Colormap;                // Setando da cor da palheta
   unsigned char Reserved;
   unsigned char NPlanes;        // Numero de planos de cor
   int BytesPerLine;             // Numero de bytes por linha de varredura
                                 // por plano de cor (sempre par)
   int PaletteInfo;              // Tipo de palheta: 1 - cor/BW
                                 //                  2 - tons de cinza
   unsigned char Filler[58];     // branco para preencher os 128 bytes
};

FILE *PCXFileIn, *PCXFileOut;
int  ModoLeituraPCX;
//////////////////////////////////////////////////////////////////////////
void SelecModoLeituraPCX (int modo)
{
    ModoLeituraPCX=modo;
}
//////////////////////////////////////////////////////////////////////////
void gravaPCX (char *nomearq, int x0,int y0,int x1,int y1)
{
   palarray vgapal;
   struct headerpcx header;
/////////////////////////////////////////////////////////////
   void gravaHeaderPCX(int x0,int y0,int x1,int y1)
   {
      const
        cmap colorMapDefault={{0  ,0 , 0},{0  , 0,  0},{0  ,0  , 0},{0  ,170,170},
                              {170,0 , 0},{170, 0,170},{170,85 , 0},{170,170,170},
                              { 85,85,85},{ 85,85,255},{ 85,255,85},{ 85,255,255},
                              {255,85,85},{255,85,255},{255,255,85},{255,255,255}};
      int i;
      header.Manufacturer = 10;
      header.Version = 5;
      header.Encoding = 1;
      header.BitsPerPixel = 8;
      header.Wxmin = x0;
      header.Wymin = y0;
      header.Wxmax = x1;
      header.Wymax = y1;
      header.HRes = getmaxx()+1;
      header.VRes = getmaxy()+1;
      header.Colormap = ColorMapDefault;
      header.Reserved = 0;
      header.NPlanes = 1;
      header.BytesPerLine = x1-x0+1;
      header.PaletteInfo = 0;
      for(i=0;i<=57;i++) header.filler[i] = 0;
      write(PCXFileOut, header, sizeof (header));
   } // GravaHeaderPCX
////////////////////////////////////////////
   void gravaCorpoPCX (int x0, int y0, int x1, int y1)
////////////////////////////////
      void descarrega (unsigned char b, int n)
      {
         unsigned char codrep;
         int rep;
         while (n > 0)
         {
            if ((n > 1) || ((b && 0xc0) == 0xc0))
            {
               rep = n;
               if (rep > 63) rep = 63;
               codrep = 0xc0 or rep;
               putc(codrep,PCXFileOut);
            }
            putc(b,PCXFileOut);
            n = n - 63;
         }
      } // descarrega
////////////////////////////////
   {
      int nrep;
      unsigned char byteant, lido;
      int x,y;
      for(y=y0;y<=y1;y++)
      {
         byteant=getPixel (x0, y);
         nrep=1;
         for(x=x0+1;x<=x1;x++)
         {
            lido=GetPixel (x, y);
            if (lido == byteant) nrep = nrep + 1
            else
            {
               descarrega(byteant, nrep);
               byteant=lido;
               nrep=1;
            }
         }
         descarrega(byteant, nrep);
      }
   } // GravaCorpoPCX
////////////////////////////////////////////
   void gravaPalettePCX(void)
   {
      int i;
      unsigned char b;
      struct REGPACK reg;
      b=12;
      putc(b,PCXFileOut);
      reg.r_cx = 256;
      reg.r_dx = FP_OFF (vgapal);
      reg.r_es = FP_SEG (vgapal);
      reg.r_ax = 0x1017;
      reg.r_bx = 0;
      intr(0x10, &reg);
      for(i=0;i<=255;i++)
      {
         vgapal[i].r = vgapal[i].r << 2;
         vgapal[i].g = vgapal[i].g << 2;
         vgapal[i].b = vgapal[i].b << 2;
      }
      write (PCXFileOut, vgapal, sizeof(palarray));
   } // gravaPalettePCX
//////////////////////////////////////////////////////
    PCXFileOut = fopen(strcat(nomearq,".PCX"),"wb");
    if (PCXFileOut == NULL)
    {
       closegraph();
       clrscr();
       printf("\n\n*** Nome de arquivo invalido [%s]. ***",nomearq);
       printf("\n*** Programa Cancelado. ***");
       exit(1);
    }
    gravaHeaderPCX (x0,y0,x1,y1);
    gravaCorpoPCX (x0,y0,x1,y1);
    gravaPalettePCX();
    fclose(PCXFileOut);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void LePCX (char *nomearq, int x0, int y0)
////////////////////////////////////////////////
void openFile (char *nomearq)
{
   pcxfileIn=fopen(strcat(nomearq,".PCX"),"rb");
   if (pcxfileIn == NULL)
   {
      closegraph();
      clrscr();
      printf("\n\n*** O arquivo %s.PCX nao foi encontrado. ***",nomearq);
      printf("\n*** Programa Cancelado. ***");
      exit();
   }
}
////////////////////////////////////////////////
void decodHeader(void)
{
   int i;  getc(pcxfileIn);
   header.manufacturer=getc(pcxfileIn);
   header.version=getc(pcxfileIn);
   header.encoding=getc(pcxfileIn);
   header.bitsPerPixel=getc(pcxfileIn);
   header.Wxmin=getw(pcxfileIn);
   header.Wymin=getw(pcxfileIn);
   header.Wxmax=getw(pcxfileIn);
   header.Wymax=getw(pcxfileIn);
   header.hres=getw(pcxfileIn);
   header.vres=getw(pcxfileIn);
   for(i=0;i<=15;i++)
   {
      ColorMap[i].r=getc(pcxfileIn);
      ColorMap[i].g=getc(pcxfileIn);
      ColorMap[i].b=getc(pcxfileIn);
   }
   header.reserved=getc(pcxfileIn);
   header.nplanes=getc(pcxfileIn);
   header.bytesPerLine=getc(pcxfileIn);
   header.paletteInfo=getc(pcxfileIn);
}
////////////////////////////////////////////////
void decod256palette(void)
{
   int i;
   long fsize;
   unsigned char byte;
   fsize = filelength (pcxfileIn);
   lseek(pcxfileIn,fsize-769,0);
   tag=getc(pcxfileIn);
   if (tag == 12)
   {
      for(i=0;i<=255;i++)
         with  do
         {
            vgaPal[i].r=getc(pcxfileIn);
            vgaPal[i].g=getc(pcxfileIn);
            vgaPal[i].b=getc(pcxfileIn);
            r = r >> 2;
            g = g >> 2;
            b = b >> 2;
            if (ModoLeituraPCX == UsaPalettePCX) setrgbpalette (i,r,g,b);
         }
   }
}
////////////////////////////////////////////////
void decodFile(int x0,int y0)
{
   unsigned  x, y;
   unsigned char b, count, value;
   int i, j, YSize, TotalBytes;
   headerPCX header;
   palArray vgaPal;
   lseek(pcxfileIn, 128,0);
   YSize=header.Wymax - header.Wymin + 1;
   TotalBytes = header.NPlanes * header.BytesPerLine;
   for(y=1;y<=YSize;y++)
   {
      i= 0;
      while (i < TotalBytes) do
      {
         b=getc(pcxfileIn);
         if ((b & 0xc0) == 0xc0)
         {
            count = b & 0x3f;
            value=getc(pcxfileIn);
         }
         else
         {
            count = 1;
            value = b;
         }
         for(j=0;j<=count-1;j++)
         {
            if ((x0+i <= getmaxx) && (y0+y <= getmaxy)) then
            {
               if (ModoLeituraPCX==ConvertePalette)
               {
                   VGADither(x0+i,y0+y,VGAPal[value].r<<2,VGAPal[value].g<<2,VGAPal[value].b<<2);
               }
               else PutPixel (x0+i,y0+y,value);
            }
            i= i + 1;
         }
      }
   }
}
////////////////////////////////////////////////
{
   headerPCX header;
   openFile (nomearq);
   decodHeader();
   if (header.version == 5) decod256palette();
   decodFile (x0,y0);
   fclose(pcxfileIn);
}
