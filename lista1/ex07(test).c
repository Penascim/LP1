#include <stdio.h>

typedef struct s_time
{
        char            *time;
        int             vitoria;
        int             derrota;
        int             empate;
        struct t_time   *next
} t_time

int main(void)
{
        int i;
        t_time time;

        i = 20;
        while (i > 0)
        {

                printf("digite o nome do time:\n");
                scanf("%s\n", &t_time->time);
                printf("digite quantas vitorias o time possui:\n");
                scanf("%d\n", &t_time->vitoria);
                printf("digite quantas derrotas o time possui:\n");
                scanf("%d\n", &t_time->derrota);
                printf("digite quantos empates o time possui:\n");
                scanf("%d\n", &t_time->empate);
                i--;

        }
}
