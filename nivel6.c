/* NIVEL 6 */
#include "nivel6.h"

/* ---------------------- */
/* METODO MAIN */
int main(int argc, char* argv[])
{
    char line[COMMAND_LINE_SIZE];
    jobs_list[0].pid = 0;
    jobs_list[0].estado = 'N';
    memset(jobs_list[0].cmd, '\0', strlen(jobs_list[0].cmd));
    strcpy(mi_shell, argv[0]);

    // Manejadores
    signal(SIGCHLD, reaper);
    signal(SIGINT, ctrlc);
    signal(SIGTSTP, ctrlz);

    while (read_line(line))
    {
        execute_line(line);
    }
    return 0;
}

/* --- COMANDOS INTERNOS --- */

/* ------------------------- */
/* INTERNAL CD */
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

/* ---------------------- */
/* INTERNAL EXPORT */
int internal_export(char **args) {
    // Antes de nada: Comprobar si hay argumento siquiera
    if (args[1] == NULL)
    {
        printf(ROJO_T "No has puesto ningún argumento. Uso correcto: export NOMBRE=VALOR\n" RESET);
        return 0;
    }

    // Separar NOMBRE y VALOR
    char *nombre = strtok(args[1], "=");
    size_t nombre_length = strlen(nombre);
    char *valor = (char *)malloc(strlen(args[1]) - nombre_length);

    // Comprobar que el argumento no esté 'vacío' (p ej "export =" da error)
    if (valor == NULL) {
        fprintf(stderr, ROJO_T "El argumento del valor esta vacio" RESET);
        return 0;

    } else {
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
    for (int i = 0; i <= n_job; i++) {
        printf("[%d] %d\t%c\t%s\n", i, jobs_list[i].pid, jobs_list[i].estado, jobs_list[i].cmd);
    }
    return 0;
}

/* ---------------------- */
/* INTERNAL FG */
int internal_fg(char **args)
{
    // Implementar lógica para llevar un trabajo a primer plano
    /*printf(GRIS_T "[internal_fg()→Esta función pone en primer plano una que esta ejecutandose en segundo plano]\n");
    return 1; // TRUE*/

    //1. COMPROBAR QUE SEA UN NÚMERO
    if (args[1] == NULL)
    {
        printf(ROJO_T "No has puesto ningún argumento. Uso correcto: fg num_trabajo_pos\n" RESET);
        return 0;
    }else if(!is_number(args[1])){
        printf(ROJO_T "No has puesto un número válido. Uso correcto: fg num_trabajo_pos\n" RESET);
        return 0;
    }

    //convertir de char a numero
    int pos = atoi(args[1]);

    //2. Si pos > n_job o pos = 0 entonces error("no existe ese trabajo") y retornar.
    if(pos > n_job || pos==0){
        printf(ROJO_T "No existe ese trabajo\n" RESET);
        return 0;
    }

    //3.Si su estado es ‘D’ entonces enviar a jobs_list[pos].pid la señal SIGCONT (señal nº 18), y provisionalmente notificarlo por pantalla. (Si el estado es ‘E’ no es necesario enviársela).
    if(jobs_list[pos].estado == 'D'){
        kill(jobs_list[pos].pid, SIGCONT);
        printf(GRIS_T "[internal_fg()→Señal SIGCONT enviada a %d(%s)]\n" RESET, jobs_list[pos].pid, jobs_list[pos].cmd);
    }

    //4. Copiar los datos de jobs_list[pos] a jobs_list[0], habiendo eliminado previamente del cmd el ' &' (en caso de que lo tuviera), y poniendo el estado a 'E'.
    borrarCaracter(jobs_list[pos].cmd, '&');
    jobs_list[0].pid = jobs_list[pos].pid;
    jobs_list[0].estado = 'E';
    strcpy(jobs_list[0].cmd, jobs_list[pos].cmd);
    
    //5.Eliminar jobs_list[pos] utilizando la función jobs_list_remove().
    jobs_list_remove(pos);

    //6. Mostrar por pantalla el cmd (habiendo eliminado el ‘&’ si lo tenía). Eso indicará que ese comando se está ejecutando en primer plano.
    printf(GRIS_T "[internal_fg()→Se ha puesto en primer plano el proceso %d(%s)]\n" RESET, jobs_list[0].pid, jobs_list[0].cmd);

    //7. Mientras haya un proceso en foregroud pausamos
    while (jobs_list[0].pid > 0) {
        pause();
    }

}

/* ---------------------- */
/* INTERNAL BG */
int internal_bg(char **args)
{
    // Implementar lógica para llevar un trabajo a segundo plano
    //printf(GRIS_T "[internal_bg)→Esta función reanuda el proceso que esta en segundo plano]\n");
    //return 1; // TRUE

    //1. COMPROBAR QUE SEA UN NÚMERO
    if (args[1] == NULL)
    {
        printf(ROJO_T "No has puesto ningún argumento. Uso correcto: bg num_trabajo_pos\n" RESET);
        return 0;
    }else if(!is_number(args[1])){
        printf(ROJO_T "No has puesto un número válido. Uso correcto: bg num_trabajo_pos\n" RESET);
        return 0;
    }

    //convertir de char a numero
    int pos = atoi(args[1]);;

    //2. Si pos > n_job o pos = 0 entonces error("no existe ese trabajo") y retornar.
    if(pos > n_job || pos==0){
        printf(ROJO_T "No existe ese trabajo\n" RESET);
        return 0;
    }

    //3. Si el estado de jobs_list[pos] es 'E' entonces error("el trabajo ya está en 2º plano") y salir.
    if(jobs_list[pos].estado == 'E'){
        printf(ROJO_T "El trabajo ya está en 2º plano\n" RESET);
        return 0;
    }

    //4. Cambiar el estado de ese trabajo a 'E' y añadir “ &” a su cmd.
    jobs_list[pos].estado = 'E';
    strcat(jobs_list[pos].cmd, " &");

    //5. Enviar a jobs_list[pos].pid la señal SIGCONT y provisionalmente notificarlo por pantalla.
    kill(jobs_list[pos].pid, SIGCONT);
    printf(GRIS_T "[internal_bg()→Señal SIGCONT enviada a %d(%s)]\n" RESET, jobs_list[pos].pid, jobs_list[pos].cmd);

    //6. Mostrar por pantalla el nº de trabajo, el PID, el estado y el cmd &, al igual que hacíamos cuando se generaba un comando en 2º plano.
    printf("[%d] %d\t%c\t%s &\n", pos, jobs_list[pos].pid, jobs_list[pos].estado, jobs_list[pos].cmd);
}

/* --- --- --- --- --- ---*/



/* --- MÉTODOS PRINCIPALES --- */

/* ---------------------- */
/* CHECK INTERNAL */
int check_internal(char **args) {
    if (args[0] == NULL)
    {
        return 0; // No hay comando
    }
    else if (strcmp(args[0], "exit") == 0)
    {
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
/* READ LINE */
char *read_line(char *line) {
    // Forzar el vaciado del buffer de salida
    fflush(stdout);

    // Esperar 0.5 segundos
    sleep(0.5);

    imprimir_prompt();
    // Lee la línea y comprueba si no es nula
    while (1) {
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

        else if (errno == EINTR) {
            clearerr(stdin);
            continue;
        }
        
        else
        { // línea nula
            perror("fgets"); //DEBUG
            return NULL;
        }

    }
}

/* ---------------------- */
/* IMPRIMIR PROMPT */
int imprimir_prompt()
{
    
    printf(NEGRITA ROJO_T "%s", getenv("USER"));
    printf(RESET ":");
    printf(NEGRITA AZUL_T "~%s", getenv("PWD"));
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

        args[i] = token;

        //Miramos si es un comentario
        if (strncmp(args[i], "#", 1) == 0)
        {
            break;
        }

        i++;
        // NULL porque en realidad estamos iterando sobre el propio token
        token = strtok(NULL, " \n\t\r");
        
    }

    args[i] = NULL; // Último elemento debe ser NULL
    strtok(line, "\n\r"); // ELiminamos el salto de linea

    // Devolvemos núm de tokens !NULL (el contador i)
    return i;
}

/* ---------------------- */
/* EXECUTE LINE */
int execute_line(char *line) {

    char* args[ARGS_SIZE];
    pid_t pid;
    int bkgd;
    char comandos[COMMAND_LINE_SIZE];
    strcpy(comandos, line);
    parse_args(args, line);

    if (!check_internal(args)) {

        bkgd = is_background(args);
        pid = fork();

        if (pid == 0) {//HIJO

            signal(SIGCHLD, SIG_DFL);
            signal(SIGINT, SIG_IGN);
            signal(SIGSTOP, SIG_IGN);

            fprintf(stderr, GRIS_T "[execute_line()→ PID hijo: %d(%s)]\n" RESET, getpid(), jobs_list[0].cmd);
            
            //Desde aquí el hijo llamará a la función anterior, is_output_redirection(), justo antes de la llamada a execvp(), para modificar, o no, los argumentos que se le pasarán y para redireccionar la salida, si así lo indica el comando. En caso de producirse redireccionamiento, éste acaba su efecto tras la ejecución del execvp().
            if (is_output_redirection(args)) {
                fprintf(stderr, GRIS_T "[execute_line()→ Redireccionamiento de salida a fichero]\n" RESET);
            }

            
            execvp(args[0], args);
            //En caso de producirse redireccionamiento, éste acaba su efecto tras la ejecución del execvp().
            
            
            
            fprintf(stderr, ROJO_T "%s: no se encontró la orden\n" RESET, line);
            exit(-1);
        }
        else if (pid > 0) {//PADRE
            
            fprintf(stderr, GRIS_T "[execute_line()→ PID Padre %d(%s)]\n" RESET, getpid(), mi_shell);

            if (bkgd == 1) {
                jobs_list_add(pid, 'E', comandos);

            } else {
                jobs_list[0].pid = pid;
                jobs_list[0].estado = 'E';
                strcpy(jobs_list[0].cmd, comandos);

                //Mientras haya un proceso en foregroud pausamos
                while (jobs_list[0].pid > 0) {
                    pause();
                }
            }
        }
        else {
            fprintf(stderr, ROJO_T "Error %d: %s\n" RESET, errno, strerror(errno));
        }
    }
    return 0;
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
/* REAPER */
// Manejador de la senal SIGCHLD
void reaper(int signum) {

    signal(SIGCHLD, reaper);

    pid_t ended;
    int estado;

    while ((ended = waitpid(-1, &estado, WNOHANG)) > 0) {

        if (ended == jobs_list[0].pid) {
            //Reseteamos pid
            jobs_list[0].pid = 0;
            jobs_list[0].estado = 'F';
            memset(jobs_list[0].cmd, '\0', strlen(jobs_list[0].cmd));
        }

        if (WIFEXITED(estado)) {
            char mensaje[1200];
            sprintf(mensaje, GRIS_T "[reaper()→ Proceso hijo %d(%s) finalizado con exit code %d]\n" RESET, ended, jobs_list[0].cmd, WEXITSTATUS(estado));
            write(2, mensaje, strlen(mensaje)); //2 es el flujo stderr

        } else if (WIFSIGNALED(estado)) {
            char mensaje[1200];
            sprintf(mensaje, GRIS_T "[reaper()→ Proceso hijo %d(%s) finalizado con exit code %d]\n" RESET, ended, jobs_list[0].cmd, WTERMSIG(estado));
            write(2, mensaje, strlen(mensaje)); //2 es el flujo stderr
        }
    }
}

/* ---------------------- */
/* CONTRLC */
// Manejador de la senal SIGINT
void ctrlc(int signum) {

    signal(SIGINT, ctrlc);
    printf("\n");
    fflush(stdout); // Impresión inmediata del print

    // Miramos si es un proceso hijo
    if (jobs_list[0].pid > 0) {

        //Miramos si es nuestro shell
        if (strcmp(jobs_list[0].cmd, mi_shell)) {
            fprintf(stderr, GRIS_T "[ctrlc()→ Soy el proceso con PID %d(%s), el proceso en foreground es %d (%s)]\n" RESET,
                getpid(), mi_shell, jobs_list[0].pid, jobs_list[0].cmd);

            printf(GRIS_T "[ctrlc()→ Señal %d enviada a %d(%s) por %d(%s)]" RESET, 
                SIGTERM, jobs_list[0].pid, jobs_list[0].cmd, getpid(), mi_shell);

            kill(jobs_list[0].pid, SIGTERM);

        } else {
            fprintf(stderr, GRIS_T "[ctrlc()→ Soy el proceso con PID %d(%s), el proceso en foreground es %d(%s)]\n" RESET,
                getpid(), mi_shell, jobs_list[0].pid, jobs_list[0].cmd);

            fprintf(stderr, GRIS_T "[ctrlc()→ Señal %d no enviada por %d(%s) debido a que su proceso en foreground es el shell]\n" RESET,
                SIGTERM, getpid(), mi_shell);
        }

    } else {
        fprintf(stderr, GRIS_T "\n[ctrlc()→ Soy el proceso con PID %d(%s), el proceso en foreground es %d(%s)]\n" RESET,
            getpid(), mi_shell, jobs_list[0].pid, jobs_list[0].cmd);

        fprintf(stderr, GRIS_T "[ctrlc()→ Señal %d no enviada por %d(%s)] debido a que no hay proceso en foreground]\n" RESET,
            SIGTERM, getpid(), mi_shell);
    }

    printf("\n");
    fflush(stdout); //Impresión inmediata del print
}

/* ---------------------- */
/* CTRLZ */
void ctrlz(int signum) {

    signal(SIGTSTP, ctrlz);
    // Miramos si es un proceso hijo
    if (jobs_list[0].pid > 0) {

        //Miramos si es nuestro shell
        if (strcmp(jobs_list[0].cmd, mi_shell)) {

            kill(jobs_list[0].pid, SIGSTOP);

            fprintf(stderr, GRIS_T "[ctrlz()→ Soy el proceso con PID %d(%s), el proceso en foreground es %d (%s)]\n" RESET,
                getpid(), mi_shell, jobs_list[0].pid, jobs_list[0].cmd);

            printf(GRIS_T "[ctrlz()→ Señal %d enviada a %d(%s) por %d(%s)]" RESET, 
                SIGSTOP, jobs_list[0].pid, jobs_list[0].cmd, getpid(), mi_shell);

            jobs_list[0].estado = 'D';
            jobs_list_add(jobs_list[0].pid, jobs_list[0].estado, jobs_list[0].cmd);

            jobs_list[0].pid = 0;
            jobs_list[0].estado = 'N';
            memset(jobs_list[0].cmd, '\0', sizeof(jobs_list[0].cmd));

            

        } else {
            fprintf(stderr, GRIS_T "[ctrlz()→ Soy el proceso con PID %d(%s), el proceso en foreground es %d(%s)]\n" RESET,
                getpid(), mi_shell, jobs_list[0].pid, jobs_list[0].cmd);

            fprintf(stderr, GRIS_T "[ctrlz()→ Señal %d no enviada por %d(%s) debido a que su proceso en foreground es el shell]\n" RESET,
                SIGTSTP, getpid(), mi_shell);
        }

    } else {
        fprintf(stderr, GRIS_T "\n[ctrlz()→ Soy el proceso con PID %d(%s), el proceso en foreground es %d(%s)]\n" RESET,
            getpid(), mi_shell, jobs_list[0].pid, jobs_list[0].cmd);

        fprintf(stderr, GRIS_T "[ctrlz()→ Señal %d no enviada por %d(%s)] debido a que no hay proceso en foreground]\n" RESET,
            SIGTSTP, getpid(), mi_shell);
    }
}

/* ---------------------- */
/* IS_BACKGROUND */
int is_background(char **args) {
    int i = 0;
    while (args[i] != NULL) {
        i++;
    }
    if (i>0 && /*para evitar segmentation fault*/ !strcmp(args[i - 1], "&")) {
        args[i - 1] = NULL;
        return 1;
    }
    else {
        return 0;
    }
}

//Comprueba si es un número válido, 1 si sí, 0 si no; para el fg y el bg
int is_number(char* string) {
  const int string_len = strlen(string);
  for(int i = 0; i < string_len; ++i) {
    if(!isdigit(string[i])) 
      return 0;
  }
  return 1;
}

/* ---------------------- */
/* JOBS_LIST_ADD */
int jobs_list_add(pid_t pid, char estado, char *cmd) {

    if (n_job < N_JOBS) {
        n_job++;
        jobs_list[n_job].pid = pid;
        jobs_list[n_job].estado = estado;
        strcpy(jobs_list[n_job].cmd, cmd);
        return n_job;

    } else {
        fprintf(stderr, ROJO_T "Límite de procesos en jobs_list alcanzado\n");
        return -1;
    }
}

/* ---------------------- */
/* JOBS_LIST_FIND */
int jobs_list_find(pid_t pid) {

    int i = 1;
    while (i < N_JOBS) {
        if (jobs_list[0].pid == pid) {
            return i;
        }
        i++;
    }
}

/* ---------------------- */
/* JOBS_LIST_REMOVE */
int jobs_list_remove(int pos) {

    if (pos > 0 && pos < n_job) {
        n_job--;
        jobs_list[pos].pid = jobs_list[n_job].pid;
        jobs_list[pos].estado = jobs_list[n_job].estado;
        strcpy(jobs_list[pos].cmd, jobs_list[n_job].cmd);

    } else {
        return -1;
    }
    return 0;
}



/* ------------ */
int is_output_redirection (char **args){
    /*Función booleana que recorrerá la lista de argumentos buscando un token ‘>’, seguido de otro token que será un nombre de fichero. En tal caso el valor de retorno será TRUE (1), significando que se trata de un redireccionamiento, y substituirá el argumento ‘>’ por NULL (recordemos que así lo requiere execvp()). En caso contrario, el valor de retorno será FALSE (0) y no modificará los argumentos.
    Si se trata de un redireccionamiento (con la sintaxis correcta), se abrirá con open() el fichero de salida que se ha indicado en el comando, para obtener su descriptor de fichero fd.
    Mediante la llamada al sistema dup() o dup2(fd, 1) lo que sucede es que el descriptor 1, que es el correspondiente a la salida estándard stdout, ahora se referirá al fichero descrito con el fd obtenido al hacer el open(). A partir de ese momento, el resultado de la ejecución de un comando cuyo destino fuera la pantalla, así como cualquier printf(...), cualquier fprintf(stdout, ...), o cualquier write(1, ...), escribirá el contenido en ese fichero en vez de por la pantalla. 
    Una vez hecha la asociación de los descriptores, liberamos al sistema el descriptor del fichero con un close(fd).
    Hay que gestionar los posibles errores en las llamadas al sistema.
    */
    int i = 0;
    int fd;
    while (args[i] != NULL) {
        if (strcmp(args[i], ">") == 0) {
            args[i] = NULL;
            fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd == -1) {
                perror("open");
                return 0;
            }
            if (dup2(fd, 1) == -1) {
                perror("dup2");
                return 0;
            }
            if (close(fd) == -1) {
                perror("close");
                return 0;
            }
            return 1;
        }
        i++;
    }
    return 0;
}