/* NIVEL 1 */
#include "nivel2.h"
/* RESUMEN TO-DO 14/11/2023
    cd avanzado en internal_cd()
    actualizar prompt al haber hecho internal_cd()
    
    comprobar argumento (que no sea vacío tipo "export =" o "export =abcd" etc) en internal_export()

*/

/* --- COMANDOS INTERNOS --- */
int internal_cd(char **args) {
    if (args[1] == NULL) {
        // no hay argumento -> ir a HOME
        chdir(getenv("HOME"));
    } else if (args[2] == NULL) {
        // Hay argumento -> chdir a lo que diga el argumento
        if (chdir(args[1]) != 0) {
            perror("cd");
        }
    } else {
        // ª
        // TO-DO: cd avanzado
        printf("TO-DO: Más de 1 argumento en cd (cd avanzado)");
    }

    // <<En este nivel, a modo de test, muestra por pantalla el directorio al que nos hemos trasladado.>>
    char cwd[COMMAND_LINE_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Directorio actual: %s\n", cwd);
    } else {
        perror("getcwd");
    }

    //TO-DO: actualizar prompt al cambiar de directorio

    return 1;
}

int internal_export(char **args) {
    // Antes de nada: Comprobar si hay argumento siquiera
    if (args[1] == NULL) {
        printf("No has puesto ningún argumento. Uso correcto: export NOMBRE=VALOR\n");
        return 0;
    }
    //**** PREGUNTA: Se debería comprobar si hay más de 1 argumento ?? 
    // (p ej: "export VAR1=VAL1 VAR2=VAL2 VAR3=VAL3"...) 
    // Y en ese caso dar error ?

    // TO-DO: comprobar que el argumento no esté 'vacío' (p ej "export =" da error)

    // Separar NOMBRE y VALOR
    char *nombre = strtok(args[1], "=");
    size_t nombre_length = strlen(nombre);
    char *valor = (char *)malloc(strlen(args[1]) - nombre_length);;
    strcpy(valor, args[1] + nombre_length + 1);

    //TEMPORALMENTE imprimir tokens obtenidos
    printf("PARÁMETRO NOMBRE: %s\n",nombre);
    printf("PARÁMETRO VALOR: %s\n",valor);

    // TEMPORALMENTE: Mostrar valor inicial
    char *valInicial = getenv(nombre);
    if (valInicial != NULL) {
        printf("Valor inicial de %s: %s\n", nombre, valInicial);
    } else {
        printf("Valor inicial de %s es nulo / no existe \n", nombre);
    }

    // ASIGNAR variable con setenv()
    if (setenv(nombre, valor, 1) != 0) {
        perror("setenv");
        return 0; 
    }

    // TEMPORALMENTE comprobar el nuevo valor de la variable
    char *pruebaNuevo = getenv(nombre);
    if (pruebaNuevo != NULL) {
        printf("Nuevo valor para %s: %s\n", nombre, pruebaNuevo);
    } else {
        perror("getenv");
        return 0;
    }

    return 1;
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