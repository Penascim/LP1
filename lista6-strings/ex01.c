#include <stdio.h>

int     tamanho(char *str)
{
        int     i;

        i = 0;
        while(str[i] != '\0')
                i++;
        return (i);
}

int     main(int ac, char **av)
{
        int     i;

        i = 1;
        if (ac > 1)
        {
                while (i < ac)
                        printf("%d\n", tamanho(av[i++]));
        }
}
