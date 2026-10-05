#include <stdio.h>

int main(void) {
    int m[5][5], i, j;
    int linha4 = 0, coluna2 = 0, principal = 0, secundaria = 0, total = 0;

    printf("Digite os 25 elementos da matriz 5x5:\n");
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            scanf("%d", &m[i][j]);
            total += m[i][j];
            if (i == 3)
                linha4 += m[i][j];
            if (j == 1)
                coluna2 += m[i][j];
            if (i == j)
                principal += m[i][j];
            if (i + j == 4)
                secundaria += m[i][j];
        }
    }

    printf("\nMatriz:\n");
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++)
            printf("%5d", m[i][j]);
        printf("\n");
    }

    printf("\na) Soma da linha 4: %d\n", linha4);
    printf("b) Soma da coluna 2: %d\n", coluna2);
    printf("c) Soma da diagonal principal: %d\n", principal);
    printf("d) Soma da diagonal secundaria: %d\n", secundaria);
    printf("e) Soma de todos os elementos: %d\n", total);
    return 0;
}
