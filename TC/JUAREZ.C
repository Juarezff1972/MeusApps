#include <stdio.h>
long double x;
int a;
void main()
	{ printf("Entre com o numero");
	  scanf("%d",&a);
	  printf("\n");
	 x = 1;
		do { printf("%79.0Lf\n", x);
		x = x * a;
	}
	while (x != 0);
	}
