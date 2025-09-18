#include <stdio.h>

typedef struct Atleta {
    char nome[50];
    char posicao[20];
    int idade;
    int gols;
    int assistencias;
    int partidas_jogadas;
} t_atleta;

void exibeAtleta(t_atleta atleta) {
    printf("=================================================\n");
    printf("            Ficha do Atleta\n");
    printf("=================================================\n");
    printf("Nome: %s\n", atleta.nome);
    printf("Posição: %s\n", atleta.posicao);
    printf("Idade: %d anos\n", atleta.idade);
    printf("Gols: %d\n", atleta.gols);
    printf("Assistências: %d\n", atleta.assistencias);
    printf("Partidas Jogadas: %d\n", atleta.partidas_jogadas);
    printf("=================================================\n\n");
}

int main(void) {
    t_atleta atleta = {"Cristiano Ronaldo", "Atacante", 36, 800, 200, 1000};
    exibeAtleta(atleta);
    return 0;
}
