/*
 * UERJ - IME/DICC - Linguagem de Programacao I
 * Trabalho 1 - Controle de Clientes
 *
 * Processa informacoes sobre clientes e seus pedidos, armazenadas nos
 * arquivos binarios Cliente.dat e Pedidos.dat.
 *
 * Os registros ficam em vetores na memoria (10 clientes e 20 pedidos),
 * carregados dos arquivos no inicio do programa e regravados a cada
 * insercao, alteracao ou exclusao. Nao ha variaveis globais: os dados sao
 * declarados na main e passados por parametro para as funcoes.
 *
 * Compilar: gcc -Wall -Wextra -std=c99 -o controle_clientes controle_clientes.c
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CLIENTES 10
#define MAX_PEDIDOS  20
#define MAX_ITENS    6      /* item 1 + mais 5 repeticoes */

#define ARQ_CLIENTES "Cliente.dat"
#define ARQ_PEDIDOS  "Pedidos.dat"

/* Tamanhos dos campos de texto (ja contando o '\0') */
#define TAM_LINHA      256
#define TAM_CNPJ       15   /* 14 caracteres, sem pontuacao */
#define TAM_CNPJ_FMT   19   /* 00.000.000/0000-00 */
#define TAM_NOME       51
#define TAM_RAZAO      61
#define TAM_LOGRADOURO 61
#define TAM_CIDADE     41
#define TAM_UF         3
#define TAM_CEP        9    /* 8 digitos, sem pontuacao */
#define TAM_CEP_FMT    10   /* 00000-000 */
#define TAM_IE         21
#define TAM_TELEFONE   21
#define TAM_EMAIL      61
#define TAM_DESCRICAO  21   /* string de 20 caracteres */
#define TAM_DATA       16
#define TAM_VALOR      32   /* numero convertido em texto para os prompts */

#define PAGTO_A_VISTA   1
#define PAGTO_DUPLICATA 2

#define MSG_CNPJ_INVALIDO "CNPJ invalido. Informe os 14 caracteres (com ou sem pontuacao)."

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    char logradouro[TAM_LOGRADOURO];   /* Rua/Av. e numero */
    char cidade[TAM_CIDADE];
    char estado[TAM_UF];
    char cep[TAM_CEP];
} Endereco;

typedef struct {
    char cnpj[TAM_CNPJ];
    char nome[TAM_NOME];
    char razaoSocial[TAM_RAZAO];
    Endereco endereco;
    char inscricaoEstadual[TAM_IE];    /* "ISENTO" quando nao tiver */
    char telefone[TAM_TELEFONE];
    char email[TAM_EMAIL];
    char contato[TAM_NOME];
} Cliente;

typedef struct {
    char descricao[TAM_DESCRICAO];
    int quantidade;
    double valorUnitario;
} Item;

typedef struct {
    int numero;
    char cnpjCliente[TAM_CNPJ];
    Data dataPedido;                   /* data em que o pedido foi enviado para processamento */
    Data dataEntrega;                  /* zerada enquanto o pedido nao for entregue */
    int previsaoEntrega;               /* dias previstos para a entrega */
    Item itens[MAX_ITENS];
    int qtdItens;
    double total;                      /* soma de quantidade * valor unitario dos itens */
    int formaPagamento;                /* PAGTO_A_VISTA ou PAGTO_DUPLICATA */
} Pedido;

/* Usado no calculo da media mensal de compras de um cliente */
typedef struct {
    int mes;
    int ano;
    int qtdPedidos;
    double total;
} TotalMes;

/* ---------- Leitura do teclado ---------- */
int lerLinha(const char *prompt, char *buf, int tam);
void lerCampo(const char *prompt, char *dest, int tam, int obrigatorio,
              int (*valida)(char *), const char *msgErro);
void lerInteiro(const char *prompt, int *valor, int min, int max, int obrigatorio);
void lerReal(const char *prompt, double *valor, double min, double max, int obrigatorio);
void lerData(const char *prompt, Data *d, int obrigatorio);
int lerSimNao(const char *prompt);
void montarPrompt(char *prompt, int tam, const char *rotulo, const char *atual);

/* ---------- Validacao e formatacao ---------- */
int validaCnpj(char *s);
int validaUF(char *s);
int validaCep(char *s);
int validaEmail(char *s);
int validaIE(char *s);
void formatarCnpj(const char *cnpj, char *buf);
void formatarCep(const char *cep, char *buf);
void formatarData(Data d, char *buf);
const char *textoOuPadrao(const char *s);
const char *nomeFormaPagamento(int forma);

/* ---------- Datas ---------- */
int anoBissexto(int ano);
int diasNoMes(int mes, int ano);
int dataValida(Data d);
int dataVazia(Data d);
int converterData(const char *s, Data *d);
long dataParaDias(Data d);
int compararDatas(Data a, Data b);
Data somarDias(Data d, int dias);
Data dataHoje(void);

/* ---------- Arquivos ---------- */
int carregarArquivo(const char *nome, void *vetor, size_t tamRegistro, int max);
int gravarArquivo(const char *nome, const void *vetor, size_t tamRegistro, int qtd);
int salvarClientes(const Cliente clientes[], int qtdClientes);
int salvarPedidos(const Pedido pedidos[], int qtdPedidos);

/* ---------- Buscas ---------- */
int buscarCliente(const Cliente clientes[], int qtdClientes, const char *cnpj);
int buscarPedido(const Pedido pedidos[], int qtdPedidos, int numero);
int contarPedidosCliente(const Pedido pedidos[], int qtdPedidos, const char *cnpj);
int proximoNumeroPedido(const Pedido pedidos[], int qtdPedidos);
int selecionarCliente(const Cliente clientes[], int qtdClientes);
int selecionarPedido(const Pedido pedidos[], int qtdPedidos);

/* ---------- Clientes ---------- */
void imprimirCliente(const Cliente *c);
void preencherCliente(Cliente *c, int edicao);
void inserirCliente(Cliente clientes[], int *qtdClientes);
void alterarCliente(Cliente clientes[], int qtdClientes);
void excluirCliente(Cliente clientes[], int *qtdClientes, Pedido pedidos[], int *qtdPedidos);
void consultarClientes(const Cliente clientes[], int qtdClientes);
void menuClientes(Cliente clientes[], int *qtdClientes, Pedido pedidos[], int *qtdPedidos);

/* ---------- Pedidos ---------- */
double calcularTotal(const Pedido *p);
Data dataPrevista(const Pedido *p);
void situacaoPedido(const Pedido *p, Data hoje, char *buf, int tam);
void imprimirPedido(const Pedido *p, const Cliente clientes[], int qtdClientes, Data hoje);
void imprimirResumo(int qtd, double soma);
int lerClientePedido(Pedido *p, const Cliente clientes[], int qtdClientes, int edicao);
void lerDataEntrega(Pedido *p);
void preencherItens(Pedido *p, int edicao);
int preencherPedido(Pedido *p, const Cliente clientes[], int qtdClientes, int edicao);
void inserirPedido(Pedido pedidos[], int *qtdPedidos, const Cliente clientes[], int qtdClientes);
void alterarPedido(Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes);
void excluirPedido(Pedido pedidos[], int *qtdPedidos, const Cliente clientes[], int qtdClientes);
void consultarPedidosCliente(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes);
void consultarPedidosPeriodo(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes);
void consultarPedidosEntregues(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes);
void consultarPedidosEmAberto(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes);
void consultarMediaMensal(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes);
void consultarPedidoNumero(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes);
void listarPedidos(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes);
void menuConsultaPedidos(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes);
void menuPedidos(Pedido pedidos[], int *qtdPedidos, const Cliente clientes[], int qtdClientes);

/* ======================================================================
 * Leitura do teclado
 * ====================================================================== */

/* Le uma linha, sem o '\n' e sem espacos nas pontas, e retorna seu tamanho.
   Se a entrada acabar, encerra o programa (os dados ja estao gravados,
   pois os arquivos sao salvos a cada alteracao). */
int lerLinha(const char *prompt, char *buf, int tam) {
    int len, ini = 0;

    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buf, tam, stdin) == NULL) {
        printf("\nFim da entrada de dados. Encerrando o programa.\n");
        exit(0);
    }
    len = (int) strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[--len] = '\0';
    } else {
        int c;  /* linha maior que o buffer: descarta o restante */
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }
    while (len > 0 && isspace((unsigned char) buf[len - 1]))
        buf[--len] = '\0';
    while (isspace((unsigned char) buf[ini]))
        ini++;
    if (ini > 0)
        memmove(buf, buf + ini, len - ini + 1);
    return len - ini;
}

/* Le um campo de texto para 'dest' (de tamanho 'tam').
   Se o campo nao for obrigatorio e o usuario apenas teclar ENTER, o valor
   que ja esta em 'dest' e mantido. 'valida' (opcional) confere e
   normaliza o texto digitado, retornando 0 se ele for invalido. */
void lerCampo(const char *prompt, char *dest, int tam, int obrigatorio,
              int (*valida)(char *), const char *msgErro) {
    char buf[TAM_LINHA];

    for (;;) {
        if (lerLinha(prompt, buf, TAM_LINHA) == 0) {
            if (!obrigatorio)
                return;
            printf("  Campo obrigatorio.\n");
            continue;
        }
        if (valida != NULL && !valida(buf)) {
            printf("  %s\n", msgErro);
            continue;
        }
        if ((int) strlen(buf) >= tam) {
            printf("  Maximo de %d caracteres.\n", tam - 1);
            continue;
        }
        strcpy(dest, buf);
        return;
    }
}

/* Le um inteiro entre min e max. Mesma regra do ENTER de lerCampo. */
void lerInteiro(const char *prompt, int *valor, int min, int max, int obrigatorio) {
    char buf[TAM_LINHA];
    char *fim;
    long v;

    for (;;) {
        if (lerLinha(prompt, buf, TAM_LINHA) == 0) {
            if (!obrigatorio)
                return;
            printf("  Campo obrigatorio.\n");
            continue;
        }
        v = strtol(buf, &fim, 10);
        if (*fim != '\0' || v < min || v > max) {
            printf("  Digite um numero inteiro entre %d e %d.\n", min, max);
            continue;
        }
        *valor = (int) v;
        return;
    }
}

/* Le um numero real entre min e max (aceita virgula ou ponto decimal). */
void lerReal(const char *prompt, double *valor, double min, double max, int obrigatorio) {
    char buf[TAM_LINHA];
    char *fim, *virgula;
    double v;

    for (;;) {
        if (lerLinha(prompt, buf, TAM_LINHA) == 0) {
            if (!obrigatorio)
                return;
            printf("  Campo obrigatorio.\n");
            continue;
        }
        virgula = strchr(buf, ',');
        if (virgula != NULL)
            *virgula = '.';
        v = strtod(buf, &fim);
        if (*fim != '\0' || v < min || v > max) {
            printf("  Digite um valor entre %.2f e %.2f.\n", min, max);
            continue;
        }
        *valor = v;
        return;
    }
}

/* Le uma data no formato dd/mm/aaaa. Mesma regra do ENTER de lerCampo. */
void lerData(const char *prompt, Data *d, int obrigatorio) {
    char buf[TAM_LINHA];
    Data nova;

    for (;;) {
        if (lerLinha(prompt, buf, TAM_LINHA) == 0) {
            if (!obrigatorio)
                return;
            printf("  Campo obrigatorio.\n");
            continue;
        }
        if (converterData(buf, &nova)) {
            *d = nova;
            return;
        }
        printf("  Data invalida. Use o formato dd/mm/aaaa.\n");
    }
}

/* Retorna 1 para S (sim) e 0 para N (nao). */
int lerSimNao(const char *prompt) {
    char buf[TAM_LINHA];

    for (;;) {
        if (lerLinha(prompt, buf, TAM_LINHA) == 1) {
            char c = (char) toupper((unsigned char) buf[0]);
            if (c == 'S')
                return 1;
            if (c == 'N')
                return 0;
        }
        printf("  Responda S ou N.\n");
    }
}

/* Monta "rotulo: " ou, quando ha valor atual, "rotulo [atual]: ". */
void montarPrompt(char *prompt, int tam, const char *rotulo, const char *atual) {
    if (atual != NULL && atual[0] != '\0')
        snprintf(prompt, tam, "%s [%s]: ", rotulo, atual);
    else
        snprintf(prompt, tam, "%s: ", rotulo);
}

/* ======================================================================
 * Validacao e formatacao
 * ====================================================================== */

/* Remove a pontuacao do CNPJ e confere os digitos verificadores.
   Aceita tambem o CNPJ alfanumerico (letras nas 12 primeiras posicoes),
   em que cada caractere vale seu codigo ASCII menos 48 no calculo. */
int validaCnpj(char *s) {
    const int pesos[13] = {6, 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2};
    char limpo[TAM_CNPJ];
    int n = 0, i, k;

    for (i = 0; s[i] != '\0'; i++) {
        char c = (char) toupper((unsigned char) s[i]);
        if (c == '.' || c == '/' || c == '-' || c == ' ')
            continue;
        if (!isalnum((unsigned char) c) || n == TAM_CNPJ - 1)
            return 0;
        limpo[n++] = c;
    }
    if (n != TAM_CNPJ - 1)
        return 0;
    limpo[n] = '\0';

    /* os digitos verificadores sao sempre numericos */
    if (!isdigit((unsigned char) limpo[12]) || !isdigit((unsigned char) limpo[13]))
        return 0;

    /* rejeita sequencias repetidas, como 00000000000000 */
    for (i = 1; i < 14 && limpo[i] == limpo[0]; i++)
        ;
    if (i == 14)
        return 0;

    /* k = 12: primeiro digito verificador; k = 13: segundo */
    for (k = 12; k <= 13; k++) {
        int soma = 0, resto, dv;
        for (i = 0; i < k; i++)
            soma += (limpo[i] - '0') * pesos[i + 13 - k];
        resto = soma % 11;
        dv = resto < 2 ? 0 : 11 - resto;
        if (limpo[k] - '0' != dv)
            return 0;
    }
    strcpy(s, limpo);
    return 1;
}

int validaUF(char *s) {
    const char *ufs = "AC AL AP AM BA CE DF ES GO MA MT MS MG PA PB PR PE PI RJ RN RS RO RR SC SP SE TO";

    if (strlen(s) != 2)
        return 0;
    s[0] = (char) toupper((unsigned char) s[0]);
    s[1] = (char) toupper((unsigned char) s[1]);
    return strstr(ufs, s) != NULL;
}

/* Aceita 00000-000 ou 00000000 e guarda apenas os digitos. */
int validaCep(char *s) {
    char limpo[TAM_CEP];
    int n = 0, i;

    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] == '-' || s[i] == '.')
            continue;
        if (!isdigit((unsigned char) s[i]) || n == TAM_CEP - 1)
            return 0;
        limpo[n++] = s[i];
    }
    if (n != TAM_CEP - 1)
        return 0;
    limpo[n] = '\0';
    strcpy(s, limpo);
    return 1;
}

/* Confere o formato basico usuario@dominio.ext */
int validaEmail(char *s) {
    char *arroba = strchr(s, '@');
    char *ponto;

    if (strchr(s, ' ') != NULL || arroba == NULL || arroba == s || strchr(arroba + 1, '@') != NULL)
        return 0;
    ponto = strrchr(arroba, '.');
    return ponto != NULL && ponto > arroba + 1 && ponto[1] != '\0';
}

/* A inscricao estadual e guardada em maiusculas (ex.: "isento" -> "ISENTO"). */
int validaIE(char *s) {
    int i;

    for (i = 0; s[i] != '\0'; i++)
        s[i] = (char) toupper((unsigned char) s[i]);
    return 1;
}

void formatarCnpj(const char *cnpj, char *buf) {
    snprintf(buf, TAM_CNPJ_FMT, "%.2s.%.3s.%.3s/%.4s-%.2s",
             cnpj, cnpj + 2, cnpj + 5, cnpj + 8, cnpj + 12);
}

void formatarCep(const char *cep, char *buf) {
    snprintf(buf, TAM_CEP_FMT, "%.5s-%.3s", cep, cep + 5);
}

void formatarData(Data d, char *buf) {
    if (dataVazia(d))
        strcpy(buf, "--/--/----");
    else
        snprintf(buf, TAM_DATA, "%02d/%02d/%04d", d.dia, d.mes, d.ano);
}

const char *textoOuPadrao(const char *s) {
    return s[0] != '\0' ? s : "(nao informado)";
}

const char *nomeFormaPagamento(int forma) {
    if (forma == PAGTO_A_VISTA)
        return "A vista";
    if (forma == PAGTO_DUPLICATA)
        return "Duplicata";
    return "(nao informada)";
}

/* ======================================================================
 * Datas
 * ====================================================================== */

int anoBissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0;
}

int diasNoMes(int mes, int ano) {
    const int dias[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (mes == 2 && anoBissexto(ano))
        return 29;
    return dias[mes - 1];
}

int dataValida(Data d) {
    return d.ano >= 1900 && d.ano <= 2100 && d.mes >= 1 && d.mes <= 12 &&
           d.dia >= 1 && d.dia <= diasNoMes(d.mes, d.ano);
}

/* Data zerada = ainda nao informada (usada na data de entrega) */
int dataVazia(Data d) {
    return d.dia == 0 && d.mes == 0 && d.ano == 0;
}

/* Converte "dd/mm/aaaa" em Data. Retorna 0 se o texto nao for uma data valida. */
int converterData(const char *s, Data *d) {
    Data nova;
    char sobra;

    if (sscanf(s, "%d/%d/%d%c", &nova.dia, &nova.mes, &nova.ano, &sobra) != 3 || !dataValida(nova))
        return 0;
    *d = nova;
    return 1;
}

/* Numero de dias desde uma origem fixa: a diferenca entre duas datas
   e a diferenca entre seus valores. */
long dataParaDias(Data d) {
    long a = d.ano, m = d.mes;

    if (m <= 2) {           /* conta o ano a partir de marco */
        a--;
        m += 12;
    }
    return 365 * a + a / 4 - a / 100 + a / 400 + (153 * (m - 3) + 2) / 5 + d.dia - 1;
}

/* Negativo se a < b, zero se iguais, positivo se a > b */
int compararDatas(Data a, Data b) {
    long da = dataParaDias(a), db = dataParaDias(b);

    return (da > db) - (da < db);
}

Data somarDias(Data d, int dias) {
    while (dias > 0) {
        d.dia++;
        if (d.dia > diasNoMes(d.mes, d.ano)) {
            d.dia = 1;
            d.mes++;
            if (d.mes > 12) {
                d.mes = 1;
                d.ano++;
            }
        }
        dias--;
    }
    return d;
}

Data dataHoje(void) {
    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);
    Data d;

    d.dia = t->tm_mday;
    d.mes = t->tm_mon + 1;
    d.ano = t->tm_year + 1900;
    return d;
}

/* ======================================================================
 * Arquivos binarios
 * ====================================================================== */

/* Le ate 'max' registros do arquivo para o vetor e retorna quantos leu.
   Se o arquivo ainda nao existir, retorna 0. */
int carregarArquivo(const char *nome, void *vetor, size_t tamRegistro, int max) {
    FILE *arq = fopen(nome, "rb");
    int qtd;

    if (arq == NULL)
        return 0;
    qtd = (int) fread(vetor, tamRegistro, max, arq);
    if (qtd == max && fgetc(arq) != EOF)
        printf("Aviso: %s tem mais de %d registros; apenas os %d primeiros foram carregados.\n",
               nome, max, max);
    fclose(arq);
    return qtd;
}

/* Regrava o arquivo inteiro com os 'qtd' registros do vetor. */
int gravarArquivo(const char *nome, const void *vetor, size_t tamRegistro, int qtd) {
    FILE *arq = fopen(nome, "wb");
    int ok;

    if (arq == NULL) {
        printf("Erro: nao foi possivel abrir %s para gravacao.\n", nome);
        return 0;
    }
    ok = fwrite(vetor, tamRegistro, qtd, arq) == (size_t) qtd;
    if (fclose(arq) != 0)
        ok = 0;
    if (!ok)
        printf("Erro ao gravar %s.\n", nome);
    return ok;
}

int salvarClientes(const Cliente clientes[], int qtdClientes) {
    return gravarArquivo(ARQ_CLIENTES, clientes, sizeof(Cliente), qtdClientes);
}

int salvarPedidos(const Pedido pedidos[], int qtdPedidos) {
    return gravarArquivo(ARQ_PEDIDOS, pedidos, sizeof(Pedido), qtdPedidos);
}

/* ======================================================================
 * Buscas
 * ====================================================================== */

int buscarCliente(const Cliente clientes[], int qtdClientes, const char *cnpj) {
    int i;

    for (i = 0; i < qtdClientes; i++)
        if (strcmp(clientes[i].cnpj, cnpj) == 0)
            return i;
    return -1;
}

int buscarPedido(const Pedido pedidos[], int qtdPedidos, int numero) {
    int i;

    for (i = 0; i < qtdPedidos; i++)
        if (pedidos[i].numero == numero)
            return i;
    return -1;
}

int contarPedidosCliente(const Pedido pedidos[], int qtdPedidos, const char *cnpj) {
    int i, n = 0;

    for (i = 0; i < qtdPedidos; i++)
        if (strcmp(pedidos[i].cnpjCliente, cnpj) == 0)
            n++;
    return n;
}

int proximoNumeroPedido(const Pedido pedidos[], int qtdPedidos) {
    int i, maior = 0;

    for (i = 0; i < qtdPedidos; i++)
        if (pedidos[i].numero > maior)
            maior = pedidos[i].numero;
    return maior + 1;
}

/* Pede um CNPJ e retorna o indice do cliente, ou -1 (nao encontrado ou cancelado). */
int selecionarCliente(const Cliente clientes[], int qtdClientes) {
    char cnpj[TAM_CNPJ] = "";
    int i;

    if (qtdClientes == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return -1;
    }
    lerCampo("CNPJ do cliente (ENTER cancela): ", cnpj, TAM_CNPJ, 0, validaCnpj, MSG_CNPJ_INVALIDO);
    if (cnpj[0] == '\0')
        return -1;
    i = buscarCliente(clientes, qtdClientes, cnpj);
    if (i < 0)
        printf("Cliente nao cadastrado.\n");
    return i;
}

/* Pede o numero de um pedido e retorna seu indice, ou -1. */
int selecionarPedido(const Pedido pedidos[], int qtdPedidos) {
    int numero = 0, i;

    if (qtdPedidos == 0) {
        printf("Nenhum pedido cadastrado.\n");
        return -1;
    }
    lerInteiro("Numero do pedido (ENTER cancela): ", &numero, 1, 999999, 0);
    if (numero == 0)
        return -1;
    i = buscarPedido(pedidos, qtdPedidos, numero);
    if (i < 0)
        printf("Pedido nao encontrado.\n");
    return i;
}

/* ======================================================================
 * Clientes
 * ====================================================================== */

void imprimirCliente(const Cliente *c) {
    char cnpj[TAM_CNPJ_FMT], cep[TAM_CEP_FMT];

    formatarCnpj(c->cnpj, cnpj);
    formatarCep(c->endereco.cep, cep);
    printf("------------------------------------------------------------\n");
    printf("CNPJ...............: %s\n", cnpj);
    printf("Nome...............: %s\n", c->nome);
    printf("Razao social.......: %s\n", c->razaoSocial);
    printf("Endereco...........: %s\n", c->endereco.logradouro);
    printf("                     %s/%s - CEP %s\n", c->endereco.cidade, c->endereco.estado, cep);
    printf("Inscricao estadual.: %s\n", c->inscricaoEstadual);
    printf("Telefone...........: %s\n", textoOuPadrao(c->telefone));
    printf("Email..............: %s\n", textoOuPadrao(c->email));
    printf("Contato............: %s\n", textoOuPadrao(c->contato));
}

/* Le os dados do cliente (exceto o CNPJ). Na alteracao (edicao = 1) os
   valores atuais aparecem entre colchetes e ENTER os mantem. */
void preencherCliente(Cliente *c, int edicao) {
    char prompt[TAM_LINHA], cep[TAM_CEP_FMT];
    int obrig = !edicao;

    montarPrompt(prompt, TAM_LINHA, "Nome do cliente", edicao ? c->nome : NULL);
    lerCampo(prompt, c->nome, TAM_NOME, obrig, NULL, NULL);

    montarPrompt(prompt, TAM_LINHA, "Razao social", edicao ? c->razaoSocial : NULL);
    lerCampo(prompt, c->razaoSocial, TAM_RAZAO, obrig, NULL, NULL);

    printf("Endereco\n");
    montarPrompt(prompt, TAM_LINHA, "  Rua/Av. e numero", edicao ? c->endereco.logradouro : NULL);
    lerCampo(prompt, c->endereco.logradouro, TAM_LOGRADOURO, obrig, NULL, NULL);

    montarPrompt(prompt, TAM_LINHA, "  Cidade", edicao ? c->endereco.cidade : NULL);
    lerCampo(prompt, c->endereco.cidade, TAM_CIDADE, obrig, NULL, NULL);

    montarPrompt(prompt, TAM_LINHA, "  Estado (UF)", edicao ? c->endereco.estado : NULL);
    lerCampo(prompt, c->endereco.estado, TAM_UF, obrig, validaUF, "UF invalida (ex.: RJ, SP, MG).");

    formatarCep(c->endereco.cep, cep);
    montarPrompt(prompt, TAM_LINHA, "  CEP", edicao ? cep : NULL);
    lerCampo(prompt, c->endereco.cep, TAM_CEP, obrig, validaCep, "CEP invalido. Informe 8 digitos.");

    if (edicao)
        montarPrompt(prompt, TAM_LINHA, "Inscricao estadual (digite ISENTO se nao tiver)", c->inscricaoEstadual);
    else
        montarPrompt(prompt, TAM_LINHA, "Inscricao estadual (ENTER = ISENTO)", NULL);
    lerCampo(prompt, c->inscricaoEstadual, TAM_IE, 0, validaIE, NULL);
    if (c->inscricaoEstadual[0] == '\0')
        strcpy(c->inscricaoEstadual, "ISENTO");

    montarPrompt(prompt, TAM_LINHA, "Telefone", edicao ? c->telefone : NULL);
    lerCampo(prompt, c->telefone, TAM_TELEFONE, 0, NULL, NULL);

    montarPrompt(prompt, TAM_LINHA, "Email", edicao ? c->email : NULL);
    lerCampo(prompt, c->email, TAM_EMAIL, 0, validaEmail, "Email invalido (ex.: nome@empresa.com.br).");

    montarPrompt(prompt, TAM_LINHA, "Contato (nome do contato no cliente)", edicao ? c->contato : NULL);
    lerCampo(prompt, c->contato, TAM_NOME, 0, NULL, NULL);
}

void inserirCliente(Cliente clientes[], int *qtdClientes) {
    Cliente novo;

    printf("\n--- Inserir cliente ---\n");
    if (*qtdClientes >= MAX_CLIENTES) {
        printf("Limite de %d clientes atingido.\n", MAX_CLIENTES);
        return;
    }
    memset(&novo, 0, sizeof(novo));
    lerCampo("CNPJ (ENTER cancela): ", novo.cnpj, TAM_CNPJ, 0, validaCnpj, MSG_CNPJ_INVALIDO);
    if (novo.cnpj[0] == '\0')
        return;
    if (buscarCliente(clientes, *qtdClientes, novo.cnpj) >= 0) {
        printf("Ja existe um cliente cadastrado com este CNPJ.\n");
        return;
    }
    preencherCliente(&novo, 0);

    clientes[*qtdClientes] = novo;
    (*qtdClientes)++;
    if (salvarClientes(clientes, *qtdClientes))
        printf("Cliente cadastrado com sucesso.\n");
}

void alterarCliente(Cliente clientes[], int qtdClientes) {
    Cliente editado;
    int i;

    printf("\n--- Alterar cliente ---\n");
    i = selecionarCliente(clientes, qtdClientes);
    if (i < 0)
        return;
    imprimirCliente(&clientes[i]);
    printf("Informe os novos dados (ENTER mantem o valor entre colchetes).\n");
    printf("O CNPJ identifica o cliente e nao pode ser alterado.\n");

    editado = clientes[i];
    preencherCliente(&editado, 1);
    clientes[i] = editado;
    if (salvarClientes(clientes, qtdClientes))
        printf("Cliente alterado com sucesso.\n");
}

/* Exclui o cliente e, apos confirmacao, tambem os pedidos dele. */
void excluirCliente(Cliente clientes[], int *qtdClientes, Pedido pedidos[], int *qtdPedidos) {
    int i, j, n, qtdRestante = 0;

    printf("\n--- Excluir cliente ---\n");
    i = selecionarCliente(clientes, *qtdClientes);
    if (i < 0)
        return;
    imprimirCliente(&clientes[i]);

    n = contarPedidosCliente(pedidos, *qtdPedidos, clientes[i].cnpj);
    if (n > 0)
        printf("Atencao: este cliente possui %d pedido(s), que tambem serao excluidos.\n", n);
    if (!lerSimNao("Confirma a exclusao? (S/N): ")) {
        printf("Exclusao cancelada.\n");
        return;
    }

    if (n > 0) {
        for (j = 0; j < *qtdPedidos; j++)
            if (strcmp(pedidos[j].cnpjCliente, clientes[i].cnpj) != 0)
                pedidos[qtdRestante++] = pedidos[j];
        *qtdPedidos = qtdRestante;
        salvarPedidos(pedidos, *qtdPedidos);
    }

    for (j = i; j < *qtdClientes - 1; j++)
        clientes[j] = clientes[j + 1];
    (*qtdClientes)--;
    if (salvarClientes(clientes, *qtdClientes))
        printf("Cliente excluido com sucesso.\n");
}

void consultarClientes(const Cliente clientes[], int qtdClientes) {
    int opcao, i;

    printf("\n--- Consultar clientes ---\n");
    printf("1 - Consultar por CNPJ\n");
    printf("2 - Listar todos os clientes\n");
    printf("0 - Voltar\n");
    lerInteiro("Opcao: ", &opcao, 0, 2, 1);

    if (opcao == 1) {
        i = selecionarCliente(clientes, qtdClientes);
        if (i >= 0)
            imprimirCliente(&clientes[i]);
    } else if (opcao == 2) {
        if (qtdClientes == 0) {
            printf("Nenhum cliente cadastrado.\n");
            return;
        }
        for (i = 0; i < qtdClientes; i++)
            imprimirCliente(&clientes[i]);
        printf("------------------------------------------------------------\n");
        printf("%d cliente(s) cadastrado(s).\n", qtdClientes);
    }
}

void menuClientes(Cliente clientes[], int *qtdClientes, Pedido pedidos[], int *qtdPedidos) {
    int opcao;

    do {
        printf("\n========== CLIENTES (%d/%d) ==========\n", *qtdClientes, MAX_CLIENTES);
        printf("1 - Inserir\n");
        printf("2 - Alterar\n");
        printf("3 - Excluir\n");
        printf("4 - Consultar\n");
        printf("0 - Voltar\n");
        lerInteiro("Opcao: ", &opcao, 0, 4, 1);

        switch (opcao) {
        case 1:
            inserirCliente(clientes, qtdClientes);
            break;
        case 2:
            alterarCliente(clientes, *qtdClientes);
            break;
        case 3:
            excluirCliente(clientes, qtdClientes, pedidos, qtdPedidos);
            break;
        case 4:
            consultarClientes(clientes, *qtdClientes);
            break;
        }
    } while (opcao != 0);
}

/* ======================================================================
 * Pedidos
 * ====================================================================== */

double calcularTotal(const Pedido *p) {
    double total = 0;
    int i;

    for (i = 0; i < p->qtdItens; i++)
        total += p->itens[i].quantidade * p->itens[i].valorUnitario;
    return total;
}

/* Data limite para a entrega: data do pedido + previsao (em dias) */
Data dataPrevista(const Pedido *p) {
    return somarDias(p->dataPedido, p->previsaoEntrega);
}

void situacaoPedido(const Pedido *p, Data hoje, char *buf, int tam) {
    char data[TAM_DATA];
    long dias;

    if (!dataVazia(p->dataEntrega)) {
        formatarData(p->dataEntrega, data);
        dias = dataParaDias(p->dataEntrega) - dataParaDias(dataPrevista(p));
        if (dias > 0)
            snprintf(buf, tam, "Entregue em %s (com %ld dia(s) de atraso)", data, dias);
        else
            snprintf(buf, tam, "Entregue em %s (no prazo)", data);
    } else {
        dias = dataParaDias(dataPrevista(p)) - dataParaDias(hoje);
        if (dias >= 0)
            snprintf(buf, tam, "Em aberto - no prazo (faltam %ld dia(s))", dias);
        else
            snprintf(buf, tam, "Em aberto - FORA DO PRAZO (atrasado ha %ld dia(s))", -dias);
    }
}

void imprimirPedido(const Pedido *p, const Cliente clientes[], int qtdClientes, Data hoje) {
    char cnpj[TAM_CNPJ_FMT], data[TAM_DATA], prevista[TAM_DATA], situacao[TAM_LINHA];
    int c = buscarCliente(clientes, qtdClientes, p->cnpjCliente);
    int i;

    formatarCnpj(p->cnpjCliente, cnpj);
    formatarData(p->dataPedido, data);
    formatarData(dataPrevista(p), prevista);
    situacaoPedido(p, hoje, situacao, TAM_LINHA);

    printf("------------------------------------------------------------\n");
    printf("Pedido no %d - data do pedido: %s\n", p->numero, data);
    printf("Cliente............: %s - %s\n", cnpj,
           c >= 0 ? clientes[c].nome : "(cliente nao cadastrado)");
    printf("Previsao de entrega: %d dia(s) (ate %s)\n", p->previsaoEntrega, prevista);
    printf("Situacao...........: %s\n", situacao);
    printf("  #  %-20s %8s %12s %12s\n", "Descricao", "Qtd", "Valor unit.", "Subtotal");
    for (i = 0; i < p->qtdItens; i++) {
        const Item *it = &p->itens[i];
        printf("  %d  %-20s %8d %12.2f %12.2f\n", i + 1, it->descricao, it->quantidade,
               it->valorUnitario, it->quantidade * it->valorUnitario);
    }
    printf("Total do pedido....: R$ %.2f\n", p->total);
    printf("Forma de pagamento.: %s\n", nomeFormaPagamento(p->formaPagamento));
}

void imprimirResumo(int qtd, double soma) {
    if (qtd == 0) {
        printf("Nenhum pedido encontrado.\n");
        return;
    }
    printf("------------------------------------------------------------\n");
    printf("%d pedido(s) - valor total: R$ %.2f\n", qtd, soma);
}

/* Le o CNPJ do cliente do pedido, que precisa estar cadastrado.
   Retorna 0 se a operacao for cancelada ou o cliente nao existir. */
int lerClientePedido(Pedido *p, const Cliente clientes[], int qtdClientes, int edicao) {
    char prompt[TAM_LINHA], cnpjAtual[TAM_CNPJ_FMT], cnpj[TAM_CNPJ];
    int c;

    strcpy(cnpj, p->cnpjCliente);
    if (edicao) {
        formatarCnpj(p->cnpjCliente, cnpjAtual);
        montarPrompt(prompt, TAM_LINHA, "CNPJ do cliente", cnpjAtual);
    } else {
        montarPrompt(prompt, TAM_LINHA, "CNPJ do cliente (ENTER cancela)", NULL);
    }
    lerCampo(prompt, cnpj, TAM_CNPJ, 0, validaCnpj, MSG_CNPJ_INVALIDO);
    if (cnpj[0] == '\0')
        return 0;

    c = buscarCliente(clientes, qtdClientes, cnpj);
    if (c < 0) {
        printf("Cliente nao cadastrado. Cadastre o cliente antes de registrar seus pedidos.\n");
        return 0;
    }
    strcpy(p->cnpjCliente, cnpj);
    printf("  Cliente: %s\n", clientes[c].nome);
    return 1;
}

/* Data da entrega: ENTER mantem a atual, N marca o pedido como nao entregue. */
void lerDataEntrega(Pedido *p) {
    char buf[TAM_LINHA], prompt[TAM_LINHA], atual[TAM_DATA];
    Data d;
    int n;

    if (dataVazia(p->dataEntrega))
        strcpy(atual, "nao entregue");
    else
        formatarData(p->dataEntrega, atual);
    snprintf(prompt, TAM_LINHA, "Data da entrega (dd/mm/aaaa ou N = nao entregue) [%s]: ", atual);

    for (;;) {
        n = lerLinha(prompt, buf, TAM_LINHA);
        if (n == 0) {
            d = p->dataEntrega;
        } else if (n == 1 && toupper((unsigned char) buf[0]) == 'N') {
            memset(&p->dataEntrega, 0, sizeof(Data));
            return;
        } else if (!converterData(buf, &d)) {
            printf("  Data invalida. Use o formato dd/mm/aaaa.\n");
            continue;
        }
        if (!dataVazia(d) && compararDatas(d, p->dataPedido) < 0) {
            printf("  A data da entrega nao pode ser anterior a data do pedido.\n");
            continue;
        }
        p->dataEntrega = d;
        return;
    }
}

/* Le os itens do pedido e recalcula o total. Na alteracao, os itens que
   ja existiam mostram seus valores atuais e ENTER os mantem. */
void preencherItens(Pedido *p, int edicao) {
    char prompt[TAM_LINHA], atual[TAM_VALOR];
    int qtdAnterior = edicao ? p->qtdItens : 0;
    int i;

    snprintf(atual, TAM_VALOR, "%d", p->qtdItens);
    montarPrompt(prompt, TAM_LINHA, "Quantidade de itens (1 a 6)", edicao ? atual : NULL);
    lerInteiro(prompt, &p->qtdItens, 1, MAX_ITENS, !edicao);

    for (i = 0; i < MAX_ITENS; i++) {
        Item *it = &p->itens[i];
        int existe = i < qtdAnterior;

        if (i >= p->qtdItens) {     /* posicoes sem item ficam zeradas */
            memset(it, 0, sizeof(Item));
            continue;
        }
        printf("Item %d\n", i + 1);
        montarPrompt(prompt, TAM_LINHA, "  Descricao (ate 20 caracteres)", existe ? it->descricao : NULL);
        lerCampo(prompt, it->descricao, TAM_DESCRICAO, !existe, NULL, NULL);

        snprintf(atual, TAM_VALOR, "%d", it->quantidade);
        montarPrompt(prompt, TAM_LINHA, "  Quantidade", existe ? atual : NULL);
        lerInteiro(prompt, &it->quantidade, 1, 1000000, !existe);

        snprintf(atual, TAM_VALOR, "%.2f", it->valorUnitario);
        montarPrompt(prompt, TAM_LINHA, "  Valor unitario (R$)", existe ? atual : NULL);
        lerReal(prompt, &it->valorUnitario, 0.01, 1000000.0, !existe);
    }
    p->total = calcularTotal(p);
}

/* Le os dados do pedido (exceto o numero). Retorna 0 se for cancelado. */
int preencherPedido(Pedido *p, const Cliente clientes[], int qtdClientes, int edicao) {
    char prompt[TAM_LINHA], atual[TAM_VALOR];

    if (!lerClientePedido(p, clientes, qtdClientes, edicao))
        return 0;

    formatarData(p->dataPedido, atual);
    montarPrompt(prompt, TAM_LINHA, "Data do pedido (dd/mm/aaaa)", atual);
    lerData(prompt, &p->dataPedido, 0);

    snprintf(atual, TAM_VALOR, "%d", p->previsaoEntrega);
    montarPrompt(prompt, TAM_LINHA, "Previsao de entrega (numero de dias)", edicao ? atual : NULL);
    lerInteiro(prompt, &p->previsaoEntrega, 0, 3650, !edicao);

    lerDataEntrega(p);
    preencherItens(p, edicao);
    printf("Total do pedido: R$ %.2f\n", p->total);

    printf("Forma de pagamento: %d - A vista | %d - Duplicata\n", PAGTO_A_VISTA, PAGTO_DUPLICATA);
    snprintf(atual, TAM_VALOR, "%d", p->formaPagamento);
    montarPrompt(prompt, TAM_LINHA, "Opcao", edicao ? atual : NULL);
    lerInteiro(prompt, &p->formaPagamento, PAGTO_A_VISTA, PAGTO_DUPLICATA, !edicao);
    return 1;
}

void inserirPedido(Pedido pedidos[], int *qtdPedidos, const Cliente clientes[], int qtdClientes) {
    Pedido novo;

    printf("\n--- Inserir pedido ---\n");
    if (*qtdPedidos >= MAX_PEDIDOS) {
        printf("Limite de %d pedidos atingido.\n", MAX_PEDIDOS);
        return;
    }
    if (qtdClientes == 0) {
        printf("Nenhum cliente cadastrado. Cadastre um cliente antes de inserir pedidos.\n");
        return;
    }
    memset(&novo, 0, sizeof(novo));
    novo.numero = proximoNumeroPedido(pedidos, *qtdPedidos);
    novo.dataPedido = dataHoje();
    printf("Pedido numero %d (ENTER aceita o valor entre colchetes)\n", novo.numero);
    if (!preencherPedido(&novo, clientes, qtdClientes, 0)) {
        printf("Pedido nao cadastrado.\n");
        return;
    }

    pedidos[*qtdPedidos] = novo;
    (*qtdPedidos)++;
    if (salvarPedidos(pedidos, *qtdPedidos))
        printf("Pedido %d cadastrado com sucesso.\n", novo.numero);
}

void alterarPedido(Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes) {
    Pedido editado;
    int i;

    printf("\n--- Alterar pedido ---\n");
    i = selecionarPedido(pedidos, qtdPedidos);
    if (i < 0)
        return;
    imprimirPedido(&pedidos[i], clientes, qtdClientes, dataHoje());
    printf("Informe os novos dados (ENTER mantem o valor entre colchetes).\n");

    /* altera uma copia, para nao perder os dados se a alteracao for cancelada */
    editado = pedidos[i];
    if (!preencherPedido(&editado, clientes, qtdClientes, 1)) {
        printf("Alteracao cancelada.\n");
        return;
    }
    pedidos[i] = editado;
    if (salvarPedidos(pedidos, qtdPedidos))
        printf("Pedido alterado com sucesso.\n");
}

void excluirPedido(Pedido pedidos[], int *qtdPedidos, const Cliente clientes[], int qtdClientes) {
    int i, j;

    printf("\n--- Excluir pedido ---\n");
    i = selecionarPedido(pedidos, *qtdPedidos);
    if (i < 0)
        return;
    imprimirPedido(&pedidos[i], clientes, qtdClientes, dataHoje());
    if (!lerSimNao("Confirma a exclusao? (S/N): ")) {
        printf("Exclusao cancelada.\n");
        return;
    }

    for (j = i; j < *qtdPedidos - 1; j++)
        pedidos[j] = pedidos[j + 1];
    (*qtdPedidos)--;
    if (salvarPedidos(pedidos, *qtdPedidos))
        printf("Pedido excluido com sucesso.\n");
}

/* a) Todos os pedidos feitos por um cliente */
void consultarPedidosCliente(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes) {
    Data hoje = dataHoje();
    double soma = 0;
    int c, i, n = 0;

    printf("\n--- Pedidos de um cliente ---\n");
    c = selecionarCliente(clientes, qtdClientes);
    if (c < 0)
        return;
    printf("Cliente: %s\n", clientes[c].nome);
    for (i = 0; i < qtdPedidos; i++) {
        if (strcmp(pedidos[i].cnpjCliente, clientes[c].cnpj) == 0) {
            imprimirPedido(&pedidos[i], clientes, qtdClientes, hoje);
            soma += pedidos[i].total;
            n++;
        }
    }
    imprimirResumo(n, soma);
}

/* b) Todos os pedidos realizados no intervalo entre duas datas */
void consultarPedidosPeriodo(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes) {
    Data hoje = dataHoje(), inicio, fim;
    double soma = 0;
    int i, n = 0;

    printf("\n--- Pedidos realizados entre duas datas ---\n");
    lerData("Data inicial (dd/mm/aaaa): ", &inicio, 1);
    for (;;) {
        lerData("Data final (dd/mm/aaaa): ", &fim, 1);
        if (compararDatas(fim, inicio) >= 0)
            break;
        printf("  A data final deve ser igual ou posterior a data inicial.\n");
    }

    for (i = 0; i < qtdPedidos; i++) {
        if (compararDatas(pedidos[i].dataPedido, inicio) >= 0 &&
            compararDatas(pedidos[i].dataPedido, fim) <= 0) {
            imprimirPedido(&pedidos[i], clientes, qtdClientes, hoje);
            soma += pedidos[i].total;
            n++;
        }
    }
    imprimirResumo(n, soma);
}

/* c) Pedidos ja entregues cuja data de entrada (data do pedido) e igual
      ou posterior a data informada */
void consultarPedidosEntregues(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes) {
    Data hoje = dataHoje(), inicio;
    double soma = 0;
    int i, n = 0;

    printf("\n--- Pedidos entregues a partir de uma data de entrada ---\n");
    lerData("Data de entrada inicial (dd/mm/aaaa): ", &inicio, 1);

    for (i = 0; i < qtdPedidos; i++) {
        if (!dataVazia(pedidos[i].dataEntrega) && compararDatas(pedidos[i].dataPedido, inicio) >= 0) {
            imprimirPedido(&pedidos[i], clientes, qtdClientes, hoje);
            soma += pedidos[i].total;
            n++;
        }
    }
    imprimirResumo(n, soma);
}

/* d) Pedidos em aberto: primeiro os que estao no prazo, depois os atrasados */
void consultarPedidosEmAberto(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes) {
    Data hoje = dataHoje();
    char data[TAM_DATA];
    double soma = 0;
    int i, grupo, n, total = 0;

    formatarData(hoje, data);
    printf("\n--- Pedidos em aberto (data de hoje: %s) ---\n", data);

    for (grupo = 0; grupo < 2; grupo++) {
        printf("\n>>> %s\n", grupo == 0 ? "PEDIDOS EM ABERTO NO PRAZO" : "PEDIDOS EM ABERTO FORA DO PRAZO");
        n = 0;
        for (i = 0; i < qtdPedidos; i++) {
            int noPrazo = compararDatas(hoje, dataPrevista(&pedidos[i])) <= 0;
            if (dataVazia(pedidos[i].dataEntrega) && noPrazo == (grupo == 0)) {
                imprimirPedido(&pedidos[i], clientes, qtdClientes, hoje);
                soma += pedidos[i].total;
                n++;
            }
        }
        if (n == 0)
            printf("Nenhum pedido.\n");
        total += n;
    }
    imprimirResumo(total, soma);
}

/* e) Media mensal de valores comprados pelo cliente */
void consultarMediaMensal(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes) {
    TotalMes meses[MAX_PEDIDOS];
    double soma = 0;
    int qtdMeses = 0, qtdPeriodo, c, i, j;

    printf("\n--- Media mensal de compras do cliente ---\n");
    c = selecionarCliente(clientes, qtdClientes);
    if (c < 0)
        return;

    /* agrupa os pedidos por mes/ano da data do pedido, em ordem cronologica */
    for (i = 0; i < qtdPedidos; i++) {
        const Pedido *p = &pedidos[i];
        int chave = p->dataPedido.ano * 12 + p->dataPedido.mes - 1;

        if (strcmp(p->cnpjCliente, clientes[c].cnpj) != 0)
            continue;
        for (j = 0; j < qtdMeses && meses[j].ano * 12 + meses[j].mes - 1 < chave; j++)
            ;
        if (j == qtdMeses || meses[j].ano * 12 + meses[j].mes - 1 != chave) {
            memmove(&meses[j + 1], &meses[j], (qtdMeses - j) * sizeof(TotalMes));
            meses[j].mes = p->dataPedido.mes;
            meses[j].ano = p->dataPedido.ano;
            meses[j].qtdPedidos = 0;
            meses[j].total = 0;
            qtdMeses++;
        }
        meses[j].qtdPedidos++;
        meses[j].total += p->total;
        soma += p->total;
    }

    printf("Cliente: %s\n", clientes[c].nome);
    if (qtdMeses == 0) {
        printf("Nenhum pedido encontrado para este cliente.\n");
        return;
    }
    printf("\n  Mes/Ano   Pedidos   Total comprado (R$)\n");
    for (j = 0; j < qtdMeses; j++)
        printf("  %02d/%04d   %7d   %19.2f\n", meses[j].mes, meses[j].ano, meses[j].qtdPedidos, meses[j].total);

    qtdPeriodo = (meses[qtdMeses - 1].ano * 12 + meses[qtdMeses - 1].mes) -
                 (meses[0].ano * 12 + meses[0].mes) + 1;
    printf("\nTotal comprado: R$ %.2f\n", soma);
    printf("Media mensal considerando os meses com compras (%d mes(es)): R$ %.2f\n",
           qtdMeses, soma / qtdMeses);
    printf("Media mensal no periodo de %02d/%04d a %02d/%04d (%d mes(es)): R$ %.2f\n",
           meses[0].mes, meses[0].ano, meses[qtdMeses - 1].mes, meses[qtdMeses - 1].ano,
           qtdPeriodo, soma / qtdPeriodo);
}

void consultarPedidoNumero(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes) {
    int i;

    printf("\n--- Consultar pedido pelo numero ---\n");
    i = selecionarPedido(pedidos, qtdPedidos);
    if (i >= 0)
        imprimirPedido(&pedidos[i], clientes, qtdClientes, dataHoje());
}

void listarPedidos(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes) {
    Data hoje = dataHoje();
    double soma = 0;
    int i;

    printf("\n--- Todos os pedidos ---\n");
    for (i = 0; i < qtdPedidos; i++) {
        imprimirPedido(&pedidos[i], clientes, qtdClientes, hoje);
        soma += pedidos[i].total;
    }
    imprimirResumo(qtdPedidos, soma);
}

void menuConsultaPedidos(const Pedido pedidos[], int qtdPedidos, const Cliente clientes[], int qtdClientes) {
    int opcao;

    do {
        printf("\n========== CONSULTAR PEDIDOS ==========\n");
        printf("1 - Todos os pedidos feitos por um cliente\n");
        printf("2 - Todos os pedidos realizados entre duas datas\n");
        printf("3 - Pedidos entregues a partir de uma data de entrada\n");
        printf("4 - Pedidos em aberto (no prazo e fora do prazo)\n");
        printf("5 - Media mensal de valores comprados pelo cliente\n");
        printf("6 - Consultar um pedido pelo numero\n");
        printf("7 - Listar todos os pedidos\n");
        printf("0 - Voltar\n");
        lerInteiro("Opcao: ", &opcao, 0, 7, 1);

        switch (opcao) {
        case 1:
            consultarPedidosCliente(pedidos, qtdPedidos, clientes, qtdClientes);
            break;
        case 2:
            consultarPedidosPeriodo(pedidos, qtdPedidos, clientes, qtdClientes);
            break;
        case 3:
            consultarPedidosEntregues(pedidos, qtdPedidos, clientes, qtdClientes);
            break;
        case 4:
            consultarPedidosEmAberto(pedidos, qtdPedidos, clientes, qtdClientes);
            break;
        case 5:
            consultarMediaMensal(pedidos, qtdPedidos, clientes, qtdClientes);
            break;
        case 6:
            consultarPedidoNumero(pedidos, qtdPedidos, clientes, qtdClientes);
            break;
        case 7:
            listarPedidos(pedidos, qtdPedidos, clientes, qtdClientes);
            break;
        }
    } while (opcao != 0);
}

void menuPedidos(Pedido pedidos[], int *qtdPedidos, const Cliente clientes[], int qtdClientes) {
    int opcao;

    do {
        printf("\n========== PEDIDOS (%d/%d) ==========\n", *qtdPedidos, MAX_PEDIDOS);
        printf("1 - Inserir\n");
        printf("2 - Alterar\n");
        printf("3 - Excluir\n");
        printf("4 - Consultar\n");
        printf("0 - Voltar\n");
        lerInteiro("Opcao: ", &opcao, 0, 4, 1);

        switch (opcao) {
        case 1:
            inserirPedido(pedidos, qtdPedidos, clientes, qtdClientes);
            break;
        case 2:
            alterarPedido(pedidos, *qtdPedidos, clientes, qtdClientes);
            break;
        case 3:
            excluirPedido(pedidos, qtdPedidos, clientes, qtdClientes);
            break;
        case 4:
            menuConsultaPedidos(pedidos, *qtdPedidos, clientes, qtdClientes);
            break;
        }
    } while (opcao != 0);
}

/* ======================================================================
 * Programa principal
 * ====================================================================== */

int main(void) {
    Cliente clientes[MAX_CLIENTES];
    Pedido pedidos[MAX_PEDIDOS];
    int qtdClientes, qtdPedidos, opcao;

    qtdClientes = carregarArquivo(ARQ_CLIENTES, clientes, sizeof(Cliente), MAX_CLIENTES);
    qtdPedidos = carregarArquivo(ARQ_PEDIDOS, pedidos, sizeof(Pedido), MAX_PEDIDOS);
    printf("Carregados %d cliente(s) de %s e %d pedido(s) de %s.\n",
           qtdClientes, ARQ_CLIENTES, qtdPedidos, ARQ_PEDIDOS);

    do {
        printf("\n==========================================\n");
        printf("   CONTROLE DE CLIENTES E PEDIDOS\n");
        printf("==========================================\n");
        printf("1 - Clientes\n");
        printf("2 - Pedidos\n");
        printf("0 - Sair\n");
        lerInteiro("Opcao: ", &opcao, 0, 2, 1);

        switch (opcao) {
        case 1:
            menuClientes(clientes, &qtdClientes, pedidos, &qtdPedidos);
            break;
        case 2:
            menuPedidos(pedidos, &qtdPedidos, clientes, qtdClientes);
            break;
        }
    } while (opcao != 0);

    printf("Ate logo!\n");
    return 0;
}
