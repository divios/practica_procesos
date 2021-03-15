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
//itoa() nos devuelve el número pasado por parámetro como string.
char* itoa(int i) {
    static char str[12];
    sprintf(str, "%d", i);
    return str;
}
/****************************************************************
Declaración de los prototipos de los manejadores de señal y de 
la función que mata a los procesos hijos.
****************************************************************/
void killChild(childs_t child[]);
void intHandler(int signal);
void alarmHandler(int signal);
/*******************************************************************
Declaramos el número de procesos hijos y flags para SIGALARM y para
la función killChild.
*******************************************************************/
childs_t childs[5];
int alarmFlag = 0, killFLag = 0;

int main(int argc, char **argv){
    //Activamos las señales para que el programa las pueda usar.
    signal(SIGINT, intHandler);
    signal(SIGALRM, alarmHandler);
    /*********************************************************
    Creamos todos los procesos hijos. Si uno fallase, se matan
    los ya creados.
    **********************************************************/
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
            //Aquí ejecutamos el ejecutable del programa proceso
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
    /*****************************************************************
    Manejamos a los procesos hijos matándolos cuando es necesario. La 
    variable current nos ayuda recorrer todos los procesos que tenemos 
    en marcha y cuando llega a 4 el siguiente es 0 para cumplir con los
    5 procesos puestos en marcha por fork(). La función changeStatus 
    realiza el cambio de estado de 'READY' A 'RUNNING' y viceversa.
    ******************************************************************/
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

/*****************************************************************************
Definición de las funciones que manejan las señales y la encargada de matar
a los hijos. En este caso cuando matemos los hijos, imprimiremos la tabla con 
la información pedida en el enunciado de la práctica.
*****************************************************************************/

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


