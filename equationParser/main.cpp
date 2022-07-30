#include <iostream>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <ctype.h>
#include <process.h>
#include <malloc.h>
#include <stdlib.h>
#include <windows.h>
#include <dos.h>
#include <dir.h>

using namespace std;

/*
Black        |   0
Blue         |   1
Green        |   2
Cyan         |   3
Red          |   4
Magenta      |   5
Brown        |   6
Light Gray   |   7
Dark Gray    |   8
Light Blue   |   9
Light Green  |   10
Light Cyan   |   11
Light Red    |   12
Light Magenta|   13
Yellow       |   14
White        |   15
*/

#define RED 4
#define WHITE 15
#define BLINK 16*6

//char ecuaciones[]="((((4*5)+2)*3)-7)";
//char ecuaciones[]="((1*(2+3))/(5-(3*4)))";
//char ecuaciones[]="a*b*c+(4+5/(2*3)+8-(3+4*5)-(5/d+3)+((2*g)+5))";
//char ecuaciones[]="2*3+4*5";
//char ecuaciones[]="a+b*c-d/e";
//char ecuaciones[]="4*5+2-5*3";
char *ecuaciones;
char *indice;
char token[80], esquerda[80], direita[80];
int size, i, aux, val;
char *tempstr;
char openpars;
int LastPut;
///////////////////////////////////////////////////////////////////
void gotoxy (int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition (GetStdHandle (STD_OUTPUT_HANDLE), pos);
}
///////////////////////////////////////////////////////////////////
int wherex()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo (GetStdHandle (STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.X;
}
///////////////////////////////////////////////////////////////////
int wherey()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo (GetStdHandle (STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.Y;
}
///////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////
int Min (char *ind)
{
    int aux, pos;
    size_t g;
    pos = 0;
    aux = 99;
    for (g = 0; g <= strlen (ind) - 1; g++)
    {
        if ( ( (int) ind[g] < aux) && ( (int) ind[g] != -1))
        {
            aux = ind[g];
            pos = g;
        }
    }
    return pos;
}
///////////////////////////////////////////////////////////////////
void substr (char *dest, char *str1, char tam, char inic)
{
    int q;
    for (q = 0; q <= tam; q++)
        dest[q] = str1[q + inic];
    dest[q] = 0;
}
///////////////////////////////////////////////////////////////////
void EmOrdem (int addr)
{
    if ( (token[addr] != -1) && (addr != -1))
    {
        EmOrdem (esquerda[addr]);
        putch (ecuaciones[ (int) token[addr]]);
        EmOrdem (direita[addr]);
    }
}
///////////////////////////////////////////////////////////////////
void PreOrdem (int addr)
{
    if ( (token[addr] != -1) && (addr != -1))
    {
        putch (ecuaciones[ (int) token[addr]]);
        PreOrdem (esquerda[addr]);
        PreOrdem (direita[addr]);
    }
}
///////////////////////////////////////////////////////////////////
void PosOrdem (int addr)
{
    if ( (token[addr] != -1) && (addr != -1))
    {
        PosOrdem (esquerda[addr]);
        PosOrdem (direita[addr]);
        putch (ecuaciones[ (int) token[addr]]);
    }
}
///////////////////////////////////////////////////////////////////
void Tree (char *substrg, char stpoint, char dir)
{
    char pos, *substrg1, *substrg2;
    int i, aux;
    pos = Min (substrg);
    aux = strlen (substrg);
    substrg1 = (char *) malloc (pos + 1);
    substrg2 = (char *) malloc (aux - pos + 2);
    for (i = 0; i <= pos - 1; i++)
        substrg1[i] = -1;
    for (i = 0; i <= aux - pos + 1; i++)
        substrg2[i] = -1;
    substr (substrg1, substrg, pos - 1, 0);
    substr (substrg2, substrg, aux - pos + 1, pos + 1);
    if ( (strlen (substrg1) < 1) || (strlen (substrg2) < 1))
    {
        if (strlen (substrg1) < 1)
            esquerda[val] = -1;
        if (strlen (substrg2) < 1)
            direita[val] = -1;
        token[val] = stpoint + pos;
        return;
    }
    token[val] = stpoint + pos;
    val++;
    esquerda[val - 1] = val;
    Tree (substrg1, stpoint, 0);
    val++;
    if (strlen (substrg2) != 0)
        LastPut++;
    direita[LastPut] = val;
    Tree (substrg2, stpoint + pos + 1, 1);
}
///////////////////////////////////////////////////////////////////
void Inic (void)
{
    int y;
    openpars = 0;
    val = 0;
    LastPut = 0;
    indice = (char *) malloc (strlen (ecuaciones));
    for (i = 0; i <= 80; i++)
    {
        indice[i] = -1;
        token[i] = -1;
        esquerda[i] = -1;
        direita[i] = -1;
    }
    size = strlen (ecuaciones);
    i = 0;
    aux = 0;
    for (y = 0; y <= size - 1; y++)
    {
        if (ecuaciones[y] == '(')
            i = i + 10;
        if (ecuaciones[y] == ')')
            i = i - 10;
        if ( (ecuaciones[y] == '*') || (ecuaciones[y] == '/'))
            indice[y] = i + 2;
        if ( (ecuaciones[y] == '+') || (ecuaciones[y] == '-'))
            indice[y] = i + 1;
        if (isalnum (toupper (ecuaciones[y])) != 0)
            indice[y] = i + 3;
    }
    indice[size] = 0;
}
///////////////////////////////////////////////////////////////////
void SpaceCleaner (void)
{
    int k, v;
    size_t e;
    char *aux;
    aux = (char *) malloc (80);
    k = 0;
    for (e = 0; e <= strlen (ecuaciones); e++)
    {
        if (ecuaciones[e] != 32)
        {
            aux[k] = ecuaciones[e];
            k++;
        }
    }
    for (v = 0; v <= k; v++)
        ecuaciones[v] = aux[v];
    ecuaciones[v] = 0;
}
///////////////////////////////////////////////////////////////////
char FindError (void)
{
    int error = 0;
    size_t g;
    char op = 0, cp = 0;
    for (g = 0; g <= strlen (ecuaciones); g++)
    {
        if (ecuaciones[g] == 40)
            op++;
        if (ecuaciones[g] == 41)
            cp++;
        if (cp > op)
        {
            error = g;
            textcolor (RED);
            printf ("\r\n\r\nERRO!! Fecha parˆntese extra! \r\n\r\n");
            printf (ecuaciones);
            textcolor (RED + BLINK);
            gotoxy (error + 1, wherey());
            putch (ecuaciones[error]);
            break;
        }
    }
    if (error == 0)
    {
        op = 0;
        cp = 0;
        for (g = strlen (ecuaciones); (int) g >= 0; g--)
        {
            if (ecuaciones[g] == 40)
                op++;
            if (ecuaciones[g] == 41)
                cp++;
            if (cp < op)
            {
                error = g;
                textcolor (RED);
                printf ("\r\n\r\nERRO!! Abre parˆntese extra! \r\n\r\n");
                printf (ecuaciones);
                textcolor (RED + BLINK);
                gotoxy (error + 1, wherey());
                putch (ecuaciones[error]);
                break;
            }
        }
    }
    if (error == 0)
    {
        for (g = 0; g < strlen (ecuaciones); g++)
        {
            if ( (strchr ("+-/*(", ecuaciones[g]) != 0)
                    && (strchr ("+-/*)", ecuaciones[g + 1]) != 0))
            {
                error = g;
                textcolor (RED);
                printf ("\r\n\r\nERRO!! Operador ou parˆntese inv lido! \r\n\r\n");
                printf (ecuaciones);
                textcolor (RED + BLINK);
                gotoxy (error + 1, wherey());
                putch (ecuaciones[error]);
                break;
            }
        }
    }
    if (error == 0)
    {
        for (g = 0; g < strlen (ecuaciones); g++)
        {
            if ( (strchr ("+-/*", ecuaciones[g]) != 0)
                    && ( (ecuaciones[g + 1] == 0) || (g == 0)))
            {
                error = g;
                textcolor (RED);
                printf ("\r\n\r\nERRO!! Operador deve ter dois operandos! \r\n\r\n");
                printf (ecuaciones);
                textcolor (RED + BLINK);
                gotoxy (error + 1, wherey());
                putch (ecuaciones[error]);
                if (error == 0)
                    error = -1;
                break;
            }
        }
    }
    if (error == 0)
    {
        for (g = 0; g < strlen (ecuaciones); g++)
        {
            if ( (isalnum (toupper (ecuaciones[g])) != 0)
                    && (isalnum (toupper (ecuaciones[g + 1])) != 0))
            {
                error = g;
                textcolor (RED);
                printf ("\r\n\r\nERRO!! Operando deve ter apenas um caracter! \r\n\r\n");
                printf (ecuaciones);
                textcolor (RED + BLINK);
                gotoxy (error + 1, wherey());
                putch (ecuaciones[error]);
                if (error == 0)
                    error = -1;
                break;
            }
        }
    }
    textcolor (WHITE);
    gotoxy (1, wherey() + 2);
    return error;
}
///////////////////////////////////////////////////////////////////
void GetData (void)
{
    char MeepMeep = 0;
    int c;
    ecuaciones = (char *) malloc (80);
    do
    {
        printf ("\nEntre com a equa‡„o: ");
        gets (ecuaciones);
        SpaceCleaner();
        c = strcmp (ecuaciones, "0");
        if (c == 0)
            exit (0);
        MeepMeep = FindError();
    }
    while (MeepMeep != 0);
}
///////////////////////////////////////////////////////////////////

int main()
{
    system ("cls"); //clrscr();
    GetData();
    Inic();
    Tree (indice, 0, -1);
    textcolor (15);
    putch (13);
    putch (10);
    printf ("\nFormato Pr‚-ordem: ");
    PreOrdem (0);
    putch (13);
    putch (10);
    printf ("\nFormato P¢s-ordem: ");
    PosOrdem (0);
    putch (13);
    putch (10);
    printf ("\nFormato Central: ");
    EmOrdem (0);
    printf ("\n\n\n");
    return 0;
}
