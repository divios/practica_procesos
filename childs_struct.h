

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



 /// Cambia el estado del hijo
 ///
 /// @param child - Puntero a la struct que quieres cambiar
 /// @param status - Estado a establecer (-1: KIll, 0: Parar, 1: Continuar)

void changeStatus(childs_t *child, int status);

/// Devuelve el estado de un childs_t en forma de char y lo deposita en str
/// ( -1: STOPPED, 0: READY, 1: RUNNING)
///
/// @param child - struct de la que se consigue el estado
/// @param str - puntero de memoria al char donde se quiere guardar el estado

void getStatusStr(childs_t child, char *str);

#endif //PRACTICA_PROCESOS_CHILDS_STRUCT_H
