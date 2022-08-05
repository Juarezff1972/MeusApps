#include <windows.h>
//////////////////////////////////////////////////////////////////////////
int far pascal somatorio(int val1, int val2, int val3)
{
   int aux;
   aux = val1+val2;
   aux = aux+val3;
   return aux;
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
