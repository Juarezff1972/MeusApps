#include <dos.h>
#include <math.h>
/////////////////////////////////////////////////////////////
unsigned ButtonNum;
typedef struct
{
   unsigned Button;
   unsigned x;
   unsigned y;
} Status;
/////////////////////////////////////////////////////////////
char InitMouse(void)
{
   _AX=0;
   geninterrupt(0x33);
   ButtonNum=_BX;
   return _AX;
}
/////////////////////////////////////////////////////////////
void ShowCursor(void)
{
   _AX=1;
   geninterrupt(0x33);
}
/////////////////////////////////////////////////////////////
void HideCursor(void)
{
   _AX=2;
   geninterrupt(0x33);
}
/////////////////////////////////////////////////////////////
Status GetStatus(void)
{
   Status aux;
   _AX=3;
   geninterrupt(0x33);
   aux.Button=_BX;
   aux.x=_CX;
   aux.y=_DX;
   return aux;
}
/////////////////////////////////////////////////////////////
void main(void)
{
   int i;
   InitMouse();
   ShowCursor();
   for(i=1;i==1;i++);
   HideCursor();
}