#include <stdio.h>

#define TOTAL 50

typedef struct {
    char nome[50];
    float nota;
} Aluno;

int main(void) {
    Aluno turma[TOTAL], aux;
    float soma = 0, media;
    int i, j, mostrados = 0;

    for (i = 0; i < TOTAL; i++) {
        printf("Aluno %d - nome: ", i + 1);
        scanf(" %49[^\n]", turma[i].nome);
        printf("Aluno %d - nota: ", i + 1);
        scanf("%f", &turma[i].nota);
        soma += turma[i].nota;
    }
    media = soma / TOTAL;

    for (i = 0; i < TOTAL - 1; i++) {
        for (j = 0; j < TOTAL - 1 - i; j++) {
            if (turma[j].nota < turma[j + 1].nota) {
                aux = turma[j];
                turma[j] = turma[j + 1];
                turma[j + 1] = aux;
            }
        }
    }

    printf("\nMedia da turma: %.2f\n", media);
    printf("Maiores notas acima da media:\n");
    for (i = 0; i < TOTAL && mostrados < 5; i++) {
        if (turma[i].nota > media) {
            printf("%d. %s - %.2f\n", mostrados + 1, turma[i].nome, turma[i].nota);
            mostrados++;
        }
    }
    if (mostrados == 0)
        printf("Nenhum aluno ficou acima da media\n");
    return 0;
}
