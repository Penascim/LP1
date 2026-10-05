#include <stdio.h>

int main(void) {
    int n = 1;

    while (n % 3 != 2 || n % 5 != 3 || n % 7 != 4)
        n++;

    printf("O menor inteiro positivo e %d\n", n);
    printf("%d / 3 = %d e resto %d\n", n, n / 3, n % 3);
    printf("%d / 5 = %d e resto %d\n", n, n / 5, n % 5);
    printf("%d / 7 = %d e resto %d\n", n, n / 7, n % 7);
    return 0;
}
