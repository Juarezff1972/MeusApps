/* +******************************************************+
   | Funcoes para a producao de Efeitos Sonoros e na Tela |
   | Desenvolvidas por Ricardo Cividanes da Silva         |
   +******************************************************+ */

//******************> Incluindo arquivos header <****************************
#include <stdio.h>
#include <conio.h>
#include <dos.h>

//***********************> Prototypes das Funcoes <**************************
void Som(int Tom, int Temp);
void Cor(int Cor_Frente, int Cor_Fundo);
void Janela(char X_S, char Y_S, char X_I, char Y_I, char T, char Preenche);
void Apaga_Tela(int XS, int YS, int XI, int YI);
void Mostra_Texto(int X, int Y, char *texto);

//**********************> Descricao das Funcoes <****************************
void Som(int Tom, int Temp)
{
     sound(Tom);
     delay(Temp);
     nosound();
}

/* Esta Funcao define uma cor para a frente e para o fundo
   da tela em modo texto */
void Cor(int Cor_Frente, int Cor_Fundo)
{
     textcolor(Cor_Frente);
     textbackground(Cor_Fundo);
}

/* Abre uma janela nas coordenadas especificadas */
void Janela(char X_S, char Y_S, char X_I, char Y_I, char T, char Preenche)
{
     char i, j;
     if (T == 'S')
     {
	// Desenha a parte superior da janela em linha simples
	gotoxy(X_S,Y_S);
	cprintf("Ú");
	for (i = X_S; i <= X_I; i++)
	    cprintf("Ä");
	cprintf("¿");
	gotoxy(X_S,Y_S++);

	// desenha as linhas verticais da janela em linha simples
	for (i = Y_S ; i <= Y_I; ++i)
	{   gotoxy(X_S,i);
	    cprintf("³");
	    for (j = X_S; j <= X_I; j++)
		cprintf(&Preenche);
	    cprintf("³");
	}

	// Desenha a linha inferior da Janela em linha simples
	gotoxy(X_S,Y_I);
	cprintf("À");
	for (i = X_S; i <= X_I; i++)
	    cprintf("Ä");
	cprintf("Ù");
     }
     else
     {
	// Desenha a parte superior da janela em linha dupla
	gotoxy(X_S,Y_S);
	cprintf("É");
	for (i = X_S; i <= X_I; i++)
	    cprintf("Í");
	cprintf("»");
	gotoxy(X_S,Y_S++);

	// desenha as linhas verticais da janela em linha dupla
	for (i = Y_S ; i <= Y_I; ++i)
	{   gotoxy(X_S,i);
	    cprintf("º");
	    for (j = X_S; j <= X_I; j++)
		cprintf(&Preenche);
	    cprintf("º");
	}

	// Desenha a linha inferior da Janela em linha dupla
	gotoxy(X_S,Y_I);
	cprintf("È");
	for (i = X_S; i <= X_I; i++)
	    cprintf("Í");
	cprintf("¼");
     }
}

void Apaga_Tela(int XS, int YS, int XI, int YI)
{
     int XS_AJ;
     while (YS <= YI)
     {
	   gotoxy(XS,YS);
	   for (XS_AJ = XS; XS_AJ <= XI; XS_AJ++)
	       cprintf(" ");
	   YS++;
     }
}

// Mostra um texto na posicao indicada em modo texto
void Mostra_Texto(int X, int Y, char *texto)
{
     gotoxy(X,Y);
     cprintf("%s", texto);
}