#include <stdio.h>

int ehPrimo(unsigned long long n) {
    unsigned long long i;

    if (n < 2)
        return 0;
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int main(void) {
    unsigned long long a = 1, b = 1, proximo;
    int n, encontrados = 0;

    printf("Quantos numeros deseja (1 a 12)? ");
    scanf("%d", &n);
    if (n < 1 || n > 12) {
        printf("Valor invalido\n");
        return 1;
    }

    while (encontrados < n) {
        proximo = a + b;
        a = b;
        b = proximo;
        if (ehPrimo(b)) {
            printf("%llu\n", b);
            encontrados++;
        }
    }
    return 0;
}
