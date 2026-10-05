#include <stdio.h>

int main(void) {
    int n, i, soma = 0;

    printf("Digite um numero de 1 a 32767: ");
    scanf("%d", &n);
    if (n < 1 || n > 32767) {
        printf("Numero fora do intervalo\n");
        return 1;
    }

    for (i = 1; i < n; i++) {
        if (n % i == 0)
            soma += i;
    }

    if (soma == n)
        printf("%d e um inteiro perfeito\n", n);
    else
        printf("%d nao e um inteiro perfeito\n", n);
    return 0;
}
