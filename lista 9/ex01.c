#include <stddef.h>
#include <stdio.h>

struct Caixa {
    int valor;
    struct Caixa* prox;
};

void imprimirLista(struct Caixa* cabeca) {
    struct Caixa* p = cabeca;
    while (p != NULL) {
        printf("%d -> ", p->valor);
        p = p->prox; 
    }
    printf("NULL\n");
}

int main(void) {
    struct Caixa c1, c2, c3, c4, c5;

    c1.valor = 1;
    c2.valor = 3;
    c3.valor = 9;
    c4.valor = 7;
    c5.valor = 5;
    
    c5.prox = NULL;
    c4.prox = &c5;
    c3.prox = &c4;
    c2.prox = &c3;
    c1.prox = &c2;

    imprimirLista(&c1);

    return 0;
}
