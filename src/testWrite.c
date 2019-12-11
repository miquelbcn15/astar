#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readCsv.h"
#include "writebin.h"
#include "auxFun.h"

/*
 * To compile:
    gcc -o testw -g -Wall testMain.c readCvs.c auxFun.c writebin.c
 * Exemple of execution:
    ./test cataluna.csv
 */

/*
 * USEFUL SHIT:
    cut -d '|' -f3 cataluna.csv | awk '{if($0) print($0)}' |head      
 * GIVES ALL NAMES IN ORDER
    cut -d '|' -f3 cataluna.csv | grep -n 'La Roca'      
 * GIVES THE LINE NUMBER OF FIRST NAME, WHICH IS LA ROCA (648)
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

    readFirst(NOMF, &nnodes, &nways);
    fprintf(stderr, "readFirst(): nnodes %lu\n", nnodes);
    fprintf(stderr, "readFirst(): nways %lu\n", nways);

    node *nodes;

    readNodes(NOMF, &nodes, nnodes, nways);
    /* printf("node 0. Nsucc: %d , succ %lu\n", nodes[0].nsucc, nodes[0].successors[0]); */
    /* printf("FIRST name: %s \n", nodes[644].name); //should print La Roca. For control. */
    /* printf("last node. Nsucc: %d \n", nodes[nnodes-1].nsucc); */
    
    writeBin(NOMF, nodes, nnodes);
    
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
    
    /* Reading the written file */
    unsigned long *allsuccessors;
    unsigned long numnodos, numtotnsucc;
    readBin("cataluna.bin", &nodes, &allsuccessors, &numnodos, &numtotnsucc);
    /* printf("node 0. Nsucc: %d , succ %lu\n", nodes[0].nsucc, nodes[0].successors[0]); */
    /* printf("FIRST name: %s \n", nodes[644].name); //should print La Roca. For control. */
    /* printf("last node. Nsucc: %d \n", nodes[nnodes-1].nsucc); */
    /* cleaning: eliminates flags from valgrind so we can see actual problems*/
    //erased by accident. Fuck
    return 0; 
}
