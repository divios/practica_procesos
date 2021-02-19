//
// Created by student on 18/2/21.
//

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char **argv){

    char *args[] = {"/.proceso", NULL};
    int childs[5];

    printf("hola\n");

    char snum[5];
    snum[0] = '1';
    snum[1] = '2';
    snum[2] = '3';
    snum[3] = '4';
    snum[4] = '5';


    for (int i = 0; i < 5; i++) {
        childs[i] = execvp(args[0], snum[i]);
    }

    printf("hola2\n");

}
