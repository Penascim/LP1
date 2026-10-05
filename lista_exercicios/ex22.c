#include <stdio.h>
#include <string.h>

int main(void) {
    char texto[82];
    int i, total, minusculas = 0, maiusculas = 0;

    printf("Digite um texto (ate 80 caracteres): ");
    fgets(texto, 82, stdin);
    texto[strcspn(texto, "\n")] = '\0';
    total = strlen(texto);

    for (i = 0; i < total; i++) {
        if (strchr("aeiou", texto[i]) != NULL)
            minusculas++;
        else if (strchr("AEIOU", texto[i]) != NULL)
            maiusculas++;
    }

    printf("Vogais minusculas: %d\n", minusculas);
    printf("Vogais maiusculas: %d\n", maiusculas);
    printf("Total de vogais: %d\n", minusculas + maiusculas);
    if (total > 0)
        printf("Percentual de vogais: %.2f%%\n", 100.0 * (minusculas + maiusculas) / total);
    return 0;
}
