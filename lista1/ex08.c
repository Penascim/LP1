#include <stdio.h>

int main(void)
{
  int n1;
  int n2;

  printf("digite o primeiro número: \n");
  scanf("%d", &n1);
 
  printf("digite o segundo número: \n");
  scanf("%d", &n2);
  if (n1 > n2)
  { 
    while (n1 >= n2)
      printf("%d\n", n2++);
  }
  else if (n1 < n2)
  {
      while (n1 <= n2)
        printf("%d\n", n1++);
  }
  else
    return 0;
}
