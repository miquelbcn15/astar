#ifndef _AUXFUN_H
#define _AUXFUN_H

typedef struct {
    unsigned long id;
    char* name;; //was name[187]
    double lat, lon;
    unsigned short nsucc;
    unsigned long *successors;
} node;

typedef char Queue;

enum whichQueue {NONE, OPEN, CLOSED};

typedef struct { 
    double g, h;
    unsigned long parent;
    Queue whq;
} AStarStatus;

typedef struct List {
    double f;
    node *vertex;
    AStarStatus *status;
    List *next;
} list;

void ExitError(char*, int); 
unsigned long binarySearch(node*, unsigned long, unsigned long);
#endif /* _AUXFUN_H */
