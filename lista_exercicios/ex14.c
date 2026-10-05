#include <stdio.h>
#include <time.h>

int main(void) {
    int horas = 0, minutos = 0, segundos = 0;
    clock_t inicio;

    while (1) {
        printf("\r%02d: %02d: %02d", horas, minutos, segundos);
        fflush(stdout);

        inicio = clock();
        while (clock() - inicio < CLOCKS_PER_SEC) {
        }

        segundos++;
        if (segundos == 60) {
            segundos = 0;
            minutos++;
        }
        if (minutos == 60) {
            minutos = 0;
            horas++;
        }
        if (horas == 24)
            horas = 0;
    }
    return 0;
}
