#include <stdio.h>
#include <string.h>

void concatena(char* d, char* o1, char* o2)
{
    int i = 0;
    int j = 0;

    // Copia o1 para d
    while (o1[i] != '\0') {
        d[i] = o1[i];
        i++;
    }

    // Copia o2 para d, após o1
    while (o2[j] != '\0') {
        d[i] = o2[j];
        i++;
        j++;
    }

    d[i] = '\0'; // Finaliza string
}

int main(void)
{
    char str1[] = "Olá, ";
    char str2[] = "mundo!";
    char destino[100];  // Deve ser grande o suficiente para conter o1 + o2

    concatena(destino, str1, str2);

    printf("Resultado: %s\n", destino);

    return 0;
}
