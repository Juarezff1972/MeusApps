/*================================================================*/
/* ANIMATE1.C  Animacao Simples usando os Graficos no TURBO-C ++  */
/* Imagens e Labirinto foram criados para EGA ou para resolucoes  */
/* superiores - CGA necessita de ajustes nas Imagens/Labirinto e  */
/* consequentemente este DEMO somente rodara em sistemas CGA      */
/*================================================================*/

#ifdef _TINY_
#error Demos Graficos nao rodarao no modelo TINY.
#endif

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <graphics.h>
#include <dos.h>

#define RIGHT 0
#define LEFT  1
#define UP    2
#define DOWN  3

#define NSTEP 0
#define LSTEP 1
#define RSTEP 2
int       TestFlash=0;
int       GraphDriver;			/* driver grafico */
int       GraphMode;				/* valor do modo grafico */
int       MaxColors;				/* maximo de cores disponiveis */
int       ErrorCode = 0;		/* reporta erros em graficos */
int       XPos = 3, YPos = 3;		/* posicao inicial */
int       Move, LastMove;				/* direcoes de movimento */
void      *Flash[3];						/* estrelas ao chocar-se com as paredes */
void      *Man[4] [3];					/* imagens atraves do labirinto */
unsigned  TimeOut = 100;
char       Pilha_Coord[1000];
int        Matriz_Temp[10][12];
int        L=0,C=0;
//////////////////////////////////////////////////////////////////////
void Initialize()
{										/* inicializa sistema grafico e reporta erros */
	GraphDriver = DETECT;		/* solicita auto-deteccao */
	initgraph( &GraphDriver, &GraphMode, "");
	ErrorCode = graphresult();		/* testa resultados da inicializacao */
	if (ErrorCode != grOk)			/* caso ocorra erro na inicializacao */
	{
		printf("Erro no Sistema Grafico: %s\n",grapherrormsg(ErrorCode));
		exit(1);
	}
	MaxColors = getmaxcolor() + 1;	/* serie maxima de cores */
}
//////////////////////////////////////////////////////////////////////
void *SaveImage(int left, int top, int right, int bottom)
{
	void *image;

	image = malloc(imagesize(left, top, right, bottom));
	getimage(left, top, right, bottom, image);						/* salva */
	putimage(left, top, image, XOR_PUT);									/* apaga */
	return(image);														/* retorna ponteiro */
}
//////////////////////////////////////////////////////////////////////
void CreateImages()
{
	int i, j, MaxColor = getmaxcolor();
	int pflash1[] = { 100, 40, 110, 60, 100, 70, 120,
		65, 140, 80, 130, 60, 140, 50, 120, 65, 100, 40 };
	int pflash2[] = { 120, 40, 110, 55, 90, 60, 110,
		65, 120, 80, 130, 65, 150, 60, 130, 65, 120, 40 };
	int pflash3[] = { 140, 40, 130, 60, 140, 70, 120,
		65, 100, 80, 110, 60, 100, 50, 120, 55, 140, 40 };
	int bkarm_u[] = { 100, 62, 102, 68, 105, 70,
			  108, 69, 109, 65 };
	int ftarm_u[] = { 140, 62, 143, 52,
			  130, 47, 125, 52 };
	int bkarm_r[] = { 121, 145, 128, 147, 130, 150,
			  129, 153, 125, 154 };
	int ftarm_r[] = { 117, 176, 106, 176,
			  100, 165, 107, 160 };
	randomize();
	setcolor(random(MaxColor)+1);
	setfillstyle(random(11)+1, random(MaxColor)+1);
	fillpoly(sizeof(pflash1)/(2*sizeof(int)), pflash1);
	Flash[0] = SaveImage( 100, 40, 140, 80);

	setcolor(random(MaxColor)+1);
	setfillstyle(random(11)+1, random(MaxColor)+1);
	fillpoly(sizeof(pflash2)/(2*sizeof(int)), pflash2);
	Flash[1] = SaveImage(90, 40, 150, 80);

	setcolor(random(MaxColor)+1);
	setfillstyle(random(11)+1, random(MaxColor)+1);
	fillpoly(sizeof(pflash3)/(2*sizeof(int)), pflash3);
	Flash[2] = SaveImage(100, 40, 140, 80);

	setcolor(MaxColor);
	setfillstyle(SOLID_FILL, MaxColor);
	ellipse(120, 60, 0, 360, 10, 10);
																		/* desenha e preenche cabecas */
	floodfill(120, 60, MaxColor);
	ellipse(220, 160, 0, 360, 13, 7);
	floodfill(220, 160, MaxColor);
	ellipse(120, 51, 0, 180, 3, 3);
																		/* adiciona narizes */
	ellipse(207, 160, 90, 270, 3, 2);
	setfillstyle(HATCH_FILL, MaxColor);
																		/* troca estilo de preenchimento */
	ellipse(128, 60, 270, 90, 12, 6);
																		/* adiciona ombros */
	floodfill(135, 60, MaxColor);
	ellipse(112, 60, 90, 270, 12, 6);
	floodfill(105, 60, MaxColor);
	ellipse(220, 154, 0, 180, 8, 9);
	floodfill(220, 150, MaxColor);
	ellipse(220, 166, 180, 360, 8, 9);
	floodfill(220, 170, MaxColor);

	for (i=100; i<=145; i++)
		for (j=45; j<=70; j++)
			putpixel(345-i, 120-j, getpixel(i,j));
	for (i=204; i<=235; i++)
		for (j=145; j<=175; j++)
			putpixel(345-i, j, getpixel(i,j));

	Man[UP] [NSTEP]    = SaveImage(100, 45, 140, 70);
	Man[DOWN] [NSTEP]  = SaveImage(205, 50, 245, 75);
	Man[LEFT] [NSTEP]  = SaveImage(203, 140, 234, 180);
	Man[RIGHT] [NSTEP] = SaveImage(111, 140, 142, 180);

	setfillstyle(CLOSE_DOT_FILL, MaxColor);

	putimage(100, 45, Man[UP] [NSTEP], COPY_PUT);
																								/* adiciona bracos */
	fillpoly(sizeof(bkarm_u)/(2*sizeof(int)), bkarm_u);
	fillpoly(sizeof(ftarm_u)/(2*sizeof(int)), ftarm_u);
								/* fillpoly preenche os contornos calculados entre os */
								/* pontos finais - pode sobrepor parte da imagem ori- */
								/* ginal e da fronteira - restaura a imagem original  */
								/* utilizando OR_PUT 																	*/
	putimage(100, 45, Man[UP] [NSTEP], OR_PUT);

	for (i=100; i<=145; i++)
		for (j=45; j<=70; j++)
		{
			putpixel(345-i, j, getpixel(i, j));
																				/* roda para esquerda/direita */
			putpixel(345-i, 220-j, getpixel(i,j));
																				/* e inverte */
			putpixel(i, 220-j, getpixel(i, j));
																				/* de cabeca para baixo */
		}
																				/* salva todas as imagens */
		Man[UP] [LSTEP]	  = SaveImage(100, 45, 145, 70);
		Man[UP] [RSTEP]   = SaveImage(200, 45, 245, 70);
		Man[DOWN] [LSTEP] = SaveImage(100, 150, 145, 175);
		Man[DOWN] [RSTEP] = SaveImage(200, 150, 245, 175);

		putimage(100, 140, Man[LEFT] [NSTEP], COPY_PUT);
		fillpoly(sizeof(bkarm_r)/(2*sizeof(int)), bkarm_r);
																						/* adiciona bracos */
		fillpoly(sizeof(ftarm_r)/(2*sizeof(int)), ftarm_r);
		putimage(100, 140, Man[LEFT] [NSTEP], OR_PUT);

		for (i=100; i<=132; i++)
			for (j=144; j<=177; j++)
			{
				putpixel(345-i, j, getpixel(i, j));
																					/* roda para cima/baixo */
				putpixel(345-i, 220-j, getpixel(i, j));
																					/* e inverte */
				putpixel(i, 220-j, getpixel(i, j));
																					/* para a direita ... */
			}
		Man[LEFT] [RSTEP]		= SaveImage(100, 144, 132, 177);
		Man[LEFT] [LSTEP]		= SaveImage(100, 43, 132, 76);
		Man[RIGHT] [LSTEP]	= SaveImage(213, 43, 245, 76);
		Man[RIGHT] [RSTEP]	= SaveImage(213, 144, 245, 177);
}
//////////////////////////////////////////////////////////////////////
void FillSquare(int my,int mx)
{
   int i,j;
   int auxc=(my)*50;
   int auxl=(mx)*40;
   setviewport(0+auxc,0+auxl,50+auxc,40+auxl,1);
	setfillstyle(SOLID_FILL,GREEN);
	floodfill( 3,3,2);
	setviewport(0,0,626,390,6);
}
//////////////////////////////////////////////////////////////////////
void CreateMaze()
{
	int i, j,k;
	int maze1[]		= {800, 40, 50, 40, 50, 160, 100, 160} ;
	int maze2[]		= {100, 80, 100, 120, 200, 120, 200, 240};
	int maze3[]		= {0, 200, 100, 200};
	int maze4[]		= {150, 160, 150, 240, 50, 240, 50, 280};
	int maze5[]		= {150, 80, 300, 80};
	int maze6[]		= {250, 80, 250, 160};
	int maze7[]		= {100, 280, 100, 320};
	int maze8[]		= {150, 280, 250, 280, 250, 240, 300, 240};
	int maze9[]		= {250, 40, 350, 40, 350, 80};
	int maze10[]	        = {200, 200, 300, 200, 300, 120, 400, 120, 400, 80, 400, 160};
	int maze11[]	        = {300, 280, 350, 280, 350, 160};
	int maze12[]	        = {400, 0, 400, 40};
	int maze13[]	        = {450, 120, 450, 40,550, 40, 550, 80};
	int maze14[]	        = {500, 80, 500, 160,400, 160, 400, 240};
	int maze15[]	        = {400, 280, 400, 320};
	int maze16[]	        = {550, 120, 550, 200, 450, 200,450, 280, 550, 280};
	int maze17[]	        = {500, 240, 600, 240};
	struct viewporttype vp;

	setcolor(MaxColors - 1);
														/* determina cor de desenho p/ branco */
	setviewport(0,0,640,480,1);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	outtextxy(20, 330, "Use as setas para mover George " " <F>im para sair do programa");

	setviewport(0,0,640,250,1);
   setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
	getviewsettings(&vp);
	rectangle(0, 0, vp.right - vp.left ,vp.bottom - vp.top );
	clearviewport();
	lineto(123,54);
	outtextxy(10, 20, "INICIO");
	drawpoly(sizeof(maze1)/(2*sizeof(int)),  maze1);
	drawpoly(sizeof(maze2)/(2*sizeof(int)),  maze2);
	drawpoly(sizeof(maze3)/(2*sizeof(int)),  maze3);
	drawpoly(sizeof(maze4)/(2*sizeof(int)),  maze4);
	drawpoly(sizeof(maze5)/(2*sizeof(int)),  maze5);
	drawpoly(sizeof(maze6)/(2*sizeof(int)),  maze6);
	drawpoly(sizeof(maze7)/(2*sizeof(int)),  maze7);
	drawpoly(sizeof(maze8)/(2*sizeof(int)),  maze8);
	drawpoly(sizeof(maze9)/(2*sizeof(int)),  maze9);
	drawpoly(sizeof(maze10)/(2*sizeof(int)), maze10);
	drawpoly(sizeof(maze11)/(2*sizeof(int)), maze11);
	drawpoly(sizeof(maze12)/(2*sizeof(int)), maze12);
	drawpoly(sizeof(maze13)/(2*sizeof(int)), maze13);
	drawpoly(sizeof(maze14)/(2*sizeof(int)), maze14);
	drawpoly(sizeof(maze15)/(2*sizeof(int)), maze15);
	drawpoly(sizeof(maze16)/(2*sizeof(int)), maze16);
	drawpoly(sizeof(maze17)/(2*sizeof(int)), maze17);
	outtextxy(550, 300, "FIM");    clearviewport();
	rectangle(0,0,600,400);      j=0;
	for(i=0;i<12;i++) {     line(50+j,0,50+j,400); j=j+50;  }
	j=0;
   for(i=0;i<10;i++) {     line(0,40+j,600,40+j); j=j+40; }
   FillSquare(4,7);
	FillSquare(10,4);
	FillSquare(8,3);
	FillSquare(5,1);
}
//////////////////////////////////////////////////////////////////////

void FlashImage(int x, int y)
{
	int i, j, k;
	for (i=1; i<=3; i++)
		for (j=0; j<3; j++)
			for (k=0; k<2; k++)
			{
				putimage(x, y, Flash[j], XOR_PUT);
				sound(random(100) + 100);
			}
	nosound();
	TestFlash=1;
}
//////////////////////////////////////////////////////////////////////
void Put_Image(int Step)
{
	int x = XPos, y = YPos;
	switch(Move)
	{
		case RIGHT:
		case LEFT: x+=16; break;
		case UP:
		case DOWN: y+=5;
	}
	putimage(x, y, Man[Move] [Step], XOR_PUT);
}
//////////////////////////////////////////////////////////////////////
int Test_Move()
{
	int i;

	switch(Move)
	{
		case RIGHT: for (i = XPos+47; i<=XPos+96; i++)
				 if ( (getpixel(i, YPos+5)) ==GREEN )
				   return(0); break;
		case LEFT: for (i=XPos; i>=XPos-50; i--)
				if( (getpixel(i, YPos+5)) ==GREEN)
				  return(0);  break;
		case   UP: for (i=YPos; i>=YPos-40; i--)
				if( (getpixel(XPos+5, i))==GREEN)
				  return(0);  break;
		case DOWN: for(i=YPos+37; i<=YPos+76; i++)
			       if ((getpixel(XPos+5, i))==GREEN
			       )
				 return(0);
	}
	return(1);
}
//////////////////////////////////////////////////////////////////////
void Take_Step(int x, int y, int Step)
{
	Put_Image(Step);
	delay(TimeOut);
	Put_Image(Step);

	XPos += x;
	YPos += y;
	if(Step != NSTEP) LastMove = Step;
}
//////////////////////////////////////////////////////////////////////
void Move_Image(int NextStep)
{
	int i, XFake, YFake, XStep, YStep, XFlash, YFlash;

	Put_Image(NSTEP);
	Move=NextStep;
	Put_Image(NSTEP);
	switch(Move)
	{
		case RIGHT: XFake = 5; YFake = 0;
			    XStep = 5; YStep = 0;
			    XFlash = 5; YFlash = 0; break;
		case  LEFT: XFake = -5; YFake = 0;
			    XStep = -5; YStep = 0;
			    XFlash = 2; YFlash = 0; break;
		case 	UP: XFake = 0; YFake = -5;
			    XStep = 0; YStep = -4;
			    XFlash = 0; YFlash = 2; break;
		case  DOWN: XFake = 0; YFake = 5;
			    XStep = 0; YStep = 4;
			    XFlash = 0; YFlash = 0;
	}
	if(!Test_Move())
	{
		Put_Image(NSTEP);
		XPos += XFake;
		YPos += YFake;
		Put_Image(LSTEP);
		FlashImage(XPos+XFlash, YPos+YFlash);
		Put_Image(LSTEP);
		XPos -= XFake;
		YPos -= YFake;
		Put_Image(NSTEP);
	}
	else
	{
		Put_Image(NSTEP);
		XPos += XStep;
		YPos += YStep;
		if(LastMove != LSTEP)
		{
			for(i=1; i<=2; i++)
			{
				Take_Step(XStep, YStep, LSTEP);
				Take_Step(XStep, YStep, NSTEP);
				Take_Step(XStep, YStep, RSTEP);
				Take_Step(XStep, YStep, NSTEP);
			}
			Take_Step(XStep, YStep, LSTEP);
			Put_Image(NSTEP);
			delay(TimeOut);
		}

		else
		{
			for(i=1; i<=2; i++)
			{
				Take_Step(XStep, YStep, RSTEP);
				Take_Step(XStep, YStep, NSTEP);
				Take_Step(XStep, YStep, LSTEP);
				Take_Step(XStep, YStep, NSTEP);
			}
			Take_Step(XStep, YStep, RSTEP);
			Put_Image(NSTEP);
			delay(TimeOut);
		}
	}
}
//////////////////////////////////////////////////////////////////////
void move_right(void)
{
   while(TestFlash==0 && L<11)
	{
	    Move_Image(RIGHT);
	}
}
//////////////////////////////////////////////////////////////////////
void move_down(void)
{
   while(TestFlash==0 && C<9)
	{
      Move_Image(DOWN);
	}
}
//////////////////////////////////////////////////////////////////////
void move_left(void)
{
   while(TestFlash==0 && L>0)
	{
      Move_Image(LEFT);
	}
}
//////////////////////////////////////////////////////////////////////
void move_up(void)
{
   while(TestFlash==0 && C>0)
	{
      Move_Image(UP);
	}
}
//////////////////////////////////////////////////////////////////////
void StartGame()
{
//	int TestCoord=0;
	Move = LastMove = RIGHT;
	Put_Image(NSTEP);
	move_down();
	move_right();
	move_up();
	move_left();
}
//////////////////////////////////////////////////////////////////////

void ClearImages(void)
{
	int i, j;

	for(i=0; i<=2; i++)			free(Flash[i]);
	for(i=0;i<=3; i++)
		for(j=0; j<=2; j++)		free(Man[i] [j]);
}

void main(void)
{
	Initialize();														/* seleciona modo grafico */
	outtextxy(10, 10, "AGUARDE UM MOMENTO");
	setactivepage(1);
	CreateImages();										/* cria e salva imagens */
	setactivepage(0);
	clearviewport();
	CreateMaze();											/* cria labirinto */
	StartGame();											/* inicia o demonstrativo */
	ClearImages();										/* libera memoria utilizada */
	closegraph();											/* restaura modo texto */
}
