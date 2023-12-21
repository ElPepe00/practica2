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


//Mètodo para leer lineas de comandos
char *read_line(char *line) {
    imprimir_prompt();
    // Lee la línea y comprueba si no es nula
    if (fgets(line, COMMAND_LINE_SIZE, stdin) != NULL) {
        // Sustituir new line del final por '\0'
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

    // Forzar el vaciado del buffer de salida
    fflush(stdout);

    // Esperar 0.5 segundos
    sleep(0.5);
}


//Método que imprime el prompt personalizado
int imprimir_prompt() {
    printf(NEGRITA ROJO_T "%s", getenv("USER"));
    printf(RESET ":");
    printf(NEGRITA AZUL_T "~%s", getenv("PWD"));
    printf(RESET "%c ", PROMPT); //printf("$ ")
    return 0;
}

int parse_args(char **args, char *line){
    char *token;
    int i = 0;

    token = strtok(line, " ");
    while (token != NULL) {
        if (i >= ARGS_SIZE) {
            // Esto se ejeucta si: comentario o núm max de argumentos
            args[i] = NULL;
            break;
        }

        if (token[0] == "#") {
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

    // [PRUEBA, QUITAR EN NIVELES POSTERIORES]: imprimir los tokens obtenidos
    #if DEBUGN1
        for (int j = 0; args[j] != NULL; j++)
        { fprintf(stderr,GRIS_T "[parse_args()-> token %d: %s]\n"RESET, j, args[j]); }
    #endif

    
    

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

int check_internal(char **args){
    if (args[0] == NULL) {
        // No hay comando para verificar
        return 0; // FALSE
    }

    // Comparar con comandos internos y llamar a las funciones correspondientes
    if (strcmp(args[0], "cd") == 0) {
        return internal_cd(args);
    } else if (strcmp(args[0], "export") == 0) {
        return internal_export(args);
    } else if (strcmp(args[0], "source") == 0) {
        return internal_source(args);
    } else if (strcmp(args[0], "jobs") == 0) {
        return internal_jobs(args);
    } else if (strcmp(args[0], "fg") == 0) {
        return internal_fg(args);
    } else if (strcmp(args[0], "bg") == 0) {
        return internal_bg(args);
    } else if (strcmp(args[0], "exit") == 0) {
        printf("EXIT\n");
        exit(0); // Llamada a la función exit
    }

    return 0; // FALSE para comandos no reconocidos
}

/* --- COMANDOS INTERNOS --- */

int internal_cd(char **args) {
    // Implementar lógica para cambiar de directorio
    // Devolver 1 para indicar que se ejecutó un comando interno
    #if DEBUGN1
        fprintf(stderr,GRIS_T "[internal_cd()→Esta función cambiará de directorio]\n"RESET );
    #endif
    return 1; // TRUE
}

int internal_export(char **args) {
    // Implementar lógica para exportar una variable
    #if DEBUGN1
        fprintf(stderr,GRIS_T "[internal_export()→Esta función asignará valores a variablescd de entorno]\n"RESET );
    #endif
    return 1; // TRUE
}

int internal_source(char **args) {
    // Implementar lógica para ejecutar un script desde un archivo
    #if DEBUGN1
        fprintf(stderr,GRIS_T "[internal_source()→Esta función ejecutará un fichero de líneas de comandos]\n"RESET);
    #endif
    return 1; // TRUE
}

int internal_jobs(char **args) {
    // Implementar lógica para mostrar trabajos en segundo plano
    #if DEBUGN1
        fprintf(stderr,GRIS_T "[internal_jobs()→Esta función mostrará el PID de los procesos que no estén en foreground]\n"RESET);
    #endif
    return 1; // TRUE
}

int internal_fg(char **args) {
    // Implementar lógica para llevar un trabajo a primer plano
    #if DEBUGN1
        fprintf(stderr,GRIS_T "[internal_fg()→Esta función pone en primer plano una que esta ejecutandose en segundo plano]\n"RESET);
    #endif
    return 1; // TRUE
}

int internal_bg(char **args) {
    // Implementar lógica para llevar un trabajo a segundo plano
    #if DEBUGN1
        fprintf(stderr,GRIS_T "[internal_bg)→Esta función reanuda el proceso que esta en segundo plano]\n"RESET);
    #endif
    return 1; // TRUE
}