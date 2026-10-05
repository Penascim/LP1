#include <stdio.h>
#include <string.h>

int main(void) {
    char texto[82];
    int i, j, repetido, distintos = 0;

    printf("Digite um texto (ate 80 caracteres): ");
    fgets(texto, 82, stdin);
    texto[strcspn(texto, "\n")] = '\0';

    for (i = 0; texto[i] != '\0'; i++) {
        repetido = 0;
        for (j = 0; j < i; j++) {
            if (texto[j] == texto[i])
                repetido = 1;
        }
        if (!repetido)
            distintos++;
    }

    printf("Caracteres distintos: %d\n", distintos);
    return 0;
}
