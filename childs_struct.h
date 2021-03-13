

#ifndef PRACTICA_PROCESOS_CHILDS_STRUCT_H
#define PRACTICA_PROCESOS_CHILDS_STRUCT_H

#include <sys/types.h>

typedef struct childs {
    pid_t pid;
    int argv;
    int n_times;
    int status;
    int last_status;
} childs_t;

void changeStatus(childs_t *child, int status);

void getStatusStr(childs_t child, char *str);

#endif //PRACTICA_PROCESOS_CHILDS_STRUCT_H
