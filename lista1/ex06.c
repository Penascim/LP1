#include <stdio.h>

int main(void)
{
    int i;
    int N;
    int soma;
    
    soma = 0;
    scanf("%d", &i);
    N = i;
    while (N >= 1)
    {
        soma = soma + N * N;
        N--;
    }
    printf("%d", soma);
}
