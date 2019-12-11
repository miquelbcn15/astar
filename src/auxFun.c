#include <stdio.h>
#include "auxFun.h"
#include <stdlib.h>

void ExitError(char* miss, int errcode) {
  fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
}

/*---------------------------------------------------------*/

long binarySearch(node *nodes, unsigned long key, unsigned long nnodes) {
    register unsigned long start = 0UL, middle, afterend = nnodes;
    register unsigned long try;
    while (afterend > start) {
        middle = start + ((afterend - start - 1) >> 1);
        try = nodes[middle].id;
        if (key == try) return middle;
        else if (key > try) start = middle + 1;
        else   afterend = middle;
    }
    return -1;
}

/*---------------------------------------------------------*/

void clean(int ** v) {
    free(*v); v=NULL; 
}
