#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "readCsv.h"
#include "writebin.h"
#include "auxFun.h"

/*
 * To compile:
    gcc -o testw -g -Wall testMain.c readCvs.c auxFun.c writebin.c
 * Exemple of execution:
    ./test cataluna.csv
 */

int main(int argc, char *argv[]){

    unsigned long nnodes = 0;
    unsigned long nways  = 0;

#define NOMF argv[1]
#define NARGS 2
    if (argc<NARGS
       ) {
        fprintf(stderr,"%s file\n", argv[0]);
        return -1;
    }

    clock_t timeA = clock();
    readFirst(NOMF, &nnodes, &nways);
    fprintf(stderr, "readFirst(): nnodes %lu\n", nnodes);
    fprintf(stderr, "readFirst(): nways %lu\n", nways);

    node *nodes;

    readNodes(NOMF, &nodes, nnodes, nways);
    writeBin(NOMF, nodes, nnodes);
    clock_t timeB = clock();
    fprintf(stdout, "Total time reading and storing the file: %lf \n",
            (double)(timeB - timeA)/CLOCKS_PER_SEC);
    
    /* Free memory */
    unsigned long i;
    for (i = 0; i < nnodes; i++) {
        if( nodes[i].nsucc > 0 ) {
            free(nodes[i].successors);
            nodes[i].successors=NULL;
        }
    }       
    free(nodes);
    nodes=NULL;
    
    return 0; 
}
