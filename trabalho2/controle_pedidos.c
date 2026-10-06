#include <stdio.h>
#include <stdlib.h>

typedef struct { int cod; char nome[30]; char tp; float preco; } prod;
typedef struct { int cod; char nome[30]; } garc;
typedef struct ped { int cod, qtd; struct ped *prox; } ped;
typedef struct mesa { int num, codg; float tot; ped *peds; struct mesa *ant, *prox; } mesa;

int buscaProd(int cod, prod *pr) {
    FILE *f = fopen("cardapio.dat", "rb");
    int achou = 0;

    if (!f) return 0;
    while (!achou && fread(pr, sizeof(prod), 1, f) == 1) achou = pr->cod == cod;
    fclose(f);
    return achou;
}

int buscaGarc(int cod, garc *g) {
    FILE *f = fopen("garcons.dat", "rb");
    int achou = 0;

    if (!f) return 0;
    while (!achou && fread(g, sizeof(garc), 1, f) == 1) achou = g->cod == cod;
    fclose(f);
    return achou;
}

void menuProd(void) {
    prod v[100];
    int n = 0, i = 0, op, cod;
    FILE *f = fopen("cardapio.dat", "rb");

    if (f) { n = fread(v, sizeof(prod), 100, f); fclose(f); }
    printf("1 - Inserir  2 - Excluir  3 - Alterar  4 - Consultar: "); scanf("%d", &op);
    if (op == 4)
        for (i = 0; i < n; i++) printf("%4d  %-25s %c  R$ %7.2f\n", v[i].cod, v[i].nome, v[i].tp, v[i].preco);
    if (op < 1 || op > 3) return;

    printf("Codigo: "); scanf("%d", &cod);
    while (i < n && v[i].cod != cod) i++;
    if (op == 1 && (i < n || n == 100)) { printf("Codigo ja existe ou cardapio cheio\n"); return; }
    if (op != 1 && i == n) { printf("Codigo nao encontrado\n"); return; }

    if (op == 2) {
        for (n--; i < n; i++) v[i] = v[i + 1];
    } else {
        if (op == 1) n++;
        v[i].cod = cod;
        printf("Nome: "); scanf(" %29[^\n]", v[i].nome);
        printf("Tipo (P - prato, B - bebida): "); scanf(" %c", &v[i].tp);
        printf("Preco: "); scanf("%f", &v[i].preco);
    }
    f = fopen("cardapio.dat", "wb");
    fwrite(v, sizeof(prod), n, f);
    fclose(f);
    printf("Cardapio atualizado\n");
}

void menuGarc(void) {
    garc v[50];
    int n = 0, i = 0, op, cod;
    FILE *f = fopen("garcons.dat", "rb");

    if (f) { n = fread(v, sizeof(garc), 50, f); fclose(f); }
    printf("1 - Inserir  2 - Excluir  3 - Alterar  4 - Consultar: "); scanf("%d", &op);
    if (op == 4)
        for (i = 0; i < n; i++) printf("%4d  %s\n", v[i].cod, v[i].nome);
    if (op < 1 || op > 3) return;

    printf("Codigo: "); scanf("%d", &cod);
    while (i < n && v[i].cod != cod) i++;
    if (op == 1 && (i < n || n == 50)) { printf("Codigo ja existe ou lista cheia\n"); return; }
    if (op != 1 && i == n) { printf("Codigo nao encontrado\n"); return; }

    if (op == 2) {
        for (n--; i < n; i++) v[i] = v[i + 1];
    } else {
        if (op == 1) n++;
        v[i].cod = cod;
        printf("Nome: "); scanf(" %29[^\n]", v[i].nome);
    }
    f = fopen("garcons.dat", "wb");
    fwrite(v, sizeof(garc), n, f);
    fclose(f);
    printf("Garcons atualizados\n");
}

mesa *lerMesa(mesa *inicio) {
    int num;

    printf("Numero da mesa: "); scanf("%d", &num);
    while (inicio && inicio->num != num) inicio = inicio->prox;
    if (!inicio) printf("Mesa nao encontrada\n");
    return inicio;
}

void mesaGarc(mesa **inicio, int abrir) {
    mesa *m = *inicio;
    garc g;
    int num, cod;

    printf("Numero da mesa: "); scanf("%d", &num);
    while (m && m->num != num) m = m->prox;
    if (abrir && m) { printf("Mesa ja esta aberta\n"); return; }
    if (!abrir && !m) { printf("Mesa nao encontrada\n"); return; }
    printf("Codigo do garcom: "); scanf("%d", &cod);
    if (!buscaGarc(cod, &g)) { printf("Garcom nao encontrado\n"); return; }

    if (abrir) {
        m = malloc(sizeof(mesa));
        m->num = num; m->tot = 0; m->peds = NULL; m->ant = NULL; m->prox = *inicio;
        if (*inicio) (*inicio)->ant = m;
        *inicio = m;
    }
    m->codg = cod;
    printf("Mesa %d com o garcom %s\n", num, g.nome);
}

void pedidos(mesa *inicio, int op) {
    mesa *m = lerMesa(inicio);
    ped *p, *ant = NULL;
    prod pr;
    int cod, qtd;

    if (!m) return;
    printf("Codigo do produto: "); scanf("%d", &cod);
    if (!buscaProd(cod, &pr)) { printf("Produto nao encontrado\n"); return; }
    for (p = m->peds; p && p->cod != cod; p = p->prox) ant = p;
    if (op != 1 && !p) { printf("Esse produto nao foi pedido nessa mesa\n"); return; }

    if (op == 3) {
        m->tot -= p->qtd * pr.preco;
        if (ant) ant->prox = p->prox; else m->peds = p->prox;
        free(p);
        if (!m->peds) m->tot = 0;
    } else {
        printf("Quantidade: "); scanf("%d", &qtd);
        if (qtd < 1) { printf("Quantidade invalida\n"); return; }
        if (op == 1) {
            p = malloc(sizeof(ped));
            p->cod = cod; p->qtd = 0; p->prox = m->peds; m->peds = p;
        }
        m->tot += (qtd - p->qtd) * pr.preco;
        p->qtd = qtd;
    }
    printf("Total da mesa: R$ %.2f\n", m->tot);
}

void consultar(mesa *inicio) {
    ped *p;
    prod pr;
    garc g;

    if (!inicio) printf("Nenhuma mesa aberta\n");
    for (; inicio; inicio = inicio->prox) {
        printf("\nMesa %d | garcom: %s | total: R$ %.2f\n", inicio->num,
               buscaGarc(inicio->codg, &g) ? g.nome : "?", inicio->tot);
        for (p = inicio->peds; p; p = p->prox)
            if (buscaProd(p->cod, &pr)) printf("  %4d  %-25s x %d\n", p->cod, pr.nome, p->qtd);
    }
}

void libera(mesa *m) {
    ped *p;

    while (m->peds) { p = m->peds; m->peds = p->prox; free(p); }
    free(m);
}

void fechar(mesa **inicio) {
    mesa *m = lerMesa(*inicio);
    ped *p;
    prod pr;
    garc g;

    if (!m) return;
    printf("\n========== NOTA FISCAL - MESA %d ==========\n", m->num);
    for (p = m->peds; p; p = p->prox)
        if (buscaProd(p->cod, &pr))
            printf("%-22s %3d x %7.2f = %8.2f\n", pr.nome, p->qtd, pr.preco, p->qtd * pr.preco);
    printf("Total: R$ %.2f\nGarcom: %s\n", m->tot, buscaGarc(m->codg, &g) ? g.nome : "?");
    printf("============================================\n");

    if (m->ant) m->ant->prox = m->prox; else *inicio = m->prox;
    if (m->prox) m->prox->ant = m->ant;
    libera(m);
}

int main(void) {
    mesa *inicio = NULL, *m;
    int op;

    do {
        printf("\n1 - Cardapio\n2 - Garcons\n3 - Abrir mesa\n4 - Fazer pedido\n5 - Alterar pedido\n6 - Excluir pedido\n"
               "7 - Trocar garcom da mesa\n8 - Consultar mesas\n9 - Fechar conta\n0 - Sair\nOpcao: ");
        op = 0;
        scanf("%d", &op);
        if (op == 1) menuProd();
        else if (op == 2) menuGarc();
        else if (op == 3 || op == 7) mesaGarc(&inicio, op == 3);
        else if (op >= 4 && op <= 6) pedidos(inicio, op - 3);
        else if (op == 8) consultar(inicio);
        else if (op == 9) fechar(&inicio);
    } while (op != 0);

    while (inicio) { m = inicio; inicio = inicio->prox; libera(m); }
    return 0;
}
