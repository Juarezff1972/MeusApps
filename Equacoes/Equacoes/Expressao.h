#pragma once

#include <string.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <conio.h>
#include <stdarg.h>
#include <stdlib.h>
//#include <alloc.h>
#include <process.h>
#include <wchar.h>
#include <msclr\marshal.h>
#include "msclr\marshal_cppstd.h"

#define VERDADE 1
#define FALSO 0
#define MAIS 0
#define MENOS 1
#define VEZES 2
#define DIVIDIR 3
#define EXP 4
#define MODULO 5
#define APAREN 6
#define FPAREN 7
#define NUM 8
#define CELL 9
#define FUNC 10
#define EOLN 11
#define MAU 12
#define TEXTO 0
#define VALOR 1
#define FORMULA 2
#define MAXFUNCNAMELEN 5
#define PARSERSTACKSIZE 20
#define MSGSTACKERROR "\nERRO: estouro de pilha.\n"
#define MAXINPUT 79
#define MAXCOLS 5   /* Maximo e' 702 */
#define MAXROWS 5
///////////////////////////////////////////////////////////////////////
//extern "C"
//{
	struct TOKENREC
	{
		char state;
		union
		{
			double value;
			struct c1
			{
				int row, col;
			} c;
			char funcname[MAXFUNCNAMELEN + 1];
		} x;
	}; //TOKENREC1;
	///////////////////////////////
	struct CELLREC
	{
		char attrib;
		union
		{
			char text[MAXINPUT + 1];
			double value;
			struct
			{
				double fvalue;
				char formula[MAXINPUT + 1];
			} f;
		} v;
	};// CELLREC1;
//}
///////////////////////////////////////////////////////////////////////
class Expressao
{
public:
	struct TOKENREC stack[PARSERSTACKSIZE], curtoken;
	int stacktop, tokentype, error;
	char* input, isformula;
	char* funcao, * funct;
	//typedef struct CELLREC* CELLPTR;
	CELLREC cell[MAXCOLS][MAXROWS];
	int rowwidth(int row);
	int formulastart(char** input, int* col, int* row);
	//int matherror(struct exception* e);
	int isfunc(char* s);
	int nexttoken(void);
	void push(TOKENREC* token);
	TOKENREC pop(void);
	int gotostate(int production);
	double cellvalue(int col, int row);
	void shift(int state);
	void reduce(int reduction);
	double parse(char* s, int* att);
	

	double iniciar(System::Windows::Forms::TextBox^ t, System::Windows::Forms::ListBox^ s);
	Expressao()
	{

	}
	~Expressao()
	{

	}

private:
	gcroot<System::Windows::Forms::ListBox^> lista1;
};

