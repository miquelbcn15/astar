#ifndef _ASTAR_H
#define _ASTAR_H

#include "auxFun.h"
double w(node u, node v);   
void cleanList(list *start);
void push(list** start, unsigned long ind, AStarStatus stat, node* nodes);
list* pop(list** start);
void deleteElement (list** start, list** element);
void computeF(list *link, AStarStatus stat);
unsigned long Astar(node* nodes, AStarStatus *allstatus, unsigned long nnodes, unsigned long source, unsigned long dest,heur h);
void readList(list *start,node*nodes);
void showPath(node* nodes, AStarStatus* stats, int goal);
#endif
