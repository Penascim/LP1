#include <stdio.h>

int bissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0;
}

long contarDias(int dia, int mes, int ano) {
    int diasMes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    long total = dia;
    int i;

    for (i = 1; i < ano; i++) {
        if (bissexto(i))
            total += 366;
        else
            total += 365;
    }
    for (i = 1; i < mes; i++) {
        total += diasMes[i - 1];
        if (i == 2 && bissexto(ano))
            total++;
    }
    return total;
}

int main(void) {
    int D1, M1, A1, D2, M2, A2;
    long diferenca;

    printf("Primeira data (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &D1, &M1, &A1);
    printf("Segunda data (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &D2, &M2, &A2);

    diferenca = contarDias(D2, M2, A2) - contarDias(D1, M1, A1);
    if (diferenca < 0)
        diferenca = -diferenca;

    printf("Quantidade de dias entre as datas: %ld\n", diferenca);
    return 0;
}
