#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define NUM_PID 5

pid_t pid[NUM_PID];

void execute(int signal){
    pause();
}

int main(){
    signal(SIGALRM, execute);
    int i=0;
    for(i=0; i<NUM_PID;i++){
        pid[i] = fork();
        if(pid[i]<0){
            fprintf(stderr, ("Fallo en el fork "));
        }
    }

    while(1){
         int i=0;
        for(i=0; i<NUM_PID;i++){
            pid[i] = fork();
            if(pid[i]<0){
                fprintf(stderr, ("Fallo en el fork "));
                exit(-1);
            }
            printf("Mi PID es: %i\n", getpid());
            /*No se si aqui se deberían matar los procesos o seguir con ellos, hay un momento en el que da error al ejecutar*/
            /*Igual eso hay que tocarlo en la señal*/
            alarm(1);
        }
        i=0;
    }


}