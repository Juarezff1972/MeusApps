main()
int getmode(int *ncols)
{
reg.h.ah=0x0f;
int86(0x10,&reg,&reg);
*ncols=reg.h.ah;
return reg.h.al;
}
void setmode(int mode)
{
reg.h.ah=0;
reg.h.al=mode;
int86(0x10,&reg,&reg);
}
void gotoxy(int column,int row,int page)
{
reg.h.ah=2;
reg.h.bh=page;
reg.h.dh=row;
reg.h.dl=column;
int86(0x10,&reg,&reg);
}
void getxy(int *column,int *row,int page)
{
reg.h.ah=3;
reg.h.bh=page;
int86(0x10,&reg,&reg);
*row=reg.h.dl;
*column=reg.h.dh;
}
void setpage(int page)
{
reg.h.ah=5;
reg.h.al=page;
int86(0x10,&reg,&reg);
}
int getpage(void)
{
reg.h.ah=0x0f;
int86(0x10,&reg,&reg);
return(reg.h.bh);
}
void setwindow(int x1,int y1,int x2,int y2,int color)
{
reg.h.ah=6;
reg.h.al=0;
reg.h.bh=color;
reg.h.cl=x1;
reg.h.ch=y1;
reg.h.dl=x2;
reg.h.dh=y2;
int86(0x10,&reg,&reg);
}
void scrollwindowup(int x1,int y1,int x2,int y2,int color)
{
reg.h.ah=6;
reg.h.al=0;
reg.h.bh=color;
reg.h.cl=x1;
reg.h.ch=y1;
reg.h.dl=x2;
reg.h.dh=y2;
int86(0x10,&reg,&reg);
}
void scrollwindowdn(int x1,int y1,int x2,int y2,int color)
{
reg.h.ah=7;
reg.h.al=1;
reg.h.bh=color;
reg.h.cl=x1;
reg.h.ch=y1;
reg.h.dl=x2
reg.h.dh=y2;
int86(0x10,&reg,&reg);
}
void setCGApalette(int paletteno)
{
reg.h.ah=0x0b;
reg.h.bl=paletteno;
reg.h.bl=paletteno;
int86(0x10,&reg,&reg);
}
void plot(int x,int y,int color)
{
reg.h.ah=0x0c;
reg.h.al=color;
reg.h.bh=0;
reg.x.cx=x;
reg.x.dx=y;
int86(0x010,&reg,&reg);
}
