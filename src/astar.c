#include "auxFun.h"
#include <float.h>
#include <math.h>
#include <limits.h> //for the ulongmax
#include <stdlib.h>
#include <stdio.h>

double w(node u, node v){ //to test compilation  
    double lat1 = PI/180 *u.lat , lon1 = PI/180 *u.lon;
    double lat2 = PI/180 *v.lat ,   lon2 = PI/180 *v.lon;
    double d_lon = fabs(lon1 - lon2); 
    double d_lat = fabs(lat1 - lat2); 
    double x = d_lon * cos((lat1 + lat2)/2);
    return R * sqrt(x*x + d_lat*d_lat);
}

/*----------------------------------------------------------------*/

void cleanList(list *start){
    list *aux,*aux2;
    aux=start;
    aux=aux->next;
    while(aux!=NULL) {
      aux2=aux->next;
      free(aux);
      aux=aux2;
    }
    free(start); start=NULL;  }

/*----------------------------------------------------------------*/

void push(list **start, unsigned long ind){ //at the start
   list* aux;
   aux=(*start);
   if( ((*start)=(list*)malloc(sizeof(list)))==NULL ) ExitError("allocating memory for list element",25);  
   (**start).index=ind;
   (**start).next=aux;
}

/*----------------------------------------------------------------*/

list* pop(list **start){ //MEMORY IS NOT FREED IN THIS FUNCTION
 if((*start)==NULL){
  printf("Null queue to extract from. Check for errors, I'm stopping\n"); exit(0); 
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
 list* this;
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

unsigned long Astar(node* nodes, AStarStatus *allstatus, long nnodes, unsigned long source, unsigned long dest,heur h){ //source and dest are expected to be id
  unsigned long i;
  /* for (i=0; i < nnodes; i++) allstatus[i].g = DBL_MAX; */
  /*identification of source and destination*/
  unsigned long s,g;
  s=binarySearch(nodes, source, nnodes);
  g=binarySearch(nodes, dest, nnodes); /*now s and g are the indices of source and goal*/
  
  /*creating the open list and adding the source node*/
  list *start;
  start=NULL;
  push(&start,s);
  
  /*initializing the values of the source node*/
  allstatus[s].g=0; allstatus[s].h=h(nodes[s],nodes[g]); computeF(start, allstatus[s]); allstatus[s].parent=ULONG_MAX;
  
  /*locating all nodes in a list*/
  for(i=0;i<nnodes;i++){ //this has to be done. Anything to reuse the loop?
    allstatus[i].where=NONE; 
  }
  allstatus[s].where=OPEN;
  printf("#heuristic distance to goal from source: %f\n",h(nodes[s],nodes[g]));
  /*declaring auxiliar variables needed in the loop*/
  list* u; //i'll be extracting an element from the list
  double newDist; 
  double prevF=0; //to check if the heuristic is monotone
  unsigned long v; //the index of an adjacent node
  int monotone=1;
  /*main loop*/
  while (start!=NULL){
      u=pop(&start);
//      printf("%lu|%f|%f\n",nodes[u->index].id,nodes[u->index].lat,nodes[u->index].lon);
//      printf("popped g=%f,h=%f,f=%f\n",allstatus[u->index].g,allstatus[u->index].h,u->f);
      if(u==NULL) ExitError("something went very wrong popping a node",26);
      /*checking monotonicity*/
      if (u->f<prevF) monotone=0;
      prevF=u->f;
      /*checking ending of the function*/
      if(u->index==g){printf("#done!\n"); break;} //maybe a return here
      
      /*loop through all successors of the popped node*/
     // printf("-----\n");
      for (i=0;i<nodes[u->index].nsucc;i++){
	  v=nodes[u->index].successors[i];
          double we=w(nodes[u->index],nodes[v]);
	  if(!we) continue; 
	  newDist=allstatus[u->index].g+we;
	  
	  /*which list is v in? act in consequence*/
	  if (allstatus[v].where==OPEN){
	      if (allstatus[v].g<newDist) continue;} 
	  else if (allstatus[v].where==CLOSED){
	      if (allstatus[v].g<newDist) continue; 
	      push(&start, v);
	      allstatus[v].where=OPEN;
	  }
	  else{
	      push(&start, v);
	      allstatus[v].where=OPEN;
	      allstatus[v].h=h(nodes[v],nodes[g]); 
	  }
	  allstatus[v].g=newDist;
	  allstatus[v].parent=u->index;
	  computeF(start,allstatus[v]);
      }
      allstatus[u->index].where=CLOSED;  
      free(u); u=NULL; //the list elemement popped now is cleaned
  }
 if(u==NULL) {printf("something was wrong\n"); cleanList(start); return -1;}
 cleanList(start);
 if(monotone) printf("#heuristic is monotone\n"); else printf("#nope, heuristic is not monotone\n");
 v=u->index; //reusing variable unsigned long to store the index i want to return
 free(u); u=NULL;
 return v; 
}

/*--------------------------------------------------------------*/

void readList(list *start,node*nodes){
 list *aux;
 aux=start;
 while(aux!=NULL){
  printf("%lu|%f|%f",nodes[aux->index].id,nodes[aux->index].lat,nodes[aux->index].lon);
 // if(nodes[aux->index].name!=NULL) printf(" called %s",nodes[aux->index].name);
  printf("\n");
  aux=(*aux).next;
 }
}

void showPath(node* nodes, AStarStatus* stats, int goal){
 printf("#Total distance: %f m\n",stats[goal].g);
 unsigned long i;
 i=goal;
 list* start;start=NULL;
 push(&start, i);
 i=stats[i].parent;
 while(i!=ULONG_MAX){
   push(&start,i);
   i=stats[i].parent;
 }
 printf("#Route:\n");
 readList(start,nodes);
 //printf("You have reached your destination\n");
 cleanList(start);
}










