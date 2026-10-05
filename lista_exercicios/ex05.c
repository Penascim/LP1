#include <stdio.h>

int main(void) {
    int moedas[6] = {100, 50, 25, 10, 5, 1};
    int valor, i, qtd;

    printf("Digite o valor em centavos (1 a 100): ");
    scanf("%d", &valor);
    if (valor < 1 || valor > 100) {
        printf("Valor invalido\n");
        return 1;
    }

    printf("Troco de R$ %d,%02d:\n", valor / 100, valor % 100);
    for (i = 0; i < 6; i++) {
        qtd = valor / moedas[i];
        valor = valor % moedas[i];
        if (qtd > 0)
            printf("%d moeda(s) de R$ %d,%02d\n", qtd, moedas[i] / 100, moedas[i] % 100);
    }
    return 0;
}
