#include <stdio.h>

int main(void) {
    int n, i;
    unsigned long long fatorial = 1;

    printf("Digite um inteiro positivo (ate 20): ");
    scanf("%d", &n);
    if (n < 0 || n > 20) {
        printf("Valor invalido\n");
        return 1;
    }

    for (i = 2; i <= n; i++)
        fatorial *= i;

    printf("%d! = %llu\n", n, fatorial);
    return 0;
}
