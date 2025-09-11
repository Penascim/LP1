#include <stdio.h>

void troca(int *x, int *y)
{
        int aux;

        aux = *x;
        *x = *y;
        *y = aux;
}

int main(void)
{
        int x;
        int y;

        x = 10;
        y = 20;
        troca(&x, &y);
        printf("%d %d\n", x, y);
}
