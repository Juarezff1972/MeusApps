#include <windows.h>
#include <conio.h>
/* Pinos 1 e 3 devem estar conectados */
//////////////////////////////////////////////////////////////////////////
int far pascal readbutton(void)
{
   int result;
   int port = 0x201;
   outp(port,255);
   do
   {
      result = inp(port);
      if ((GetAsyncKeyState(0x0d)<0)||(GetAsyncKeyState(0x02)<0)) break;
   } while (result==inport(port));
   return 0;
}
//////////////////////////////////////////////////////////////////////////
int far pascal LIBMAIN (HANDLE H, WORD W1, WORD W2, LPSTR L)
{
   if (W2>0) UnlockData(0);
   return 1;              
}

/*
HANDLE hInstance, WORD wDataSeg, WORD cbHeapSize, LPSTR lpCmdLine
*/
