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
void pto(HDC DC,int x,int y,COLORREF cor)
{
   SetPixel(DC,x,y,cor);
}
//////////////////////////////////////////////////////////////
void imp_ponto(HDC DC,float x,float y,COLORREF cor)
{
   int curx,cury;
   float auxx,auxy;
   auxx=(x-ESQ)*(DIR-ESQ)/(float)XRes;
   auxy=YRes-((y-BASE)*(TOPO-BASE)/(float)YRes);
   curx=(int)(auxx*((float)(XMAX-XMIN)/(float)XRes))+XMIN;
   cury=(int)(auxy*((float)(YMAX-YMIN)/(float)YRes))+YMIN;
   if (((curx>=XMIN)&&(curx<=XMAX))&&((cury>=YMIN)&&(cury<=YMAX)))
      pto(DC,curx,cury,cor);
}
//////////////////////////////////////////////////////////////
COLORREF pegapto(HDC DC,unsigned x,unsigned y)
{
   return GetPixel(DC,x,y);
}
//////////////////////////////////////////////////////////////
void limpatela(HDC DC,COLORREF cor)
{
   int x,y;
   for(x=XMIN;x<=XMAX;x++) for(y=YMIN;y<=YMAX;y++) pto(DC,x,y,cor);
}
//////////////////////////////////////////////////////////////
void inicializa(void)
{
   XRes=BitMapWidth;
   YRes=BitMapHeight;
   asp=(1600.0/1200.0)*((float)YRes/(float)XRes);
   XRes--;
   YRes--;
   XMIN=0;YMIN=0;XMAX=XRes;YMAX=YRes;TVX=XRes;TVY=YRes;
}
//////////////////////////////////////////////////////////////
void texto(HDC DC,int x,int y,char *str,COLORREF cor)
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
            if ((byte & 0x80) != 0) pto(DC,x+i,y+j,cor);
            byte <<= 1;
         }
      }
      x += 8;
   }
}
