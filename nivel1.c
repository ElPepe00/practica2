/* NIVEL 1 */
#include "nivel1.h"

/* --- COMANDOS INTERNOS --- */
int internal_cd(char **args){
    puts("cd aún no hace nada\n"); // TO-DO
}

int internal_export(char **args){
    puts("export aún no hace nada\n"); // TO-DO
}

int internal_source(char **args){
    puts("source aún no hace nada\n"); // TO-DO
}

int internal_jobs(char **args){
    puts("jobs aún no hace nada\n"); // TO-DO
}

int internal_fg(char **args){
    puts("fg aún no hace nada\n"); // TO-DO
}

int internal_bg(char **args){
    puts("bg aún no hace nada\n"); // TO-DO
}

/* --- --- --- ---*/

int check_internal(char **args) {
    if (args[0] == NULL) {
        return 0; // No hay comando
    } else if(strcmp(args[0], "exit") == 0) {
        printf("\rEXIT.\n");
        exit(0);
    } else if (strcmp(args[0], "cd") == 0) {
         internal_cd(args);
         return 1;
    } else if (strcmp(args[0], "export") == 0) {
         internal_export(args);
         return 1;
    } else if (strcmp(args[0], "source") == 0) {
         internal_source(args);
         return 1;
    } else if (strcmp(args[0], "jobs") == 0) {
         internal_jobs(args);
         return 1;
    } else if (strcmp(args[0], "fg") == 0) {
         internal_fg(args);
         return 1;
    } else if (strcmp(args[0], "bg") == 0) {
         internal_bg(args);
         return 1;
    }
    return 0; // No es un comando aceptado
}

//Método main
int main() {
    char line[COMMAND_LINE_SIZE];
    while (read_line(line)) {
        execute_line(line);
    }
    return 0;
}


//Mètodo para leer lineas de comandos
char *read_line(char *line) {
    imprimir_prompt();
    // Lee la línea y comprueba si no es nula
    if (fgets(line, COMMAND_LINE_SIZE, stdin) != NULL) {
        // Sustituye el carácter final de new line por '\0'
        size_t length = strlen(line);
        if (line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        return line;
    } else if (feof(stdin)) { //Ha pulsado Ctrl+D (salir)
        printf("\rCTRL+D. FIN.\n");
        exit(0);
    } else { //línea nula
        return NULL;
    }
}


//Método que imprime el prompt personalizado
int imprimir_prompt() {
    printf(NEGRITA ROJO_T "%s", getenv("USER"));
    printf(RESET ":");
    printf(NEGRITA AZUL_T "~%s", getenv("PWD"));
    printf(RESET "%c ", PROMPT); //printf("$ ")
    return 0;

    // Forzar el vaciado del buffer de salida
    fflush(stdout);

    // Esperar 0.5 segundos
    sleep(0.5);
}

int parse_args(char **args, char *line){
    char *token;
    int i = 0;

    token = strtok(line, " ");
    while (token != NULL) {
        if (i >= ARGS_SIZE || token[0] == '#') {
            // Esto se ejeucta si: comentario o núm max de argumentos
            args[i] = NULL; 
            break;
        }

        // Asignar suficiente memoria a args[i]
        args[i] = (char *)malloc(strlen(token) + 1);
        
        // Copiar el token a args[i]
        strcpy(args[i], token);

        //NULL porque en realidad estamos iterando sobre el propio token
        token = strtok(NULL, " "); 
        i++;
    }

    args[i] = NULL; // Último elemento debe ser NULL

    // [PRUEBA, QUITAR LUEGO]: imprimir los tokens obtenidos
    for (int j = 0; args[j] != NULL; j++) {
        printf("args[%d]: %s\n", j, args[j]);
    }

    //Devolvemos núm de tokens !NULL (el contador i)
    return i;
}

int execute_line(char *line) {
    char *args[ARGS_SIZE];
    parse_args(args, line);

    if (strtok(line, " \t\r\n") > 0) {
        check_internal(args);
    }

    //Liberar memoria asignada por parse_args()
    for (int i = 0; args[i] != NULL; ++i) {
        free(args[i]);
    }
}

