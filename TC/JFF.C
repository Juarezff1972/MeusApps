#include "graphics.h"

int mode,adapter;

main()
{
mode=CGAHI;
adapter=CGA;
initgraph(&adapter,&mode,"");
setbkcolor(0);
setcolor(3);
arc(320,100,0,1,10);
circle(320,100,20);
line(0,0,639,0);
line(639,0,639,199);
line(639,199,0,199);
line(0,199,0,0);
rectangle(10,10,629,189);
}