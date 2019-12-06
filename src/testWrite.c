#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readCsv.h"
#include "writebin.h"
#include "auxFun.h"

/*
 * To compile:
    gcc -o testw -g -Wall testMain.c readCvs.c auxFun.c
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

    readFirst(NOMF, &nnodes, &nways);
    printf("nnodes %lu\n", nnodes);
    printf("nways %lu\n", nways);

    node *nodes;

    readNodes(NOMF, &nodes, nnodes, nways);
    printf("node 0. Nsucc: %d , succ %lu\n", nodes[0].nsucc, nodes[0].successors[0]);
    printf("node 1. Nsucc: %d , succ %lu\n", nodes[1].nsucc, nodes[1].successors[0]);
    
  //  writeBin(NOMF, nodes, nnodes);

/* cleaning: eliminates flags from valgrind so we can see actual problems*/
int i;
for (i=0;i<nnodes;i++){
  if(nodes[i].nsucc>0){
      free(nodes[i].successors);
      nodes[i].successors=NULL;
  }
  if(nodes[i].name!=NULL){
      free(nodes[i].name); 
      nodes[i].name=NULL;
  }
}    
free(nodes);
    return 0; 
}
