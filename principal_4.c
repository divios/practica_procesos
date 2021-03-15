

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
int alarmFlag = 0, killFlag = 0;

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

        //somos es el padre
        kill(childs[i].pid, SIGSTOP); //paramos ese proceso

    }

    int current = 0;
    alarm(1);
    changeStatus(&childs[0], 1);

    while(1) {
        
        if (killFlag == 1) {
            break;
        }
        else if (alarmFlag == 0) continue;
    /*****************************************************************
    Manejamos a los procesos hijos matándolos cuando es necesario. La 
    variable current nos ayuda recorrer todos los procesos que tenemos 
    en marcha y cuando llega a 4 el siguiente es 0 para cumplir con los
    5 procesos puestos en marcha por fork().
    ******************************************************************/
        alarmFlag = 0;
        changeStatus(&childs[current], 0);
        if(current == 4) current = 0;
        else current++;
        changeStatus(&childs[current], 1);
        alarm(1);
    }

    killChild(childs);
    //Creamos un descriptor de archivo para usarlo en nuestra tubería
    pid_t pid;
    int MYFIFO[2];
    pipe(MYFIFO);

    if ( (pid = fork()) < 0) {
        fprintf(stderr, "Error while creating stats_process\n");
        exit(-1);
    } else if ( pid == 0) {
        //Como es el hijo, ejecutamos el programa stats_process
        dup2(MYFIFO[0], STDIN_FILENO);
        execl("./stats_process","./stats_process", NULL);
        //fprintf(stderr, "Error while executing stats_process\n");
    }
    //Escribimos en la FIFO nuestros hijos.
    write(MYFIFO[1], childs, sizeof(childs));

    int status;
    wait(&status);

    exit(0);

}
/*****************************************************************************
Definición de las funciones que manejan las señales y la encargada de matar
a los hijos
*****************************************************************************/
void intHandler(int signal) {
    killFlag = 1;
}

void alarmHandler(int signal) {
    alarmFlag = 1;
}

void killChild(childs_t child[]) {
    for(int i=0; i < 5; i++) {
        kill(child[i].pid, SIGKILL);
        wait(&(child[i].last_status));
    }
}


