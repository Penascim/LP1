#include <stdio.h>

typedef struct  cartographic_plan
{
        int     x;
        int     y;
        int     z;
} c_dot;

int     main(void)
{
        c_dot v1 = { 1, 0, 5 };
        c_dot v2 = { 3, 3, 3 };
        c_dot v3 = { 0, 10, 0 };

        printf("coordenada y dos 3 pontos: %d, %d e %d\n\n", v1.y, v2.y, v3.y);
        v1.z += 10;
        v2.z += 10;
        v3.z += 10;
        printf("coordenada do ponto v1: (%d, %d, %d)\n", v1.x, v1.y, v1.z);
        printf("coordenada do ponto v2: (%d, %d, %d)\n", v2.x, v2.y, v2.z);
        printf("coordenada do ponto v3: (%d, %d, %d)\n", v3.x, v3.y, v3.z);
}
