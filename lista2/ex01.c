#include <stdio.h>

int main(void)
{
        int a, b;
        printf("valor de a: \n");
        scanf("%d", &a);
        printf("valor de b:\n");
        scanf("%d", &b);

        int *pointer;

        if(a > b)
                pointer = (int *)&a;
        if(b > a)
                pointer = (int *)&b;
        else
                printf("são iguais");
        *pointer = *pointer - 50;
        printf("valor de a: \n%d\n valor de b: \n%d\n", a, b);
        printf("valor do ponteiro: %p", &pointer);
}
