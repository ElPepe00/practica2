/* NIVEL 4 HEADER */

#define _POSIX_C_SOURCE 200112L

//Librerías
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

//Tamaño línea de comandos y número máximo de argumentos
#define COMMAND_LINE_SIZE 1024
#define ARGS_SIZE 64
#define N_JOBS 64

//Debug?
#define DEBUGN1 1
#define DEBUGN2 1
#define DEBUGN3 1
#define DEBUGN4 1

//Colores y estilos
#define RESET "\033[0m"
#define NEGRO_T "\x1b[30m"
#define NEGRO_F "\x1b[40m"
#define GRIS_T "\x1b[90m"
#define ROJO_T "\x1b[31m"
#define VERDE_T "\x1b[32m"
#define AMARILLO_T "\x1b[33m"
#define AZUL_T "\x1b[34m"
#define MAGENTA_T "\x1b[35m"
#define CYAN_T "\x1b[36m"
#define BLANCO_T "\x1b[97m"
#define NEGRITA "\x1b[1m"

//Declaración de los métodos
char *read_line(char *line);
int imprimir_prompt();
int execute_line(char *line);
int parse_args(char **args, char *line);
int check_internal(char **args);
int internal_cd(char **args);
int internal_export(char **args);
int internal_source(char **args);
int internal_jobs(char **args);
int internal_fg(char **args);
int internal_bg(char **args);

void borrarCaracter(char *args, char caracter);
void reaper(int signum);
void ctrlc(int signum);

//Declaracion constante del prompt
char const PROMPT = '$';

//Declaracion de la estructura de info_job
struct info_job {
   pid_t pid;
   char estado;
   char cmd[COMMAND_LINE_SIZE];
};

static struct info_job jobs_list[N_JOBS];
static char mi_shell[COMMAND_LINE_SIZE];