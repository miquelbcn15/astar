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
  readNodes(name,&nodes,nnodes,nways);
  printf("node 0. Nsucc: %d , succ %lu\n",nodes[0].nsucc,nodes[0].successors[0]);
  printf("node 1. Nsucc: %d , succ %lu\n",nodes[1].nsucc,nodes[1].successors[0]);
 return 0; 
}