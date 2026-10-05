#include <stdio.h>

int main(void) {
    int n, i, triangular = 0;

    printf("Digite um numero: ");
    scanf("%d", &n);

    for (i = 1; i * (i + 1) * (i + 2) <= n; i++) {
        if (i * (i + 1) * (i + 2) == n) {
            triangular = 1;
            printf("%d = %d * %d * %d\n", n, i, i + 1, i + 2);
        }
    }

    if (triangular)
        printf("%d e um numero triangular\n", n);
    else
        printf("%d nao e um numero triangular\n", n);
    return 0;
}
