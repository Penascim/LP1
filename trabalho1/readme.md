Trabalho 1 - Controle de Clientes
=================================

UERJ - IME/DICC - Linguagem de Programação I

Programa em C que processa informações sobre clientes e seus pedidos,
armazenadas em dois arquivos binários: `Cliente.dat` e `Pedidos.dat`.

## Compilar e executar

```
gcc -Wall -Wextra -std=c99 -o controle_clientes controle_clientes.c
./controle_clientes
```

Os arquivos `.dat` são criados na pasta em que o programa é executado.

## Organização

- Os registros ficam em vetores na memória (`MAX_CLIENTES = 10` e
  `MAX_PEDIDOS = 20`), declarados na `main`.
- Não há variáveis globais: os vetores e as quantidades são passados por
  parâmetro para as funções.
- No início, os vetores são carregados dos arquivos (`fread`). Após cada
  inserção, alteração ou exclusão, o arquivo correspondente é regravado
  (`fwrite`), então nada se perde se o programa for fechado.

### Cliente (`Cliente.dat`)

CNPJ, nome, razão social, endereço (rua/av., cidade, estado e CEP),
inscrição estadual (`ISENTO` quando não houver), telefone, email e contato.

### Pedido (`Pedidos.dat`)

Número, CNPJ do cliente, data do pedido, data da entrega, previsão de entrega
(em dias), até 6 itens (descrição de 20 caracteres, quantidade e valor
unitário), total do pedido e forma de pagamento (à vista ou duplicata).

## Menus

- **Clientes:** inserir, alterar, excluir e consultar (por CNPJ ou listar todos).
- **Pedidos:** inserir, alterar, excluir e consultar:
  1. todos os pedidos feitos por um cliente;
  2. todos os pedidos realizados entre duas datas;
  3. pedidos entregues a partir de uma data de entrada;
  4. pedidos em aberto (separados em "no prazo" e "fora do prazo");
  5. média mensal de valores comprados pelo cliente;
  6. um pedido pelo número;
  7. todos os pedidos.

Na alteração, os valores atuais aparecem entre colchetes; teclar ENTER mantém
o valor.

## Decisões de implementação

- **Valor do item:** o enunciado define o total como "quantidade dos itens
  vezes o valor do mesmo", então cada item guarda também seu valor unitário.
  O total é calculado pelo programa.
- **Número do pedido:** gerado automaticamente (maior número existente + 1).
- **CNPJ:** é a chave do cliente e não pode ser alterado. Pode ser digitado
  com ou sem pontuação, e os dígitos verificadores são conferidos. O CNPJ
  alfanumérico também é aceito. Exemplos válidos para teste:
  `11.222.333/0001-81`, `12.345.678/0001-95`, `98.765.432/0001-98`,
  `12.ABC.345/01DE-35`.
- **Pedidos e clientes:** só é possível inserir pedido para um cliente
  cadastrado. Ao excluir um cliente que tem pedidos, o programa avisa e,
  se confirmado, exclui também os pedidos dele.
- **Data da entrega:** fica vazia enquanto o pedido está em aberto e não pode
  ser anterior à data do pedido.
- **Prazo:** data limite = data do pedido + previsão de entrega (dias). Um
  pedido em aberto está "no prazo" se a data de hoje não passou dessa data
  limite.
- **Consulta 3 (entregues a partir de uma data de entrada):** mostra os pedidos
  já entregues cuja data do pedido (a data de entrada) é igual ou posterior à
  data informada.
- **Consulta 5 (média mensal):** agrupa os pedidos do cliente por mês/ano da
  data do pedido e mostra o total de cada mês e duas médias: a dos meses com
  compras e a do período inteiro, do primeiro ao último mês, contando também
  os meses sem compras.
