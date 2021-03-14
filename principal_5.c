

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

void killChild(childs_t child[]);

void intHandler(int signal);
void alarmHandler(int signal);
void menuHandler(int signal);

void menuHandler_1();
void menuHandler_2();
void printInfo();

childs_t childs[5];
int alarmFlag = 0, killFlag = 0, menuFlag = 0,
        current = 0, deleted[5];

int main(int argc, char **argv) {

    signal(SIGINT, intHandler);
    signal(SIGALRM, alarmHandler);
    signal(SIGQUIT, menuHandler);

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
        //printf("hola\n");
        if (killFlag == 1) {
            break;
        } else if (alarmFlag == 0) continue;

        alarmFlag = 0;
        changeStatus(&childs[current], 0);
        if (current == 4) current = 0;
        else current++;
        changeStatus(&childs[current], 1);
        alarm(1);
    }

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
        kill(child[i].pid, SIGKILL);
        wait(&(child[i].last_status));
    }
}

void menuHandler(int signal) {
    menuFlag = 1;
    alarm(0);
    kill(childs[current].pid, SIGSTOP);
    menuHandler_1();
}

int checkProcess(pid_t pid) {
    kill(pid, 0); // Check if process is still alive
    if (errno == ESRCH) {
        return -1;
    } return 1;
}

int processNext(pid_t pid) {
    if (pid != childs[current].pid) return 0;

    int n = current + 1;
    if (n >= 5) n = 0;
    for (int i = 0; i < 4; i++) {
        if (checkProcess(childs[n].pid) == 1) {
            current = n;
            return 0;
        }
        n++;
        if (n >= 5) n = 0;
    }
    return 1; // if it reachs this, all are dead
}

void menuHandler_1() {

    int resul;
    printInfo();

    printf("Ahora mismo se estaba ejecutando el proceso %i\n", childs[current].pid);
    printf("\nIntroduce el numero del proceso a eliminar:\n");

    resul = getchar();

    if (resul == -1) {
        printf("El valor no es un integer\n");
        menuHandler_1();
        return;
    } else if (resul < 0 || resul > 5) {
        printf("El valor debe estar entre el rango 0-5\n");
        menuHandler_1();
        return;
    }

    if (resul == 0) {
        printf("Continuando con el programa...\n");
        kill(SIGCONT, childs[current].pid);
        alarm(1);
    }

    if (checkProcess(childs[resul].pid) == 1) { //is still alive
        changeStatus(&childs[resul], -1);
        if (processNext(childs[resul].pid) == 1) {
            printf("Ya estan todos los hijos muertos, cerramos el programa...\n");
            killFlag = 1;
            return;
        }
    } else { // is not alive
        printf("Ese hijo ya esta muerto\n");
    }

    menuHandler_2();
}

void menuHandler_2() {
    printf("1. Eliminar mas hijos | 2. Continuar con el programa\n");

    int resul = getchar();
    if (resul < 1 || resul > 2) {
        printf("Respuesta no valida\n");
        menuHandler_2();
        return;
    }

    if (resul == 1) {
        menuHandler_1();
    } else {
        printf("Continuando con el programa...\n");
        kill(SIGCONT, childs[current].pid);
        alarm(1);
        menuFlag = 0;
    }

}


