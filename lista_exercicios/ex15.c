#include <stdio.h>

int main(void) {
    int anoAtual, ano, procedencia, idade;
    int total = 0, menos21 = 0, mulheresCapital = 0, interiorMais60 = 0, mulherMais60 = 0;
    char sexo;

    printf("Ano atual: ");
    scanf("%d", &anoAtual);

    while (1) {
        printf("\nAno de nascimento (0 para encerrar): ");
        scanf("%d", &ano);
        if (ano == 0)
            break;
        printf("Sexo (M/F): ");
        scanf(" %c", &sexo);
        printf("Procedencia (0 - Capital, 1 - Interior, 2 - Outro estado): ");
        scanf("%d", &procedencia);

        idade = anoAtual - ano;
        total++;
        if (idade < 21)
            menos21++;
        if ((sexo == 'F' || sexo == 'f') && procedencia == 0)
            mulheresCapital++;
        if (procedencia == 1 && idade > 60)
            interiorMais60++;
        if ((sexo == 'F' || sexo == 'f') && idade > 60)
            mulherMais60 = 1;
    }

    if (total == 0) {
        printf("Nenhum motorista informado\n");
        return 0;
    }

    printf("\na) Motoristas com menos de 21 anos: %.1f%%\n", 100.0 * menos21 / total);
    printf("b) Mulheres da capital: %d\n", mulheresCapital);
    printf("c) Motoristas do interior com mais de 60 anos: %d\n", interiorMais60);
    if (mulherMais60)
        printf("d) Existe mulher com mais de 60 anos: sim\n");
    else
        printf("d) Existe mulher com mais de 60 anos: nao\n");
    return 0;
}
