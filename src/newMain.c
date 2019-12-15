#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readCsv.h"
#include "writebin.h"
#include "auxFun.h"
#include "astar.h"

/*
 * To compile:
    gcc -o as -g -Wall newMain.c auxFun.c writebin.c astar.c
 * Exemple of execution:
    ./test cataluna.bin
 */



int main(int argc, char *argv[]){
#define NOMF argv[1]
#define NARGS 2
//     if (argc<NARGS
//        ) {
//         fprintf(stderr,"%s file\n", argv[0]);
//         return -1;
//     }

   node *nodes;
   unsigned long i;
   unsigned long *allsuccessors;
   unsigned long nnodes, numtotnsucc;
   readBin("cataluna.bin", &nodes, &allsuccessors, &nnodes, &numtotnsucc);
      /* Control prints, just in case something explodes*/
      printf("node 0. Nsucc: %d , succ %lu\n", nodes[0].nsucc, nodes[0].successors[0]);
      printf("FIRST name: %s \n", nodes[644].name); //should print La Roca. For control.
      printf("last node. Nsucc: %d \n", nodes[nnodes-1].nsucc);
      
   Astar(nodes,nnodes, 8670491, 8670492);
    
    
    
    
    
    
    
    
/* cleaning: eliminates flags from valgrind so we can see actual problems*/
int done=0;
  for (i=0;i<nnodes;i++){
      if(nodes[i].nsucc>0 && !done) {clean(&nodes[i].successors); done=1;} 
      /*READ THIS: allsuccessors pointer is corrupted. The first time a set of successors was asigned we "create" a copy
       * of the pointer to the first position. So doing free of that pointer, frees allsuccessors vector*/
      if(nodes[i].name!=NULL) clean(&nodes[i].name); 
  }    
  clean(&nodes);
  return 0; 
}