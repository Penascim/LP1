#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int cod;
    char nome[30];
    char tp;
    float preco;
} prod;

typedef struct {
    int cod;
    char nome[30];
} garc;

typedef struct ped {
    int cod;
    int qtd;
    struct ped *prox;
} ped;

typedef struct mesa {
    int num;
    int codg;
    float tot;
    ped *peds;
    struct mesa *ant, *prox;
} mesa;

int buscaProd(int cod, prod *pr) {
    FILE *f = fopen("cardapio.dat", "rb");
    int achou = 0;

    if (!f)
        return 0;
    while (!achou && fread(pr, sizeof(prod), 1, f) == 1)
        if (pr->cod == cod)
            achou = 1;
    fclose(f);
    return achou;
}

int buscaGarc(int cod, garc *g) {
    FILE *f = fopen("garcons.dat", "rb");
    int achou = 0;

    if (!f)
        return 0;
    while (!achou && fread(g, sizeof(garc), 1, f) == 1)
        if (g->cod == cod)
            achou = 1;
    fclose(f);
    return achou;
}

void menuProd(void) {
    prod v[100];
    int n = 0, i = 0, op, cod;
    FILE *f = fopen("cardapio.dat", "rb");

    if (f) {
        n = fread(v, sizeof(prod), 100, f);
        fclose(f);
    }
    printf("1 - Inserir  2 - Excluir  3 - Alterar  4 - Consultar: ");
    scanf("%d", &op);
    if (op < 1 || op > 4)
        return;
    if (op == 4) {
        for (i = 0; i < n; i++)
            printf("%4d  %-25s %c  R$ %7.2f\n", v[i].cod, v[i].nome, v[i].tp, v[i].preco);
        return;
    }

    printf("Codigo: ");
    scanf("%d", &cod);
    while (i < n && v[i].cod != cod)
        i++;
    if (op == 1 && (i < n || n == 100)) {
        printf("Codigo ja existe ou cardapio cheio\n");
        return;
    }
    if (op != 1 && i == n) {
        printf("Codigo nao encontrado\n");
        return;
    }

    if (op == 2) {
        for (; i < n - 1; i++)
            v[i] = v[i + 1];
        n--;
    } else {
        if (op == 1)
            n++;
        v[i].cod = cod;
        printf("Nome: ");
        scanf(" %29[^\n]", v[i].nome);
        printf("Tipo (P - prato, B - bebida): ");
        scanf(" %c", &v[i].tp);
        printf("Preco: ");
        scanf("%f", &v[i].preco);
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

    if (f) {
        n = fread(v, sizeof(garc), 50, f);
        fclose(f);
    }
    printf("1 - Inserir  2 - Excluir  3 - Alterar  4 - Consultar: ");
    scanf("%d", &op);
    if (op < 1 || op > 4)
        return;
    if (op == 4) {
        for (i = 0; i < n; i++)
            printf("%4d  %s\n", v[i].cod, v[i].nome);
        return;
    }

    printf("Codigo: ");
    scanf("%d", &cod);
    while (i < n && v[i].cod != cod)
        i++;
    if (op == 1 && (i < n || n == 50)) {
        printf("Codigo ja existe ou lista cheia\n");
        return;
    }
    if (op != 1 && i == n) {
        printf("Codigo nao encontrado\n");
        return;
    }

    if (op == 2) {
        for (; i < n - 1; i++)
            v[i] = v[i + 1];
        n--;
    } else {
        if (op == 1)
            n++;
        v[i].cod = cod;
        printf("Nome: ");
        scanf(" %29[^\n]", v[i].nome);
    }

    f = fopen("garcons.dat", "wb");
    fwrite(v, sizeof(garc), n, f);
    fclose(f);
    printf("Garcons atualizados\n");
}

mesa *achaMesa(mesa *ini, int num) {
    while (ini && ini->num != num)
        ini = ini->prox;
    return ini;
}

void abrir(mesa **ini) {
    mesa *m;
    garc g;
    int num, cod;

    printf("Numero da mesa: ");
    scanf("%d", &num);
    if (achaMesa(*ini, num)) {
        printf("Mesa ja esta aberta\n");
        return;
    }
    printf("Codigo do garcom: ");
    scanf("%d", &cod);
    if (!buscaGarc(cod, &g)) {
        printf("Garcom nao encontrado\n");
        return;
    }

    m = malloc(sizeof(mesa));
    m->num = num;
    m->codg = cod;
    m->tot = 0;
    m->peds = NULL;
    m->ant = NULL;
    m->prox = *ini;
    if (*ini)
        (*ini)->ant = m;
    *ini = m;
    printf("Mesa %d aberta, garcom %s\n", num, g.nome);
}

void trocaGarc(mesa *ini) {
    mesa *m;
    garc g;
    int num, cod;

    printf("Numero da mesa: ");
    scanf("%d", &num);
    m = achaMesa(ini, num);
    if (!m) {
        printf("Mesa nao encontrada\n");
        return;
    }
    printf("Codigo do novo garcom: ");
    scanf("%d", &cod);
    if (!buscaGarc(cod, &g)) {
        printf("Garcom nao encontrado\n");
        return;
    }
    m->codg = cod;
    printf("Mesa %d agora com o garcom %s\n", num, g.nome);
}

void pedidos(mesa *ini, int op) {
    mesa *m;
    ped *p, *ant = NULL;
    prod pr;
    int num, cod, qtd;

    printf("Numero da mesa: ");
    scanf("%d", &num);
    m = achaMesa(ini, num);
    if (!m) {
        printf("Mesa nao encontrada\n");
        return;
    }
    printf("Codigo do produto: ");
    scanf("%d", &cod);
    if (!buscaProd(cod, &pr)) {
        printf("Produto nao encontrado\n");
        return;
    }

    if (op == 1) {
        printf("Quantidade: ");
        scanf("%d", &qtd);
        if (qtd < 1) {
            printf("Quantidade invalida\n");
            return;
        }
        p = malloc(sizeof(ped));
        p->cod = cod;
        p->qtd = qtd;
        p->prox = m->peds;
        m->peds = p;
        m->tot += qtd * pr.preco;
        printf("Pedido feito. Total da mesa: R$ %.2f\n", m->tot);
        return;
    }

    for (p = m->peds; p && p->cod != cod; p = p->prox)
        ant = p;
    if (!p) {
        printf("Esse produto nao foi pedido nessa mesa\n");
        return;
    }

    if (op == 2) {
        printf("Nova quantidade: ");
        scanf("%d", &qtd);
        if (qtd < 1) {
            printf("Quantidade invalida\n");
            return;
        }
        m->tot += (qtd - p->qtd) * pr.preco;
        p->qtd = qtd;
    } else {
        m->tot -= p->qtd * pr.preco;
        if (ant)
            ant->prox = p->prox;
        else
            m->peds = p->prox;
        free(p);
        if (!m->peds)
            m->tot = 0;
    }
    printf("Total da mesa: R$ %.2f\n", m->tot);
}

void consultar(mesa *ini) {
    mesa *m;
    ped *p;
    prod pr;
    garc g;

    if (!ini)
        printf("Nenhuma mesa aberta\n");
    for (m = ini; m; m = m->prox) {
        printf("\nMesa %d | garcom: %s | total: R$ %.2f\n", m->num,
               buscaGarc(m->codg, &g) ? g.nome : "?", m->tot);
        for (p = m->peds; p; p = p->prox)
            if (buscaProd(p->cod, &pr))
                printf("  %4d  %-25s x %d\n", p->cod, pr.nome, p->qtd);
    }
}

void libera(mesa *m) {
    ped *p;

    while (m->peds) {
        p = m->peds;
        m->peds = p->prox;
        free(p);
    }
    free(m);
}

void fechar(mesa **ini) {
    mesa *m;
    ped *p;
    prod pr;
    garc g;
    int num;

    printf("Numero da mesa: ");
    scanf("%d", &num);
    m = achaMesa(*ini, num);
    if (!m) {
        printf("Mesa nao encontrada\n");
        return;
    }

    printf("\n============== NOTA FISCAL ==============\n");
    printf("Mesa %d\n\n", m->num);
    for (p = m->peds; p; p = p->prox)
        if (buscaProd(p->cod, &pr))
            printf("%-22s %3d x %7.2f = %8.2f\n", pr.nome, p->qtd, pr.preco, p->qtd * pr.preco);
    printf("-----------------------------------------\n");
    printf("Total: R$ %.2f\n", m->tot);
    printf("Garcom: %s\n", buscaGarc(m->codg, &g) ? g.nome : "?");
    printf("=========================================\n");

    if (m->ant)
        m->ant->prox = m->prox;
    else
        *ini = m->prox;
    if (m->prox)
        m->prox->ant = m->ant;
    libera(m);
}

int main(void) {
    mesa *ini = NULL, *m;
    int op;

    do {
        printf("\n1 - Cardapio\n2 - Garcons\n3 - Abrir mesa\n4 - Fazer pedido\n5 - Alterar pedido\n");
        printf("6 - Excluir pedido\n7 - Trocar garcom da mesa\n8 - Consultar mesas\n9 - Fechar conta\n0 - Sair\n");
        printf("Opcao: ");
        op = 0;
        scanf("%d", &op);

        if (op == 1)
            menuProd();
        else if (op == 2)
            menuGarc();
        else if (op == 3)
            abrir(&ini);
        else if (op >= 4 && op <= 6)
            pedidos(ini, op - 3);
        else if (op == 7)
            trocaGarc(ini);
        else if (op == 8)
            consultar(ini);
        else if (op == 9)
            fechar(&ini);
    } while (op != 0);

    while (ini) {
        m = ini;
        ini = ini->prox;
        libera(m);
    }
    return 0;
}
