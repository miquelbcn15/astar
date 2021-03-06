#ifndef _AUXFUN_H
#define _AUXFUN_H

#define R 6378100        /* earth radius in meters */ 
#define PI 3.14159265359 
#define infinity 1e6     /* something bigger than the biggest value */

typedef char Queue;

enum whichQueue {NONE, OPEN, CLOSED};

typedef struct { 
    double g, h;
    unsigned long parent;
    Queue where;
} AStarStatus;

typedef struct List {
    double f;
    unsigned long index;
    struct List *next, *prev;
} list;

typedef struct {
    unsigned long id;
    char* name;
    double lat, lon;
    unsigned short nsucc;
    unsigned long *successors;
    list* lista;
} node;

/* heuristic function */
typedef double (*heur)(node u, node s);
double h0(node source, node dest);
double h1(node source, node dest);
double h2(node source, node dest);
double h3(node source, node dest);


void ExitError(char*, int); 
long binarySearch(node*, unsigned long, unsigned long);
#endif 
