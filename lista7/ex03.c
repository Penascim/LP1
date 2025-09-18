#include <stdio.h>

typedef struct  Personagem {
    int                 forca;
    int                 energia;
    int                 experiencia;
} t_personagem;

void            preencheB(t_personagem *p)
{
        printf("digite a força do personagem: \n");
        scanf("%d", &p->forca);
        printf("digite a energia do personagem: \n");
        scanf("%d", &p->energia);
        printf("digite a experiencia do personagem: \n");
        scanf("%d", &p->experiencia);
}

void    main (void)
{
        t_personagem p1;
        preencheB(&p1);
        t_personagem p2;
        preencheB(&p2);
        printf("(%d,%d,%d) vs (%d,%d,%d)\n",
                        p1.forca, p1.energia, p1.experiencia,
                        p2.forca, p2.energia, p2.experiencia);
}
