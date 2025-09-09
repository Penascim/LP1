#include <stdio.h>

void compra(int *conta, int valor)
{
    *conta = *conta - valor;
}

int main(void)
{
    int minha_conta1;
    int minha_conta2;
    int compras[] = { 100, 50, 80, 30, 20 };
    int i;

    i = 0;
    printf("saldo da conta1: ");
    scanf("%d", &minha_conta1);
    printf("saldo da conta2: ");
    scanf("%d", &minha_conta2);

    while (i < 5)
    {
        if (minha_conta1 >= minha_conta2)
            compra(&minha_conta1, compras[i]);
        else
            compra(&minha_conta2, compras[i]);
        i++;
    }

    printf("\nsaldo da conta1: %d\n", minha_conta1);
    printf("saldo da conta2: %d\n", minha_conta2);

    return 0;
}
