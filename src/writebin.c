#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "auxFun.h"

void writeBin(char *file, node *nodes, unsigned long nnodes) {
    /* Write the binary file to name.bin */
    fprintf(stderr, "writeBin(): init\n");

    unsigned long ntotnsucc = 0UL;
    char name[257];
    FILE *fin;
    
    /* Computing the total number of successors */
    for (int i = 0; i < nnodes; i++) ntotnsucc += nodes[i].nsucc;

    /* Setting the name of the binary file */
    strcpy(name, file); strcpy(strrchr(name,'.'), ".bin");
    if ((fin = fopen(name, "wb")) == NULL) 
        ExitError("the output binary data file cannot be opened",31);

    /* Global data header */
    if ( fwrite(&nnodes, sizeof(unsigned long), 1, fin) +
         fwrite(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2 )
        ExitError("when initializing the output binary data file", 32);
    fprintf(stderr, "nnodes : %lu\nallsucc : %lu\n", nnodes, ntotnsucc);

    /* Writing all nodes */
    if ( fwrite(nodes, sizeof(nodes), nnodes, fin) != nnodes) 
        ExitError("when writing nodes to the output binary data file ", 32);

    /* Writing successors in blocks */
    for (int i = 0; i < nnodes; i++) if (nodes[i].nsucc) {
        if ( fwrite(nodes[i].successors, sizeof(unsigned long),
                    nodes[i].nsucc, fin) != nodes[i].nsucc ) 
            ExitError("when writing edges to the output binary data file", 32);
    }
    
    fclose(fin);
    fprintf(stderr, "writeBin(): finished\n");
}

/*------------------------------------------------------------*/

void readBin(char *name, node **nodes,
        unsigned long **allsuccessors, unsigned long *nnodes,
        unsigned long *ntotnsucc) {
    /* Reads the binary file */
    fprintf(stderr, "readBin(): init\n");
    FILE *fin;

    if ((fin = fopen (name, "rb")) == NULL) 
        ExitError("the data file does not exist or cannot be opened", 11);

    /* Global data --- header */
    if (fread(nnodes, sizeof(unsigned long), 1, fin) +\
        fread(ntotnsucc, sizeof(unsigned long), 1, fin) != 2 )
        ExitError("when reading the header of the binary data file", 12);

    /* Getting memory for all data */
    if (( (*nodes) = (node*)malloc((*nnodes) * sizeof(node))) == NULL ) 
        ExitError("when allocating memory for the nodes vector", 13);
    if (( (*allsuccessors) = (unsigned long*)malloc((*ntotnsucc) * sizeof(unsigned long))) == NULL)
        ExitError("when allocating memory for the edges vector", 15);
    
    fprintf(stderr, "nnodes : %lu\nallsucc : %lu\n", *nnodes, *ntotnsucc);

    /* Reading all data from file */
    /* unsigned long nodesread = fread((*nodes), sizeof(node), (*nnodes), fin); */
    /* if ( nodesread != (*nnodes) ) */
    /*     fprintf(stderr, "nodes read : %lu\n", nodesread); */
    /*     fprintf(stderr, "node 0. Nsucc: %d  \n", (*nodes)[0].nsucc); */
    /*     fprintf(stderr, "node 1. Nsucc : %d \n", (*nodes)[1].nsucc); */
    if ( fread( (*nodes), sizeof(node), (*nnodes), fin) != (*nnodes))
        ExitError("when reading nodes from the binary data file", 17);
    if ( fread((*allsuccessors), sizeof(unsigned long), (*ntotnsucc), fin) !=
            (*ntotnsucc) )
        ExitError("when reading successors from the binary data file", 18);

    fclose(fin);

    /* Setting pointers to successors */
    for (int i = 0; i < *nnodes; i++) if((*nodes[i]).nsucc) {
        (*nodes)[i].successors = *allsuccessors; *allsuccessors +=
            (*nodes)[i].nsucc;
    }

    fprintf(stderr, "readBin(): end\n");
}
