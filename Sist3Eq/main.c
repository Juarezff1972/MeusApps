#include <stdio.h>
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////
const int n = 3;
const int true = 1;
const int false = 0;
float a[4][4];
unsigned char x, y;
unsigned vai, rsp;
/////////////////////////////////////////////////////////////////////////
void LerMatriz (unsigned char x, unsigned char y)
{
	char *c, *l;
	l = "coeficiente tal";
	c = "tal";
	switch (x)
	{
		case 0 :
			l = "PRIMEIRO coeficiente";
			break;
		case 1 :
			l = "SEGUNDO coeficiente";
			break;
		case 2 :
			l = "TERCEIRO coeficiente";
			break;
		case 3 :
			l = "TERMO INDEPENDENTE";
			break;
	}
	switch (y)
	{
		case 0 :
			c = "PRIMEIRA";
			break;
		case 1 :
			c = "SEGUNDA";
			break;
		case 2 :
			c = "TERCEIRA";
			break;
	}
	printf ("Entre com o %s da %s linha:", l, c);
	scanf ("%f", &a[y][x]);
	printf ("\n");
}
/////////////////////////////////////////////////////////////////////////
void MostraMatriz()
{
	unsigned i, j;
	printf("\n\n");
	for(i = 0; i < n; i++)
	{
		printf("| ");
		for(j = 0; j <= n; j++)
		{
			printf("%.32f\t\t", a[i][j]);
		}
		printf("|\n");
	}
}
/////////////////////////////////////////////////////////////////////////
void Det (void)
{
	float x, z;
	unsigned char t, i, s, g;
	char *c;
	z = 1;
	//printf("\nEtapa 1");
	//MostraMatriz();
	for (t = 0; t < n; t++)
	{
		if (a[t][0] == 0 && a[t][1] == 0 && a[t][2] == 0 && t != n)
		{
			i = 0;
			do
			{
				i++;
				while (a[2][0] != 0 && a[2][1] != 0 && a[2][2] != 0)
				{
					for (s = 0; s < n + 1; s++)
					{
						x = a[t][s];
						a[t][s] = a[4 - i][s];
						a[4 - i][s] = x;
					}
				}
			}
			while (a[2][0] == 0 && a[2][1] == 0 && a[2][2] == 0);
		}
	}
	//printf("\nEtapa 2");
	//MostraMatriz();
	rsp = true;
	for (t = 0; t < n; t++)
	{
		vai = false;
		for (i = 0; i < n; i++)
		{
			if (a[t][i] != 0)
			{
				if (!vai)
				{
					g = i;
				}
				vai = true;
			}
		}
		//printf("\nEtapa 3");
		//MostraMatriz();
		if (vai)
		{
			//while (a[t][t] == 0 && a[t][t - 1] != 0)
			{
				for (s = 1; s <= n + 1; s++)
				{
					x = a[t][s];
					a[t][s] = a[g][s];
					a[g][s] = x;
				}
			}
			//printf("\nEtapa 4");
			//MostraMatriz();
			while ( (a[t][t] == 0) && (a[t][t - 1] != 0))
				if (a[t][t] == 0)
				{
					z = 1 / a[t][t + 1];
				}
				else
				{
					z = 1 / a[t][t];
				}
			//printf("z = %.64f \n", z);
			for (s = 0; s < n + 1; s++)
			{
				a[t][s] = z * a[t][s];
			}
			//printf("\nEtapa 5");
			//MostraMatriz();
			for (i = 0; i < n; i++)
			{
				if (i != t)
				{
					z = -a[i][t];
					//printf(">>>a = %.32f || z = %.32f \n", a[i][t], z);
					for (s = 0; s < n + 1; s++)
					{
						if (z != 0.0)
						{
							a[i][s] = a[i][s] + z * a[t][s];
						}
					}
				}
			}
			//printf("\nEtapa 6");
			//MostraMatriz();
		}
		if (!vai)
		{
			printf ("\n");
			c = "O sistema ‚ poss¡vel e indeterminado.";
			for (i = 0; i < 3; i++)
				if ( (a[i][0] == 0) && (a[i][1] == 0) && (a[i][2] == 0) && (a[i][3] != 0))
				{
					c = "O sistema ‚ imposs¡vel.";
				}
			if ( (a[2][0] == 0) && (a[2][1] == 0) && (a[2][2] == 0) && (a[2][3] == 0))
			{
				for (i = 0; i < 2; i++)
				{
					if ( (a[i][0] == 1.0) && (a[i][1] == 0) && (a[i][2] == 0))
					{
						printf ("X[1] = %.5f", a[i][4]);
					}
					if ( (a[i][0] == 0) && (a[i][1] == 1.0) && (a[i][2] == 0))
					{
						printf ("X[2] = %.5f", a[i][4]);
					}
					if ( (a[i][0] == 0) && (a[i][1] == 0) && (a[i][2] == 1.0))
					{
						printf ("X[3] = %.5f", a[i][4]);
					}
				}
				c = " ";
			}
			printf ("%s\n", c);
			rsp = false;
		}
	}
}
/////////////////////////////////////////////////////////////////////////
void resposta (void)
{
	unsigned char i;
	printf ("\n");
	for (i = 0; i < n; i++)
	{
		printf ("X[%d] = %g\n", i, a[i][n]);
	}
}
/////////////////////////////////////////////////////////////////////////
int main()
{
	rsp = true;
	printf ("\n");
	for (x = 0; x < n; x++)
	{
		for (y = 0; y < n + 1; y++)
		{
			LerMatriz (y, x);
		}
		printf ("\n");
	}
	if (rsp)
	{
		Det();
	}
	if (rsp)
	{
		resposta();
	}
	return 0;
}
