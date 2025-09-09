um#include <stdio.h>

int main(void)
{
  int a;
  int b;
  int *p;

  scanf("%d", &a);
  scanf("%d", &b);

  
  if (a > b)
    p = &a;
  else if (a < b)
    p = &b;
  else
    return (0);
  *p = *p - 50;
  printf("%d", a);
  printf("%d", b);
  printf("%d", p);
  
}
