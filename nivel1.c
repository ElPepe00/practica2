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


//Mètodo 
char *read_line(char *line) {

    imprimir_prompt();

    if (fgets(line, COMMAND_LINE_SIZE, stdin)) {
        return line;

    } else {
        return NULL;
    }
}


//Método que imprime el prompt personalizado
int imprimir_prompt() {

    printf(BOLD RED "%s", getenv("USER"));
    printf(RESET ":");
    printf(BOLD BLUE "~%s", getenv("PWD"));
    printf(RESET "%c", PROMPT); //printf("$ ")
    return 0;
}


int execute_line(char *line) {

    char *args[ARGS_SIZE];
    parse_args(args, line);

    if (strtok(line, " \t\r\n") > 0) {
        check_internal(args);
    }
}

