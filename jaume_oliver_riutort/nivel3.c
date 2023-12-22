/* NIVEL 3 */
#include "nivel3.h"
/*AUTORES:
    Antoni Jaume Lemesev
    Josep Oliver Vallespir
    Gabriel Riutort Álvarez
*/

/* --- COMANDOS INTERNOS --- */

/* ------------------------- */
/* INTERNAL CD */
int internal_cd(char **args) {

    char *linea = malloc(sizeof(char) * COMMAND_LINE_SIZE);

    if (linea == NULL) {
        fprintf(stderr, "No hay memoria dinámica disponible en este momento.\n"RESET);
    }

    //Concatenamos los args
    for (int i = 0; args[i]; i++) {
        strcat(linea, " ");
        strcat(linea, args[i]);
    }

    // Separadores: barra,comillas,comilla simple, espacio
    const int sep[] = {92, 34, 39, 32};

    if (args[2] != NULL) {
        //Miramos si es un caso especial
        int nLetsArgs1 = strlen(args[1]);
        int permiso = 1;
        char *ruta;
        
        //Si es comilla simple
        if (args[1][0] == (char)sep[1]) {
            ruta = strchr(linea, (char)(sep[1]));
            borrarCaracter(ruta, (char)sep[1]);
        }

        //Si son comillas
        else if (args[1][0] == (char)sep[2]) {
            ruta = strchr(linea, (char)(sep[2]));
            borrarCaracter(ruta, (char)sep[2]);
        }

        //Si es la barra
        else if (args[1][nLetsArgs1 - 1] == (char)sep[0]) {
            ruta = strchr(linea, args[1][0]);
            borrarCaracter(ruta, (char)sep[0]);
        }
        else {
            permiso = 0;
        }

        //Si se permiten 2 palabras después del cd
        if (!permiso) {
            fprintf(stderr, "Error: Demasiados Argumentos\n"RESET);

        } else {
            fprintf(stderr, "ruta: %s\n"RESET,ruta);

            if (chdir(ruta)) {
                perror("Error");
            }
        }
    }

    //Si es una palabra
    else {
        if (args[1] == NULL && chdir(getenv("HOME"))) {
                perror("Error");

        } else if (chdir(args[1])) {
                perror("Error");
        }
    }

    // <<En este nivel, a modo de test, muestra por pantalla el directorio al que nos hemos trasladado.>>
    #if DEBUGN2
        char *cwd = getcwd(NULL, 0);   
        fprintf(stderr,GRIS_T"Directorio actual: %s\n"RESET, cwd);
        free(cwd);
    #endif

    return 1;
}

// Método que borra un caracter de un "array/puntero"
void borrarCaracter(char *args, char caracter) {
    int i = 0;
    int iNew = 0;

    while (args[i]) {
        if (args[i] != caracter) {
            args[iNew] = args[i];
            iNew++;
        }
        i++;
    }
    args[iNew] = 0;
}


/* ---------------------- */
/* INTERNAL EXPORT */
int internal_export(char **args) {
    // Antes de nada: Comprobar si hay argumento siquiera
    if (args[1] == NULL)
    {
        printf(ROJO_T "No has puesto ningún argumento. Uso correcto: export NOMBRE=VALOR\n");
        return 0;
    }

    // Separar NOMBRE y VALOR
    char *nombre = strtok(args[1], "=");
    size_t nombre_length = strlen(nombre);
    char *valor = (char *)malloc(strlen(args[1]) - nombre_length);

    // Comprobar que el argumento no esté 'vacío' (p ej "export =" da error)
    if (valor == NULL) {
        printf(ROJO_T "El argumento del valor esta vacio");
        return 0;

    } else {
        strcpy(valor, args[1] + nombre_length + 1);
    }

    // TEMPORALMENTE imprimir tokens obtenidos
    #if DEBUGN2
        fprintf(stderr,GRIS_T"PARÁMETRO NOMBRE: %s\n"RESET, nombre);
        fprintf(stderr,GRIS_T"PARÁMETRO VALOR: %s\n"RESET, valor);

        // TEMPORALMENTE: Mostrar valor inicial
        char *valInicial = getenv(nombre);
        if (valInicial != NULL)
        {
            fprintf(stderr,GRIS_T"Valor inicial de %s: %s\n"RESET, nombre, valInicial);
        }
        else
        {
            fprintf(stderr,GRIS_T"Valor inicial de %s es nulo / no existe \n"RESET, nombre);
        }
    #endif

    // ASIGNAR variable con setenv()
    if (setenv(nombre, valor, 1) != 0)
    {
        perror("setenv");
        return 0;
    }

    #if DEBUGN2
        // TEMPORALMENTE comprobar el nuevo valor de la variable
        char *pruebaNuevo = getenv(nombre);
        if (pruebaNuevo != NULL)
        {
            fprintf(stderr,GRIS_T"Nuevo valor para %s: %s\n"RESET, nombre, pruebaNuevo);
        }
        else
        {
            perror("getenv");
            return 0;
        }
    #endif

    return 1;
}

/* ---------------------- */
/* INTERNAL SOURCE */
int internal_source(char **args) {

    char *str = (char *) malloc(sizeof(char) * COMMAND_LINE_SIZE);

    if (str) {

        FILE *fichero = fopen(args[1], "r");

        if (fichero) {

            while (fgets(str, COMMAND_LINE_SIZE, fichero)) {

                execute_line(str);
                fflush(fichero);
                printf("\n");
                
            }

            fclose(fichero);
            free(str);

            return 0;


        } else {

            perror(ROJO_T "Error");
            free(str);

        }

        
    }

    return 1;
/*
    if (!args[1]){
        fprintf(stderr, ROJO_T "Error de sintaxis. Uso: source <nombre_fichero>\n");
        return -1;
    }
    FILE *fichero;
    fichero =fopen(args[1], "r");
    if(fichero){
        fprintf(stderr, ROJO_T "Error %d: %s\n", errno, strerror(errno));
        return -1;
    }
    
    while (fgets(str, 100, fichero)){
        puts(str);
        fflush(fichero);
    }
    fclose(fichero);
  return 0;
    return 1; // TRUE
*/
}

/* ---------------------- */
/* INTERNAL JOBS */
int internal_jobs(char **args)
{
    // Implementar lógica para mostrar trabajos en segundo plano
    #if DEBUGN1
        fprintf(stderr,GRIS_T "[internal_jobs()→Esta función mostrará el PID de los procesos que no estén en foreground]\n"RESET);
    #endif
    return 1; // TRUE
}

/* ---------------------- */
/* INTERNAL FG */
int internal_fg(char **args)
{
    // Implementar lógica para llevar un trabajo a primer plano
    #if DEBUGN1
        fprintf(stderr,GRIS_T "[internal_fg()→Esta función pone en primer plano una que esta ejecutandose en segundo plano]\n"RESET);
    #endif
    return 1; // TRUE
}

/* ---------------------- */
/* INTERNAL BG */
int internal_bg(char **args)
{
    // Implementar lógica para llevar un trabajo a segundo plano
    #if DEBUGN1
        fprintf(stderr,GRIS_T "[internal_bg)→Esta función reanuda el proceso que esta en segundo plano]\n"RESET);
    #endif
    return 1; // TRUE
}

/* --- --- --- --- --- ---*/



/* ---------------------- */
/* CHECK INTERNAL */
int check_internal(char **args) {
    if (args[0] == NULL)
    {
        return 0; // No hay comando
    }
    else if (strcmp(args[0], "exit") == 0)
    {
        printf("\rEXIT.\n");
        exit(0);
    }
    else if (strcmp(args[0], "cd") == 0)
    {
        internal_cd(args);
        return 1;
    }
    else if (strcmp(args[0], "export") == 0)
    {
        internal_export(args);
        return 1;
    }
    else if (strcmp(args[0], "source") == 0)
    {
        internal_source(args);
        return 1;
    }
    else if (strcmp(args[0], "jobs") == 0)
    {
        internal_jobs(args);
        return 1;
    }
    else if (strcmp(args[0], "fg") == 0)
    {
        internal_fg(args);
        return 1;
    }
    else if (strcmp(args[0], "bg") == 0)
    {
        internal_bg(args);
        return 1;
    }
    return 0; // No es un comando aceptado
}

/* ---------------------- */
/* METODO MAIN */
int main()
{
    char line[COMMAND_LINE_SIZE];
    while (read_line(line))
    {
        execute_line(line);
    }
    return 0;
}

/* ---------------------- */
/* READ LINE */
char *read_line(char *line) {
    imprimir_prompt();
    // Lee la línea y comprueba si no es nula
    if (fgets(line, COMMAND_LINE_SIZE, stdin) != NULL)
    {
        // Sustituye el carácter final de new line por '\0'
        size_t length = strlen(line);
        if (line[length - 1] == '\n')
        {
            line[length - 1] = '\0';
        }

        return line;
    }
    else if (feof(stdin))
    { // Ha pulsado Ctrl+D (salir)
        printf("\rCTRL+D. FIN.\n");
        exit(0);
    }
    else
    { // línea nula
        return NULL;
    }

    // Forzar el vaciado del buffer de salida
    fflush(stdout);

    // Esperar 0.5 segundos
    sleep(0.5);
}

/* ---------------------- */
/* IMPRIMIR PROMPT */
int imprimir_prompt()
{
    
    printf(NEGRITA ROJO_T "%s", getenv("USER"));
    printf(RESET ":");
    printf(NEGRITA AZUL_T "~%s", /*getenv("PWD")*/getcwd(NULL, 0));
    printf(RESET VERDE_T "%c " RESET, PROMPT); // printf("$ ")
    return 0;

    fflush(stdout);
}

/* ---------------------- */
/* PARSE ARGS */
int parse_args(char **args, char *line)
{
    char *token;
    int i = 0;

    token = strtok(line, " \n\t\r");

    while (token != NULL) {
        if (i >= ARGS_SIZE || token[0] == '#')
        {
            // Esto se ejeucta si: comentario o núm max de argumentos
            args[i] = NULL;
            break;
        }

        // Asignar suficiente memoria a args[i]
        args[i] = (char *)malloc(strlen(token) + 1);

        // Copiar el token a args[i]
        strcpy(args[i], token);

        // NULL porque en realidad estamos iterando sobre el propio token
        token = strtok(NULL, " \n\t\r");
        i++;
    }

    args[i] = NULL; // Último elemento debe ser NULL

    // [PRUEBA, QUITAR LUEGO]: imprimir los tokens obtenidos ----------------------------------------------------QUITAR
    #if DEBUGN1
        for (int j = 0; args[j] != NULL; j++)
        { fprintf(stderr,GRIS_T "[parse_args()-> token %d: %s]\n"RESET, j, args[j]); }
    #endif

    // Devolvemos núm de tokens !NULL (el contador i)
    return i;
}

/* ---------------------- */
/* EXECUTE LINE */
int execute_line(char *line) {

    char* args[ARGS_SIZE];
    pid_t pid;
    int status;
    strcpy(jobs_list[0].cmd, line);
    jobs_list[0].estado = 'E';
    parse_args(args, line);
    if (!check_internal(args)) {
        #if DEBUGN3
            fprintf(stderr, GRIS_T "[execute_line()→ PID padre: %d (./nivel3)]\n", getppid());
            printf(RESET);
        #endif
        pid = fork();
        if (pid == 0) {//hijo
            #if DEBUGN3
                fprintf(stderr, GRIS_T "[execute_line()→ PID hijo: %d (%s)]\n", getpid(), jobs_list[0].cmd);
                printf(RESET);
            #endif
            execvp(args[0], args);
            fprintf(stderr, ROJO_T "%s: no se encontró la orden\n", line);
            printf(RESET);
        }
        else if (pid > 0) {//padre
            wait(&status);
            if (WIFEXITED(status)) {
                #if DEBUGN3
                    fprintf(stderr, GRIS_T "[execute_line()→ Proceso hijo %d finalizado con exit(), estado: %d]\n", pid, WEXITSTATUS(status));
                    printf(RESET);
                #endif
                //Resetear datos
                jobs_list[0].pid = 0;
                jobs_list[0].estado = 'N';
                memset(jobs_list[0].cmd, '\0', COMMAND_LINE_SIZE);
            }
            else {
                if (WIFSIGNALED(status)) {
                    #if DEBUGN3
                        fprintf(stderr, ROJO_T "[execute_line()→ Proceso hijo %d finalizado por señal %d]\n", pid, WTERMSIG(status));
                        printf(RESET);
                    #endif
                    jobs_list[0].pid = 0;
                    jobs_list[0].estado = 'N';
                    memset(jobs_list[0].cmd, '\0', COMMAND_LINE_SIZE);
                }
            }
        }
        else {
            fprintf(stderr, "Error %d: %s\n", errno, strerror(errno));
        }
    }
    return 0;
}