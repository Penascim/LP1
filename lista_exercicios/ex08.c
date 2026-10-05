#include <stdio.h>

#define TAM 50

int main(void) {
    int a[TAM], i, maior, segundo;

    printf("Digite os %d numeros inteiros (todos diferentes):\n", TAM);
    for (i = 0; i < TAM; i++)
        scanf("%d", &a[i]);

    if (a[0] > a[1]) {
        maior = a[0];
        segundo = a[1];
    } else {
        maior = a[1];
        segundo = a[0];
    }

    for (i = 2; i < TAM; i++) {
        if (a[i] > maior) {
            segundo = maior;
            maior = a[i];
        } else if (a[i] > segundo) {
            segundo = a[i];
        }
    }

    printf("Maior elemento: %d\n", maior);
    printf("Segundo maior elemento: %d\n", segundo);
    return 0;
}
