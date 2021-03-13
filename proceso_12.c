#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#define N_CHILDREN 5

int main(int argc, char** argv){
    pid_t children[N_CHILDREN];
    int i=0;
    for(i=0; i<N_CHILDREN; i++){
        if((children[i]=fork())<0){
            perror("Unexpected error");
            exit(-1);
        }
        else if(children[i]==0){
            /*exec el codigo*/
            exit(0);
        }
    }
    int status;
    pid_t pid;
    int count_children = N_CHILDREN;
    while(count_children>0){
       pid = wait(&status);
       printf("Child with pid %ld exited with status %i\n",(long)pid, status) ;
        count_children--;
    }

}