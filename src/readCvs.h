#include <stdlib.h>
#include <string.h>
#include "auxFun.h"
//queremos nnodes, nways

void readFirst(char* name, unsigned long *nnodes, unsigned long *nways);
void readNodes(char* name, node **nodes, unsigned long nnodes, unsigned long);
void createEdge(node**, unsigned long, unsigned long, int, int*);
unsigned long binarySearch(node*, unsigned long, unsigned long);
