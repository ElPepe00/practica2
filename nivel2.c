/* NIVEL 2 */
#include "nivel2.h"

/* --- COMANDOS INTERNOS --- */
int internal_cd(char **args) {

    char *linea = malloc(sizeof(char) * COMMAND_LINE_SIZE);

    if (linea == NULL) {
        fprintf(stderr, "No hay memoria dinámica disponible en este momento.\n");
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
            borradorCaracter(ruta, (char)sep[1]);
        }

        //Si son comillas
        else if (args[1][0] == (char)sep[2]) {
            ruta = strchr(linea, (char)(sep[2]));
            borradorCaracter(ruta, (char)sep[2]);
        }

        //Si es la barra
        else if (args[1][nLetsArgs1 - 1] == (char)sep[0]) {
            ruta = strchr(linea, args[1][0]);
            borradorCaracter(ruta, (char)sep[0]);
        }
        else {
            permiso = 0;
        }

        //Si se permiten 2 palabras después del cd
        if (!permiso) {
            fprintf(stderr, "Error: Demasiados Argumentos\n");

        } else {
            fprintf(stderr, "ruta: %s\n",ruta);

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
    char *cwd = getcwd(NULL, 0);
    printf("Directorio actual: %s\n", cwd);
    free(cwd);

    return 1;
}

// Método que borra un caracter de un "array/puntero"
void borradorCaracter(char *args, char caracter) {
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

int internal_export(char **args)
{
    // Antes de nada: Comprobar si hay argumento siquiera
    if (args[1] == NULL)
    {
        printf(ROJO_T "No has puesto ningún argumento. Uso correcto: export NOMBRE=VALOR\n");
        return 0;
    }
    //**** PREGUNTA: Se debería comprobar si hay más de 1 argumento ??
    // (p ej: "export VAR1=VAL1 VAR2=VAL2 VAR3=VAL3"...)
    // Y en ese caso dar error ?
    //**** CREO QUE NO ES NECESARIO CONTROLAR MAS DE 1 ARGUMENTO

    // Separar NOMBRE y VALOR
    char *nombre = strtok(args[1], "=");
    size_t nombre_length = strlen(nombre);
    char *valor = (char *)malloc(strlen(args[1]) - nombre_length);

    // Comprobar que el argumento no esté 'vacío' (p ej "export =" da error)
    if (valor == NULL)
    {
        printf(ROJO_T "El argumento del valor esta vacio");
        return 0;
    }
    else
    {
        strcpy(valor, args[1] + nombre_length + 1);
    }

    // TEMPORALMENTE imprimir tokens obtenidos
    printf("PARÁMETRO NOMBRE: %s\n", nombre);
    printf("PARÁMETRO VALOR: %s\n", valor);

    // TEMPORALMENTE: Mostrar valor inicial
    char *valInicial = getenv(nombre);
    if (valInicial != NULL)
    {
        printf("Valor inicial de %s: %s\n", nombre, valInicial);
    }
    else
    {
        printf("Valor inicial de %s es nulo / no existe \n", nombre);
    }

    // ASIGNAR variable con setenv()
    if (setenv(nombre, valor, 1) != 0)
    {
        perror("setenv");
        return 0;
    }

    // TEMPORALMENTE comprobar el nuevo valor de la variable
    char *pruebaNuevo = getenv(nombre);
    if (pruebaNuevo != NULL)
    {
        printf("Nuevo valor para %s: %s\n", nombre, pruebaNuevo);
    }
    else
    {
        perror("getenv");
        return 0;
    }

    return 1;
}

int internal_source(char **args)
{
    // Implementar lógica para ejecutar un script desde un archivo
    printf(GRIS_T "[internal_source()→Esta función ejecutará un fichero de líneas de comandos]\n");
    return 1; // TRUE
}

int internal_jobs(char **args)
{
    // Implementar lógica para mostrar trabajos en segundo plano
    printf(GRIS_T "[internal_jobs()→Esta función mostrará el PID de los procesos que no estén en foreground]\n");
    return 1; // TRUE
}

int internal_fg(char **args)
{
    // Implementar lógica para llevar un trabajo a primer plano
    printf(GRIS_T "[internal_fg()→Esta función pone en primer plano una que esta ejecutandose en segundo plano]\n");
    return 1; // TRUE
}

int internal_bg(char **args)
{
    // Implementar lógica para llevar un trabajo a segundo plano
    printf(GRIS_T "[internal_bg)→Esta función reanuda el proceso que esta en segundo plano]\n");
    return 1; // TRUE
}

/* --- --- --- ---*/

int check_internal(char **args)
{
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

// Método main
int main()
{
    char line[COMMAND_LINE_SIZE];
    while (read_line(line))
    {
        execute_line(line);
    }
    return 0;
}

// Mètodo para leer lineas de comandos
char *read_line(char *line)
{
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

// Método que imprime el prompt personalizado
int imprimir_prompt()
{
    printf(NEGRITA ROJO_T "%s", getenv("USER"));
    printf(RESET ":");
    printf(NEGRITA AZUL_T "~%s", getenv("PWD"));
    printf(RESET "%c ", PROMPT); // printf("$ ")
    return 0;
}

int parse_args(char **args, char *line)
{
    char *token;
    int i = 0;

    token = strtok(line, " ");
    while (token != NULL)
    {
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
        token = strtok(NULL, " ");
        i++;
    }

    args[i] = NULL; // Último elemento debe ser NULL

    // [PRUEBA, QUITAR LUEGO]: imprimir los tokens obtenidos
    for (int j = 0; args[j] != NULL; j++)
    {
        printf("args[%d]: %s\n", j, args[j]);
    }

    // Devolvemos núm de tokens !NULL (el contador i)
    return i;
}

int execute_line(char *line)
{
    char *args[ARGS_SIZE];
    parse_args(args, line);

    if (strtok(line, " \t\r\n") > 0)
    {
        check_internal(args);
    }

    // Liberar memoria asignada por parse_args()
    for (int i = 0; args[i] != NULL; ++i)
    {
        free(args[i]);
    }
}