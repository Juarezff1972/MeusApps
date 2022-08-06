/*circulo incremental*/
void circulo(float x,float y,float radius,unsigned char cor)
{
   long a,af,b,bf,target,r2;
   target=0;
   a=radius;
   b=0;
   r2=radius*radius;
   while (a>=b)
   {
      b=floor(sqrt(r2-(a*a))+0.5);
      trocalong(&target,&b);
      while(b<target)
      {
         af=(floor((asp*100)+0.5)*a) / 100;
         bf=(floor((asp*100)+0.5)*b) / 100;
         imp_ponto(x+af,y+b,cor);
         imp_ponto(x+bf,y+a,cor);
         imp_ponto(x-af,y+b,cor);
         imp_ponto(x-bf,y+a,cor);
         imp_ponto(x-af,y-b,cor);
         imp_ponto(x-bf,y-a,cor);
         imp_ponto(x+af,y-b,cor);
         imp_ponto(x+bf,y-a,cor);
         b++;
      }
      a--;
   }
}
//////////////////////////////////////////////////////////////
/*circulo ponto medio*/
void circulo1(float x,float y,float raio,unsigned char cor)
{
   float x1,y1,G,DeltaG1,DeltaG2;
   x1=0.0;
   y1=raio;
   G=1.0-raio;
   DeltaG1=3.0;
   DeltaG2=-2.0*raio+5.0;
   imp_ponto( x1+x, y1+y,cor);
   imp_ponto( y1+x, x1+y,cor);
   imp_ponto( y1+x,-x1+y,cor);
   imp_ponto( x1+x,-y1+y,cor);
   imp_ponto(-x1+x,-y1+y,cor);
   imp_ponto(-y1+x,-x1+y,cor);
   imp_ponto(-y1+x, x1+y,cor);
   imp_ponto(-x1+x, y1+y,cor);
   while (x1 < y1)
   {
      if (G < 0.0)
      {
         G=G+DeltaG1;
         DeltaG2=DeltaG2+2.0;
      }
      else
      {
         G=G+DeltaG2;
         DeltaG2=DeltaG2+4.0;
         y1--;
      }
      DeltaG1=DeltaG1+2.0;
      x1++;
      imp_ponto( x1+x, y1+y,cor);
      imp_ponto( y1+x, x1+y,cor);
      imp_ponto( y1+x,-x1+y,cor);
      imp_ponto( x1+x,-y1+y,cor);
      imp_ponto(-x1+x,-y1+y,cor);
      imp_ponto(-y1+x,-x1+y,cor);
      imp_ponto(-y1+x, x1+y,cor);
      imp_ponto(-x1+x, y1+y,cor);
   }
}
//////////////////////////////////////////////////////////////
/*elipse ponto medio*/
void elipse1(float x,float y,float raioX,float raioY,unsigned char cor)
{
   int xi,yi,deltai,sigma1,sigma2;
   float incX,incY,mx,my;
   if (raioX>raioY)
   {
      incX=1.0;
      incY=(float)raioY/(float)raioX;
   }
   if (raioY>raioX)
   {
      incY=1.0;
      incX=(float)raioX/(float)raioY;
   }
   if (raioX==raioY)
   {
      incX=1.0;
      incY=1.0;
   }
   if (raioY<raioX)
   {
      yi=0;
      xi=raioX;
      deltai=2*(1-raioX);
lblX1:
      mx=xi*incX;my=yi*incY;
      imp_ponto(x+mx,y+my,cor);
      imp_ponto(x+mx,y-my,cor);
      imp_ponto(x-mx,y+my,cor);
      imp_ponto(x-mx,y-my,cor);
      if (xi<=0) goto lblX4;
      if (deltai<0) goto lblX2;
      if (deltai>0) goto lblX3;
      if (deltai==0) goto lblX20;
lblX2:
      sigma1=2*deltai+2*xi-1;
      if (sigma1<=0) goto lblX10;
      if (sigma1>0) goto lblX20;
lblX3:
      sigma2=2*deltai-2*yi-1;
      if (sigma2<=0) goto lblX20;
      if (sigma2>0) goto lblX30;
lblX10:
      yi++;
      deltai=deltai+2*yi+1;
      goto lblX1;
lblX20:
      yi++;xi--;
      deltai=deltai+2*yi-2*xi+2;
      goto lblX1;
lblX30:
      xi--;
      deltai=deltai-2*xi+1;
      goto lblX1;
lblX4:
      return;
   }
   if (raioY>=raioX)
   {
      xi=0;
      yi=raioY;
      deltai=2*(1-raioY);
lblY1:
      mx=xi*incX;my=yi*incY;
      imp_ponto(x+mx,y+my,cor);
      imp_ponto(x+mx,y-my,cor);
      imp_ponto(x-mx,y+my,cor);
      imp_ponto(x-mx,y-my,cor);
      if (yi<=0) goto lblY4;
      if (deltai<0) goto lblY2;
      if (deltai>0) goto lblY3;
      if (deltai==0) goto lblY20;
lblY2:
      sigma1=2*deltai+2*yi-1;
      if (sigma1<=0) goto lblY10;
      if (sigma1>0) goto lblY20;
lblY3:
      sigma2=2*deltai-2*xi-1;
      if (sigma2<=0) goto lblY20;
      if (sigma2>0) goto lblY30;
lblY10:
      xi++;
      deltai=deltai+2*xi+1;
      goto lblY1;
lblY20:
      xi++;yi--;
      deltai=deltai+2*xi-2*yi+2;
      goto lblY1;
lblY30:
      yi--;
      deltai=deltai-2*yi+1;
      goto lblY1;
lblY4:
      return;
   }
}
//////////////////////////////////////////////////////////////
/*arco de circulo*/
void arco(float x,float y,float raio,float alfa,float beta,unsigned cor)
{
   float radiano;
   float x1,y1;
   float angulo;
   if (alfa>beta) trocafloat(&alfa,&beta);
   move_abs(raio*cos(alfa*pi/180.0)+x,y-(raio*sin(alfa*pi/180.0))/asp);
   for(angulo=alfa;angulo<=beta;angulo++)
   {
      radiano=angulo*pi/180.0;
      x1=raio*cos(radiano);
      y1=raio*sin(radiano);
      y1=-y1/asp;
      reta_rel1(x1+x,y1+y,cor);
   }
}
//////////////////////////////////////////////////////////////
/*arco de elipse*/
void arcoelip(float x,float y,float raioX,float raioY,float alfa,float beta,unsigned cor)
{
   float radiano;
   float x1,y1;
   float angulo;
   if (alfa>beta) trocafloat(&alfa,&beta);
   move_abs(raioX*cos(alfa*pi/180.0)+x,y-(raioY*sin(alfa*pi/180.0))/asp);
   for(angulo=alfa;angulo<=beta;angulo++)
   {
      radiano=angulo*pi/180.0;
      x1=raioX*cos(radiano);
      y1=raioY*sin(radiano);
      y1=-y1/asp;
      reta_rel1(x1+x,y1+y,cor);
   }
}
//////////////////////////////////////////////////////////////
void des_bezier(float x1,float y1,float x2,float y2,float x3,float y3,unsigned x4,unsigned y4,float passo,unsigned char cor)
{
   float px1,py1,px2,py2,t;
   px1=x1;
   py1=y1;
   move_abs(px1,py1);
   for(t=passo;t<1.0;t=t+passo)
   {
      px2=pow(1-t,3)*x1+3.0*t*pow(1-t,2)*x2+3.0*pow(t,2)*(1-t)*x3+pow(t,3)*x4;
      py2=pow(1-t,3)*y1+3.0*t*pow(1-t,2)*y2+3.0*pow(t,2)*(1-t)*y3+pow(t,3)*y4;
      reta_rel1(px2,py2,cor);
   }
   reta_rel1(x4,y4,cor);
}
//////////////////////////////////////////////////////////////
void bezier(unsigned char np,poli pline,unsigned char cor)
{
   unsigned char i;
   float passo=0.01;
   i=np;
   if (np==1)
   {
      des_bezier(pline[0].x,pline[0].y,pline[0].x,pline[0].y,pline[1].x,pline[1].y,pline[1].x,pline[1].y,passo,cor);
      return;
   }
   if (np==2)
   {
      des_bezier(pline[0].x,pline[0].y,pline[1].x,pline[1].y,pline[1].x,pline[1].y,pline[2].x,pline[2].y,passo,cor);
      return;
   }
   for(i=0;i<np;i=i+3)
   {
      if (i+3>np) break;
      des_bezier(pline[i].x,pline[i].y,pline[i+1].x,pline[i+1].y,pline[i+2].x,pline[i+2].y,pline[i+3].x,pline[i+3].y,passo,cor);
   }
   if ((np-i)==2) des_bezier(pline[np-2].x,pline[np-2].y,pline[np-2].x,pline[np-2].y,pline[np-1].x,pline[np-1].y,pline[np-1].x,pline[np-1].y,passo,cor);
   if ((np-i)==1) des_bezier(pline[np-3].x,pline[np-3].y,pline[np-2].x,pline[np-2].y,pline[np-2].x,pline[np-2].y,pline[np-1].x,pline[np-1].y,passo,cor);
}
//////////////////////////////////////////////////////////////
void des_spline(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4,float passo,unsigned char cor)
{
   float px1,py1,px2,py2,t;
   px1=(1.0/6.0)*(x1+4.0*x2+1.0*x3);
   py1=(1.0/6.0)*(y1+4.0*y2+1.0*y3);
   move_abs(px1,py1);
   for(t=passo;t<=1.0;t=t+passo)
   {
      px2=(1.0/6.0)*(pow(1-t,3)*x1+((3.0*pow(t,3)-6.0*pow(t,2)+4.0))*x2+((-3.0*pow(t,3)+3.0*pow(t,2)+3.0*t+1.0))*x3+(pow(t,3))*x4);
      py2=(1.0/6.0)*(pow(1-t,3)*y1+((3.0*pow(t,3)-6.0*pow(t,2)+4.0))*y2+((-3.0*pow(t,3)+3.0*pow(t,2)+3.0*t+1.0))*y3+(pow(t,3))*y4);
      reta_rel1(px2,py2,cor);
   }
}
//////////////////////////////////////////////////////////////
void spline(unsigned char np,poli pline,unsigned char cor)
{
   unsigned char i;
   float passo=0.01;
   i=np;
   if (np==1)
   {
      des_spline(pline[0].x,pline[0].y,pline[0].x,pline[0].y,pline[1].x,pline[1].y,pline[1].x,pline[1].y,passo,cor);
      return;
   }
   if (np==2)
   {
      des_spline(pline[0].x,pline[0].y,pline[1].x,pline[1].y,pline[1].x,pline[1].y,pline[2].x,pline[2].y,passo,cor);
      return;
   }
   for(i=0;i+3<=np-1;i++)
   {
      if (i+3>np) break;
      des_spline(pline[i].x,pline[i].y,pline[i+1].x,pline[i+1].y,pline[i+2].x,pline[i+2].y,pline[i+3].x,pline[i+3].y,passo,cor);
   }
   if ((np-i)==2) des_spline(pline[np-2].x,pline[np-2].y,pline[np-2].x,pline[np-2].y,pline[np-1].x,pline[np-1].y,pline[np-1].x,pline[np-1].y,passo,cor);
   if ((np-i)==1) des_spline(pline[np-3].x,pline[np-3].y,pline[np-2].x,pline[np-2].y,pline[np-2].x,pline[np-2].y,pline[np-1].x,pline[np-1].y,passo,cor);
}