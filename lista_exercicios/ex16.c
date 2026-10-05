#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    int rodada, a1, a2, b1, b2, vencedor;
    int vitorias1 = 0, vitorias2 = 0;

    srand(time(NULL));

    for (rodada = 1; rodada <= 11; rodada++) {
        a1 = rand() % 6 + 1;
        a2 = rand() % 6 + 1;
        b1 = rand() % 6 + 1;
        b2 = rand() % 6 + 1;

        if (a1 == a2 && b1 != b2)
            vencedor = 1;
        else if (b1 == b2 && a1 != a2)
            vencedor = 2;
        else if (a1 + a2 > b1 + b2)
            vencedor = 1;
        else if (b1 + b2 > a1 + a2)
            vencedor = 2;
        else
            vencedor = 0;

        printf("Rodada %2d: Jogador 1 (%d e %d) x Jogador 2 (%d e %d) -> ", rodada, a1, a2, b1, b2);
        if (vencedor == 1) {
            printf("Jogador 1\n");
            vitorias1++;
        } else if (vencedor == 2) {
            printf("Jogador 2\n");
            vitorias2++;
        } else {
            printf("Empate\n");
        }
    }

    printf("\nPlacar: Jogador 1 = %d, Jogador 2 = %d\n", vitorias1, vitorias2);
    if (vitorias1 > vitorias2)
        printf("Vencedor: Jogador 1\n");
    else if (vitorias2 > vitorias1)
        printf("Vencedor: Jogador 2\n");
    else
        printf("Houve empate\n");
    return 0;
}
