#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "readCsv.h"
#include "writebin.h"
#include "auxFun.h"
#include "astar.h"

/*
 * To compile:
    gcc -o as -g -Wall newMain.c auxFun.c writebin.c astar.c -lm
 * Exemple of execution:
    ./test cataluna.bin
 */

int main(int argc, char *argv[]){
   node *nodes;
   unsigned long i;
   unsigned long *allsuccessors;
   unsigned long nnodes, numtotnsucc;
   
#define NOMF argv[1]
#define NARGS 2
    if (argc<NARGS
       ) {
        fprintf(stderr,"%s file\n", argv[0]);
        return -1;
    }
    
    /* Reading the file */
    fprintf(stderr, "Reading the file ... \n");
    clock_t time_readA = clock();
    readBin(NOMF, &nodes, &allsuccessors, &nnodes, &numtotnsucc);
    clock_t time_readB = clock();
    fprintf(stderr, "Reading time : %lf \n",
            (double)(time_readB - time_readA)/CLOCKS_PER_SEC);
                
  unsigned long goal, source, result;
  AStarStatus *allstatus;
  if( (allstatus=(AStarStatus*)malloc(nnodes*sizeof(AStarStatus)))==NULL) ExitError("when allocating memory for status",24);
  //goal=Astar(nodes,allstatus, nnodes,8670491,8670492,h1); //cataluna.bin
  clock_t time_A = clock();
     // goal=Astar(nodes,allstatus, nnodes, 771979683, 429854583, h3); //cataluna.bin
  goal=Astar(nodes,allstatus, nnodes,240949599,195977239,h1);   //spain.bin
  time_A = clock() - time_A;
  showPath(nodes, allstatus, goal);
  fprintf(stderr, "time a star: %f\n", (double)time_A/CLOCKS_PER_SEC);
    
    
/* cleaning: eliminates flags from valgrind so we can see actual problems*/
int done=0;
  for (i=0;i<nnodes;i++){
      if(nodes[i].nsucc>0 && !done) {free(nodes[i].successors); done=1;} 
      /*READ THIS: allsuccessors pointer is corrupted. The first time a set of successors was asigned we "create" a copy
       * of the pointer to the first position. So doing free of that pointer, frees allsuccessors vector*/
      if(nodes[i].name!=NULL) free(nodes[i].name); 
  }    
  free(nodes);
  free(allstatus);
  return 0; 
}
