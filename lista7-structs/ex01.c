#include <stdio.h>

typedef struct Personagem {
    int forca;
    int energia;
    int experiencia;
} t_personagem;

int     main(void)
{
        t_personagem p1;

        p1.forca        = 10;
        p1.energia      = 50;
        p1.experiencia  = 30;

        t_personagem p2 = { 18, 20, 25 };
}
