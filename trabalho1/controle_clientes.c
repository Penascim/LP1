/*
 * UERJ - IME/DICC - Linguagem de Programacao I
 * Trabalho 1 - Controle de Clientes
 *
 * Os clientes e os pedidos ficam em vetores na memoria (10 clientes e
 * 20 pedidos). Eles sao lidos dos arquivos binarios Cliente.dat e
 * Pedidos.dat quando o programa abre e gravados de novo depois de cada
 * operacao. Nao ha variaveis globais: tudo e passado por parametro.
 *
 * Compilar: gcc -o controle_clientes controle_clientes.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CLIENTES 10
#define MAX_PEDIDOS  20
#define MAX_ITENS    6

typedef struct {
    char cnpj[15];               /* 14 numeros */
    char nome[50];
    char razaoSocial[50];
    char rua[50];                /* Rua/Av. e numero */
    char cidade[30];
    char estado[3];
    char cep[10];
    char inscricaoEstadual[20];  /* "ISENTO" quando nao tiver */
    char telefone[20];
    char email[50];
    char contato[50];
} Cliente;

typedef struct {
    int numero;
    char cnpj[15];               /* CNPJ do cliente que fez o pedido */
    int diaPedido, mesPedido, anoPedido;
    int diaEntrega, mesEntrega, anoEntrega;   /* tudo 0 enquanto nao for entregue */
    int previsaoEntrega;         /* numero de dias previstos para a entrega */
    char descricao[MAX_ITENS][21];
    int quantidade[MAX_ITENS];
    float valor[MAX_ITENS];      /* valor unitario de cada item */
    int qtdItens;
    float total;
    int formaPagamento;          /* 1 = a vista, 2 = duplicata */
} Pedido;

/* Le uma linha do teclado e tira o '\n' do final */
void lerTexto(char texto[], int tamanho) {
    int c;

    if (fgets(texto, tamanho, stdin) == NULL) {
        printf("\nFim da entrada de dados.\n");
        exit(0);
    }
    /* se o '\n' nao foi lido, a linha era maior que o tamanho: descarta o resto */
    if (strchr(texto, '\n') == NULL) {
        c = getchar();
        while (c != '\n' && c != EOF)
            c = getchar();
    }
    texto[strcspn(texto, "\r\n")] = '\0';
}

int lerInteiro(void) {
    char texto[50];
    int numero;

    lerTexto(texto, 50);
    while (sscanf(texto, "%d", &numero) != 1) {
        printf("Digite um numero: ");
        lerTexto(texto, 50);
    }
    return numero;
}

/* Le uma data no formato dd/mm/aaaa */
void lerData(int *dia, int *mes, int *ano) {
    char texto[50];
    int diasMes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    while (1) {
        lerTexto(texto, 50);
        if (sscanf(texto, "%d/%d/%d", dia, mes, ano) == 3 &&
            *ano >= 1900 && *ano <= 2100 && *mes >= 1 && *mes <= 12) {
            if (*ano % 4 == 0 && (*ano % 100 != 0 || *ano % 400 == 0))
                diasMes[1] = 29;
            else
                diasMes[1] = 28;
            if (*dia >= 1 && *dia <= diasMes[*mes - 1])
                return;
        }
        printf("Data invalida, digite no formato dd/mm/aaaa: ");
    }
}

/* Conta quantos dias se passaram de 01/01/1900 ate a data.
   Serve para calcular prazos: a diferenca entre duas contas e o numero
   de dias entre as duas datas. */
int contarDias(int dia, int mes, int ano) {
    int diasMes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int total = 0;
    int a, m;

    for (a = 1900; a < ano; a++) {
        if (a % 4 == 0 && (a % 100 != 0 || a % 400 == 0))
            total = total + 366;
        else
            total = total + 365;
    }
    for (m = 1; m < mes; m++) {
        total = total + diasMes[m - 1];
        if (m == 2 && ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0))
            total = total + 1;
    }
    return total + dia;
}

void carregarDados(Cliente clientes[], int *qtdClientes, Pedido pedidos[], int *qtdPedidos) {
    FILE *arquivo;

    *qtdClientes = 0;
    arquivo = fopen("Cliente.dat", "rb");
    if (arquivo != NULL) {
        *qtdClientes = fread(clientes, sizeof(Cliente), MAX_CLIENTES, arquivo);
        fclose(arquivo);
    }

    *qtdPedidos = 0;
    arquivo = fopen("Pedidos.dat", "rb");
    if (arquivo != NULL) {
        *qtdPedidos = fread(pedidos, sizeof(Pedido), MAX_PEDIDOS, arquivo);
        fclose(arquivo);
    }
}

void salvarDados(Cliente clientes[], int qtdClientes, Pedido pedidos[], int qtdPedidos) {
    FILE *arquivo;

    arquivo = fopen("Cliente.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao gravar Cliente.dat\n");
    } else {
        fwrite(clientes, sizeof(Cliente), qtdClientes, arquivo);
        fclose(arquivo);
    }

    arquivo = fopen("Pedidos.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao gravar Pedidos.dat\n");
    } else {
        fwrite(pedidos, sizeof(Pedido), qtdPedidos, arquivo);
        fclose(arquivo);
    }
}

/* Retorna a posicao do cliente no vetor, ou -1 se nao encontrar */
int buscarCliente(Cliente clientes[], int qtdClientes, char cnpj[]) {
    int i;

    for (i = 0; i < qtdClientes; i++) {
        if (strcmp(clientes[i].cnpj, cnpj) == 0)
            return i;
    }
    return -1;
}

void mostrarCliente(Cliente c) {
    printf("----------------------------------------\n");
    printf("CNPJ..............: %s\n", c.cnpj);
    printf("Nome..............: %s\n", c.nome);
    printf("Razao social......: %s\n", c.razaoSocial);
    printf("Endereco..........: %s - %s/%s - CEP %s\n", c.rua, c.cidade, c.estado, c.cep);
    printf("Inscricao estadual: %s\n", c.inscricaoEstadual);
    printf("Telefone..........: %s\n", c.telefone);
    printf("Email.............: %s\n", c.email);
    printf("Contato...........: %s\n", c.contato);
}

void mostrarPedido(Pedido p, Cliente clientes[], int qtdClientes) {
    int i, prazo, dias, hoje;
    time_t agora;
    struct tm *data;

    printf("----------------------------------------\n");
    printf("Pedido numero %d\n", p.numero);
    i = buscarCliente(clientes, qtdClientes, p.cnpj);
    if (i != -1)
        printf("Cliente............: %s (CNPJ %s)\n", clientes[i].nome, p.cnpj);
    else
        printf("Cliente............: CNPJ %s\n", p.cnpj);
    printf("Data do pedido.....: %02d/%02d/%04d\n", p.diaPedido, p.mesPedido, p.anoPedido);
    printf("Previsao de entrega: %d dia(s)\n", p.previsaoEntrega);

    /* prazo = data do pedido + dias previstos para a entrega */
    prazo = contarDias(p.diaPedido, p.mesPedido, p.anoPedido) + p.previsaoEntrega;
    if (p.diaEntrega != 0) {
        printf("Situacao...........: entregue em %02d/%02d/%04d", p.diaEntrega, p.mesEntrega, p.anoEntrega);
        dias = contarDias(p.diaEntrega, p.mesEntrega, p.anoEntrega) - prazo;
        if (dias > 0)
            printf(" (com %d dia(s) de atraso)\n", dias);
        else
            printf(" (no prazo)\n");
    } else {
        agora = time(NULL);
        data = localtime(&agora);
        hoje = contarDias(data->tm_mday, data->tm_mon + 1, data->tm_year + 1900);
        if (hoje <= prazo)
            printf("Situacao...........: em aberto, no prazo (faltam %d dia(s))\n", prazo - hoje);
        else
            printf("Situacao...........: em aberto, fora do prazo (atrasado %d dia(s))\n", hoje - prazo);
    }

    printf("Itens:\n");
    for (i = 0; i < p.qtdItens; i++) {
        printf("  %d) %-20s %5d x R$ %9.2f = R$ %10.2f\n", i + 1, p.descricao[i],
               p.quantidade[i], p.valor[i], p.quantidade[i] * p.valor[i]);
    }
    printf("Total do pedido....: R$ %.2f\n", p.total);
    if (p.formaPagamento == 1)
        printf("Forma de pagamento.: a vista\n");
    else
        printf("Forma de pagamento.: duplicata\n");
}

void inserirCliente(Cliente clientes[], int *qtdClientes) {
    Cliente c;
    int i, valido;

    if (*qtdClientes == MAX_CLIENTES) {
        printf("Limite de %d clientes atingido.\n", MAX_CLIENTES);
        return;
    }
    memset(&c, 0, sizeof(c));

    do {
        printf("CNPJ (somente os 14 numeros): ");
        lerTexto(c.cnpj, 15);
        valido = (strlen(c.cnpj) == 14);
        for (i = 0; i < 14 && valido; i++) {
            if (c.cnpj[i] < '0' || c.cnpj[i] > '9')
                valido = 0;
        }
        if (!valido)
            printf("CNPJ invalido.\n");
    } while (!valido);

    if (buscarCliente(clientes, *qtdClientes, c.cnpj) != -1) {
        printf("Ja existe um cliente com esse CNPJ.\n");
        return;
    }

    printf("Nome: ");
    lerTexto(c.nome, 50);
    printf("Razao social: ");
    lerTexto(c.razaoSocial, 50);
    printf("Rua/Av. e numero: ");
    lerTexto(c.rua, 50);
    printf("Cidade: ");
    lerTexto(c.cidade, 30);
    printf("Estado (UF): ");
    lerTexto(c.estado, 3);
    printf("CEP: ");
    lerTexto(c.cep, 10);
    printf("Inscricao estadual (deixe em branco se for isento): ");
    lerTexto(c.inscricaoEstadual, 20);
    if (strlen(c.inscricaoEstadual) == 0)
        strcpy(c.inscricaoEstadual, "ISENTO");
    printf("Telefone: ");
    lerTexto(c.telefone, 20);
    printf("Email: ");
    lerTexto(c.email, 50);
    printf("Contato (nome do contato no cliente): ");
    lerTexto(c.contato, 50);

    clientes[*qtdClientes] = c;
    *qtdClientes = *qtdClientes + 1;
    printf("Cliente cadastrado.\n");
}

void alterarCliente(Cliente clientes[], int qtdClientes) {
    char cnpj[15];
    int i, opcao;

    printf("CNPJ do cliente: ");
    lerTexto(cnpj, 15);
    i = buscarCliente(clientes, qtdClientes, cnpj);
    if (i == -1) {
        printf("Cliente nao encontrado.\n");
        return;
    }
    mostrarCliente(clientes[i]);

    printf("\nQual campo deseja alterar?\n");
    printf("1 - Nome\n");
    printf("2 - Razao social\n");
    printf("3 - Rua/Av. e numero\n");
    printf("4 - Cidade\n");
    printf("5 - Estado\n");
    printf("6 - CEP\n");
    printf("7 - Inscricao estadual\n");
    printf("8 - Telefone\n");
    printf("9 - Email\n");
    printf("10 - Contato\n");
    printf("0 - Cancelar\n");
    printf("Opcao: ");
    opcao = lerInteiro();

    switch (opcao) {
    case 1:
        printf("Novo nome: ");
        lerTexto(clientes[i].nome, 50);
        break;
    case 2:
        printf("Nova razao social: ");
        lerTexto(clientes[i].razaoSocial, 50);
        break;
    case 3:
        printf("Nova rua/av. e numero: ");
        lerTexto(clientes[i].rua, 50);
        break;
    case 4:
        printf("Nova cidade: ");
        lerTexto(clientes[i].cidade, 30);
        break;
    case 5:
        printf("Novo estado (UF): ");
        lerTexto(clientes[i].estado, 3);
        break;
    case 6:
        printf("Novo CEP: ");
        lerTexto(clientes[i].cep, 10);
        break;
    case 7:
        printf("Nova inscricao estadual (deixe em branco se for isento): ");
        lerTexto(clientes[i].inscricaoEstadual, 20);
        if (strlen(clientes[i].inscricaoEstadual) == 0)
            strcpy(clientes[i].inscricaoEstadual, "ISENTO");
        break;
    case 8:
        printf("Novo telefone: ");
        lerTexto(clientes[i].telefone, 20);
        break;
    case 9:
        printf("Novo email: ");
        lerTexto(clientes[i].email, 50);
        break;
    case 10:
        printf("Novo contato: ");
        lerTexto(clientes[i].contato, 50);
        break;
    default:
        printf("Nenhuma alteracao feita.\n");
        return;
    }
    printf("Cliente alterado.\n");
}

void excluirCliente(Cliente clientes[], int *qtdClientes, Pedido pedidos[], int qtdPedidos) {
    char cnpj[15], resposta[10];
    int i, j, qtdPedidosCliente;

    printf("CNPJ do cliente: ");
    lerTexto(cnpj, 15);
    i = buscarCliente(clientes, *qtdClientes, cnpj);
    if (i == -1) {
        printf("Cliente nao encontrado.\n");
        return;
    }

    /* nao deixa excluir cliente que ainda tem pedidos */
    qtdPedidosCliente = 0;
    for (j = 0; j < qtdPedidos; j++) {
        if (strcmp(pedidos[j].cnpj, cnpj) == 0)
            qtdPedidosCliente++;
    }
    if (qtdPedidosCliente > 0) {
        printf("O cliente possui %d pedido(s). Exclua os pedidos antes de excluir o cliente.\n",
               qtdPedidosCliente);
        return;
    }

    mostrarCliente(clientes[i]);
    printf("Confirma a exclusao? (S/N): ");
    lerTexto(resposta, 10);
    if (resposta[0] != 'S' && resposta[0] != 's') {
        printf("Exclusao cancelada.\n");
        return;
    }

    /* puxa os clientes seguintes uma posicao para tras */
    for (j = i; j < *qtdClientes - 1; j++)
        clientes[j] = clientes[j + 1];
    *qtdClientes = *qtdClientes - 1;
    printf("Cliente excluido.\n");
}

void consultarCliente(Cliente clientes[], int qtdClientes) {
    char cnpj[15];
    int i, opcao;

    printf("1 - Consultar por CNPJ\n");
    printf("2 - Listar todos os clientes\n");
    printf("Opcao: ");
    opcao = lerInteiro();

    if (opcao == 1) {
        printf("CNPJ do cliente: ");
        lerTexto(cnpj, 15);
        i = buscarCliente(clientes, qtdClientes, cnpj);
        if (i == -1)
            printf("Cliente nao encontrado.\n");
        else
            mostrarCliente(clientes[i]);
    } else if (opcao == 2) {
        for (i = 0; i < qtdClientes; i++)
            mostrarCliente(clientes[i]);
        printf("\nClientes cadastrados: %d\n", qtdClientes);
    } else {
        printf("Opcao invalida.\n");
    }
}

void inserirPedido(Pedido pedidos[], int *qtdPedidos, Cliente clientes[], int qtdClientes) {
    Pedido p;
    char texto[50];
    int i, j;

    if (*qtdPedidos == MAX_PEDIDOS) {
        printf("Limite de %d pedidos atingido.\n", MAX_PEDIDOS);
        return;
    }
    memset(&p, 0, sizeof(p));

    printf("CNPJ do cliente: ");
    lerTexto(p.cnpj, 15);
    if (buscarCliente(clientes, qtdClientes, p.cnpj) == -1) {
        printf("Cliente nao cadastrado.\n");
        return;
    }

    /* o numero do pedido e o maior numero existente + 1 */
    p.numero = 1;
    for (i = 0; i < *qtdPedidos; i++) {
        if (pedidos[i].numero >= p.numero)
            p.numero = pedidos[i].numero + 1;
    }
    printf("Pedido numero %d\n", p.numero);

    printf("Data do pedido (dd/mm/aaaa): ");
    lerData(&p.diaPedido, &p.mesPedido, &p.anoPedido);

    printf("Previsao de entrega (numero de dias): ");
    p.previsaoEntrega = lerInteiro();
    while (p.previsaoEntrega < 0) {
        printf("Digite um numero de dias valido: ");
        p.previsaoEntrega = lerInteiro();
    }

    printf("O pedido ja foi entregue? (S/N): ");
    lerTexto(texto, 50);
    if (texto[0] == 'S' || texto[0] == 's') {
        printf("Data da entrega (dd/mm/aaaa): ");
        lerData(&p.diaEntrega, &p.mesEntrega, &p.anoEntrega);
        /* compara as datas no formato aaaammdd */
        while (p.anoEntrega * 10000 + p.mesEntrega * 100 + p.diaEntrega <
               p.anoPedido * 10000 + p.mesPedido * 100 + p.diaPedido) {
            printf("A entrega nao pode ser antes do pedido. Digite outra data: ");
            lerData(&p.diaEntrega, &p.mesEntrega, &p.anoEntrega);
        }
    }

    printf("Quantidade de itens (1 a %d): ", MAX_ITENS);
    p.qtdItens = lerInteiro();
    while (p.qtdItens < 1 || p.qtdItens > MAX_ITENS) {
        printf("Digite um numero de 1 a %d: ", MAX_ITENS);
        p.qtdItens = lerInteiro();
    }

    p.total = 0;
    for (i = 0; i < p.qtdItens; i++) {
        printf("Item %d - descricao (ate 20 letras): ", i + 1);
        lerTexto(p.descricao[i], 21);
        do {
            printf("Item %d - quantidade: ", i + 1);
            p.quantidade[i] = lerInteiro();
        } while (p.quantidade[i] < 1);
        do {
            printf("Item %d - valor unitario (R$): ", i + 1);
            lerTexto(texto, 50);
            for (j = 0; texto[j] != '\0'; j++) {   /* aceita virgula no lugar do ponto */
                if (texto[j] == ',')
                    texto[j] = '.';
            }
            p.valor[i] = atof(texto);
        } while (p.valor[i] <= 0);
        p.total = p.total + p.quantidade[i] * p.valor[i];
    }
    printf("Total do pedido: R$ %.2f\n", p.total);

    printf("Forma de pagamento (1 - a vista, 2 - duplicata): ");
    p.formaPagamento = lerInteiro();
    while (p.formaPagamento != 1 && p.formaPagamento != 2) {
        printf("Digite 1 ou 2: ");
        p.formaPagamento = lerInteiro();
    }

    pedidos[*qtdPedidos] = p;
    *qtdPedidos = *qtdPedidos + 1;
    printf("Pedido cadastrado.\n");
}

void alterarPedido(Pedido pedidos[], int qtdPedidos, Cliente clientes[], int qtdClientes) {
    Pedido *p;
    char texto[50];
    int i, j, numero, opcao, dia, mes, ano;

    printf("Numero do pedido: ");
    numero = lerInteiro();
    for (i = 0; i < qtdPedidos; i++) {
        if (pedidos[i].numero == numero)
            break;
    }
    if (i == qtdPedidos) {
        printf("Pedido nao encontrado.\n");
        return;
    }
    p = &pedidos[i];
    mostrarPedido(*p, clientes, qtdClientes);

    printf("\nQual campo deseja alterar?\n");
    printf("1 - CNPJ do cliente\n");
    printf("2 - Data do pedido\n");
    printf("3 - Previsao de entrega\n");
    printf("4 - Data da entrega\n");
    printf("5 - Itens\n");
    printf("6 - Forma de pagamento\n");
    printf("0 - Cancelar\n");
    printf("Opcao: ");
    opcao = lerInteiro();

    switch (opcao) {
    case 1:
        printf("Novo CNPJ do cliente: ");
        lerTexto(texto, 15);
        if (buscarCliente(clientes, qtdClientes, texto) == -1) {
            printf("Cliente nao cadastrado. Nenhuma alteracao feita.\n");
            return;
        }
        strcpy(p->cnpj, texto);
        break;
    case 2:
        printf("Nova data do pedido (dd/mm/aaaa): ");
        lerData(&dia, &mes, &ano);
        if (p->diaEntrega != 0 &&
            p->anoEntrega * 10000 + p->mesEntrega * 100 + p->diaEntrega < ano * 10000 + mes * 100 + dia) {
            printf("O pedido nao pode ser depois da entrega. Nenhuma alteracao feita.\n");
            return;
        }
        p->diaPedido = dia;
        p->mesPedido = mes;
        p->anoPedido = ano;
        break;
    case 3:
        printf("Nova previsao de entrega (numero de dias): ");
        p->previsaoEntrega = lerInteiro();
        while (p->previsaoEntrega < 0) {
            printf("Digite um numero de dias valido: ");
            p->previsaoEntrega = lerInteiro();
        }
        break;
    case 4:
        printf("O pedido ja foi entregue? (S/N): ");
        lerTexto(texto, 50);
        if (texto[0] == 'S' || texto[0] == 's') {
            printf("Data da entrega (dd/mm/aaaa): ");
            lerData(&p->diaEntrega, &p->mesEntrega, &p->anoEntrega);
            while (p->anoEntrega * 10000 + p->mesEntrega * 100 + p->diaEntrega <
                   p->anoPedido * 10000 + p->mesPedido * 100 + p->diaPedido) {
                printf("A entrega nao pode ser antes do pedido. Digite outra data: ");
                lerData(&p->diaEntrega, &p->mesEntrega, &p->anoEntrega);
            }
        } else {
            p->diaEntrega = 0;
            p->mesEntrega = 0;
            p->anoEntrega = 0;
        }
        break;
    case 5:
        /* apaga os itens antigos e le todos de novo */
        for (i = 0; i < MAX_ITENS; i++) {
            strcpy(p->descricao[i], "");
            p->quantidade[i] = 0;
            p->valor[i] = 0;
        }
        printf("Quantidade de itens (1 a %d): ", MAX_ITENS);
        p->qtdItens = lerInteiro();
        while (p->qtdItens < 1 || p->qtdItens > MAX_ITENS) {
            printf("Digite um numero de 1 a %d: ", MAX_ITENS);
            p->qtdItens = lerInteiro();
        }
        p->total = 0;
        for (i = 0; i < p->qtdItens; i++) {
            printf("Item %d - descricao (ate 20 letras): ", i + 1);
            lerTexto(p->descricao[i], 21);
            do {
                printf("Item %d - quantidade: ", i + 1);
                p->quantidade[i] = lerInteiro();
            } while (p->quantidade[i] < 1);
            do {
                printf("Item %d - valor unitario (R$): ", i + 1);
                lerTexto(texto, 50);
                for (j = 0; texto[j] != '\0'; j++) {
                    if (texto[j] == ',')
                        texto[j] = '.';
                }
                p->valor[i] = atof(texto);
            } while (p->valor[i] <= 0);
            p->total = p->total + p->quantidade[i] * p->valor[i];
        }
        printf("Novo total do pedido: R$ %.2f\n", p->total);
        break;
    case 6:
        printf("Forma de pagamento (1 - a vista, 2 - duplicata): ");
        p->formaPagamento = lerInteiro();
        while (p->formaPagamento != 1 && p->formaPagamento != 2) {
            printf("Digite 1 ou 2: ");
            p->formaPagamento = lerInteiro();
        }
        break;
    default:
        printf("Nenhuma alteracao feita.\n");
        return;
    }
    printf("Pedido alterado.\n");
}

void excluirPedido(Pedido pedidos[], int *qtdPedidos, Cliente clientes[], int qtdClientes) {
    char resposta[10];
    int i, j, numero;

    printf("Numero do pedido: ");
    numero = lerInteiro();
    for (i = 0; i < *qtdPedidos; i++) {
        if (pedidos[i].numero == numero)
            break;
    }
    if (i == *qtdPedidos) {
        printf("Pedido nao encontrado.\n");
        return;
    }

    mostrarPedido(pedidos[i], clientes, qtdClientes);
    printf("Confirma a exclusao? (S/N): ");
    lerTexto(resposta, 10);
    if (resposta[0] != 'S' && resposta[0] != 's') {
        printf("Exclusao cancelada.\n");
        return;
    }

    for (j = i; j < *qtdPedidos - 1; j++)
        pedidos[j] = pedidos[j + 1];
    *qtdPedidos = *qtdPedidos - 1;
    printf("Pedido excluido.\n");
}

void consultarPedidos(Pedido pedidos[], int qtdPedidos, Cliente clientes[], int qtdClientes) {
    char cnpj[15];
    int i, j, opcao, encontrados, dia, mes, ano, inicio, fim, dataPedido, hoje, prazo, meses, repetido;
    float totalCliente, totalMes;
    time_t agora;
    struct tm *data;

    printf("1 - Todos os pedidos feitos por um cliente\n");
    printf("2 - Todos os pedidos realizados entre duas datas\n");
    printf("3 - Pedidos entregues a partir de uma data de entrada\n");
    printf("4 - Pedidos em aberto (no prazo e fora do prazo)\n");
    printf("5 - Media mensal de valores comprados por um cliente\n");
    printf("6 - Todos os pedidos\n");
    printf("Opcao: ");
    opcao = lerInteiro();
    encontrados = 0;

    if (opcao == 1) {
        printf("CNPJ do cliente: ");
        lerTexto(cnpj, 15);
        if (buscarCliente(clientes, qtdClientes, cnpj) == -1) {
            printf("Cliente nao encontrado.\n");
            return;
        }
        for (i = 0; i < qtdPedidos; i++) {
            if (strcmp(pedidos[i].cnpj, cnpj) == 0) {
                mostrarPedido(pedidos[i], clientes, qtdClientes);
                encontrados++;
            }
        }

    } else if (opcao == 2) {
        /* as datas viram numeros aaaammdd para poder comparar */
        printf("Data inicial (dd/mm/aaaa): ");
        lerData(&dia, &mes, &ano);
        inicio = ano * 10000 + mes * 100 + dia;
        printf("Data final (dd/mm/aaaa): ");
        lerData(&dia, &mes, &ano);
        fim = ano * 10000 + mes * 100 + dia;
        for (i = 0; i < qtdPedidos; i++) {
            dataPedido = pedidos[i].anoPedido * 10000 + pedidos[i].mesPedido * 100 + pedidos[i].diaPedido;
            if (dataPedido >= inicio && dataPedido <= fim) {
                mostrarPedido(pedidos[i], clientes, qtdClientes);
                encontrados++;
            }
        }

    } else if (opcao == 3) {
        /* pedidos ja entregues cuja data do pedido (entrada) e igual ou posterior a data informada */
        printf("Data de entrada (dd/mm/aaaa): ");
        lerData(&dia, &mes, &ano);
        inicio = ano * 10000 + mes * 100 + dia;
        for (i = 0; i < qtdPedidos; i++) {
            dataPedido = pedidos[i].anoPedido * 10000 + pedidos[i].mesPedido * 100 + pedidos[i].diaPedido;
            if (pedidos[i].diaEntrega != 0 && dataPedido >= inicio) {
                mostrarPedido(pedidos[i], clientes, qtdClientes);
                encontrados++;
            }
        }

    } else if (opcao == 4) {
        agora = time(NULL);
        data = localtime(&agora);
        hoje = contarDias(data->tm_mday, data->tm_mon + 1, data->tm_year + 1900);

        printf("\n*** PEDIDOS EM ABERTO NO PRAZO ***\n");
        for (i = 0; i < qtdPedidos; i++) {
            prazo = contarDias(pedidos[i].diaPedido, pedidos[i].mesPedido, pedidos[i].anoPedido) +
                    pedidos[i].previsaoEntrega;
            if (pedidos[i].diaEntrega == 0 && hoje <= prazo) {
                mostrarPedido(pedidos[i], clientes, qtdClientes);
                encontrados++;
            }
        }
        printf("\n*** PEDIDOS EM ABERTO FORA DO PRAZO ***\n");
        for (i = 0; i < qtdPedidos; i++) {
            prazo = contarDias(pedidos[i].diaPedido, pedidos[i].mesPedido, pedidos[i].anoPedido) +
                    pedidos[i].previsaoEntrega;
            if (pedidos[i].diaEntrega == 0 && hoje > prazo) {
                mostrarPedido(pedidos[i], clientes, qtdClientes);
                encontrados++;
            }
        }

    } else if (opcao == 5) {
        printf("CNPJ do cliente: ");
        lerTexto(cnpj, 15);
        i = buscarCliente(clientes, qtdClientes, cnpj);
        if (i == -1) {
            printf("Cliente nao encontrado.\n");
            return;
        }
        printf("Cliente: %s\n\n", clientes[i].nome);
        printf("Mes/Ano     Total do mes\n");
        totalCliente = 0;
        meses = 0;
        for (i = 0; i < qtdPedidos; i++) {
            if (strcmp(pedidos[i].cnpj, cnpj) != 0)
                continue;
            encontrados++;
            totalCliente = totalCliente + pedidos[i].total;

            /* se um pedido anterior do cliente ja for do mesmo mes, esse mes ja foi mostrado */
            repetido = 0;
            for (j = 0; j < i; j++) {
                if (strcmp(pedidos[j].cnpj, cnpj) == 0 && pedidos[j].mesPedido == pedidos[i].mesPedido &&
                    pedidos[j].anoPedido == pedidos[i].anoPedido)
                    repetido = 1;
            }
            if (repetido)
                continue;

            /* soma todos os pedidos do cliente nesse mes */
            meses++;
            totalMes = 0;
            for (j = i; j < qtdPedidos; j++) {
                if (strcmp(pedidos[j].cnpj, cnpj) == 0 && pedidos[j].mesPedido == pedidos[i].mesPedido &&
                    pedidos[j].anoPedido == pedidos[i].anoPedido)
                    totalMes = totalMes + pedidos[j].total;
            }
            printf("%02d/%04d     R$ %10.2f\n", pedidos[i].mesPedido, pedidos[i].anoPedido, totalMes);
        }
        if (meses > 0) {
            printf("\nTotal comprado: R$ %.2f em %d mes(es)\n", totalCliente, meses);
            printf("Media mensal..: R$ %.2f\n", totalCliente / meses);
        }

    } else if (opcao == 6) {
        for (i = 0; i < qtdPedidos; i++) {
            mostrarPedido(pedidos[i], clientes, qtdClientes);
            encontrados++;
        }

    } else {
        printf("Opcao invalida.\n");
        return;
    }

    printf("\nPedidos encontrados: %d\n", encontrados);
}

int main(void) {
    Cliente clientes[MAX_CLIENTES];
    Pedido pedidos[MAX_PEDIDOS];
    int qtdClientes, qtdPedidos, opcao;

    carregarDados(clientes, &qtdClientes, pedidos, &qtdPedidos);
    printf("%d cliente(s) e %d pedido(s) carregados.\n", qtdClientes, qtdPedidos);

    do {
        printf("\n========== CONTROLE DE CLIENTES ==========\n");
        printf("Clientes: %d/%d   Pedidos: %d/%d\n", qtdClientes, MAX_CLIENTES, qtdPedidos, MAX_PEDIDOS);
        printf("1 - Inserir cliente\n");
        printf("2 - Alterar cliente\n");
        printf("3 - Excluir cliente\n");
        printf("4 - Consultar cliente\n");
        printf("5 - Inserir pedido\n");
        printf("6 - Alterar pedido\n");
        printf("7 - Excluir pedido\n");
        printf("8 - Consultar pedidos\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        opcao = lerInteiro();

        switch (opcao) {
        case 1:
            inserirCliente(clientes, &qtdClientes);
            break;
        case 2:
            alterarCliente(clientes, qtdClientes);
            break;
        case 3:
            excluirCliente(clientes, &qtdClientes, pedidos, qtdPedidos);
            break;
        case 4:
            consultarCliente(clientes, qtdClientes);
            break;
        case 5:
            inserirPedido(pedidos, &qtdPedidos, clientes, qtdClientes);
            break;
        case 6:
            alterarPedido(pedidos, qtdPedidos, clientes, qtdClientes);
            break;
        case 7:
            excluirPedido(pedidos, &qtdPedidos, clientes, qtdClientes);
            break;
        case 8:
            consultarPedidos(pedidos, qtdPedidos, clientes, qtdClientes);
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida.\n");
        }

        /* grava os dois arquivos depois de cada operacao */
        salvarDados(clientes, qtdClientes, pedidos, qtdPedidos);
    } while (opcao != 0);

    return 0;
}
