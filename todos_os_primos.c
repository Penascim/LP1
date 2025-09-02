#include <stdio.h>

int is_prime(int n)
{
        int div;

        div = n - 1;
        while (div > 1)
        {
                if (n % div == 0)
                        return(0);
                div--;
        }
        return(1);
}

int main(void)
{
        int max;

        printf("digite um número: \n");
        scanf("%d", &max);
        printf("\nprimos: \n");
        if (max < 0)
        {
                printf("erro");
                return 0;
        }
        while (max > 1)
        {
                if (is_prime(max) == 1)
                        printf("%d\n", max);
                max--;
        }
}
