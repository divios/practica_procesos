#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>


int main(int argc, char **argv){
  if(argc<0){
    fprintf(stderr, "Argumento no entero o menor que cero\n" );
    exit(-1);
  }
  if(argc==0){
    fprintf(stderr, "No hay suficientes argumentos\n", );
  }
  if(argc==1){
    if()
  }
}
