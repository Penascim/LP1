#include <stdio.h>

int main(void) {
    int idade, qtd = 0, entre21e65 = 0;
    float soma = 0;

    printf("Idade (-1 para encerrar): ");
    scanf("%d", &idade);
    while (idade != -1) {
        qtd++;
        soma += idade;
        if (idade >= 21 && idade <= 65)
            entre21e65++;
        printf("Idade (-1 para encerrar): ");
        scanf("%d", &idade);
    }

    if (qtd == 0) {
        printf("Nenhuma idade informada\n");
        return 0;
    }

    printf("Idade media: %.2f\n", soma / qtd);
    printf("Pessoas entre 21 e 65 anos: %.2f%%\n", 100.0 * entre21e65 / qtd);
    return 0;
}
