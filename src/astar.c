#include "auxFun.h"
#include <math.h>
#include <limits.h> //for the ulongmax
#include <stdlib.h>
#include <stdio.h>

double w(node u, node v){ //to test compilation
    return sqrt((u.lat-v.lat)*(u.lat-v.lat)+(u.lon-v.lon)*(u.lon-v.lon));
}

double h(node u, node s){ //to test compilation
    return sqrt((u.lat-s.lat)*(u.lat-s.lat)+(u.lon-s.lon)*(u.lon-s.lon));
}

/*----------------------------------------------------------------*/

void cleanList(list *start){
    list *aux,*aux2;
    aux=start;
    aux=aux->next;
    while(aux!=NULL) {
      aux2=aux->next;
      aux->vertex=NULL;
      aux->status=NULL;
      free(aux);
      aux=aux2;
    }
    start->vertex=NULL;
    start->status=NULL;
    free(start); // start=NULL;  /*maybe i want to conserve the pointer declared*/
}

/*----------------------------------------------------------------*/

void push(list **start, node *put, AStarStatus*stat){ //at the start
   list* aux;
   aux=(*start);
   if( ((*start)=(list*)malloc(sizeof(list)))==NULL ) ExitError("allocating memory for list element",25);  
   (**start).vertex=put;
   (**start).status=stat;
   (**start).next=aux;
}

/*----------------------------------------------------------------*/

list* pop(list **start){ //NO MEMORY IS FREED IN THIS FUNCTION BECAUSE THE ELEMENT IS RETURNED
 if((*start)==NULL){
  printf("Null queue to extract from\n"); exit(0); 
 }
 if((*start)->next==NULL){
  list* this;
  this=(*start); /*gives the only element, and assign the start pointer to NULL to know it's empty*/
  (*start)=NULL;
  return this;
 }
 double min=infinity;
 list* aux;
 list* parent=NULL;
 list* prev=NULL;
 list* father;
 
 aux=(*start);
 father=NULL;
 while(aux!=NULL){
  if(aux->f<min){
    min=aux->f;
    parent=aux;
    prev=father;
  }
  father=aux;
  aux=aux->next; 
 }//list already recorred. I know the min, its parent and its child in the queue
 //now pop that element, but don't lose the list
 if(parent==NULL){ //to be safe
  printf("\n No node with a min distance. Can this actually happen in A*?\n");
  return NULL;
 }
 list *this;
 this=parent; //the element i want
 aux=parent->next; //reuse the variable, is what i need to link afterwards
 
 if(prev==NULL){ //if the extracted is the first element
   (*start)=aux;
 }
 else
  prev->next=aux;  //link the rest of the list
 return this;
}

/*----------------------------------------------------------------*/

void computeF(list *link, AStarStatus stat){ //so we can add complicated things later
 link->f=stat.g+stat.h; 
}

/*---------------------------------------------------------------*/

int Astar(node* nodes, unsigned long nnodes, unsigned long source, unsigned long dest ){ //source and dest are expected to be id
  unsigned long i;
  /*identification of source and destination*/
  unsigned long s,g;
  s=binarySearch(nodes, source, nnodes);
  g=binarySearch(nodes, dest, nnodes); /*now s and g are the indices of source and goal*/
  
  /*creation of the array of status*/
  AStarStatus *allstatus;
  if( (allstatus=(AStarStatus*)malloc(nnodes*sizeof(AStarStatus)))==NULL) ExitError("when allocating memory for status",24);
      /*we need all nodes to have a status entry, to know at least if they are open, closed or none*/
  
  /*creating the open list and adding the source node*/
  list *start;
  start=NULL;
  push(&start,nodes+s,allstatus+s);
  
  /*initializing the values of the source node*/
  allstatus[s].g=0; allstatus[s].h=h(nodes[s],nodes[g]); computeF(start, allstatus[s]); allstatus[s].parent=ULONG_MAX;
  
  /*locating all nodes in a list*/
  for(i=0;i<nnodes;i++){ //this has to be done. Anything to reuse the loop?
    allstatus[i].where=NONE; 
  }
  allstatus[s].where=OPEN;
  
  /*declaring auxiliar variables needed in the loop*/
  list *u; //i'll be extracting the pointer to the list element
  double newDist; 
  double prevF=0; //to check if the heuristic is monotone
  unsigned long v; //the index of an adjacent node
  
  /*main loop*/
  while (start!=NULL){
      u=pop(&start);
      if(u==NULL) ExitError("something went very wrong popping a node",26);
      /*checking monotonicity*/
      if (u->f<prevF) printf("Detected node where heuristic not monotone!\n");
      prevF=u->f;
      
      /*checking ending of the function*/
      if(u->vertex->id==dest) break; //maybe a return here
      
      /*loop through all successors of the popped node*/
      for (i=0;i<u->vertex->nsucc;i++){
	  v=u->vertex->successors[i];
	  newDist=u->status->g+w(*(u->vertex),nodes[v]);
	  
	  /*which list is v in? act in consequence*/
	  if (allstatus[v].where==OPEN)
	      if (allstatus[v].g<newDist) continue; //READ THIS:valgrind not gonna like this, because g is not initialized. But if it's not, node is not open and this if is not checked. So, do we initialize g to infinity?
	  else if (allstatus[v].where==CLOSED){
	      if (allstatus[v].g<newDist) continue; //same here
	      push(&start, nodes+v,allstatus+v);
	      allstatus[v].where=OPEN;
	  }
	  else{
	      push(&start, nodes+v,allstatus+v);
	      allstatus[v].where=OPEN;
	      h(nodes[v],nodes[g]); 
	  }
	  allstatus[v].g=newDist;
	  allstatus[v].parent=binarySearch(nodes, u->vertex->id, nnodes);
	  computeF(start,allstatus[v]);
      }
      u->status->where=CLOSED;   
  }
  
  if(u->vertex->id!=dest)  printf("the list was empty, something went wrong\n");
  else printf("done!\n");
 return 0; 
}








