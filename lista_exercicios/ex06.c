#include <stdio.h>

int main(void) {
    int n, i;
    float odometro, litros, valor, inicial = 0, anterior = 0;
    float totalLitros = 0, totalGasto = 0, distancia;

    printf("Numero total de reabastecimentos: ");
    scanf("%d", &n);
    if (n < 2) {
        printf("Sao necessarios pelo menos 2 reabastecimentos\n");
        return 1;
    }

    for (i = 1; i <= n; i++) {
        printf("\nParada %d - leitura do odometro (km): ", i);
        scanf("%f", &odometro);
        printf("Parada %d - litros abastecidos: ", i);
        scanf("%f", &litros);
        printf("Parada %d - valor pago (R$): ", i);
        scanf("%f", &valor);

        if (i == 1) {
            inicial = odometro;
        } else {
            printf("Entre a parada %d e a %d: %.2f km/l\n", i - 1, i, (odometro - anterior) / litros);
            totalLitros += litros;
            totalGasto += valor;
        }
        anterior = odometro;
    }

    distancia = anterior - inicial;
    printf("\nDistancia total: %.1f km\n", distancia);
    printf("Media da viagem: %.2f km/l\n", distancia / totalLitros);
    printf("Custo por km rodado: R$ %.2f\n", totalGasto / distancia);
    return 0;
}
