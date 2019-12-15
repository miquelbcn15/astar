#ifndef _ASTAR_H
#define _ASTAR_H

#include "auxFun.h"
unsigned long Astar(node* nodes, AStarStatus *allstatus,unsigned long nnodes, unsigned long source, unsigned long dest,heur h);
void readList(list *start,node*nodes);
void showPath(node* nodes, AStarStatus* stats, int goal);
#endif