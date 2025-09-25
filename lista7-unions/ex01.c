#include <stdio.h>

union   Ponto
{
        int     vet[3];
        struct
        {
                int     x;
                int     y;
                int     z;
        };
};

int     main(void)
{
        union Ponto p;

        p.vet[0] = 10;
        p.vet[1] = 20;
        p.vet[2] = 30;
        printf("vet: %d, %d, %d\n", p.vet[0], p.vet[1], p.vet[2]);
        printf("struct: %d, %d, %d\n", p.x, p.y, p.z);
}
