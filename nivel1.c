/* NIVEL 1 */
#include "nivel1.h"

//Método main
int main() {

    char line[COMMAND_LINE_SIZE];

    while (read_line(line)) {

        execute_line(line);
    }

    return 0;
}


