void inicia_pc(void)
{
   pc.x=TVX/2;   pc.y=TVY/2;
}
//////////////////////////////////////////////////////////////
void move_abs(float x,float y)
{
   pc.x=x;       pc.y=y;
}
//////////////////////////////////////////////////////////////
void move_rel(float dx,float dy)
{
   pc.x=pc.x+dx;   pc.y=pc.y+dy;
}
//////////////////////////////////////////////////////////////
void pto_abs(HDC DC,float x,float y,COLORREF cor)
{
   imp_ponto(DC,x,y,cor);
   move_abs(x,y);
}
//////////////////////////////////////////////////////////////
void pto_rel(HDC DC,float dx,float dy,COLORREF cor)
{
   move_rel(dx,dy);
   imp_ponto(DC,pc.x,pc.y,cor);
}
//////////////////////////////////////////////////////////////
void reta_rel1(HDC DC,float x,float y,COLORREF cor)
{
   linha(DC,pc.x,pc.y,x,y,cor);
   move_abs(x,y);
}
//////////////////////////////////////////////////////////////
void reta_rel2(HDC DC,float dx,float dy,COLORREF cor)
{
   int cx=pc.x,cy=pc.y;
   move_rel(dx,dy);
   linha(DC,cx,cy,pc.x,pc.y,cor);
}

//////////////////////////////////////////////////////////////
/*void define_marca(unsigned char estilo)
{
   marca=estilo;
} */
//////////////////////////////////////////////////////////////
void viewport(int x1,int y1,int x2,int y2)
{
   if (x1>x2) troca(&x1,&x2);   if (y1>y2) troca(&y1,&y2);
   if (x1<0) x1=0;              if (y1<0) y1=0;
   if (x2>XRes) x2=XRes;        if (y2>YRes) y2=YRes;
   XMIN=x1;XMAX=x2;YMIN=y1;YMAX=y2;
   TVX=XMAX-XMIN;
   TVY=YMAX-YMIN;
}
//////////////////////////////////////////////////////////////
void setcoord(float x1,float y1,float x2,float y2)
{
   if (x1>x2) trocafloat(&x1,&x2);
   if (y1>y2) trocafloat(&y1,&y2);
   TOPO=y1;BASE=y2;ESQ=x1;DIR=x2;
}
//////////////////////////////////////////////////////////////
void coordtran(float x,float y)
{
   tmppto.x=(x-ESQ)*(DIR-ESQ)/(float)XRes;
   tmppto.y=YRes-((y-BASE)*(TOPO-BASE)/(float)YRes);
}
//////////////////////////////////////////////////////////////
void escalona(float x,float y,float ox,float oy,float ex,float ey)
{
   tmppto.x=(x-ox)*ex+ox;
   tmppto.y=(y-oy)*ey+oy;
}
//////////////////////////////////////////////////////////////
void translada(float x,float y,float tx,float ty)
{
   tmppto.x=x+tx;
   tmppto.y=y+ty;
}
//////////////////////////////////////////////////////////////
void rotaciona(float x,float y,float rx,float ry,float ang)
{
   float rad=ang*pi/180.0;
   tmppto.x=cos(rad)*(x+rx)-sin(rad)*(y+ry)-rx;
   tmppto.y=sin(rad)*(x+rx)+cos(rad)*(y+ry)-ry;
}
//////////////////////////////////////////////////////////////
void rot_reta(float x,float y,float px1,float py1,float px2, float py2)
{
   float ang=atan((py2-py1)/(px2-px1))*180.0/pi;
   translada(x,y,-px1,-py1);
   rotaciona(tmppto.x,tmppto.y,0.0,0.0,ang);
   escalona(tmppto.x,tmppto.y,0.0,0.0,-1.0,-1.0);
   rotaciona(tmppto.x,tmppto.y,0.0,0.0,-ang);
   translada(x,y,px1,py1);
}
