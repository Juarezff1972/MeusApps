void retangulo(HDC DC,float x1,float y1,float x2,float y2,COLORREF cor)
{
   linha(DC,x1,y1,x2,y1,cor);
   linha(DC,x1,y2,x2,y2,cor);
   linha(DC,x1,y1,x1,y2,cor);
   linha(DC,x2,y1,x2,y2,cor);
}
//////////////////////////////////////////////////////////////
void poligono_regular(HDC DC,float x,float y,unsigned lados,float raio,COLORREF cor)
{
   float x1,y1;
   float ad,ar;
   unsigned i;
   if ((lados<3) || (lados>150)) return;
   move_abs(x,y);
   move_rel(raio,0);
   for(i=1;i<=lados;i++)
   {
      ad=i*360.0/lados;
      ar=ad*pi/180.0;
      x1=raio*cos(ar);
      y1=raio*sin(ar);
      y1=-y1/asp;
      reta_rel1(DC,x1+x,y1+y,cor);
   }
}
//////////////////////////////////////////////////////////////
void desenha_marca(HDC DC,float x,float y,forma figura,float escala,COLORREF cor)
{
   unsigned char numpassos=figura[0];
   unsigned char dpassos[50];
   unsigned char passos[100];
   unsigned char i;
   for (i=1;i<=numpassos/2;i++) dpassos[i-1]=figura[i];
   for (i=0;i<=numpassos;i=i+2)
   {
      passos[i]=dpassos[i/2]/16;
      passos[i+1]=dpassos[i/2]-passos[i]*16;
   }
   move_abs(x,y);
   for (i=0;i<=numpassos-1;i++)
   {
      switch (passos[i])
      {
         case 0 : move_rel( 0     ,-escala);     break;
         case 1 : move_rel( escala,-escala);     break;
         case 2 : move_rel( escala, 0);          break;
         case 3 : move_rel( escala, escala);     break;
         case 4 : move_rel( 0     , escala);     break;
         case 5 : move_rel(-escala, escala);     break;
         case 6 : move_rel(-escala, 0);          break;
         case 7 : move_rel(-escala,-escala);     break;
         case 8 : reta_rel2 (DC, 0     ,-escala,cor); break;
         case 9 : reta_rel2 (DC, escala,-escala,cor); break;
         case 10: reta_rel2 (DC, escala, 0     ,cor); break;
         case 11: reta_rel2 (DC, escala, escala,cor); break;
         case 12: reta_rel2 (DC, 0     , escala,cor); break;
         case 13: reta_rel2 (DC,-escala, escala,cor); break;
         case 14: reta_rel2 (DC,-escala, 0     ,cor); break;
         case 15: reta_rel2 (DC,-escala,-escala,cor); break;
      }
   }
   move_abs(x,y);
}
//////////////////////////////////////////////////////////////
void polilinha(HDC DC,unsigned char np,poli pline,unsigned char fecha,COLORREF cor)
{
   unsigned char i;
   move_abs(pline[0].x,pline[0].y);
   for(i=1;i<=np;i++) reta_rel1(DC,pline[i].x,pline[i].y,cor);
   if (fecha) reta_rel1(DC,pline[0].x,pline[0].y,cor);
}
//////////////////////////////////////////////////////////////
void polimarca(HDC DC,unsigned char np,poli pline,unsigned char fecha,forma figura,unsigned char escala,COLORREF cor)
{
   unsigned char i;
   move_abs(pline[0].x,pline[0].y);
   desenha_marca(DC,pline[0].x,pline[0].y,figura,escala,cor);
   for(i=1;i<=np;i++)
   {
      reta_rel1(DC,pline[i].x,pline[i].y,cor);
      desenha_marca(DC,pline[i].x,pline[i].y,figura,escala,cor);
   }
   if (fecha) reta_rel1(DC,pline[0].x,pline[0].y,cor);
}
//////////////////////////////////////////////////////////////
/*setor de elipse*/
void setorelip(HDC DC,float x,float y,float raioX,float raioY,float alfa,float beta,COLORREF cor)
{
   arcoelip(DC,x,y,raioX,raioY,alfa,beta,cor);
   linha(DC,x,y,raioX*cos(alfa*pi/180.0)+x,-(raioY*sin(alfa*pi/180.0)/asp)+y,cor);
   linha(DC,x,y,raioX*cos(beta*pi/180.0)+x,-(raioY*sin(beta*pi/180.0)/asp)+y,cor);
}
//////////////////////////////////////////////////////////////
/*setor de circulo*/
void setorcirc(HDC DC,float x,float y,float raio,float alfa,float beta,COLORREF cor)
{
   arco(DC,x,y,raio,alfa,beta,cor);
   linha(DC,x,y,raio*cos(alfa*pi/180.0)+x,-(raio*sin(alfa*pi/180.0)/asp)+y,cor);
   linha(DC,x,y,raio*cos(beta*pi/180.0)+x,-(raio*sin(beta*pi/180.0)/asp)+y,cor);
}
