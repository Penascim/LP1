#include <stdio.h>
#include <string.h>

int main(void) {
    char texto[82];
    char *p;
    int i, vogais;

    printf("Digite um texto (ate 80 caracteres): ");
    fgets(texto, 82, stdin);
    texto[strcspn(texto, "\n")] = '\0';

    printf("Palavras com tres ou mais vogais:\n");
    p = strtok(texto, " ,.;:!?");
    while (p != NULL) {
        vogais = 0;
        for (i = 0; p[i] != '\0'; i++) {
            if (strchr("aeiouAEIOU", p[i]) != NULL)
                vogais++;
        }
        if (vogais >= 3)
            printf("%s\n", p);
        p = strtok(NULL, " ,.;:!?");
    }
    return 0;
}
