	#include <stdio.h>
	#include <string.h>
	#include <errno.h>
	#include <unistd.h>
	#include <sys/wait.h>
	#include <sys/types.h>
	#include <stdlib.h>
	
	int check_param(int argc, char**argv);
	int main(int argc, char **argv){
	
	  int p, n_iti = 1, id = 0 ;
		/*if(argc != 2 || (p = atoi(argv[1])) == 0) {  
	    fprintf(stderr, "Argumentos erroneos, solo se permite 1 y debe ser integer\n");
	    exit(-1);
	  }
	
	  if (p <= 0) {
	      fprintf(stderr, "El argumento debe ser mayor que 0\n");
	      exit(-1);
	  }
	*/
		p = check_param(argc, argv);
	  char points[p];
	  for(int i = 0; i < p; i++) {
	      points[i] = '*';
	  }
	
	  id = fork();
	
	  if(id != 0) exit(-1);
		int i =0;
	  while (i!=5) {
	      fprintf(stderr, "[%i] Proceso %i [%s] iter %i \n", getpid(), p, points, n_iti);
	      n_iti++;
	      sleep(1);
		  i++;
	  }
	  	/**************************************************
		************QUITAR CUANDO ACABES 1.2**************/

		printf("END");
		exit(0);
	  	/**************************************************
		**************************************************/
	
	}

	int check_param(int argc, char** argv){
		  char *cp;
    	long lval;
    	int val;

    // skip over program name
    --argc;
    ++argv;

    if (argc < 1) {
        fprintf(stderr,"No hay suficientes argumentos\n");
        exit(-1);
    }

    cp = *argv;
    if (*cp == 0) {
        fprintf(stderr,"No hay suficientes argumentos\n");
        exit(-1);
    }

    lval = strtol(cp,&cp,10);
    if (*cp != 0) {
        fprintf(stderr,"Argumentos erroneos, solo se permite 1 y debe ser integer\n");
        exit(-1);
    }
		//Comprobamos que el valor sea entero:	
    val = (int) lval;

    
#if 1
    if (val != lval) {
        fprintf(stderr,"main: argument '%s' (with value %ld) is too large to fit into an integer -- truncated to %d\n",
            *argv,lval,val);
        exit(-1);
    }
#endif

    return val;
	  }
	