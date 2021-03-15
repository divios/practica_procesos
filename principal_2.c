#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
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
void killChild(int child[]);
void intHandler(int signal);
void alarmHandler(int signal);
/*******************************************************************
Declaramos el número de procesos hijos y flags para SIGALARM y para
la función killChild.
*******************************************************************/
pid_t childs[5];
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
        if ( (childs[i] = fork()) < 0) {
            fprintf(stderr, "Error while creating child %i\n", childs[i]);
            for (int j = 0; j < i; j++) {  //matamos todos los hijos creados hasta ahora
                int status;
                kill(childs[j], SIGKILL);
                wait(&status);
            }
            exit(0);
        }
        if (childs[i] == 0) {  //si somos el hijo
            //Aquí ejecutamos el ejecutable del programa proceso
            char *args[] = {"./proceso", "", NULL};
            args[1] = itoa(i + 1);
            execvp(args[0], args);
        }

        else { //somos es el padre
            kill(childs[i], SIGSTOP); //paramos ese proceso
        }
    }

    int current = 0;
    alarm(1);
    kill(childs[current], SIGCONT);

    while(1) {
        if (killFlag == 1) {
            alarm(0);
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
        kill(childs[current], SIGSTOP);
        if(current == 4) current = 0;
        else current++;
        kill(childs[current], SIGCONT);
        alarm(1);
    }

    killChild(childs);
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

void killChild(int child[]) {
    int status;
    pid_t pid;
    for(int i=0; i < 5; i++) {
        kill(child[i], SIGKILL);
        pid = wait(&status);
        fprintf(stderr, "Child with pid %ld exited with status %i\n", (long)pid, status);
    }
}