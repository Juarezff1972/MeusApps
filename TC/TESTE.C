#include <stdio.h>

main()
{ int a,b,sum;
  printf("Entre com os numeros ");
  scanf("%d %d", &a, &b);
  printf("%d and %d  resulta %d \n",a,b, a && b);
  printf("%d or  %d  resulta %d \n",a,b, a || b);
  printf("not    %d  resulta %d \n",a, ! a);

}