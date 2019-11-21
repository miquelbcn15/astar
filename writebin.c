#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "auxFun.h"

void writeBin(char *argv[], FILE **fin, node *nodes, unsigned long nnodes) {
    /* Write the binary file to name.bin */

    unsigned long ntotnsucc = 0UL;
    char name[257];
    
    /* Computing the total number of successors */
    for (i = 0; i < nnodes; i++) ntotnsucc += nodes[i].nsucc;

    /* Setting the name of the binary file */
    strcpy(name, argv[1]); strcpy(strrchr(name,"."), ".bin");
    if ((*fin = fopen (name, "wb") == NULL)) 
        ExitError("the output binary data file cannot be opened",31);

    /* Global data header */
    if ( fwrite(nodes, sizeof(nodes), nnodes, *fin) != nnodes) 
        ExitError("when writing nodes to the output binary data file ", 32);

    /* Writing successors in blocks */
    for (i = 0; i < nnodes; i++) if (nodes[i].nsucc) {
        if ( fwrite(nodes[i].successors, sizeof(unsigned long),\ 
                    nodes[i].nsucc, *fin) != nodes[i].nsucc ) 
            ExitError("when writing edges to the output binary data file", 32);
    }
    
    fclose(*fin);
}
