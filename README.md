# practica_procesos

Sergio Bernal Parrondo
Victor Gallego Lijarcio


La practica esta hecha hasta el paso 6, es decir completa. Todos los pasos anteriores, en especifico
las versiones del principal, estan dentro de la carpeta "principal_pasos". 

En cuanto al diseño, el programa contiene 3 ficheros:
    
    childs_struct: aqui se declara la estructura donde se guarda toda la informacion relacionada con los hijos (pid, ultimo estado, n_itner, etc)
    y algunas funciones para su manejo
    
    stats_process: funcion que imprime por pantalla la informacion de los hijos, la cual recibe por un pipe.
    
    principal: el nucleo del programa. Se puede resumir en que entra en un bucle while infinito y cada vez que salte la señal alarm cambia al siguiente
    proceso siguiendo "Round Robin". Por seguridad hemos evitado no tocar nada en los manejadores de señales, tan solo marcan su respectivo flag y 
    dentro del bucle while se manejan.


Instrucciones para compilar:
    
    1. Compilar el codigo de los hijos -> gcc -o proceso proceso.c
    2. Compilar stats_process con -> gcc -o stats_process stats_process.c child_stuct.c
    3. Compilar principal con -> gcc -o principal principal.c child_stuct.c
    4. Finalmente para ejecutar -> ./principal

