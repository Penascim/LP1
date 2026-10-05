#include <stdio.h>

int main(void) {
    int notas[6] = {200, 100, 50, 20, 10, 5};
    int estoque[6] = {100, 100, 100, 100, 100, 100};
    int usar[6];
    int valor, resto, totalNotas, i;

    while (1) {
        printf("\nValor do saque (0 para sair): ");
        scanf("%d", &valor);
        if (valor == 0)
            break;

        resto = valor;
        for (i = 0; i < 6; i++) {
            usar[i] = resto / notas[i];
            if (usar[i] > estoque[i])
                usar[i] = estoque[i];
            resto -= usar[i] * notas[i];
        }

        if (valor < 0 || resto != 0) {
            printf("Nao e possivel sacar esse valor. Operacao cancelada.\n");
            continue;
        }

        totalNotas = 0;
        for (i = 0; i < 6; i++) {
            if (usar[i] > 0) {
                printf("%d nota(s) de %d\n", usar[i], notas[i]);
                estoque[i] -= usar[i];
                totalNotas += usar[i];
            }
        }
        printf("Total: %d nota(s)\n", totalNotas);
    }
    return 0;
}
