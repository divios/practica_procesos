#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

//Declaramos los prototipos de las funciones que manejan las señales
void alarmHandler(int signal);
void quitHandler(int signal);

int alarmFlag = 1;

int main(int argc, char **argv){

  int p, n_iti = 1;
  /***************************************************************************
  Declaramos las señales que queremos que se activen, SIGALRM, SIGQUIT.
  Se activarán al segundo en el caso de SIGALRM o cuando introduzcamoS CTRL + \
  ****************************************************************************/
  signal(SIGALRM, alarmHandler);
  signal(SIGQUIT, quitHandler);
  // Mensajes de error que lanza el programa cuando el argumento no es correcto.
  if(argc != 2 || strspn(argv[1], "0123456789") != strlen(argv[1])) {
    fprintf(stderr, "Argumentos erroneos, solo se permite 1 y debe ser integer\n");
    exit(-1);
  }

  p = atoi(argv[1]);
  if (p <= 0) {
      fprintf(stderr, "El argumento debe ser mayor que 0\n");
      exit(-1);
  }
  //"Texto" creado por el nº de asteriscos introducido
  char points[p];
  for(int i = 0; i < p; i++) {
      points[i] = '*';
  }
  //Bucle que imprime el proceso y los '*' adecuados.
  while (1) {
      if (alarmFlag == 0) continue;

      alarmFlag = 0;
      fprintf(stderr, "[%i] Proceso %i [%s] iter %i \n", getpid(), p, points, n_iti);
      n_iti++;
      alarm(1);
  }
}
/*********************************************************************************
Funciones que desarollan los manejadores de señales. Recordemos que el programa
no debe cerrarse con la señal SIGQUIT y por eso está vacía su función.
*********************************************************************************/
void alarmHandler(int signal) {
    alarmFlag = 1;
}

void quitHandler(int signal) {

}

