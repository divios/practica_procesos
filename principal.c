//
// Created by student on 18/2/21.
//

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

void killChild(int child[]) {
    for(int i=0; i < 5; i++) {
        int status;
        kill(child[i], SIGKILL);
        wait(&status);
    }
}

int main(int argc, char **argv){

    pid_t childs[5];

    for (int i = 0; i < 5; i++) {
        if ( (childs[i] = fork()) < 0) {
            printf("Error al crear el hijo n %i", i);
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

    sleep(4);  // Los segundos que queremos que dure el programa
    killChild(childs);

}
