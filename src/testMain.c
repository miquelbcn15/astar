#include <stdio.h>
#include <string.h>
#include "readCvs.h"
#include "writebin.h"

/*
 * To compile:
    gcc -o test -g -Wall testMain.c readCvs.c auxFun.c
 * Exemple of execution:
    ./test cataluna.csv
 */

int main(int argc, char *argv[]){

    unsigned long nnodes = 0;
    unsigned long nways  = 0;

#define FILE argv[1]
#define NARGS 2
    if (argc<NARGS
       ) {
        fprintf(stderr,"%s file\n", argv[0]);
        return -1;
    }

    readFirst(FILE, &nnodes, &nways);
    printf("nnodes %lu\n", nnodes);
    printf("nways %lu\n", nways);

    node *nodes;

    readNodes(FILE, &nodes, nnodes, nways);
    printf("node 0. Nsucc: %d , succ %lu\n", nodes[0].nsucc, nodes[0].successors[0]);
    printf("node 1. Nsucc: %d , succ %lu\n", nodes[1].nsucc, nodes[1].successors[0]);
    
    writeBin(FILE, nodes, nnodes);

    return 0; 
}
