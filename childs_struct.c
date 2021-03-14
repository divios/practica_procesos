

#include "childs_struct.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "childs_struct.h"


void changeStatus(childs_t *child, int status) {
    if (status == 0) { //lo estamos parando
        child -> status = 0;
        kill(child->pid, SIGSTOP);
    } else if (status == 1) {
        child -> status = 1;
        child -> n_times++;
        kill(child->pid, SIGCONT);
    } else if (status == -1) {
        child -> status = -1;
        kill(child->pid, SIGKILL);
    }
}

void getStatusStr(childs_t child, char *str) {
    if (child.status == 0) {
        str = strcpy(str, "READY");
    } else if (child.status == 1) {
        str = strcpy(str, "RUNNING");
    } else if (child.status == -1) {
        str = strcpy(str, "DELETED");
    }
}

