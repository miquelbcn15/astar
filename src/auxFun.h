#ifndef _AUXFUN_H
#define _AUXFUN_H

typedef struct {
    unsigned long id;
    char *name;
    double lat, lon;
    unsigned short nsucc;
    unsigned long *successors;
} node;
void ExitError(char*, int); 
#endif /* _AUXFUN_H */
