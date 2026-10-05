Trabalho 1 - Controle de Clientes
=================================

UERJ - IME/DICC - Linguagem de Programação I

Programa em C que processa informações sobre clientes e seus pedidos,
armazenadas em dois arquivos binários: `Cliente.dat` e `Pedidos.dat`.

## Compilar e executar

```
gcc -o controle_clientes controle_clientes.c
./controle_clientes
```

Os arquivos `.dat` ficam na pasta em que o programa é executado.

## Organização

- Duas structs: `Cliente` e `Pedido`. As datas são guardadas como três
  inteiros (dia, mês e ano), e os 6 itens do pedido como vetores dentro do
  próprio `Pedido` (`descricao[6]`, `quantidade[6]` e `valor[6]`).
- Os dados ficam em vetores na `main` (10 clientes e 20 pedidos) e são
  passados por parâmetro. Não há variáveis globais.
- Ao abrir, o programa lê os dois arquivos com `fread`. Depois de cada opção
  do menu, grava os dois arquivos de novo com `fwrite`.

## Menu

```
1 - Inserir cliente        5 - Inserir pedido
2 - Alterar cliente        6 - Alterar pedido
3 - Excluir cliente        7 - Excluir pedido
4 - Consultar cliente      8 - Consultar pedidos
0 - Sair
```

A opção 8 tem as consultas pedidas no enunciado:

1. todos os pedidos feitos por um cliente;
2. todos os pedidos realizados entre duas datas;
3. pedidos entregues a partir de uma data de entrada;
4. pedidos em aberto (no prazo e fora do prazo);
5. média mensal de valores comprados por um cliente;
6. todos os pedidos.

## Decisões de implementação

- **CNPJ:** digitado só com os 14 números. Identifica o cliente e não pode
  ser alterado.
- **Inscrição estadual:** se ficar em branco, é gravada como `ISENTO`.
- **Valor do item:** o enunciado define o total como "quantidade dos itens
  vezes o valor do mesmo", então cada item guarda também seu valor unitário.
- **Número do pedido:** gerado automaticamente (maior número existente + 1).
- **Alterar:** o programa mostra o registro e pergunta qual campo mudar.
- **Excluir cliente:** só é permitido se ele não tiver pedidos.
- **Data da entrega:** fica zerada enquanto o pedido está em aberto e não pode
  ser anterior à data do pedido.
- **Prazo:** data do pedido + previsão de entrega (dias). Para fazer a conta,
  `contarDias` transforma uma data no número de dias desde 01/01/1900.
- **Comparar datas:** as datas viram um número `aaaammdd`
  (ex.: 15/09/2026 → 20260915), que pode ser comparado com `<` e `>`.
- **Consulta 3:** mostra os pedidos já entregues cuja data do pedido (a data
  de entrada) é igual ou posterior à data informada.
- **Consulta 5:** mostra o total de cada mês em que o cliente comprou e a
  média = total comprado ÷ número de meses com compras.

## Dados de exemplo (mock)

Os arquivos `Cliente.dat` e `Pedidos.dat` de exemplo têm 6 clientes e 15
pedidos (de junho a outubro de 2026). Sobram vagas para testar a inserção
(4 clientes e 5 pedidos). Para usá-los, os dois arquivos precisam estar na
pasta em que o programa é executado.

| CNPJ           | Cliente                | Cidade/UF         | Pedidos      |
|----------------|------------------------|-------------------|--------------|
| 23456789000195 | Ferragens Sao Jorge    | Rio de Janeiro/RJ | 1, 3, 10, 15 |
| 34567890000130 | Padaria Pao Dourado    | Niteroi/RJ        | 5, 8, 13     |
| 45678901000175 | Mercado Bom Preco      | Sao Paulo/SP      | 2, 6, 12     |
| 56789012000100 | Papelaria Lapis de Cor | Belo Horizonte/MG | 4, 11        |
| 67890123000116 | Farmacia Vida Leve     | Curitiba/PR       | 9            |
| 78901234000105 | Tech Solucoes Digitais | Salvador/BA       | 7, 14        |

Situações cobertas:

- pedidos entregues no prazo e com atraso (nº 3, 6 e 8);
- pedidos em aberto no prazo (nº 12, 14 e 15) e fora do prazo (nº 11 e 13).
  A situação depende da data do computador: os três "no prazo" continuam
  assim até 30/11/2026;
- pedido com os 6 itens preenchidos (nº 5) e as duas formas de pagamento;
- clientes isentos de inscrição estadual, sem email e sem contato.

Os arquivos foram gerados pelo próprio programa a partir de `mock_entrada.txt`.
Para recriá-los (por exemplo, para voltar aos dados originais depois dos
testes), apague `Cliente.dat` e `Pedidos.dat` e execute:

```
./controle_clientes < mock_entrada.txt          (Linux)
controle_clientes.exe < mock_entrada.txt        (Windows)
```
