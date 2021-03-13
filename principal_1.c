
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

char* itoa(int i) {
    static char str[12];
    sprintf(str, "%d", i);
    return str;
}

void killChild(int child[]);
void intHandler(int signal);
void alarmHandler(int signal);

int alarmFlag = 0;
pid_t childs[5];

int main(int argc, char **argv) {

    signal(SIGINT, intHandler);
    signal(SIGALRM, alarmHandler);

    for (int i = 0; i < 5; i++) {
        if ( (childs[i] = fork()) < 0) {
            fprintf(stderr, "Error while creating child %i\n", childs[i]);
            for (int j = 0; j < i; j++) {  //matamos todos los hijos creados hasta ahora
                int status;
                kill(childs[j], SIGKILL);
                wait(&status);
            }
            exit(0);
        }
        if (childs[i] == 0) {  //si somos el hijo
            char *args[] = {"./proceso", "", NULL};
            args[1] = itoa(i + 1);
            execvp(args[0], args);
        }
    }

    alarm(5); //los segundos que quieres que dure el programa
    while(1) {
        if (alarmFlag == 1) {
            alarm(0);
            break;
        }
    }
    killChild(childs);
}

void intHandler(int signal) {
    alarmFlag = 1;
}

void alarmHandler(int signal) {
    alarmFlag = 1;
}

void killChild(int child[]) {
    int status;
    pid_t pid;
    for(int i=0; i < 5; i++) {
        kill(child[i], SIGKILL);
        pid = wait(&status);
        fprintf(stderr, "Child with pid %ld exited with status %i\n",(long)pid, status);
    }
}
