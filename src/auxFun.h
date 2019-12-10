#ifndef _AUXFUN_H
#define _AUXFUN_H

typedef struct {
    unsigned long id;
    char* name;; //was name[187]
    double lat, lon;
    unsigned short nsucc;
    unsigned long *successors;
} node;
void ExitError(char*, int); 
#endif /* _AUXFUN_H */
