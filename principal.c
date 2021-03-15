

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "childs_struct.h"


char *itoa(int i) {
    static char str[12];
    sprintf(str, "%d", i);
    return str;
}

// Todos los handlers
void intHandler(int signal);
void alarmHandler(int signal);
void menuHandler(int signal);

/*      Utilidades      */

/// Itinera por la lista y manda señal de kill con el pid
/// de cada nodo
///
/// @param child lista de childs_t a mandar señal

void killChild(childs_t child[]);

/// Imprime toda la informacion de los procesos hijos por pantalla
/// llamando a stats_process y comunicandose a traves de un pipe
///

void printInfo();

/// Procesa el siguiente proceso a ejecutarse con cada itineracion de
/// "Round Robin" teniendo en cuanta los procesos muertos
///
/// @param pid - Si se mato un proceso antes, indicar su pid para decidir si cambiar de proceso.
/// Si tan solo es para procesar el siguiente, dejar a 0
/// @return 1 Si ya no quedan mas procesos o 0 si aun quedan

int processNext(pid_t pid);

/*  Funciones para manejar flujo de SIGQUIT */
void menuHandler_1();
void menuHandler_2();

childs_t childs[5];
int alarmFlag = 0, killFlag = 0, menuFlag = 0,
        current = 0, deleted = 0;



int main(int argc, char **argv) {

    signal(SIGINT, intHandler);
    signal(SIGALRM, alarmHandler);
    signal(SIGQUIT, &menuHandler);

    for (int i = 0; i < 5; i++) {
        childs[i].argv = i;
        if ((childs[i].pid = fork()) < 0) {
            fprintf(stderr, "Error while creating child %i\n", childs[i].pid);
            for (int j = 0; j < i; j++) {  //matamos todos los hijos creados hasta ahora
                int status;
                kill(childs[j].pid, SIGKILL);
                wait(&status);
            }
            exit(0);
        }
        if (childs[i].pid == 0) {  //si somos el hijo
            char *args[] = {"./proceso", "", NULL};
            args[1] = itoa(i + 1);
            execvp(args[0], args);
        }

        //somos es el padre
        kill(childs[i].pid, SIGSTOP); //paramos ese proceso

    }

    alarm(1);
    changeStatus(&childs[0], 1);

    while (1) {
        if (menuFlag == 1) {
            alarm(0);
            changeStatus(&childs[current], 0);

            menuHandler_1();
            menuFlag = 0;
            if (processNext(0) == 1) continue;
            fprintf(stderr,"Continuando con el programa...\n");

            changeStatus(&childs[current], 1);
            alarm(1);
            alarmFlag = 0;

            continue;
        }

        if (killFlag == 1) {
            break;
        } else if (alarmFlag == 0) continue;

        alarmFlag = 0;
        changeStatus(&childs[current], 0);
        processNext(0);
        changeStatus(&childs[current], 1);
        alarm(1);
    }

    char ptr[7];
    killChild(childs);
    printInfo();

    exit(0);

}

void printInfo() {
    pid_t pid;
    int MYFIFO[2];
    pipe(MYFIFO);

    if ((pid = fork()) < 0) {
        fprintf(stderr, "Error while creating stats_process\n");
        exit(-1);
    } else if (pid == 0) {
        dup2(MYFIFO[0], STDIN_FILENO);
        execl("./stats_process", "./stats_process", NULL);
        //fprintf(stderr, "Error while executing stats_process\n");
    }

    write(MYFIFO[1], childs, sizeof(childs));

    int status;
    wait(&status);
}

void intHandler(int signal) {
    if (menuFlag) return;
    killFlag = 1;
}

void alarmHandler(int signal) {
    alarmFlag = 1;
}

void killChild(childs_t child[]) {
    for (int i = 0; i < 5; i++) {
        if (child[i].status == -1) continue;
        kill(child[i].pid, SIGKILL);
        wait(&(child[i].last_status));
    }
}

void menuHandler(int signal) {
    menuFlag = 1;
}

int processNext(pid_t pid) {

    if (deleted == 5) return 1;

    if (pid != 0 &&
    (pid != childs[current].pid)) return 0;

    int n = current + 1;
    if (n >= 5) n = 0;
    for (int i = 0; i < 4; i++) {

        if (childs[n].status != -1) {
            current = n;
            return 0;
        }
        n++;
        if (n >= 5) n = 0;
    }
}

void menuHandler_1() {

    int resul;
    printInfo();

    //fflush(stderr);
    fprintf(stderr,"\nAhora mismo se estaba ejecutando el proceso %i\n", childs[current].pid);
    fprintf(stderr,"Introduce el numero del proceso a eliminar:\n");

    scanf ("%d", &resul);

    if (resul == -1) {
        fprintf(stderr,"El valor no es un integer\n");
        menuHandler_1();
        return;
    } else if ((resul < 0 && resul != -2) || resul > 4) {
        fprintf(stderr,"El valor debe estar entre el rango 0-5\n");
        menuHandler_1();
        return;
    }

    if (resul == -2) {
        return;
    }

    if (childs[resul].status != -1) { //is still alive
        changeStatus(&childs[resul], -1);
        deleted++;
        if (processNext(childs[resul].pid) == 1) {
            fprintf(stderr,"Ya estan todos los hijos muertos, cerramos el programa...\n");
            killFlag = 1;
            return;
        }
    } else { // is not alive
        fprintf(stderr,"Ese hijo ya esta muerto\n");
    }

    menuHandler_2();
}

void menuHandler_2() {
    fprintf(stderr,"1. Eliminar mas hijos | 2. Continuar con el programa\n");

    int resul;
    scanf ("%d", &resul);

    if (resul < 1 || resul > 2) {
        fprintf(stderr,"Respuesta no valida\n");
        menuHandler_2();
        return;
    }

    if (resul == 1) {
        menuHandler_1();
    }
}


