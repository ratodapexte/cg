#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    char palavra[50] = {"Bom!dia.simpatia Bom dia"};
    char *pt;

    pt = strtok(palavra, "!. ");
    while(pt){
        printf("token: %s\n", pt);
        pt = strtok(NULL, "!. ");
    }

    return 0;
}