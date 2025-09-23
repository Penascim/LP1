#include <stdio.h>

typedef struct  s_list
{
        char    a;
        char    b;
        char    c;
        char    d;
}               t_list;

void            preenche(t_list *lista)
{
        printf("digite um caractere: ");
        scanf("%s", &lista->a);
        printf("digite outro caractere: ");
        scanf("%s", &lista->b);
        printf("digite outro caractere: ");
        scanf("%s", &lista->c);
        printf("digite outro caractere: ");
        scanf("%s", &lista->d);
}

int             main(void)
{
        t_list teste;
        preenche(&teste);
        printf("%s \n", &teste.a);
}
