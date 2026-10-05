#include <stdio.h>

int main(void) {
    int a[10][10], b[10][10], linhas, colunas, i, j;

    printf("Numero de linhas e de colunas (ate 10): ");
    scanf("%d %d", &linhas, &colunas);
    if (linhas < 1 || linhas > 10 || colunas < 1 || colunas > 10) {
        printf("Tamanho invalido\n");
        return 1;
    }

    printf("Digite os elementos da matriz A:\n");
    for (i = 0; i < linhas; i++)
        for (j = 0; j < colunas; j++)
            scanf("%d", &a[i][j]);

    printf("Digite os elementos da matriz B:\n");
    for (i = 0; i < linhas; i++)
        for (j = 0; j < colunas; j++)
            scanf("%d", &b[i][j]);

    printf("\nA + B:\n");
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++)
            printf("%6d", a[i][j] + b[i][j]);
        printf("\n");
    }
    return 0;
}
