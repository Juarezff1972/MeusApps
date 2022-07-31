#include "pch.h"
#include "Expressao.h"

using namespace msclr::interop;


int Expressao::rowwidth(int row)
{
	return((row == 0) ? 1 : (int)log10(row + 1) + 1);
}
///////////////////////////////////////////////////////////////////////
int Expressao::formulastart(char** input, int* col, int* row)
/* Retorna VERDADE se o string e' o inicio de uma formula, sen„o FALSO.
   Tambem retorna a coluna e linha da formula.
*/
{
	int len, maxlen = rowwidth(MAXROWS);
	char* start, numstring[10];
	if (!isalpha(**input)) return(FALSO);
	*col = *((*input)++) - 'A';
	if (isalpha(**input))
	{
		*col *= 26;
		*col += *((*input)++) - 'A' + 26;
	}
	if (*col >= MAXCOLS) return(FALSO);
	start = *input;
	for (len = 0; len < maxlen; len++)
	{
		if (!isdigit(*((*input)++)))
		{
			(*input)--;
			break;
		}
	}
	if (len == 0) return(FALSO);
	strncpy(numstring, start, len);
	numstring[len] = 0;
	*row = atoi(numstring) - 1;
	if ((*row >= MAXROWS) || (*row == -1)) return(FALSO);
	return(VERDADE);
}
///////////////////////////////////////////////////////////////////////
/*int Expressao::matherror(struct exception* e)
{
	e->retval = HUGE_VAL;
	return(1);
}*/
///////////////////////////////////////////////////////////////////////
int Expressao::isfunc(char* s)
/* Retorna VERDADE se o string e' uma fun‡„o legal, sen„o FALSO. */
{
	int len = strlen(s);
	if (strncmp(s, Expressao::input, len) == 0)
	{
		strncpy(curtoken.x.funcname, input, len);
		curtoken.x.funcname[len] = 0;
		input += len;
		return(VERDADE);
	}
	return(FALSO);
}
///////////////////////////////////////////////////////////////////////
int Expressao::nexttoken(void)
/* Pega o proximo token da corrente de entrada */
{
	char* start, numstring[80];
	int decimal, len, numlen;
	while (*input == ' ') input++;
	if (*input == 0) return(EOLN);
	//lista1->Items->Add("input: " + *input);
	if (strchr("0123456789.", *input))
	{
		start = input;
		len = 0;
		decimal = FALSO;
		while ((isdigit(*input)) || ((*input == '.') && (!decimal)))
		{
			if (*input == '.') decimal = VERDADE;
			input++;
			len++;
		}
		if ((len == 1) && (start[0] == '.')) return(MAU);
		if (*input == 'E')
		{
			input++;
			len++;
			if (strchr("+-", *input) != NULL)
			{
				input++;
				len++;
			}
			numlen = 0;
			while ((isdigit(*input)) && (++numlen <= 3))
			{
				input++;
				len++;
			}
		}
		strncpy(numstring, start, len);
		numstring[len] = 0;

		curtoken.x.value = atof(numstring);
		if (errno == ERANGE) return(MAU);
		return(NUM);
	}
	else if (isalpha(*input))
	{
		if (isfunc("ABS") || isfunc("ACOS") || isfunc("ASIN") ||
			isfunc("ATAN") || isfunc("COSH") || isfunc("COS") ||
			isfunc("EXP") || isfunc("LOG10") || isfunc("LOG") ||
			isfunc("POW10") || isfunc("ROUND") || isfunc("SINH") ||
			isfunc("SIN") || isfunc("SQRT") || isfunc("SQR") ||
			isfunc("TANH") || isfunc("TAN") || isfunc("TRUNC"))
			return(FUNC);
		if (formulastart(&input, &curtoken.x.c.col, &curtoken.x.c.row))
		{
			isformula = VERDADE;
			return(CELL);
		}
		else return(MAU);
	}
	else switch (*(input++))
	{
	case '+': return(MAIS);
	case '-': return(MENOS);
	case '*': return(VEZES);
	case '/': return(DIVIDIR);
	case '^': return(EXP);
	case ':': return(MODULO);
	case '(': return(APAREN);
	case ')': return(FPAREN);
	default: return(MAU);
	}
}
///////////////////////////////////////////////////////////////////////
void Expressao::push(struct TOKENREC* token)
/* Coloca o novo token na pilha */
{
	if (stacktop == PARSERSTACKSIZE - 1)
	{
		printf(MSGSTACKERROR);
		error = VERDADE;
	}
	else stack[++stacktop] = *token;
}
///////////////////////////////////////////////////////////////////////
struct TOKENREC Expressao::pop(void)
	/* Tira o token do topo da pilha */
{
	return(stack[stacktop--]);
}
///////////////////////////////////////////////////////////////////////
int Expressao::gotostate(int production)
/* Acha o novo estado baseado na ultima produção e o estado-topo. */
{
	int state = stack[stacktop].state;
	lista1->Items->Add("--> produção: " + production + " | estado: " + state);
	if (production <= 3)
	{
		switch (state)
		{
		case 0: return(1);
		case 9: return(19);
		case 20: return(28);
		}
	}
	else if (production <= 6)
	{
		switch (state)
		{
		case 0:
		case 9:
		case 20: return(2);
		case 12: return(21);
		case 13: return(22);
		}
	}
	else if (production <= 8)
	{
		switch (state)
		{
		case 0:
		case 9:
		case 12:
		case 13:
		case 20: return(3);
		case 14: return(23);
		case 15: return(24);
		case 16: return(25);
		}
	}
	else if (production <= 10)
	{
		switch (state)
		{
		case 0:
		case 9:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 20: return(4);
		}
	}
	else if (production <= 12)
	{
		switch (state)
		{
		case 0:
		case 9:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 20: return(6);
		case 5: return(17);
		}
	}
	else
	{
		switch (state)
		{
		case 0:
		case 5:
		case 9:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 20: return(8);
		}
	}
	return(30);
}
///////////////////////////////////////////////////////////////////////
double Expressao::cellvalue(int col, int row)
/* Acha o valor de uma celula */
{
	//if (cell[col][row] == NULL) return(0);
	if (cell[col][row].attrib == TEXTO) return(HUGE_VAL);
	if (cell[col][row].attrib == FORMULA) return(cell[col][row].v.f.fvalue);
	return(cell[col][row].v.value);
}
///////////////////////////////////////////////////////////////////////
void Expressao::shift(int state)
/* Desloca um token na pilha */
{
	System::String^ aux;
	aux = "";
	switch (state)
	{
	case 5:
		aux = "MENOS (20)";
		break;
	case 7:
		aux = "CÈLULA (20)";
		break;
	case 9:
		aux = "ABRE PARENTESE (5)";
		break;
	case 10:
		aux = "NÚMERO";
		break;
	case 11:
		aux = "FUNÇÃO";
		break;
	case 12:
		aux = "MAIS";
		break;
	case 13:
		aux = "MENOS (1)";
		break;
	case 14:
		aux = "VEZES";
		break;
	case 15:
		aux = "DIVIDIR";
		break;
	case 16:
		aux = "EXPOENTE";
		break;
	case 18:
		aux = "MÓDULO";
		break;
	case 20:
		aux = "ABRE PARENTESE (11)";
		break;
	case 26:
		aux = "CÈLULA (18)";
		break;
	case 27:
		aux = "FECHA PARENTESE (19)";
		break;
	case 29:
		aux = "FECHA PARENTESE (28)";
		break;
	}

	lista1->Items->Add("---------> shift estado: " + aux);

	curtoken.state = state;
	push(&curtoken);
	tokentype = nexttoken();
}
///////////////////////////////////////////////////////////////////////
void Expressao::reduce(int reduction)
/* Completa uma redu‡„o */
{
	struct TOKENREC token1, token2;
	int counter;
	System::String^ aux;
	switch (reduction)
	{
	case 1:
		token1 = pop();
		pop();
		token2 = pop();
		curtoken.x.value = token1.x.value + token2.x.value;
		lista1->Items->Add("token: " + token1.x.value + " + " + token2.x.value + " = " + curtoken.x.value);
		break;
	case 2:
		token1 = pop();
		pop();
		token2 = pop();
		curtoken.x.value = token2.x.value - token1.x.value;
		lista1->Items->Add("token: " + token2.x.value + " - " + token1.x.value + " = " + curtoken.x.value);
		break;
	case 4:
		token1 = pop();
		pop();
		token2 = pop();
		curtoken.x.value = token1.x.value * token2.x.value;
		lista1->Items->Add("token: " + token1.x.value + " * " + token2.x.value + " = " + curtoken.x.value);
		break;
	case 5:
		token1 = pop();
		pop();
		token2 = pop();
		if (token1.x.value == 0) curtoken.x.value = HUGE_VAL;
		else curtoken.x.value = token2.x.value / token1.x.value;
		lista1->Items->Add("token: " + token2.x.value + " / " + token1.x.value + " = " + curtoken.x.value);
		break;
	case 7:
		token1 = pop();
		pop();
		token2 = pop();
		curtoken.x.value = pow(token2.x.value, token1.x.value);
		lista1->Items->Add("token: " + token2.x.value + " ^ " + token1.x.value + " = " + curtoken.x.value);
		break;
	case 9:
		token1 = pop();
		pop();
		curtoken.x.value = -token1.x.value;
		lista1->Items->Add("token: -" + token1.x.value);
		break;
	case 11:
		token1 = pop();
		pop();
		token2 = pop();
		curtoken.x.value = 0;
		if (token1.x.c.row == token2.x.c.row)
		{
			if (token1.x.c.col < token2.x.c.col) error = VERDADE;
			else
			{
				for (counter = token2.x.c.col; counter <= token1.x.c.col; counter++)
					curtoken.x.value += cellvalue(counter, token1.x.c.row);
			}
		}
		else if (token1.x.c.col == token2.x.c.col)
		{
			if (token1.x.c.row < token2.x.c.row) error = VERDADE;
			else
			{
				for (counter = token2.x.c.row; counter <= token1.x.c.row; counter++)
					curtoken.x.value += cellvalue(token1.x.c.col, counter);
			}
		}
		else error = VERDADE;
		lista1->Items->Add("token: " + token1.x.value + " | " + token2.x.value + " | " + curtoken.x.value);
		break;
	case 13:
		curtoken = pop();
		curtoken.x.value = cellvalue(curtoken.x.c.col, curtoken.x.c.row);
		lista1->Items->Add("token: " + curtoken.x.value);
		break;
	case 14:
		pop();
		curtoken = pop();
		pop();
		aux = gcnew System::String(curtoken.x.funcname);
		lista1->Items->Add("token: " + aux);
		break;
	case 16:
		pop();
		curtoken = pop();
		pop();
		token1 = pop();
		aux = gcnew System::String(token1.x.funcname);
		aux = aux + "(" + curtoken.x.value + ")";
		lista1->Items->Add("------->> função token: " + aux);
		if (strcmp(token1.x.funcname, "ABS") == 0)
			curtoken.x.value = fabs(curtoken.x.value);
		else
			if (strcmp(token1.x.funcname, "ACOS") == 0)
				curtoken.x.value = acos(curtoken.x.value);
			else
				if (strcmp(token1.x.funcname, "ASIN") == 0)
					curtoken.x.value = asin(curtoken.x.value);
				else
					if (strcmp(token1.x.funcname, "ATAN") == 0)
						curtoken.x.value = atan(curtoken.x.value);
					else
						if (strcmp(token1.x.funcname, "COSH") == 0)
							curtoken.x.value = cosh(curtoken.x.value);
						else
							if (strcmp(token1.x.funcname, "COS") == 0)
								curtoken.x.value = cos(curtoken.x.value);
							else
								if (strcmp(token1.x.funcname, "EXP") == 0)
									curtoken.x.value = exp(curtoken.x.value);
								else
									if (strcmp(token1.x.funcname, "LOG10") == 0)
										curtoken.x.value = log10(curtoken.x.value);
									else
										if (strcmp(token1.x.funcname, "LOG") == 0)
											curtoken.x.value = log(curtoken.x.value);
										else
											if (strcmp(token1.x.funcname, "ROUND") == 0)
												curtoken.x.value = (int)(curtoken.x.value + 0.5);
											else
												if (strcmp(token1.x.funcname, "POW10") == 0)
													curtoken.x.value = pow(10, curtoken.x.value);
												else
													if (strcmp(token1.x.funcname, "SINH") == 0)
														curtoken.x.value = sinh(curtoken.x.value);
													else
														if (strcmp(token1.x.funcname, "SIN") == 0)
															curtoken.x.value = sin(curtoken.x.value);
														else
															if (strcmp(token1.x.funcname, "SQRT") == 0)
																curtoken.x.value = sqrt(curtoken.x.value);
															else
																if (strcmp(token1.x.funcname, "SQR") == 0)
																	curtoken.x.value *= curtoken.x.value;
																else
																	if (strcmp(token1.x.funcname, "TANH") == 0)
																		curtoken.x.value = tanh(curtoken.x.value);
																	else
																		if (strcmp(token1.x.funcname, "TAN") == 0)
																			curtoken.x.value = tan(curtoken.x.value);
																		else
																			if (strcmp(token1.x.funcname, "TRUNC") == 0)
																				curtoken.x.value = (int)curtoken.x.value;

		lista1->Items->Add("token: " + aux + " = " + curtoken.x.value);
		break;
	case 3:
	case 6:
	case 8:
	case 10:
	case 12:
	case 15:
		curtoken = pop();
		lista1->Items->Add("valor: " + curtoken.x.value + " | redução: " + reduction);
		break;
	}
	//lista1->Items->Add("token: " + token1.x.value + " | " + token2.x.value +" | " + curtoken.x.value);
	curtoken.state = gotostate(reduction);
	push(&curtoken);
}
///////////////////////////////////////////////////////////////////////
double Expressao::parse(char* s, int* att)
/* Parse sobre o string s - retorna o valor do string analisado, e coloca
   o atributo em att: TEXTO = 0, CONSTANTE = 1, FORMULA = 2.
*/
{
	struct TOKENREC firsttoken;
	char accepted = FALSO;
	char copy[80];
	error = FALSO;
	isformula = FALSO;
	input = copy;
	strupr(strcpy(copy, s));
	stacktop = -1;
	firsttoken.state = 0;
	firsttoken.x.value = 0;
	push(&firsttoken);
	tokentype = nexttoken();
	System::String^ aux;
	aux = "";
	switch (tokentype)
	{
	case MENOS:
		aux = "MENOS";
		break;
	case CELL:
		aux = "CÈLULA";
		break;
	case APAREN:
		aux = "ABRE PARENTESE";
		break;
	case NUM:
		aux = "NÚMERO";
		break;
	case FUNC:
		aux = "FUNÇÃO";
		break;
	case MAIS:
		aux = "MAIS";
		break;
	case VEZES:
		aux = "VEZES";
		break;
	case DIVIDIR:
		aux = "DIVIDIR";
		break;
	case EXP:
		aux = "EXPOENTE";
		break;
	case MODULO:
		aux = "MÓDULO";
		break;
	case FPAREN:
		aux = "FECHA PARENTESE (19)";
		break;
	case EOLN:
		aux = "FIM DE LINHA";
		break;
	}

	lista1->Items->Add("-----> tokentype: " + aux);
	do
	{
		switch (stack[stacktop].state)
		{
		case  0:
		case  9:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 20:
			if (tokentype == NUM) shift(10);
			else if (tokentype == CELL) shift(7);
			else if (tokentype == FUNC) shift(11);
			else if (tokentype == MENOS) shift(5);
			else if (tokentype == APAREN) shift(9);
			else error = VERDADE;
			break;
		case 1:
			if (tokentype == EOLN) accepted = VERDADE;
			else if (tokentype == MAIS) shift(12);
			else if (tokentype == MENOS) shift(13);
			else error = VERDADE;
			break;
		case 2:
			if (tokentype == VEZES) shift(14);
			else if (tokentype == DIVIDIR) shift(15);
			else reduce(3);
			break;
		case 3:
			reduce(6);
			break;
		case 4:
			if (tokentype == EXP) shift(16);
			else reduce(8);
			break;
		case 5:
			if (tokentype == NUM) shift(10);
			else if (tokentype == CELL) shift(7);
			else if (tokentype == FUNC) shift(11);
			else if (tokentype == APAREN) shift(9);
			else error = VERDADE;
			break;
		case 6:
			reduce(10);
			break;
		case 7:
			if (tokentype == MODULO) shift(18);
			else reduce(13);
			break;
		case 8:
			reduce(12);
			break;
		case 10:
			reduce(15);
			break;
		case 11:
			if (tokentype == APAREN) shift(20);
			else error = VERDADE;
			break;
		case 17:
			reduce(9);
			break;
		case 18:
			if (tokentype == CELL) shift(26);
			else error = VERDADE;
			break;
		case 19:
			if (tokentype == MAIS) shift(12);
			else if (tokentype == MENOS) shift(13);
			else if (tokentype == FPAREN) shift(27);
			else error = VERDADE;
			break;
		case 21:
			if (tokentype == VEZES) shift(14);
			else if (tokentype == DIVIDIR) shift(15);
			else reduce(1);
			break;
		case 22:
			if (tokentype == VEZES) shift(14);
			else if (tokentype == DIVIDIR) shift(15);
			else reduce(2);
			break;
		case 23:
			reduce(4);
			break;
		case 24:
			reduce(5);
			break;
		case 25:
			reduce(7);
			break;
		case 26:
			reduce(11);
			break;
		case 27:
			reduce(14);
			break;
		case 28:
			if (tokentype == MAIS) shift(12);
			else if (tokentype == MENOS) shift(13);
			else if (tokentype == FPAREN) shift(29);
			else error = VERDADE;
			break;
		case 29:
			reduce(16);
			break;
		case 30:
			error = VERDADE;
			break;
		}
	} while ((!accepted) && (!error));
	if (error)
	{
		*att = TEXTO;
		return(0);
	}
	if (isformula) *att = FORMULA;
	else *att = VALOR;
	strcpy(s, copy);
	return(stack[stacktop].x.value);
}
///////////////////////////////////////////////////////////////////////

auto wide_to_char(std::string inp)
{
	int i;
	int j;
	wchar_t w;

	i = inp.size();

	char* str = new char[i + 1];
	memset(str, 0, i + 1);

	for (j = 0; j < i; j++)
	{
		str[j] = inp[j];
	}
	return str;
}

/*System::String convertToString(char* a, int size)
{
	int i;
	System::String s = "";
	for (i = 0; i < size; i++)
	{
		s = s + a[i];
	}
	return s;
}*/

double Expressao::iniciar(System::Windows::Forms::TextBox^ t, System::Windows::Forms::ListBox^ s)
{
	double a;
	int attr;
	int compr;
	int i, ii;
	System::String^ str;
	funct = (char*)malloc(t->Text->Length * 2 + 5);
	funcao = (char*)malloc(t->Text->Length + 5);
	//cell[0][0] = 1;
	cell[0][0].v.value = 10.5;
	cell[0][0].attrib = FORMULA;

	lista1 = s;

	lista1->Items->Clear();

	str = t->Text;

	strcpy(funcao, wide_to_char(marshal_as<std::string>(str)));

	//strcpy(funcao, "1+2-3*4/5");
	strcpy(funct, "");
	compr = strlen(funcao);
	for (i = 0; i < compr; i++) funcao[i] = toupper(funcao[i]);
	strcpy(funct, funcao);
	for (i = 0; i < (t->Text->Length + 5) - compr; i++) strcat(funct, " ");
	i = 0; ii = 0;
	while (i <= compr + 1)
	{
		if (funcao[ii] == 'X')
		{
			funct[i] = 'A'; funct[i + 1] = '1'; i = i + 2;
		}
		else
		{
			funct[i] = funcao[ii]; i = i + 1;
		}
		ii = ii + 1;
	}
	free(funcao);
	a = parse(funct, &attr);
	//if (s->Text == "") s->Text = strcat("Atributo: ", itoa(attr)); else s->Text = strcat("\n", strcat("Atributo: ", itoa(attr)));

	lista1->Items->Add("Atributo: " + attr.ToString());

	return a;
}
