#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void puxarCarta(int *soma, int *ases) {
    char *nomes[14] = {"", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    int carta = rand() % 13 + 1;

    printf(" %s", nomes[carta]);
    if (carta == 1) {
        *soma += 11;
        (*ases)++;
    } else if (carta > 10) {
        *soma += 10;
    } else {
        *soma += carta;
    }

    while (*soma > 21 && *ases > 0) {
        *soma -= 10;
        (*ases)--;
    }
}

int main(void) {
    int cliente = 0, asesCliente = 0, banca = 0, asesBanca = 0;
    char resposta;

    srand(time(NULL));

    printf("Suas cartas:");
    puxarCarta(&cliente, &asesCliente);
    puxarCarta(&cliente, &asesCliente);
    printf("  (soma %d)\n", cliente);

    while (cliente < 21) {
        printf("Deseja outra carta? (S/N): ");
        scanf(" %c", &resposta);
        if (resposta != 'S' && resposta != 's')
            break;
        printf("Carta:");
        puxarCarta(&cliente, &asesCliente);
        printf("  (soma %d)\n", cliente);
    }

    if (cliente > 21) {
        printf("Voce passou de 21. A banca ganhou!\n");
        return 0;
    }

    printf("Cartas da banca:");
    while (banca < cliente)
        puxarCarta(&banca, &asesBanca);
    printf("  (soma %d)\n", banca);

    if (banca > 21)
        printf("A banca passou de 21. Voce ganhou!\n");
    else if (banca == cliente)
        printf("Empate!\n");
    else
        printf("A banca ganhou!\n");
    return 0;
}
