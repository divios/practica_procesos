#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "../childs_struct.h"
char* itoa(int i) {
    static char str[12];
    sprintf(str, "%d", i);
    return str;
}

void killChild(childs_t child[]);
void intHandler(int signal);
void alarmHandler(int signal);

childs_t childs[5];
int alarmFlag = 0, killFLag = 0;

int main(int argc, char **argv){

    signal(SIGINT, intHandler);
    signal(SIGALRM, alarmHandler);

    for (int i = 0; i < 5; i++) {
        childs[i].argv = i;
        if ( (childs[i].pid = fork()) < 0) {
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

        else { //somos es el padre
            kill(childs[i].pid, SIGSTOP); //paramos ese proceso
        }
    }

    int current = 0;
    alarm(1);
    changeStatus(&childs[0], 1);

    while(1) {
        if (killFLag == 1) {
            alarm(0);
            break;
        }
        else if (alarmFlag == 0) continue;

        alarmFlag = 0;
        changeStatus(&childs[current], 0);
        if(current == 4) current = 0;
        else current++;
        changeStatus(&childs[current], 1);
        alarm(1);
    }

    killChild(childs);
    exit(0);

}

void intHandler(int signal) {
    killFLag = 1;
}

void alarmHandler(int signal) {
    alarmFlag = 1;
}

void killChild(childs_t child[]) {
    printf("\n Arg  \tPid  \tULT ESTADO  \tNUM EJEC \tSTATUS\n");
    int status;
    for(int i=0; i < 5; i++) {
        char ptr[7];
        getStatusStr(child[i], ptr);
        kill(child[i].pid, SIGKILL);
        wait(&status);
        printf("  %2i  %6i  %12s  %12i  %12i\n",
               child[i].argv, pid/*child[i].pid*/, ptr, child[i].n_times, status);
    }
}


