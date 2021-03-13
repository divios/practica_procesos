

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

int main(int argc, char **argv){

    childs_t childs[5];
    read(STDIN_FILENO, childs, sizeof(childs));

    fprintf(stderr, "\n Arg  \tPid  \tULT ESTADO  \tNUM EJEC \tSTATUS\n");
    pid_t pid;
    for(int i=0; i < 5; i++) {
        char ptr[7];
        getStatusStr(childs[i], ptr);
        fprintf(stderr, "  %2i  %6i  %12s  %12i  %12i\n",
                childs[i].argv, childs[i].pid, ptr, childs[i].n_times, childs[i].last_status);
    }

}

