#include <stdio.h>

int main(void)
{
        float valor;
        float cotagem_dolar;
        float budget;

        budget = 1000;
        cotagem_dolar = 3.17;
        printf("digite o valor do telefone em dólares:\n");
        scanf("%e", &valor);
        if ((valor * cotagem_dolar) <= budget)
                printf("BOM NEGÓCIO\n");
        else if (valor < 0)
                printf("valor inválido\n");
        else
                printf("NÃO É UM BOM NEGÓCIO\n");
}
