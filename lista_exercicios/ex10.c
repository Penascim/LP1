#include <stdio.h>
#include <string.h>

int main(void) {
    char texto[82], palavras[40][81];
    char *p;
    int qtd = 0, i, j, repetida, contagem;

    printf("Digite um texto (ate 80 caracteres): ");
    fgets(texto, 82, stdin);
    texto[strcspn(texto, "\n")] = '\0';

    p = strtok(texto, " ,.;:!?");
    while (p != NULL) {
        strcpy(palavras[qtd], p);
        qtd++;
        p = strtok(NULL, " ,.;:!?");
    }

    printf("Palavra              Frequencia\n");
    for (i = 0; i < qtd; i++) {
        repetida = 0;
        for (j = 0; j < i; j++) {
            if (strcmp(palavras[i], palavras[j]) == 0)
                repetida = 1;
        }
        if (repetida)
            continue;

        contagem = 0;
        for (j = i; j < qtd; j++) {
            if (strcmp(palavras[i], palavras[j]) == 0)
                contagem++;
        }
        printf("%-20s %d\n", palavras[i], contagem);
    }
    return 0;
}
