#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <windows.h>

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance,LPSTR lpszCmdLine, int cmdShow)
{
   int result;
   int port = 0x201;
   outp(port,255);
   do
   {
      result = inport(port);
      if (kbhit()) break;
   } while (result==inport(port));
//   printf("Word read from port %X = 0x%X\n", port, result);
   return 0;                      
}

