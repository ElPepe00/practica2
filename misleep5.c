#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int sec = 5; // Cambiado a 5 segundos
    for (int i = 0; i < sec; i++) {
        // fprintf(stderr, "%d\n", i);
        sleep(1);
    }
    return 0;
}
