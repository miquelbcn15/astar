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
    gcc -o as -g -Wall main.c auxFun.c writebin.c astar.c -lm
 * Exemple of execution:
    echo 771979683 429854583 | ./astar spain.bin .5=weight .0=epsilon 
 */

int main(int argc, char *argv[]){
   node *nodes;
   unsigned long i;
   unsigned long *allsuccessors;
   unsigned long nnodes, numtotnsucc;
   weight  = .5;
   epsilon = .0; 
   
#define NOMF argv[1]
#define NARGS 2
    if (argc<NARGS
       ) {
        fprintf(stderr,"%s file\n", argv[0]);
        return -1;
    }
    if (argc >= NARGS + 1) {
        if (sscanf(argv[NARGS], "%lf", &weight) != 1) {
            fprintf(stderr, "main(): error reading weight\n");
            return -1;
        }
    }
    if (argc >= NARGS + 2) {
        if (sscanf(argv[NARGS + 1], "%lf", &epsilon) != 1) {
            fprintf(stderr, "main(): error reading epsilon\n");
            return -1;
        }
    }
    
    /* Reading the file */
    fprintf(stderr, "Reading the file ... \n");
    clock_t time_readA = clock();
    readBin(NOMF, &nodes, &allsuccessors, &nnodes, &numtotnsucc);
    clock_t time_readB = clock();
    fprintf(stdout, "# Reading time : %lf \n",
            (double)(time_readB - time_readA)/CLOCKS_PER_SEC);
                
    unsigned long goal, source, result;
    AStarStatus *allstatus;
    if( (allstatus = (AStarStatus*)malloc(nnodes*sizeof(AStarStatus))) == NULL)
        ExitError("when allocating memory for status",24);
    
    while ( scanf(" %lu %lu", &source, &goal) == 2) {
        clock_t time_A = clock();
        /* 
         * h0 : dijkstra
         * h1 : haversine
         * h2 : spherical law
         * h3 : equirectangular
         */
        result = Astar(nodes,allstatus, nnodes, source, goal, h2);
        if ( nodes[result].id != goal) { 
            fprintf(stderr, "Path not found\n");
            break;
        }
        time_A = clock() - time_A;
        fprintf(stdout, "# Time astar: %f\n", (double)time_A/CLOCKS_PER_SEC);
        showPath(nodes, allstatus, result);
    }
    
    /* cleaning: eliminates flags from valgrind so we can see actual problems*/
    int done=0;
    for ( i = 0; i < nnodes; i++) {
        if(nodes[i].nsucc > 0 && !done) { free(nodes[i].successors); done = 1;} 
        if(nodes[i].name!=NULL) free(nodes[i].name); 
     }    
     free(nodes);
     free(allstatus);
     
     return 0; 
}
