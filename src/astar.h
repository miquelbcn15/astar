#ifndef _ASTAR_H
#define _ASTAR_H

#include "auxFun.h"

extern double weight, epsilon;

double w(node u, node v);   
void cleanList(list *start);
void push(list** start, unsigned long ind, AStarStatus stat, node* nodes,
        unsigned short* depth);
list* pop(list** start);
void deleteElement (list** start, list** element);
void computeF(list *link, AStarStatus stat, unsigned short depth);
unsigned long Astar(node* nodes, AStarStatus *allstatus, unsigned long nnodes, unsigned long source, unsigned long dest,heur h);
void push_start(list **start, unsigned long ind);
void readList(list *start,node*nodes);
void showPath(node* nodes, AStarStatus* stats, int goal);
#endif
