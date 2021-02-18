#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>


int main(int argc, char **argv){

  int p, n_iti = 1, id = 0;


  if(argc != 2 || (p = atoi(argv[1])) == 0) {
    fprintf(stderr, "Argumentos erroneos, solo se permite 1 y debe ser integer\n");
    exit(-1);
  }

  if (p <= 0) {
      fprintf(stderr, "El argumento debe ser mayor que 0\n");
      exit(-1);
  }

  char points[p];
  for(int i = 0; i < p; i++) {
      points[i] = '*';
  }

  id = fork();

  if(id != 0) exit(-1);

  while (1) {
      fprintf(stderr, "[%i] Proceso %i [%s] iter %i \n", getpid(), p, points, n_iti);
      n_iti++;
      sleep(1);
  }


}
