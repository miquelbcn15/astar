#include <stdio.h>
#include <string.h>
#include "readCvs.h"

int main(){
  char *name="cataluna.csv";
  unsigned long nnodes=0;
  unsigned long nways=0;
  readFirst(name,&nnodes, &nways);
  printf("nnodes %lu\n",nnodes);
  printf("nways %lu\n",nways);
  node*nodes;
  readNodes(name,&nodes,nnodes);
  printf("id node 0:%lu",nodes[0].id);
 return 0; 
}