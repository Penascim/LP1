#include <stdio.h>

void ordenar(int v[], int n) {
    int i, j, aux;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (v[j] > v[j + 1]) {
                aux = v[j];
                v[j] = v[j + 1];
                v[j + 1] = aux;
            }
        }
    }
}

int main(void) {
    int v[3][10], i, j;

    for (i = 0; i < 3; i++) {
        printf("Digite os 10 numeros do vetor %d:\n", i + 1);
        for (j = 0; j < 10; j++)
            scanf("%d", &v[i][j]);
    }

    for (i = 0; i < 3; i++) {
        ordenar(v[i], 10);
        printf("Vetor %d em ordem crescente:", i + 1);
        for (j = 0; j < 10; j++)
            printf(" %d", v[i][j]);
        printf("\n");
    }
    return 0;
}
