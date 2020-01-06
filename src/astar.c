#include "auxFun.h"
#include <float.h>
#include <math.h>
#include <limits.h> /* for the ulongmax */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

double weight  = .5;
double epsilon = .0; 

#define N 65535

double w(node u, node v){   
    double lat1 = PI/180. *u.lat , lon1 = PI/180. *u.lon;
    double lat2 = PI/180. *v.lat ,   lon2 = PI/180. *v.lon;
    double d_lon = fabs(lon1 - lon2); 
    double d_lat = fabs(lat1 - lat2); 
    double x = d_lon * cos((lat1 + lat2)/2.);
    return R * sqrt(x*x + d_lat*d_lat);
}

/*----------------------------------------------------------------*/

void computeF(list *link, AStarStatus stat, unsigned short depth) {
    link->f = (1. - weight) * stat.g + weight * stat.h * 
              (1. + epsilon * (1. - depth/N));
}

/*----------------------------------------------------------------*/

void cleanList(list *start) {
    list *aux, *aux2;
    aux = start;
    aux = aux->next;
    while ( aux != NULL ) {
      aux2 = aux->next;
      free(aux);
      aux = aux2;
    }
    free(start); start=NULL;  
}

/*----------------------------------------------------------------*/

void push(list** start, unsigned long ind, AStarStatus stat, node* nodes,
        unsigned short* depth) {
    /* pushing the element ind sorted*/
    list* element;

    if ( (element = (list*)malloc(sizeof(list))) == NULL) 
        ExitError("allocating memory for list element", 25);
    element->index = ind;
    computeF(element, stat, depth[ind]);
    nodes[ind].lista = element;
    
    if ( (*start) == NULL ) {   /* the list is empty */
        (*start) = element;
        (*start)->next = NULL;
        (*start)->prev = NULL;
    }
    else {
        list  *aux, *father;
        aux    = (*start);
        father = NULL;
        while (aux != NULL) {
            if (element->f < aux->f) {
                /* Insert node before aux and return */
                element->next     = aux;
                element->prev     = father;
                aux->prev         = element;
                if (aux == (*start)) *start = element;
                else (father)->next = element;
                return;
            }
            father = aux;
            aux    = aux->next;
        }
        /* Insert node at the end of the list */
        element->next = NULL;
        element->prev = father;
        father->next  = element;
    }
}

/*----------------------------------------------------------------*/

list* pop(list** start) { 
    /* returns the first element */
    if ( (*start) == NULL ) {   /* if list is null */ 
        fprintf(stderr, "pop(): Null queue to extract from. Check for errors,\
                I'm stoppin\n");
        exit(0);
    }

    list* this;
    this           = (*start);
    (*start)       = this->next;
    /* 
     * If there is only one element
     * Gives the only element and assign the start pointer to NULL to know
     * it's empty 
     */
    if ( (*start) != NULL ) {
        (*start)->prev = NULL;       /* updating pointers */
    }
    return this;
}    

/*----------------------------------------------------------------*/

void deleteElement (list** start, list** element) {
    /* deletes the structure element from the list start */
    if ( (*start)->next == NULL && (*start)->prev == NULL ) {    
        /* list with just one element remains NULL after removing */
        (*element) = NULL;
        (*start)   = NULL;
    }
    else if ( (*start) == (*element) ) {   /* removing the first element */
        (*start)       = (*element)->next;
        (*start)->prev = NULL;
        (*element)     = NULL;
    }
    else if ( (*element)->next == NULL) {  /* removing the last element */
        ((*element)->prev)->next = NULL;
        (*element)               = NULL;
    }
    else {
        ((*element)->next)->prev = (*element)->prev;
        ((*element)->prev)->next = (*element)->next;
        (*element)               = NULL;
    }
}

/*---------------------------------------------------------------*/

unsigned long Astar(node* nodes, AStarStatus *allstatus, long nnodes, 
        unsigned long source, unsigned long dest, heur h) { 
    /* source and dest are expected to be id */
    unsigned long i;
    fprintf(stderr, "Astar(): init\n");

    unsigned short *depth;
    if ( (depth = (unsigned short*)malloc(nnodes * sizeof(unsigned short)))
            == NULL) 
        ExitError("when allocating memory depth", 25);
    memset(depth, USHRT_MAX, nnodes * sizeof(unsigned short));

    /* identification of source and destination */
    unsigned long s, g;
    s = binarySearch(nodes, source, nnodes);
    g = binarySearch(nodes, dest, nnodes); 
    if ( s == -1 || g == -1 ) { 
        fprintf(stderr, "Astar(): nodes not found\n");
        return -1; 
    } 
    /* now s and g are the indices of source and goal */

    /* initializing the values of the source node */
    allstatus[s].g      = 0; 
    allstatus[s].h      = h(nodes[s], nodes[g]); 
    allstatus[s].parent = ULONG_MAX;
    depth[s]            = 0U;

    /* locating all nodes in a list, all nodes element list set to NULL */
    for ( i = 0; i < nnodes; i++) { 
        allstatus[i].where = NONE;
        nodes[i].lista     = NULL; 
    }
    allstatus[s].where = OPEN;

    /* creating the open list and adding the source node */
    list *start;
    start = NULL;
    push(&start, s, allstatus[s], nodes, depth);

    fprintf(stdout, "# Heuristic distance to goal from source: %f\n", 
            h(nodes[s], nodes[g]));

    /*declaring auxiliar variables needed in the loop*/
    list *u, *z;            /* an element from the list will be pop*/
    double newDist; 
    double prevF = 0;       /* to check if the heuristic is monotone */
    unsigned long v;        /* the index of an adjacent node */
    int monotone = 1;

    /*main loop*/
    while ( start != NULL ) {
        /* removes the first element */
        u = pop(&start);
        nodes[u->index].lista = NULL;
        if ( u == NULL) ExitError("something went very wrong popping a node",26);

        /*checking monotonicity*/
        if ( u->f < prevF) monotone = 0;
        prevF = u->f;
        
        /*checking ending of the function*/
        if( u->index == g ) { fprintf(stderr, "Astar(): done"); break; } 

        /* loop through all successors of the popped node */
        for ( i = 0; i < nodes[u->index].nsucc; i++ ) {
            v = nodes[u->index].successors[i];
            z = nodes[v].lista;  /* element of the list */
            if (depth[v] == SHRT_MAX) depth[v] = depth[u->index] + 1; 
            
            double we = w(nodes[u->index], nodes[v]);
            if ( !we ) continue; /*for duplicated nodes*/
            newDist = allstatus[u->index].g + we;

            /* which list is v in? act in consequence */
            if (allstatus[v].where == OPEN) {
                if (allstatus[v].g < newDist) continue;
                deleteElement(&start, &z /*list*/);
                nodes[v].lista = NULL;
            }
            else if (allstatus[v].where == CLOSED) {
                if (allstatus[v].g < newDist) continue; 
                allstatus[v].where = OPEN;
            }
            else {
                allstatus[v].where = OPEN;
                allstatus[v].h     = h(nodes[v],nodes[g]); 
            }
            allstatus[v].g      = newDist;
            allstatus[v].parent = u->index;
            push(&start, v, allstatus[v], nodes, depth);
        }
        allstatus[u->index].where = CLOSED;  
        free(u); u = NULL;
    }
    if ( u== NULL ) { 
        fprintf(stderr, "Something was wrong during Astar\n"); 
        cleanList(start); 
        return -1;
    }
    cleanList(start);
    
    if ( monotone ) fprintf(stderr, "Astar(): heuristic is monotone\n"); 
    else fprintf(stderr, "Astar(): heuristic is not monotone\n");
    
    v = u->index; 
    /* reusing variable unsigned long to store the index i want to return */
    free(u); u = NULL;
    fprintf(stderr, "Astar(): done\n");
    return v; 
}

/*--------------------------------------------------------------*/
void push_start(list **start, unsigned long ind) {
    list *aux;
    aux=(*start);
    if ( ((*start) = (list*)malloc(sizeof(list))) == NULL) 
        ExitError("allocating memory for list element", 25);
    (**start).index = ind;
    (**start).next  = aux;
}

/*--------------------------------------------------------------*/

void readList(list *start, node *nodes, AStarStatus *stats) {
    list *aux;
    aux = start;
    fprintf(stdout, "# Optimal path found\n# Id | Distance | Name | Latitude |"
            " Longitude \n");
    while ( aux != NULL) {
        fprintf(stdout, " %lu | %#.2f | %s | %3.6f |"
                " %3.6f",
                nodes[aux->index].id, stats[aux->index].g,
                nodes[aux->index].name, nodes[aux->index].lat,
                nodes[aux->index].lon);
        printf("\n");
        aux = (*aux).next;
    }
}

/*----------------------------------------------------------------*/

void showPath(node *nodes, AStarStatus *stats, int goal) {
    fprintf(stdout, "# Total distance: %f m\n", stats[goal].g);
    
    unsigned long i;
    i = goal;
    list* start; start = NULL;
    push_start(&start, i);
    i = stats[i].parent;
    while ( i != ULONG_MAX){
        push_start(&start,i);
        i = stats[i].parent;
    }
    fprintf(stdout, "# Route:\n");
    readList(start, nodes, stats);
    cleanList(start);
}

#undef N
