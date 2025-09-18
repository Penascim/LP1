#include <stdio.h>

typedef struct  Personagem {
    int                 forca;
    int                 energia;
    int                 experiencia;
} t_personagem;

t_personagem    preencheA(void)
{
        t_personagem    p;
        printf("digite a força do personagem: \n");
        scanf("%d", &p.forca);
        printf("digite a energia do personagem: \n");
        scanf("%d", &p.energia);
        printf("digite a experiencia do personagem: \n");
        scanf("%d", &p.experiencia);
        return          p;
}

void    main (void)
{
        t_personagem p1 = preencheA();
        t_personagem p2 = preencheA();
        printf("(%d,%d,%d) vs (%d,%d,%d)\n",
                        p1.forca, p1.energia, p1.experiencia,
                        p2.forca, p2.energia, p2.experiencia);
}
