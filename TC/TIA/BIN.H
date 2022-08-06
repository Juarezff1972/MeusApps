void troca(int *a,int *b)
{
	int c;
	c=*a;
	*a=*b;
	*b=c;
}
//////////////////////////////////////////////////////////////
void trocalong(long *a,long *b)
{
	long c;
	c=*a;
	*a=*b;
	*b=c;
}
//////////////////////////////////////////////////////////////
void trocafloat(float *a,float *b)
{
	float c;
	c=*a;
	*a=*b;
	*b=c;
}
//////////////////////////////////////////////////////////////
void pagvesa(unsigned pagina)
{
   //pagina=pagina*64;
   asm{
      mov ax,0x4f05
      xor bx,bx
      mov dx,pagina
      int 0x10
   }
}
//////////////////////////////////////////////////////////////
void pto(int x,int y,unsigned char cor)
{
   unsigned tmp=XRes+1;
   unsigned long lres;
   unsigned sres;
   if ((vidmode==0x04)||(vidmode==0x05)||(vidmode==0x06)||(vidmode==0x0d)||(vidmode==0x0e)||(vidmode==0x0f)||(vidmode==0x11)||(vidmode==0x12)||(vidmode==0x102))
   {
      asm{
         mov ah,0x0c
         mov bh,0
         mov al,cor
         mov cx,x
         mov dx,y
         int 0x10
      }
   }
   if (vidmode==0x13)
   {
      asm{
         mov ax,y
         mul tmp
         mov bx,ax
         add bx,x
         mov ax,0xa000
         mov es,ax
         mov dh,cor
         mov byte ptr es:[bx],dh
      }
   }
   if ((vidmode==0x100)||(vidmode==0x101)||(vidmode==0x103)||(vidmode==0x105)||(vidmode==0x107)||(vidmode==0x120))
   {
      lres=(unsigned long)y*tmp+x;
      tmp=(unsigned)(lres/65536);
      if (vesapg!=tmp) {pagvesa(tmp);vesapg=tmp;}
      lres=lres-(65536*tmp);
      sres=(unsigned)lres;
      asm{
         mov bx,sres
         mov ax,0xa000
         mov es,ax
         mov dh,cor
         mov byte ptr es:[bx],dh
      }
   }
   if ((vidmode==0x0)||(vidmode==0x1)||(vidmode==0x2)||(vidmode==0x3)||(vidmode==0x7))
   {
      textcolor(cor);
      gotoxy(x+1,y+1);
      cprintf("Û");
   }
}
//////////////////////////////////////////////////////////////
void imp_ponto(float x,float y,unsigned char cor)
{
   int curx,cury;
   float auxx,auxy;
   auxx=(x-ESQ)*(DIR-ESQ)/(float)XRes;
   auxy=YRes-((y-BASE)*(TOPO-BASE)/(float)YRes);
   curx=(int)(auxx*((float)(XMAX-XMIN)/(float)XRes))+XMIN;
   cury=(int)(auxy*((float)(YMAX-YMIN)/(float)YRes))+YMIN;
   if (((curx>=XMIN)&&(curx<=XMAX))&&((cury>=YMIN)&&(cury<=YMAX)))
      pto(curx,cury,cor);
}
//////////////////////////////////////////////////////////////
unsigned char PegaPto(unsigned x,unsigned y)
{
   unsigned char aux=0;
   if ((x<=XRes) && (y<=YRes))
   {
      asm{
         mov ah,0x0d
         mov bh,0
         mov cx,x
         mov dx,y
         int 0x10
         mov aux,al
      }
   }
   return aux;
}
//////////////////////////////////////////////////////////////
void limpatela(void)
{
   unsigned x,y;
   for(x=XMIN;x<=XMAX;x++) for(y=YMIN;y<=YMAX;y++) pto(x,y,0);
}
//////////////////////////////////////////////////////////////
void mudar(unsigned modo)
{
   asm{
      mov ax,0x4f02
      mov bx,modo
      int 0x10
   }
}
//////////////////////////////////////////////////////////////
void mudamodo(unsigned modo)
{
   unsigned char z=0;
   if (modo==0x0  ) {XRes=40; YRes=25; MaxCor=15; mudar(modo);z=1;} //ok texto
   if (modo==0x1  ) {XRes=40; YRes=25; MaxCor=15; mudar(modo);z=1;} //ok texto
   if (modo==0x2  ) {XRes=80; YRes=25; MaxCor=15; mudar(modo);z=1;} //ok texto
   if (modo==0x3  ) {XRes=80; YRes=25; MaxCor=15; mudar(modo);z=1;} //ok texto
   if (modo==0x4  ) {XRes=320;YRes=200;MaxCor=4;  mudar(modo);z=1;} //ok bios
   if (modo==0x5  ) {XRes=320;YRes=200;MaxCor=4;  mudar(modo);z=1;} //ok bios
   if (modo==0x6  ) {XRes=640;YRes=200;MaxCor=2;  mudar(modo);z=1;} //ok bios
   if (modo==0x7  ) {XRes=80; YRes=25; MaxCor=2;  mudar(modo);z=1;} //ok texto
   if (modo==0xd  ) {XRes=320;YRes=200;MaxCor=16; mudar(modo);z=1;} //ok bios
   if (modo==0xe  ) {XRes=640;YRes=200;MaxCor=16; mudar(modo);z=1;} //ok bios
   if (modo==0xf  ) {XRes=640;YRes=350;MaxCor=2;  mudar(modo);z=1;} //ok bios
   if (modo==0x10 ) {XRes=640;YRes=350;MaxCor=16; mudar(modo);z=1;} //ok bios
   if (modo==0x11 ) {XRes=640;YRes=480;MaxCor=2;  mudar(modo);z=1;} //ok bios
   if (modo==0x12 ) {XRes=640;YRes=480;MaxCor=15; mudar(modo);z=1;} //ok bios
   if (modo==0x13 ) {XRes=320;YRes=200;MaxCor=255;mudar(modo);z=1;} //ok direto
   if (temvesa==1)
   {
      if (modo==0x100) {XRes=640; YRes=400; MaxCor=255;mudar(modo);z=1;} //ok vesa
      if (modo==0x101) {XRes=640; YRes=480; MaxCor=255;mudar(modo);z=1;} //ok vesa
      if (modo==0x102) {XRes=800; YRes=600; MaxCor=15; mudar(modo);z=1;} //ok bios vesa
      if (modo==0x103) {XRes=800; YRes=600; MaxCor=255;mudar(modo);z=1;} //ok vesa
      if (modo==0x105) {XRes=1024;YRes=768; MaxCor=255;mudar(modo);z=1;} //ok vesa
      if (modo==0x107) {XRes=1280;YRes=1024;MaxCor=255;mudar(modo);z=1;} //ok vesa
      if (modo==0x120) {XRes=1600;YRes=1200;MaxCor=255;mudar(modo);z=1;} //ok vesa
   }
   if (z==0)
   {
      printf("Modo de video nao suportado!!\n");
      exit(EXIT_FAILURE);
   }
   asp=(1600.0/1200.0)*((float)YRes/(float)XRes);
   XRes--;
   YRes--;
   XMIN=0;YMIN=0;XMAX=XRes;YMAX=YRes;TVX=XRes;TVY=YRes;
   vidmode=modo;
}
//////////////////////////////////////////////////////////////
void vesa(void)
{
   unsigned char buf[256];
   unsigned char sts,sup;
   unsigned ofst,segm;
   ofst=FP_OFF(buf);
   segm=FP_SEG(buf);
   asm{
      mov ax,0x4f00
      mov es,segm
      mov di,ofst
      int 0x10
      mov sup,al
      mov sts,ah
   }
   if (sup!=0x4f)
   {
      printf("VESA nao encontrado");
      temvesa=0;
   }
   else
   {
      temvesa=1;
      printf("%c%c%c%c %u.%u \n",buf[0],buf[1],buf[2],buf[3],buf[5],buf[4]);
   }
}
//////////////////////////////////////////////////////////////
void texto(int x,int y,char *str,unsigned char cor)
{
   unsigned char byte;
   int           i,j,k,tam,ch;
   unsigned char *font;
   font = font8x16;
   tam = strlen(str);
   for (k = 0; k < tam; k++)
   {
      ch = str[k];
      for (j = 0; j < 16; j++)
      {
         byte = *(font + ch * 16 + j);
         for (i = 0; i < 8; i++)
         {
            if ((byte & 0x80) != 0) pto(x+i,y+j,cor);
            byte <<= 1;
         }
      }
      x += 8;
   }
}
//////////////////////////////////////////////////////////////
void iniciamouse(void)
{
   asm{
      mov ax,0
      int 0x33
      mov temmouse,ax
      mov mousebtn,bx
      mov ax,0x21
      int 0x33
      mov ax,0x35
      int 0x33
   }
}
//////////////////////////////////////////////////////////////
void showmouse(void)
{
   asm{
      mov ax,1
      int 0x33
   }
}
//////////////////////////////////////////////////////////////
void hidemouse(void)
{
   asm{
      mov ax,2
      int 0x33
   }
}
//////////////////////////////////////////////////////////////
void getmouse(void)
{
   unsigned x,y;
   asm{
      mov ax,3
      int 0x33
      mov mousests,bx
      mov x,cx
      mov y,dx
   }
   rato.x=x;
   rato.y=y;
}
//////////////////////////////////////////////////////////////
void setmouse(unsigned x,unsigned y)
{
   asm{
      mov ax,4
      mov cx,x
      mov dx,y
      int 0x33
   }
}