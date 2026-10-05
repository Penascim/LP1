#include <stdio.h>

int main(void) {
    int n, i, primo = 1;

    printf("Digite um inteiro positivo: ");
    scanf("%d", &n);

    if (n < 2)
        primo = 0;
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            primo = 0;
            break;
        }
    }

    if (primo)
        printf("%d e primo\n", n);
    else
        printf("%d nao e primo\n", n);
    return 0;
}
