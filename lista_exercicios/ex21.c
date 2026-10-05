#include <stdio.h>

int main(void) {
    int valores[13] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    char *simbolos[13] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int n, i;

    printf("Digite um numero de 1 a 3999: ");
    scanf("%d", &n);
    if (n < 1 || n > 3999) {
        printf("Numero fora do limite\n");
        return 1;
    }

    printf("%d = ", n);
    for (i = 0; i < 13; i++) {
        while (n >= valores[i]) {
            printf("%s", simbolos[i]);
            n -= valores[i];
        }
    }
    printf("\n");
    return 0;
}
