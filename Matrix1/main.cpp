#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <process.h>
#include <errno.h>
#include <windows.h>
#include <direct.h>
#include <io.h>

void wait (float seconds)
{
    clock_t endwait;
    endwait = clock () + seconds * CLK_TCK ;

    while (clock() < endwait)
    {;}
}
void clrscr()
{
    system ("cls");
}
void textcolor (int ForgC)
{
    WORD wColor;
    HANDLE hStdOut = GetStdHandle (STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    //We use csbi for the wAttributes word.
    if (GetConsoleScreenBufferInfo (hStdOut, &csbi))
    {
        //Mask out all but the background attribute, and add in the forgournd     color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute (hStdOut, wColor);
    }

    return;
}

using namespace std;

int main()
{
    char MATRIX[] = "wake up, Neo... The Matrix has you... Follow the white rabbit. Knock, knock, Neo.";
    int i = 0;
    textcolor (10);
    clrscr();
    textcolor (10);

    while (i != 5)
    {
        wait (.3);
        _cprintf ("%c", MATRIX[i]);
        i++;
    }

    while (i != 10)
    {
        wait (.2);
        _cprintf ("%c", MATRIX[i]);
        i++;
    }

    while (i != 13)
    {
        wait (.2);
        _cprintf ("%c", MATRIX[i]);
        i++;
    }

    while (i != 16)
    {
        wait (.3);
        _cprintf ("%c", MATRIX[i]);
        i++;
    }

    wait (2);
    clrscr();

    while (i != 22)
    {
        wait (.3);
        _cprintf ("%c", MATRIX[i]);
        i++;
    }

    while (i != 28)
    {
        wait (.2);
        _cprintf ("%c", MATRIX[i]);
        i++;
    }

    while (i != 33)
    {
        wait (.1);
        _cprintf ("%c", MATRIX[i]);
        i++;
    }

    while (i != 38)
    {
        wait (.3);
        _cprintf ("%c", MATRIX[i]);
        i++;
    }

    wait (2);
    clrscr();

    while (i != 43)
    {
        wait (.3);
        _cprintf ("%c", MATRIX[i]);
        i++;
    }

    while (i != 52)
    {
        wait (.2);
        _cprintf ("%c", MATRIX[i]);
        i++;
    }

    while (i != 60)
    {
        wait (.3);
        _cprintf ("%c", MATRIX[i]);
        i++;
    }

    while (i != 63)
    {
        wait (.2);
        _cprintf ("%c", MATRIX[i]);
        i++;
    }

    wait (2.3);
    clrscr();
    _cprintf ("Knock, knock, Neo.");
    wait (3);
    //cout << "Hello world!" << endl;
    return 0;
}
