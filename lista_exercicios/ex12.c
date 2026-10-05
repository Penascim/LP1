#include <stdio.h>

int main(void) {
    char *unidades[10] = {"zero", "um", "dois", "tres", "quatro", "cinco", "seis", "sete", "oito", "nove"};
    char *dez[10] = {"dez", "onze", "doze", "treze", "quatorze", "quinze", "dezesseis", "dezessete",
                     "dezoito", "dezenove"};
    char *dezenas[10] = {"", "", "vinte", "trinta", "quarenta", "cinquenta", "sessenta", "setenta",
                         "oitenta", "noventa"};
    char *centenas[10] = {"", "cento", "duzentos", "trezentos", "quatrocentos", "quinhentos", "seiscentos",
                          "setecentos", "oitocentos", "novecentos"};
    int n, milhar, centena, dezena, unidade, resto;

    printf("Digite um numero de 0 a 9999: ");
    scanf("%d", &n);
    if (n < 0 || n > 9999) {
        printf("Numero fora do limite\n");
        return 1;
    }

    milhar = n / 1000;
    centena = n % 1000 / 100;
    dezena = n % 100 / 10;
    unidade = n % 10;
    resto = n % 1000;

    printf("%d = ", n);
    if (n == 0)
        printf("zero");

    if (milhar > 0) {
        if (milhar > 1)
            printf("%s ", unidades[milhar]);
        printf("mil");
        if (resto > 0 && (resto < 100 || resto % 100 == 0))
            printf(" e ");
        else if (resto > 0)
            printf(" ");
    }

    if (centena > 0) {
        if (resto == 100)
            printf("cem");
        else
            printf("%s", centenas[centena]);
        if (n % 100 > 0)
            printf(" e ");
    }

    if (dezena == 1) {
        printf("%s", dez[unidade]);
    } else {
        if (dezena > 1) {
            printf("%s", dezenas[dezena]);
            if (unidade > 0)
                printf(" e ");
        }
        if (unidade > 0)
            printf("%s", unidades[unidade]);
    }
    printf("\n");
    return 0;
}
