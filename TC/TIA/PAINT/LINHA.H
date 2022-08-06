/*Reta ponto medio*/
void linha(HDC DC,float x1,float y1,float x2,float y2,COLORREF cor)
{
   float x,y,a=fabs(x1-x2),b=fabs(y1-y2),deltaG1,deltaG2,G,d=-1.0;
   if (a>b)
   {
      deltaG1=2.0*(b-a);
      deltaG2=2.0*b;
      G=deltaG2-a;
      if (x1>=x2) {x=x2;y=y2;if (y>y1) d=1.0;} else {x=x1;y=y1;if (y>y2) d=1.0;}
      imp_ponto(DC,x,y,cor);
      while(x<(x1>x2?x1:x2))
      {
         if (G<=0.0) G=G+deltaG2; else {y=y-d;G=G+deltaG1;}
         x++;
         imp_ponto(DC,x,y,cor);
      }
   }
   else
   {
      deltaG1=2.0*(a-b);
      deltaG2=2.0*a;
      G=deltaG2-b;
      if (y1>=y2)
      {
         x=x2;
         y=y2;
         if (x>x1) d=1.0;
      }
      else
      {
         x=x1;
         y=y1;
         if (x>x2) d=1.0;
      }
      imp_ponto(DC,x,y,cor);
      while(y<(y1>y2?y1:y2))
      {
         if (G<=0.0) G=G+deltaG2; else {x=x-d;G=G+deltaG1;}
         y++;
         imp_ponto(DC,x,y,cor);
      }
   }
}
/////////////////////////////////////////////////////////////////////////
unsigned char CompOutCode(float x,float y,
                          float xmin,float ymin,float xmax,float ymax)
{
   unsigned char code;
   const LEFT=1,RIGHT=2,BOTTOM=4,TOP=8;
   code=0;
   if (y>ymax) code=TOP; else if (y<ymin) code=BOTTOM;
   if (x>xmax) code=code+RIGHT; else if (x<xmin) code=code+LEFT;
   return code;
}
/////////////////////////////////////////////////////////////////////////
void cortalinha(HDC DC,float x0,float y0,float x1,float y1,
                float xmin,float ymin,float xmax,float ymax,COLORREF cor)
{
   const LEFT=1,RIGHT=2,BOTTOM=4,TOP=8;
   unsigned char outcode0,outcode1,outcodeOut,aceita,pronto;
   float x,y;
   aceita=0;pronto=0;
   outcode0=CompOutCode(x0,y0,xmin,ymin,xmax,ymax);
   outcode1=CompOutCode(x1,y1,xmin,ymin,xmax,ymax);
   do
   {
      if (outcode0==0 && outcode1==0)
      {
         aceita=1;pronto=1;
      }
      else
      if ((outcode0 & outcode1)!=0) pronto=1;
      else
      {
         if (outcode0!=0) outcodeOut=outcode0; else outcodeOut=outcode1;
      }
      if ((outcodeOut & TOP)==TOP)
      {
         x=x0+(x1-x0)*(ymax-y0)/(y1-y0);
         y=ymax;
      }
      else
      if ((outcodeOut & BOTTOM)==BOTTOM)
      {
         x=x0+(x1-x0)*(ymin-y0)/(y1-y0);
         y=ymin;
      }
      else
      if ((outcodeOut & RIGHT)==RIGHT)
      {
         y=y0+(y1-y0)*(xmax-x0)/(x1-x0);
         x=xmax;
      }
      else
      if ((outcodeOut & LEFT)==LEFT)
      {
         y=y0+(y1-y0)*(xmin-x0)/(x1-x0);
         x=xmin;
      }
      if (outcode0==outcodeOut)
      {
         x0=x;y0=y;outcode0=CompOutCode(x0,y0,xmin,ymin,xmax,ymax);
      }
      else
      if (outcode1==outcodeOut)
      {
         x1=x;y1=y;outcode1=CompOutCode(x1,y1,xmin,ymin,xmax,ymax);
      }
   }
   while (pronto);
   if (aceita) linha(DC,x0,y0,x1,y1,cor);
}

