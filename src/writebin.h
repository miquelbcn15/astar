#ifndef _WRITEBIN_H
#define _WRITEBIN_H

#include "auxFun.h"
void writeBin(char *, node *, unsigned long );
void readBin(char *name, node **nodes,
        unsigned long **allsuccessors, unsigned long *nnodes,
        unsigned long *ntotnsucc); 
#endif
