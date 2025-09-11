#include <stdio.h>
#include <string.h>

int     tamanho(char *str)
{
        int     i;

        i = 0;
        while(str[i] != '\0')
                i++;
        return (i);
}

void concatena(char* d, char* o1, char* o2)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (i < tamanho(o1))
    {
        d[i] = o1[i];
        i++;
    }
    while (j < tamanho(o2))
    {
        d[i] = o2[j];
        i++;
        j++;
    }
    d[i] = '\0';
}

int main(void)
{
    char str1[] = "TESTE";
    char str2[] = "aaaaaa12312345312";
    char destino[10000];
    concatena(destino, str1, str2);
    printf("d: %s\n", destino);
    return 0;
}
