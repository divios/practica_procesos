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
    for(int i=0; i < (sizeof(*child) / sizeof(int)); i++) {
        kill(child[i], SIGKILL);
    }
}

int main(int argc, char **argv){

    int child[] = {-1, -1, -1, -1, -1}, error_flag = 0;

    for(int i = 0; i < 5; i++) {
        if ((child[i] = fork()) == 0) {
            char *args[] = {"./proceso", "", NULL};
            args[1] = itoa(i + 1);
            execvp(args[0], args);
        }
        else {
            kill(child[i], 0);  /* Checks if process alive */
            if (errno == ESRCH) {
                error_flag = 1;
                break;
            }
        }
    }

    if (error_flag) {   /* If some child wasn't created */
        killChild(child);
        exit(-1);
    }

    sleep(3);
    killChild(child);

}
