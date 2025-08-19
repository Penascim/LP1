#include <stdio.h>

int main(void)
{
        float velocidade;
        printf("insira a velocidade atual:\n");
        scanf("%e", &velocidade);
        if (velocidade >= 50 && velocidade <= 62.5)
                printf("MANTENHA");
        if (velocidade < 50)
                printf("ACELERE");
        if (velocidade > 62.5)
                printf("DESACELERE");
}
