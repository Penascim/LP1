#include <stdio.h>

int main(void) {
    int v, e, d;
    int time1, time2, time3, time4, time5;
    int time6, time7, time8, time9, time10;
    int time11, time12, time13, time14, time15;
    int time16, time17, time18, time19, time20;

    printf("Digite vitorias, empates e derrotas do time 1: ");
    scanf("%d %d %d", &v, &e, &d);
    time1 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 2: ");
    scanf("%d %d %d", &v, &e, &d);
    time2 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 3: ");
    scanf("%d %d %d", &v, &e, &d);
    time3 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 4: ");
    scanf("%d %d %d", &v, &e, &d);
    time4 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 5: ");
    scanf("%d %d %d", &v, &e, &d);
    time5 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 6: ");
    scanf("%d %d %d", &v, &e, &d);
    time6 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 7: ");
    scanf("%d %d %d", &v, &e, &d);
    time7 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 8: ");
    scanf("%d %d %d", &v, &e, &d);
    time8 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 9: ");
    scanf("%d %d %d", &v, &e, &d);
    time9 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 10: ");
    scanf("%d %d %d", &v, &e, &d);
    time10 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 11: ");
    scanf("%d %d %d", &v, &e, &d);
    time11 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 12: ");
    scanf("%d %d %d", &v, &e, &d);
    time12 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 13: ");
    scanf("%d %d %d", &v, &e, &d);
    time13 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 14: ");
    scanf("%d %d %d", &v, &e, &d);
    time14 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 15: ");
    scanf("%d %d %d", &v, &e, &d);
    time15 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 16: ");
    scanf("%d %d %d", &v, &e, &d);
    time16 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 17: ");
    scanf("%d %d %d", &v, &e, &d);
    time17 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 18: ");
    scanf("%d %d %d", &v, &e, &d);
    time18 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 19: ");
    scanf("%d %d %d", &v, &e, &d);
    time19 = v * 3 + e;

    printf("Digite vitorias, empates e derrotas do time 20: ");
    scanf("%d %d %d", &v, &e, &d);
    time20 = v * 3 + e;

    int times[20] = {
        time1, time2, time3, time4, time5,
        time6, time7, time8, time9, time10,
        time11, time12, time13, time14, time15,
        time16, time17, time18, time19, time20
    };

       int i = 1;
    int maior = times[0], menor = times[0];
    int time_maior = 1, time_menor = 1;

    while (i < 20) {
        if (times[i] > maior) {
            maior = times[i];
            time_maior = i + 1;
        }
        if (times[i] < menor) {
            menor = times[i];
            time_menor = i + 1;
        }
        i++;
    }
    }

    printf("\nTime com mais pontos: Time %d (%d pontos)\n", time_maior, maior);
    printf("Time com menos pontos: Time %d (%d pontos)\n", time_menor, menor);

    return 0;
}
