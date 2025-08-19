#include <stdio.h>
int main(void)
{
        int valor1;
        int valor2;
        printf("insira o primeiro valor:\n");
        scanf("%d", &valor1);
        printf("insira o segundo valor:\n");
        scanf("%d", &valor2);
        
        if (valor1 < valor2)
                printf("maior valor: %d\n", valor2);
        else if (valor1 > valor2)
                printf("maior valor: %d\n", valor1);
        else if (valor1 == valor2)
                printf("valores iguais\n");
}
