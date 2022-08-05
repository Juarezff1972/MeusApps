#include <windows.h>
#include <stdio.h>
#include <conio.h>
////////////////////////////////////////////////////////////////////
int kbrd(void)
{
   if ((GetAsyncKeyState(0x0d)<0)||(GetAsyncKeyState(0x02)<0)) return 1; else return 0;
}
////////////////////////////////////////////////////////////////////
int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance,
  LPSTR lpszCmdLine, int cmdShow)
{
   while (!kbrd());

   return 0;
}


