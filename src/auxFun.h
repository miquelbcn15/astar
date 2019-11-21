#include <stdio.h>

typedef struct {
    unsigned long id; 
    char *name;
    double lat, lon;
    unsigned short nsucc;
    unsigned long *successors;
} node;

void ExitError(char*, int); 
