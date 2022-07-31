#include <graphics.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_STARS 300
#define SCALE 10

typedef struct
{
	unsigned int x,y;
}POINT;

typedef struct
{
	signed int x,y;
}SIGPOINT;

typedef struct
{
	POINT Loc;
	SIGPOINT Dir;
	int Size;
	int Color;
}STAR;

struct
{
	STAR Stars[MAX_STARS];
	int NumStars;
	POINT Screen;
	int Speed;
}g;
/********************************************************************/
void DrawStar(STAR *Star,int Color)
{
	int x,y;
	x=Star->Loc.x/SCALE;
	y=Star->Loc.y/SCALE;
	putpixel(x,y,Color);
	if (Star->Size==2)
	{
		putpixel(x+1,y,Color);
		putpixel(x,y+1,Color);
		putpixel(x+1,y+1,Color);
	}
}
/********************************************************************/
void NewStar(STAR *Star)
{
	Star->Loc.x=(g.Screen.x/2-SCALE+random(SCALE*2));
	Star->Loc.y=(g.Screen.y/2-SCALE+random(SCALE*2));
	Star->Size=random(10)==9?2:1;
	do
	{
		Star->Dir.x=SCALE*10-random(SCALE*20);
		Star->Dir.y=SCALE*10-random(SCALE*20);
	}
	while(Star->Dir.x==0 && Star->Dir.y==0);

	switch(random(40))
	{
		case 39:Star->Color=LIGHTRED;break;
		case 38:Star->Color=LIGHTGRAY;break;
		case 37:Star->Color=LIGHTBLUE;break;
		case 36:Star->Color=YELLOW;break;
		case 35:Star->Color=LIGHTMAGENTA;break;
		case 34:Star->Color=DARKGRAY;break;
		default:Star->Color=WHITE;
	}
}
/********************************************************************/
void MoveStars(void)
{
	int i;
	STAR *Star;
	for(Star=&g.Stars[g.NumStars-1];Star>=g.Stars;Star--)
	{
		DrawStar(Star,BLACK);
		Star->Loc.x+=Star->Dir.x*g.Speed/30;
		Star->Loc.y+=Star->Dir.y*g.Speed/30;
		if (Star->Loc.x<1||Star->Loc.x>=g.Screen.x
			||Star->Loc.y<1||Star->Loc.y>=g.Screen.y)
		{
			NewStar(Star);
		}
		else
		{
			DrawStar(Star,Star->Color);
		}
	}
}
/********************************************************************/
void main(void)
{
	int gDriver=DETECT,gMode,GErr;
	puts("\nStarfield simulation.\n");
	puts("Keys:");
	puts("+ Faster");
	puts("- Slower");
	puts("<ESC>Exits\n");
	puts("Press any key to begin...");
	getch();
	initgraph(&gDriver,&gMode,"");
	srand(0);
	g.Screen.x=getmaxx()*SCALE;
	g.Screen.y=getmaxy()*SCALE;
	g.Speed=20;
	if((GErr=graphresult())!=grOk)
	{
		printf("Graphics error: %s\n",grapherrormsg(GErr));
	}
	else
	{
		while(1)
		{
			while(!kbhit())
			{
				if(g.NumStars<MAX_STARS)
				{
					NewStar(&g.Stars[g.NumStars]);
					g.NumStars++;
				}
				MoveStars();
			}
			switch(toupper(getch()))
			{
				case '+':if(g.Speed<300) g.Speed++;break;
				case '-':if(g.Speed>0) g.Speed--;break;
				case 27:closegraph();exit(0);
			}
		}
	}
}
